#include <Arduino.h>
#include <gfx.hpp>
#include "config.h"
#include "interface.h"
#include "tetris.hpp"
// import the htcw_gfx primary namespace
using namespace gfx;

// create a tetris board that can draw to our panel
using tetris_t = tetris<typename panel_t::pixel_type>;
tetris_t game;
void setup()
{
    Serial.begin(115200);
#ifdef M5STACK_CORE2
    // m5 stack core2 requires
    // power management chip
    // to be tickled
    power.initialize();
#endif
    panel.initialize();
    // set up the game and start it
    game.dimensions(panel.dimensions());
    game.restart();
}

void loop()
{
    // timeout value for connection detection
    static uint32_t watchdog_ts = 0;
    // any serial data?
    int cmd = Serial.read();
    if(-1!=cmd) { // process it
        // start/restart the connection watchdog
        watchdog_ts = millis();
        // move according to serial input
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
    // if we haven't had serial input for 1 second
    // go back to automated mode
    if(watchdog_ts!=0 && millis()>watchdog_ts+1000) {
        watchdog_ts = 0;
    }
    // check if the game board is dirty
    if(game.needs_draw()) {
        // if so, suspend all display
        // (only does anything on supporting devices)
        draw::suspend(panel);
        // draw the game at (0,0)
        game.draw(panel,point16::zero());
        // resume display
        draw::resume(panel);
    }
    // if not connected, just move 
    // back and forth and rotate
    if(!watchdog_ts) {
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
    // pump the game loop
    game.update();
    // if it's not running anymore
    // that means game over
    if(!game.running()) {
        // just restart it
        game.restart();
    }
}
