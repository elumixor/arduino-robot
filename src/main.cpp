#include "Arduino.h"
#include "HardwareSerial.h"
#include "SoftwareSerial.h"

void ser_out(const char *msg) {
  uint16_t len = strlen(msg);
  Serial.write((uint8_t *)&len, sizeof(len));
  Serial.write(msg, strlen(msg));
}

String ser_in() {
  char len_buf[sizeof(uint16_t)];

  while (Serial.available() < sizeof(uint16_t))
    delay(1);

  Serial.readBytes(len_buf, sizeof(uint16_t));
  uint16_t len = *(uint16_t *)len_buf;

  while (Serial.available() < len)
    delay(1);

  char buf[len];
  Serial.readBytes(buf, len);
  return {buf};
}

// 2 - yellow (RX)
// 3 - blue (TX)
SoftwareSerial esp_serial(2, 3);

void setup() {
  Serial.begin(9600);
  esp_serial.begin(9600);

  while (!Serial) {
  }

  ser_out("Serial ready");

  while (!esp_serial) {
  }

  ser_out("ESP serial ready");
}

void loop() {
  while (true) {
    const auto msg = ser_in();
    ser_out(msg.c_str());
  }
}
