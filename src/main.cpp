#include <Arduino.h>
#include <gfx.hpp>
#include "config.h"
#include "interface.h"
#ifdef M5STACK_CORE2
#include "m5core2_power.hpp"
static m5core2_power power;
#endif
#include "tetris.hpp"
using namespace gfx;

using tetris_t = tetris<typename panel_t::pixel_type>;
tetris_t game;
void setup()
{
    Serial.begin(115200);
#ifdef M5STACK_CORE2
    power.initialize();
#endif
    panel.initialize();
    game.dimensions(panel.dimensions());
    game.restart();
}

void loop()
{
    static bool isconnected = false;
    static uint32_t watchdog_ts = 0;
    int cmd = Serial.read();
    if(-1!=cmd) {
        isconnected = true;
        watchdog_ts = millis();
        switch((CMD_ID)cmd) {
            case CMD_MOVE_LEFT:
                game.move_left();
                break;
            case CMD_MOVE_RIGHT:
                game.move_right();
                break;
            case CMD_ROTATE_LEFT:
                game.rotate_left();
                break;
            case CMD_ROTATE_RIGHT:
                game.rotate_right();
                break;
            case CMD_DROP:
                game.drop();
                break;
            default:
                break;
        }
    }
    if(watchdog_ts!=0 && millis()>watchdog_ts+1000) {
        watchdog_ts = 0;
        isconnected = false;
    }
    
    if(game.needs_draw()) {    
        draw::suspend(panel);
        game.draw(panel,point16::zero());
        draw::resume(panel);
    }
    if(!isconnected) {
        static uint32_t ts = 0;
        static bool delta = true;
        if(millis()>ts+game.advance_time()/2) {
            ts = millis();
            if(delta) {
                if(!game.move_right()) {
                    delta = false;
                    game.move_left();
                }
                game.rotate_left();
            } else {
                if(!game.move_left()) {
                    delta = true;
                    game.move_right();
                }
                game.rotate_right();
            }
        }
    }
    game.update();
    if(!game.running()) {
        game.restart();
    }
    //delay(250);
}
