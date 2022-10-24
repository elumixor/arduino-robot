#include "Arduino.h"
#include "HardwareSerial.h"
#include "SoftwareSerial.h"

// 2 - yellow
// 3 - blue
SoftwareSerial esp_serial(3, 2);

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

  char buf[len + 1];
  Serial.readBytes(buf, len);
  buf[len] = '\0';

  return {buf};
}

String esp_in(const char *msg) {
  esp_serial.write(msg, strlen(msg));
  esp_serial.write("\r\n");

  static char buffer[128];
  uint8_t i = 0;

  // Receive data until either OK or ERROR is received
  while (true) {
    while (esp_serial.available() == 0)
      delay(1);

    char c = esp_serial.read();
    buffer[i++] = c;

    if (i >= 2 && buffer[i - 2] == 'O' && buffer[i - 1] == 'K')
      break;
    if (i >= 5 && buffer[i - 5] == 'E' && buffer[i - 4] == 'R' &&
        buffer[i - 3] == 'R' && buffer[i - 2] == 'O' && buffer[i - 1] == 'R')
      break;
  }

  buffer[i] = '\0';

  return {buffer};
}

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
  const auto msg = ser_in();
  const auto esp_response = esp_in(msg.c_str());
  ser_out(esp_response.c_str());
}
