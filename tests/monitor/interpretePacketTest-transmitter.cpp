#include <messages.h>

void setup() {
    vdn::messages::message msg = {
        .username = {"vdn"},
        .text = {"Hello, General!"}
    };

    vdn::messages::sendGeneral(msg);
}

void loop() {}

// Отправлено сообщение для общего чата
