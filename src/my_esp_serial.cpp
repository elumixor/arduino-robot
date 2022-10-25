#include "my_esp_serial.h"

MyEspSerial::MyEspSerial(int esp_rx, int esp_tx, int esp_baud_rate)
    : esp_serial(esp_tx, esp_rx) {
  esp_serial.begin(esp_baud_rate);

  while (!esp_serial)
    ;
}

String MyEspSerial::send(const String &msg) {
  send_forget(msg);

  // Receive data until either OK or ERROR is received
  while (true) {
    while (esp_serial.available() == 0)
      delay(1);

    char c = esp_serial.read();
    current_message[i++] = c;

    if (i >= 2 && current_message[i - 2] == 'O' && current_message[i - 1] == 'K')
      break;
    if (i >= 5 && current_message[i - 5] == 'E' && current_message[i - 4] == 'R' &&
        current_message[i - 3] == 'R' && current_message[i - 2] == 'O' && current_message[i - 1] == 'R')
      break;
  }

  current_message[i] = '\0';

  return {current_message};
}

void MyEspSerial::send_forget(const String &msg) {
  esp_serial.write(msg.c_str(), msg.length());
  esp_serial.write("\r\n");
}

String MyEspSerial::check_message() {
  // Receive data until either OK or ERROR is received
  while (esp_serial.available() > 0) {

    char c = esp_serial.read();
    current_message[i++] = c;

    if ((i >= 2 && current_message[i - 2] == 'O' &&
         current_message[i - 1] == 'K') ||
        (i >= 5 && current_message[i - 5] == 'E' &&
         current_message[i - 4] == 'R' && current_message[i - 3] == 'R' &&
         current_message[i - 2] == 'O' && current_message[i - 1] == 'R')) {
      current_message[i] = '\0';
      i = 0;
      return {current_message};
    }
  }

  return {""};
}
