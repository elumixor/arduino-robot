#include "Arduino.h"
#include "HardwareSerial.h"
#include "SoftwareSerial.h"
#include "my_serial.h"
#include "my_esp_serial.h"

MySerial *serial;
MyEspSerial *esp_serial;

void setup() {
  serial = new MySerial();
  esp_serial = new MyEspSerial();

  serial->send("Setup complete");
}

void loop() {
  const auto msg = serial->read();
  const auto esp_response = esp_serial->send(msg);
  serial->send(esp_response);
}
