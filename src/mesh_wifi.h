#pragma once

/* SENDER: 78:21:84:7C:1C:74
   RECVR:  30:C6:F7:1E:28:B4
   78:21:84:7C:1C:74
   */

#define WIFI_ROLE_PIN (16)

typedef struct struct_message {
  bool flirt;
} flirt_message_struct;

typedef void (*voidFuncPtr)(const uint8_t * mac, const uint8_t *incomingData, int len);

void setup_wifi();
bool wifi_sender();
bool wifi_send(flirt_message_struct &flirt);
void set_callback(voidFuncPtr callBackFunction);

// void sendMessage() ; 
// void setup_mesh();
// bool mesh_loop();
