
#include "HardwareSerial.h"

class MySerial {
public:
  MySerial(int baud_rate = 9600);

  void send(const String &message) const;

  String read() const;
};
