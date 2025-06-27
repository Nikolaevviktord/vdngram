#ifndef MESSAGES_H
#define MESSAGES_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <map>
#include <vector>

#include <structs.h>
#include <monitor.h>

namespace vdn {
    namespace messages {
        std::map<MACad, uName> users;
        uName localUsername = {"user"};

        std::vector<message> general;
        std::map<uName, std::vector<message>> direct;
        std::map<uName, std::vector<mText>> channels;

        void recieve(
            const esp_now_recv_info_t *info,
            const uint8_t *data,
            int size
        );

        void init() {
            WiFi.mode(WIFI_STA);
            esp_now_init();
            esp_now_register_recv_cb(receive);
        }

        void sendGeneral(message & msg) {
            packet data = {
                .TYPE = GENERAL_MESSAGE,
                .MSG = msg
            };

            esp_now_peer_info_t peer = {};
            MACad broadcast_address = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
            std::copy(broadcast_address.begin(), broadcast_address.end(), peer.peer_addr);
            esp_now_add_peer(&peer);

            esp_err_t result = esp_now_send(
                peer.peer_addr,
                (uint8_t * ) &data,
                sizeof(packet)
            );
            
            if (result != ESP_OK) { 
                Serial.println(esp_err_to_name(result)); 
            }

            general.push_back(msg);

            vdn::monitor::printStr("Отправлено сообщение для общего чата");
        }

        void sendDirect(MACad & recipient, message & msg) {
            packet data = {
                .TYPE = DIRECT_MESSAGE,
                .MSG = msg
            };

            uint8_t target_mac[6];
            std::copy(recipient.begin(), recipient.end(), target_mac);

            esp_now_peer_info_t peer = { 0 };
            memcpy(peer.peer_addr, target_mac, sizeof(peer.peer_addr));
            esp_now_add_peer(&peer);

            esp_err_t result = esp_now_send(
                target_mac,
                (uint8_t * ) &data,
                sizeof(packet)
            );

            if (result != ESP_OK) { 
                Serial.println(esp_err_to_name(result)); 
            }

            direct[msg.username].push_back(msg);

            vdn::monitor::printStr("Отправлено личное сообщение");
        }

        void sendChanel(message & msg) {
            packet data = {
                .TYPE = CHANNEL_MESSAGE,
                .MSG = msg
            };

            esp_now_peer_info_t peer = {};
            MACad broadcast_address = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
            std::copy(broadcast_address.begin(), broadcast_address.end(), peer.peer_addr);
            esp_now_add_peer(&peer);

            esp_err_t result = esp_now_send(
                peer.peer_addr,
                (uint8_t * ) &data,
                sizeof(packet)
            );

            if (result != ESP_OK) { 
                Serial.println(esp_err_to_name(result)); 
            }

            channels[msg.username].push_back(msg.text);

            vdn::monitor::printStr("Отправлен пост в канал");
        }

        void recieve(
            const esp_now_recv_info_t *info,
            const uint8_t *data,
            int size
        ) {
            if (size != sizeof(packet)) { 
                return; 
            }

            const auto &decoded = (const packet *) data;

            switch (decoded->TYPE) {
            case GENERAL_MESSAGE:
                general.push_back(decoded->MSG);
                break;

            case DIRECT_MESSAGE:
                direct[decoded->MSG.username].push_back(decoded->MSG);
                break;

            case CHANNEL_MESSAGE:
                channels[decoded->MSG.username].push_back(decoded->MSG.text);
                break;
            
            default:
                break;
            }

            vdn::monitor::interpretePacket(decoded);
        }
    }
}

#endif
