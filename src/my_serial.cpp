#include "my_serial.h"
#include "Arduino.h"

MySerial::MySerial(int baud_rate) {
  Serial.begin(baud_rate);

  while (!Serial)
    ;
}

void MySerial::send(const String &message) const {
  uint16_t len = message.length();
  Serial.write((uint8_t *)&len, sizeof(len));
  Serial.write(message.c_str(), len);
}

String MySerial::read() const {
  char len_buf[sizeof(uint16_t)];

  while (Serial.available() < sizeof(uint16_t))
    delay(1);

  Serial.readBytes(len_buf, sizeof(uint16_t));
  uint16_t len = *(uint16_t *)len_buf;

  while (Serial.available() < len)
    delay(1);

  char buf[len + 1];
  Serial.readBytes(buf, len);
  buf[len] = '\0';

  return {buf};
}
