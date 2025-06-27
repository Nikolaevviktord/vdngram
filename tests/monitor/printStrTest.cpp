#include <monitor.h>

void setup() {
  vdn::monitor::printStr("String1");
  vdn::monitor::printStr("String2");
}

void loop() {}

// Serial-порт инициализирован с частотой 115200 baud
// String1
// String2
