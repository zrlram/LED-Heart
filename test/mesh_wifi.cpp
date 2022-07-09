#include "mesh_wifi.h"

// https://randomnerdtutorials.com/esp-mesh-esp32-esp8266-painlessmesh/

#include "painlessMesh.h"

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

boolean flirt = false;

bool mesh_loop() {
  // it will run the user scheduler as well
  mesh.update();

  return flirt;

}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

// when a new node joins or leaves
void changedConnectionCallback() {
  Serial.printf("Changed connections - node joined or left\n");
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  if (msg.startsWith("flirting")) {
    flirt = true;
  } else {
    flirt = false;
  }
}

void sendMessage() {
  String msg = "flirting";
  // msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 5, TASK_SECOND * 15 ));
}

void setCallback() {
    mesh.onReceive(&receivedCallback);
}

void setup_mesh() {

    //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

    mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    //mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

    userScheduler.addTask( taskSendMessage );
    taskSendMessage.enable(); 
}