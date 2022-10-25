#include "Arduino.h"
#include "SoftwareSerial.h"

class MyEspSerial {
  SoftwareSerial esp_serial;

  uint8_t i{0};
  char current_message[256];
public:
  /**
   * ESP RX -> 2 - yellow
   * ESP TX -> 3 - blue
   */
  MyEspSerial(int esp_rx = 2, int esp_tx = 3, int esp_baud_rate = 9600);

  String send(const String &message);
  void send_forget(const String &message);
  String check_message();
};
