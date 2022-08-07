#pragma once

// Good code examples
// BLE client / server: https://randomnerdtutorials.com/esp32-ble-server-client/

#include "sys/time.h"
#include <Arduino.h>
#include <heart.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "BLEBeacon.h"
#include "esp_sleep.h"

#define RSSI_THRESHOLD (-40)
#define SCAN_TIME (3)     // scan for 3 seconds every now and then 

bool device_found = false;
bool connected = false;

// String knownBLEAddresses[] = {"78:21:84:7c:1c:76", "30:c6:f7:1e:28:b6"}; // lowercase!
static BLEAddress serverAddress = BLEAddress("78:21:84:7c:1c:76");
// #define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))

// #define GPIO_DEEP_SLEEP_DURATION     10  // sleep x seconds and then wake up
// RTC_DATA_ATTR static time_t last;        // remember last boot in RTC Memory
// RTC_DATA_ATTR static uint32_t bootcount; // remember number of boots in RTC Memory

BLEAddress *pServerAddress;
BLEScan* pBLEScan;
BLEService *pService;
BLECharacteristic *pCharacteristic;
BLEAdvertising *pAdvertising;   // BLE Advertisement type

#define SERVICE_UUID              "31337000-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID        "31337000-1fb5-459e-8fcc-c5c9c331914b"
#define SERVERNAME      "THEHEART"

/*
#define BEACON_UUID           "31337000-1fb5-459e-8fcc-c5c9c331914b"

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

*/

bool connectToServer(BLEAddress pAddress) {

  Serial.print("Forming a connection to: ");
  Serial.print(pAddress.toString().c_str());

  BLEClient*  pClient  = BLEDevice::createClient();
  bool res = pClient->connect(pAddress);
  Serial.print(" - Connected to server status: ");
  Serial.print(res);

  Serial.print(" - RSSI: ");
  Serial.println(pClient->getRssi());

  return res;

  /* THIS CODE IS PRETTY BAD - 
     https://esp32.com/viewtopic.php?t=4350

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    return;
  }

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  notifyChar = pRemoteService->getCharacteristic(notifyUUID);
  writeChar = pRemoteService->getCharacteristic(writeUUID);

  if (notifyChar == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(notifyUUID.toString().c_str());
    return;
  }
  
  // Read the value of the characteristic.
  std::string value = notifyChar->readValue();
  Serial.print("The characteristic value was: ");
  Serial.println(value.c_str());
  
  result = value.c_str();
  
  notifyChar->registerForNotify(notifyCallback);
  Serial.println("Done connecting");
  delay(400);
  */
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {

    void onResult(BLEAdvertisedDevice advertisedDevice) {
      
    // Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
 
      /*
      if (!advertisedDevice.haveServiceUUID())
      {
        if (advertisedDevice.haveManufacturerData() == true)
        {
          std::string strManufacturerData = advertisedDevice.getManufacturerData();

          uint8_t cManufacturerData[100];
          strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);

          if (strManufacturerData.length() == 25 && cManufacturerData[0] == 0x4C && cManufacturerData[1] == 0x00)
          {
            //Serial.print("RSSI: ");
            //Serial.println(advertisedDevice.getRSSI());
            //BLEBeacon oBeacon = BLEBeacon();
            //oBeacon.setData(strManufacturerData);
            //Serial.printf("ID: %04X Major: %d Minor: %d UUID: %s Power: %d\n", oBeacon.getManufacturerId(), ENDIAN_CHANGE_U16(oBeacon.getMajor()), ENDIAN_CHANGE_U16(oBeacon.getMinor()), oBeacon.getProximityUUID().toString().c_str(), oBeacon.getSignalPower());
            device_found = true;
          }
        }
        return;
      }
      */

      // Serial.println(advertisedDevice.getName().c_str());
      // Serial.println(advertisedDevice.getAddress().toString().c_str());

      // TODO: Fix to use the SERVERNAME - somehow not advertised by the server
      // if (advertisedDevice.getName() == SERVERNAME) { //Check if the name of the advertiser matches
      // DOES THIS WORK ??
      // if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(serviceUUID)) {

      if (advertisedDevice.getAddress().equals(serverAddress)) { //Check if the name of the advertiser matches

          Serial.println(advertisedDevice.getRSSI());

          if (advertisedDevice.getRSSI() < RSSI_THRESHOLD) {
            device_found = true;
            pServerAddress = new BLEAddress(advertisedDevice.getAddress()); //Address of advertiser is the one we need
          } 
          advertisedDevice.getScan()->stop(); // stop scanning anyways. We know we found our partner, but it's too far away
      }
    }
};

// https://randomnerdtutorials.com/esp32-ble-server-client/
class MyServerCallbacks: public BLEServerCallbacks {

  void onConnect(BLEServer* pServer) {
    Serial.println("SERVER _ NERVOUS _ CLIENT CONN");
    device_found = true;
    connected = true;
  };

  void onDisconnect(BLEServer* pServer) {
    device_found = false;
    connected = false;
  }

};

bool get_sender() {
  return is_server();
}

void setup_ble() {

    // gettimeofday(&now, NULL);
    //Serial.printf("start ESP32 %d\n", bootcount++);
    //Serial.printf("deep sleep (%lds since last reset, %lds since last boot)\n", now.tv_sec, now.tv_sec - last);
    // last = now.tv_sec;

  

    if (is_server()) {
        BLEDevice::init(SERVERNAME);
        BLEServer *pServer = BLEDevice::createServer();
        pServer->setCallbacks(new MyServerCallbacks());
        pService = pServer->createService(SERVICE_UUID);
        pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
        pCharacteristic->setValue("Hello, World!");
        pService->start();
        pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->addServiceUUID(SERVICE_UUID);
        // setBeacon();
        pServer->getAdvertising()->start();

        // pAdvertising->start();

        //pAdvertising->addServiceUUID(SERVICE_UUID);
        //pAdvertising->setScanResponse(true);
        //pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
        //pAdvertising->setMinPreferred(0x12);
        Serial.println("Advertizing started...");
        // delay(100);
        /* pAdvertising->stop();
        Serial.printf("enter deep sleep\n");
        esp_deep_sleep(1000000LL * GPIO_DEEP_SLEEP_DURATION);
        Serial.printf("in deep sleep\n");
        */

    } else {

        BLEDevice::init("Flirty-Recipient");
        
        pBLEScan = BLEDevice::getScan();    //create new scan
        pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); //Init Callback Function
        pBLEScan->setActiveScan(false); //active scan uses more power, but get results faster
        //pBLEScan->setInterval(80); // set Scan interval
        //pBLEScan->setWindow(30);  // less or equal setInterval value
    }

    Serial.print("BLE_Address: ");
    BLEAddress Addr = BLEDevice::getAddress();
    Serial.println(Addr.toString().c_str());

}

void ble_scan() {
  // start or continue the scan

  // TODO: Should this be true not false here?
  if (!is_server() && !connected) {
    pBLEScan->start(SCAN_TIME, nullptr, true);    // https://github.com/espressif/arduino-esp32/issues/6090
    Serial.println("BLE_Scan");
  }

}

bool ble_loop() {

  if (connected) {
    return true;
  }

  if (device_found) {
    if (connectToServer(*pServerAddress))  {      

      connected = true;

    } else {
      Serial.println("Connecting to Server failed");
      connected = false;
    }
    device_found = false;
    pBLEScan->clearResults();
    return true;
  } 

  /*
  if (!connected && !device_found) {
    return false; 
  }
  */

  return false;

}