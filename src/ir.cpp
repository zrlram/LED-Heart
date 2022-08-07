#include "ir.h"
#include <IRremote.hpp>  

boolean is_ir_sender = false;

void setup_ir() {

    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver

    /*
    // firgure out the role / sender or just receiver?
    // set up the role pin
    pinMode(IR_ROLE_PIN, INPUT);
    digitalWrite(IR_ROLE_PIN,HIGH);
    delay(20);    // To get a solid reading on the role pin
    if ( digitalRead(IR_ROLE_PIN) ) {
      Serial.println("I am the flirty one");
      is_ir_sender = true;
    }
    else {
      Serial.println("I am flirted with");
      is_ir_sender = false;
    }

    if (is_ir_sender) {
      IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK); // Specify send pin and enable feedback LED at default feedback LED pin
    }
    */

}

void send_flirt() {
    uint16_t rawData[8]={ 1, 1, 1, 1, 1, 1, 1, 1 };     // FLIRT

    IrReceiver.stop();
    IrSender.sendRaw(rawData, 8, 38); // data, length, khz

    /*
    Serial.println(F("Sent flirt "));

    IrReceiver.start();
    */

}

uint32_t read_ir() {

  if (IrReceiver.decode()) {
      uint32_t ret = IrReceiver.decodedIRData.decodedRawData;
      /*
      if (result == 0xFFFFFFFF) {
        result = previous;
      }
      */

      if (DEBUG_IR) {
        if (ret != 0) {
          Serial.print("IR: ");
          Serial.println(ret, HEX);
        }
      }

      IrReceiver.resume();

      return ret;

  }
  return 0;

}

void debug_ir() {

    if (IrReceiver.decode()) {
      Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
      // IrReceiver.printIRResultShort(&Serial); // optional use new print version
      IrReceiver.resume(); // Enable receiving of the next value
    }


}