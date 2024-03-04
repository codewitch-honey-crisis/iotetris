#ifndef INTERFACE_H
#define INTERFACE_H
#include <stdint.h>
enum CMD_ID : uint8_t {
    CMD_MOVE_LEFT = 1,
    CMD_MOVE_RIGHT = 2,
    CMD_ROTATE_LEFT = 3,
    CMD_ROTATE_RIGHT = 4,
    CMD_DROP = 5,
    CMD_PING = 6
};
#endif