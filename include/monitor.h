#ifndef MONITOR_H
#define MONITOR_H

#include <Arduino.h>

#include <messages.h>

namespace vdn {
    namespace monitor {
        void init(int64_t baudrate=115200) {
            Serial.begin(baudrate);
            
            Serial.printf("Serial-порт инициализирован с частотой %lld baud", baudrate);
        }

        void interpretePacket(const messages::packet & data) {
            switch (data.TYPE) {
            case messages::GENERAL_MESSAGE:
                Serial.printf("Получено сообщение для общего чата\nОтправитель: ");
                break;

            case messages::DIRECT_MESSAGE:
                Serial.printf("Получено личное сообщение от ");
                break;

            case messages::CHANNEL_MESSAGE:
                Serial.printf("Получен новый пост в канале \"");
                break;
            
            default:
                break;
            }

            Serial.printf("%s\n", data.MSG.username);

            Serial.printf("Текст сообщения: %s\n", data.MSG.text);
        }
    }
}

#endif
