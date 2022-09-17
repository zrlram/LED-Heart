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
#include "esp_sleep.h"
#include "arduino-timer.h"

#define RSSI_THRESHOLD (-40)
#define SCAN_TIME (3)     // scan for 3 seconds every now and then 

bool device_found = false;
bool connected = false;
bool overwrite_nervous = false;
bool _just_disconnected = false;

auto bt_timeout = timer_create_default();        // we are only nervous for a given time
// auto nervous_timeout = timer_create_default();   // we want to prevent from being nervous and then nervous right away

// String knownBLEAddresses[] = {"78:21:84:7c:1c:76", "30:c6:f7:1e:28:b6"}; // lowercase!
// static BLEAddress serverAddress = BLEAddress("78:21:84:7c:1c:76");
// #define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))

BLEAddress *pServerAddress;
BLEScan* pBLEScan;
BLEClient* pClient;
BLEService *pService;
BLECharacteristic *pCharacteristic;
BLEAdvertising *pAdvertising;   // BLE Advertisement type

#define SERVICE_UUID               "31337000-1fb5-459e-8fcc-c5c9c331914b"
static BLEUUID service_UUID        ("31337000-1fb5-459e-8fcc-c5c9c331914b");
#define CHARACTERISTIC_UUID        "31337000-1fb5-459e-8fcc-c5c9c331914b"
#define SERVERNAME                 "THEHEART"

bool clear_overwrite_nervous(void *) {
  _just_disconnected = false;
  Serial.println("Clear just_disconnected");
  return false;
}

// used to reset the timer back after 2 minutes after we just disconnected
void just_disconnected() {
  _just_disconnected = true;

  if (bt_timeout.empty()) {        // if there is not an actual timer already - don't want to overwrite the existing one
    // bt_timeout.cancel();
    bt_timeout.in(120000, clear_overwrite_nervous);
    Serial.println("Setting up just_disconnected");
  }
}

/* https://github.com/espressif/arduino-esp32/blob/master/libraries/BLE/examples/BLE_client/BLE_client.ino */
class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
    connected = true;
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    overwrite_nervous = false;
    just_disconnected();
    Serial.println("Client - onDisconnect");
  }
};

bool connectToServer(BLEAddress pAddress) {

  Serial.print("Forming a connection to: ");
  Serial.print(pAddress.toString().c_str());

  pClient  = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());

  bool res = pClient->connect(pAddress);
  Serial.print(" - Connected to server status: ");
  Serial.print(res);

  Serial.print(" - RSSI: ");
  Serial.println(pClient->getRssi());

  return res;

}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {

    void onResult(BLEAdvertisedDevice advertisedDevice) {
      
      // Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
 
      if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(service_UUID)) {

        Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());

        /*
          Serial.println(advertisedDevice.getName().c_str());
          Serial.println(advertisedDevice.getAddress().toString().c_str());
        */

        if (advertisedDevice.getRSSI() < RSSI_THRESHOLD) {
            device_found = true;
            pServerAddress = new BLEAddress(advertisedDevice.getAddress()); //Address of advertiser is the one we need
        } 
        advertisedDevice.getScan()->stop(); // stop scanning anyways. We know we found our partner, but it's too far away
      }
    }
};

/* For the server */
// https://randomnerdtutorials.com/esp32-ble-server-client/
// and: https://github.com/espressif/arduino-esp32/blob/master/libraries/BLE/examples/BLE_server_multiconnect/BLE_server_multiconnect.ino
class MyServerCallbacks: public BLEServerCallbacks { 

  void onConnect(BLEServer* pServer) {
    Serial.println("SERVER _ NERVOUS _ CLIENT CONN");
    connected = true;
  };

  void onDisconnect(BLEServer* pServer) {
    Serial.println("Disconnecting from Client");
    connected = false;
    overwrite_nervous = false;
    just_disconnected();

    pServer->getAdvertising()->start();
  }

};

bool get_sender() {
  return is_server();
}

void setup_ble() {

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
        pAdvertising->setScanResponse(true);
        pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
        pAdvertising->setMinPreferred(0x12);
        pServer->getAdvertising()->start();
        Serial.println("Advertizing started...");

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

  if (!is_server() && !connected) {
    pBLEScan->start(SCAN_TIME, nullptr, true);    // https://github.com/espressif/arduino-esp32/issues/6090
    Serial.println("BLE_Scan");
  }

}

// main calls this when we want to stop being nervous, we select random() or a show. 
// this is set util BLE disconnects - on server or client
void set_overwrite_nervous() {
  overwrite_nervous = true;
  //just_disconnected();                            // just reset all these variables in two minutes as well
}

/* return: true if connected and wasn't overwritten by IR and we didn't _just_ disconnect within 2 minutes
           false if not connected or in overwrite mode       */
bool ble_loop() {

  bt_timeout.tick();

  /*
  Serial.print(_just_disconnected);
  Serial.print(" ");
  Serial.print(overwrite_nervous);
  Serial.print(" ");
  Serial.print(connected);
  Serial.print(" ");
  Serial.println(device_found);
  */

  // there is a two minute grace period when we disconnect before we get nervous again
  if (_just_disconnected) {
    return false;
  }

  // only return true when we are not in 'overwrite mode'
  // meaning, we are still connected, but IR was used to overwrite
  // overwrite_nervous is overwritten when we lose connectivity
  if (connected && !overwrite_nervous) {
    // check timer to see if we should go into random mode and return a connected of false
    /* 
    if (!is_server() && pClient->isConnected()) {
      Serial.print(" - RSSI: ");
      Serial.println(pClient->getRssi());
    }
    */
    return true;
  }

  if (device_found) {     // client code
    if (connectToServer(*pServerAddress))  {      
      connected = true;
      // we are keeping track of being nervous and reset that after 2 minutes 
      // bt_timeout.cancel();
      // bt_timeout.in(120000, clear_overwrite_nervous);       // in milliseconds 1000 = 1 sec - this is 2 minutes
    } else {
      Serial.println("Connecting to Server failed");
      connected = false;
    }
    device_found = false;     // reset scanning
    pBLEScan->clearResults();
    return connected;
  } 

  // we are connected, but we are in overwrite_nervous
  return false;

}