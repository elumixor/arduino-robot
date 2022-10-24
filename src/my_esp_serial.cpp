#include "my_esp_serial.h"

MyEspSerial::MyEspSerial(int esp_rx, int esp_tx, int esp_baud_rate)
    : esp_serial(esp_tx, esp_rx) {
  esp_serial.begin(esp_baud_rate);

  while (!esp_serial)
    ;
}

String MyEspSerial::send(const String &msg) {
  esp_serial.write(msg.c_str(), msg.length());
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
