#pragma once

#include "sys/time.h"
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "BLEBeacon.h"
#include "esp_sleep.h"

#define ROLE_PIN (16)
bool is_sender = false;

String knownBLEAddresses[] = {"78:21:84:7C:1C:74", "30:C6:F7:1E:28:B4"};
int RSSI_THRESHOLD = -55;
bool device_found;
int scanTime = 5; //In seconds
BLEScan* pBLEScan;

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))

#define GPIO_DEEP_SLEEP_DURATION     10  // sleep x seconds and then wake up
RTC_DATA_ATTR static time_t last;        // remember last boot in RTC Memory
RTC_DATA_ATTR static uint32_t bootcount; // remember number of boots in RTC Memory

// BLEService *pService;
// BLECharacteristic *pCharacteristic;

BLEAdvertising *pAdvertising;   // BLE Advertisement type
struct timeval now;

#define SERVICE_UUID        "3fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define BEACON_UUID "aeb5483e-36e1-4688-b7f5-ea07361b26a8"

void setBeacon() {

  BLEBeacon oBeacon = BLEBeacon();
  oBeacon.setManufacturerId(0x4C00); // fake Apple 0x004C LSB (ENDIAN_CHANGE_U16!)
  oBeacon.setProximityUUID(BLEUUID(BEACON_UUID));
  oBeacon.setMajor((bootcount & 0xFFFF0000) >> 16);
  oBeacon.setMinor(bootcount & 0xFFFF);
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  BLEAdvertisementData oScanResponseData = BLEAdvertisementData();
  oAdvertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED 0x04

  std::string strServiceData = "";

  strServiceData += (char)26;     // Len
  strServiceData += (char)0xFF;   // Type
  strServiceData += oBeacon.getData();
  oAdvertisementData.addData(strServiceData);

  pAdvertising->setAdvertisementData(oAdvertisementData);
  // pAdvertising->setScanResponseData(oScanResponseData);
  pAdvertising->setAdvertisementType(ADV_TYPE_NONCONN_IND);


}


class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      if (!advertisedDevice.haveServiceUUID())
      {
        if (advertisedDevice.haveManufacturerData() == true)
        {
          std::string strManufacturerData = advertisedDevice.getManufacturerData();

          uint8_t cManufacturerData[100];
          strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);

          if (strManufacturerData.length() == 25 && cManufacturerData[0] == 0x4C && cManufacturerData[1] == 0x00)
          {
            /*Serial.print("RSSI: ");
            Serial.println(advertisedDevice.getRSSI());
            BLEBeacon oBeacon = BLEBeacon();
            oBeacon.setData(strManufacturerData);
            Serial.printf("ID: %04X Major: %d Minor: %d UUID: %s Power: %d\n", oBeacon.getManufacturerId(), ENDIAN_CHANGE_U16(oBeacon.getMajor()), ENDIAN_CHANGE_U16(oBeacon.getMinor()), oBeacon.getProximityUUID().toString().c_str(), oBeacon.getSignalPower());
            */
            device_found = true;
          }
        }
        return;
      }
      /*
      for (int i = 0; i < (sizeof(knownBLEAddresses) / sizeof(knownBLEAddresses[0])); i++)
      {
        if (strcmp(advertisedDevice.getAddress().toString().c_str(), knownBLEAddresses[i].c_str()) == 0) {
          device_found = true;
          Serial.println(advertisedDevice.getRSSI());
          break;
        }
        else
          device_found = false;
      }
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      */
    }
};

void ble_setup() {

    gettimeofday(&now, NULL);
    //Serial.printf("start ESP32 %d\n", bootcount++);
    //Serial.printf("deep sleep (%lds since last reset, %lds since last boot)\n", now.tv_sec, now.tv_sec - last);
    last = now.tv_sec;

    // firgure out the role / sender or just receiver?
    // set up the role pin
    pinMode(ROLE_PIN, INPUT);
    digitalWrite(ROLE_PIN,HIGH);
    delay(20);    // To get a solid reading on the role pin
    if ( digitalRead(ROLE_PIN) ) {
      Serial.println("I am the flirty one");
      is_sender = true;
    } else {
      Serial.println("I am flirted with");
      is_sender = false;
    }



    if (is_sender) {
        BLEDevice::init("Flirty-Server");
        // BLEServer *pServer = BLEDevice::createServer();
        //pService = pServer->createService(SERVICE_UUID);
        //pCharacteristic = pService->createCharacteristic(
                                         //CHARACTERISTIC_UUID,
                                         //BLECharacteristic::PROPERTY_READ |
                                         //BLECharacteristic::PROPERTY_WRITE
                                       //);
        // pCharacteristic->setValue("Hello, World!");
        //pService->start();
        pAdvertising = BLEDevice::getAdvertising();
        setBeacon();
        pAdvertising->start();
        //pAdvertising->addServiceUUID(SERVICE_UUID);
        //pAdvertising->setScanResponse(true);
        //pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
        //pAdvertising->setMinPreferred(0x12);
        Serial.println("Advertizing started...");
        delay(100);
        /* pAdvertising->stop();
        Serial.printf("enter deep sleep\n");
        esp_deep_sleep(1000000LL * GPIO_DEEP_SLEEP_DURATION);
        Serial.printf("in deep sleep\n");
        */

    } else  {
        // receiver
        BLEDevice::init("Flirty-Recipient");
        pBLEScan = BLEDevice::getScan(); //create new scan
        pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); //Init Callback Function
        // pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
        pBLEScan->setInterval(1349); // set Scan interval
        pBLEScan->setWindow(499);  // less or equal setInterval value
        // pBLEScan->start(5, false);
    }


}

void ble_loop() {
  if (device_found) {
    Serial.println("Yayy... my mat-ey");
  }
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  for (int i=0; i<foundDevices.getCount(); i++) 
  {
    BLEAdvertisedDevice device = foundDevices.getDevice(i);
    int rssi = device.getRSSI();
    Serial.print("RSSI: ");
    Serial.println(rssi);
    if (rssi > RSSI_THRESHOLD && device_found)
      Serial.println("There she is!!");
    else
      Serial.println("So lonely");
  }
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
}