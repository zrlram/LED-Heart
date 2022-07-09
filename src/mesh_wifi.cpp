#include <Arduino.h>
#include <WiFi.h>
#include <mesh_wifi.h>
#include <esp_now.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x30, 0xc6, 0xF7, 0x1e, 0x28, 0xb4};

bool is_wifi_sender = false;
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

static voidFuncPtr callBackPointer;

void set_callback(voidFuncPtr callBackFunction) {
  callBackPointer = callBackFunction;
  esp_now_register_recv_cb(callBackPointer);
}


bool wifi_sender() {
    return is_wifi_sender;
}

bool wifi_send(flirt_message_struct &flirt) {
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &flirt, sizeof(flirt));
    if (result == ESP_OK) 
        return true;
    else
        return false;
}

void setup_wifi() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    Serial.println(WiFi.macAddress());

    // firgure out the role / sender or just receiver?
    // set up the role pin
    pinMode(WIFI_ROLE_PIN, INPUT);
    digitalWrite(WIFI_ROLE_PIN,HIGH);
    delay(20);    // To get a solid reading on the role pin
    if ( digitalRead(WIFI_ROLE_PIN) ) {
      Serial.println("I am the flirty one");
      is_wifi_sender = true;
    } else {
      Serial.println("I am flirted with");
      is_wifi_sender = false;
    }

    // setup ESP NOW
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
    }

    if (is_wifi_sender) {
        esp_now_register_send_cb(OnDataSent);
        // Register peer
        memcpy(peerInfo.peer_addr, broadcastAddress, 6);
        peerInfo.channel = 0;  
        peerInfo.encrypt = false;

        // Add peer        
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
            Serial.println("Failed to add peer");
            return;
        }
    } else  {
        // receiver
        // esp_now_register_recv_cb(callBackPointer);
        // have to register with set_callback()
    }



}