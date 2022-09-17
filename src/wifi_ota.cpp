#include <WiFi.h>
#include <ArduinoOTA.h>
#include <Arduino.h>


#define STA_SSID "The_Heart"
#define STA_PASS "flirtingisfun"

int wifi_on = false;


bool wifi_toggle() {
    if (!wifi_on) {
        //adc_power_on();
        WiFi.disconnect(false);  // Reconnect the network
        WiFi.mode(WIFI_STA);    // Switch WiFi off
        // TODO: Create an AP, don't connect to the local network!
        WiFi.begin(STA_SSID, STA_PASS);
        while (WiFi.waitForConnectResult() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println(WiFi.localIP());
        ArduinoOTA.setHostname("theheart");
        ArduinoOTA.setPassword("heart");

        ArduinoOTA.onStart([]() {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH) {
                type = "sketch";
            } else {  // U_FS
                type = "filesystem";
            }

            // NOTE: if updating FS this would be the place to unmount FS using FS.end()
            Serial.println("Start updating " + type);
        });
        ArduinoOTA.onEnd([]() {
            Serial.println("\nEnd");
        });
        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        });
        ArduinoOTA.onError([](ota_error_t error) {
                Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) {
                Serial.println("Auth Failed");
            } else if (error == OTA_BEGIN_ERROR) {
                Serial.println("Begin Failed");
            } else if (error == OTA_CONNECT_ERROR) {
                Serial.println("Connect Failed");
            } else if (error == OTA_RECEIVE_ERROR) {
                Serial.println("Receive Failed");
            } else if (error == OTA_END_ERROR) {
                Serial.println("End Failed");
            }
        });
        ArduinoOTA.begin();

        return true;

    } else {
        // adc_power_off();
        WiFi.disconnect(true);  // Disconnect from the network
        WiFi.mode(WIFI_OFF);    // Switch WiFi off

        return false;
    }

}

void wifi_loop() {
    ArduinoOTA.handle();
}
