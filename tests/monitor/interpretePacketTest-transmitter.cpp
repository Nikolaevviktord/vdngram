#include <messages.h>

void setup() {
    vdn::message msg = {
        .username = {"vdn"},
        .text = {"Hello, General!"}
    };

    vdn::messages::sendGeneral(msg);
}

void loop() {}
