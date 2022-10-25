#include "Arduino.h"
#include "HardwareSerial.h"
#include "SoftwareSerial.h"
#include "my_serial.h"
#include "my_esp_serial.h"

#define DEBUG 1

MySerial *serial;

// Now what we need is a TELNET communication
class MyTelnetCommunicator {
  // We'll need a ESP serial communicator
  MyEspSerial esp_serial;

  void command(const char *command) {
    auto result = esp_serial.send(command);

#ifdef DEBUG
    // Check if result ends with "OK" string
    auto is_ok = result.endsWith("OK");

    if (is_ok) {
      serial->send(String(command) + " OK");
    } else { 
      serial->send(String("Command") + command + " failed");
      serial->send(result);
    }
#endif
  }

  public:
  MyTelnetCommunicator() {
    command("AT");
    // Set to STAtion mode (we want to connect)
    command("AT+CWMODE=1");

    // Connect to WiFi
    command("AT+CWJAP=\"Top Kek's amazing WIFI\",\"Solo322Dota1Love\"");

    // Set wifi address
    command("AT+CIPSTA=\"192.168.0.111\"");

    // Enable multiple connections
    command("AT+CIPMUX=1");

    // Accept at port 80
    command("AT+CIPSERVER=1,80");
  }

  String check_message() {
    return esp_serial.check_message();
  }
};

MyTelnetCommunicator *esp;

void setup() {
  serial = new MySerial();
  esp = new MyTelnetCommunicator();

  serial->send("Setup complete");
}

void loop() {
  auto message = esp->check_message();
  if (message.length() != 0) serial->send(message);
}
