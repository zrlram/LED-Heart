#include "bluetooth.h"

void read_bt() {
  SerialBT.begin("ram_bt");

  SerialBT.write('h');
  /*
  if (SerialBT.available())
  {
    Serial.write(SerialBT.read());
  }
  delay(20);
  */
}