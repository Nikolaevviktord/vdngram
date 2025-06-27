#ifndef STRUCTS_H
#define STRUCTS_H

using uName = std::array<char, 8>;
using mText = std::array<char, 128>;
using MACad = std::array<uint8_t, 6>;

namespace vdn {
    namespace messages {
        const int8_t GENERAL_MESSAGE = 0;
        const int8_t DIRECT_MESSAGE = 1;
        const int8_t CHANNEL_MESSAGE = 2;

        const bool me = 0;
        const bool other = 1;
      
        struct message {
            uName username;
            mText text;
        };
      
        struct packet {
            int8_t TYPE;
            message MSG;
        };
    }
}

#endif
