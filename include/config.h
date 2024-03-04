#ifndef CONFIG_H
#define CONFIG_H

#ifdef M5STACK_CORE2
#include <tft_io.hpp>
#include <ili9341.hpp>
#include <m5core2_power.hpp>
static m5core2_power power;
#define LCD
#define LCD_SPI_HOST    VSPI
#define LCD_DMA
#define LCD_BCKL_ON_LEVEL 1
#define LCD_BCKL_OFF_LEVEL !LCD_BCKL_ON_LEVEL
#define LCD_PIN_NUM_MOSI 23
#define LCD_PIN_NUM_CLK 18
#define LCD_PIN_NUM_CS 5
#define LCD_PIN_NUM_DC 15
#define LCD_PANEL esp_lcd_new_panel_ili9342
#define LCD_HRES 320
#define LCD_VRES 240
#define LCD_COLOR_SPACE ESP_LCD_COLOR_SPACE_BGR
#define LCD_PIXEL_CLOCK_HZ (40 * 1000 * 1000)
#define LCD_GAP_X 0
#define LCD_GAP_Y 0
#define LCD_MIRROR_X false
#define LCD_MIRROR_Y false
#define LCD_INVERT_COLOR true
#define LCD_SWAP_XY false
using tft_bus_t = arduino::tft_spi_ex<LCD_SPI_HOST,LCD_PIN_NUM_CS,LCD_PIN_NUM_MOSI,-1,LCD_PIN_NUM_CLK,0,false>;
using panel_t = arduino::ili9342c<LCD_PIN_NUM_DC,-1,-1,tft_bus_t,1>;
static panel_t panel;
#endif // M5STACK_CORE2
#ifdef TTGO_T1
#include <tft_io.hpp>
#include <st7789.hpp>
#define LCD
#define LCD_SPI_HOST    VSPI
#define LCD_DMA
#define LCD_BCKL_ON_LEVEL 1
#define LCD_BCKL_OFF_LEVEL !LCD_BCKL_ON_LEVEL
#define LCD_PIN_NUM_MOSI 19
#define LCD_PIN_NUM_CLK 18
#define LCD_PIN_NUM_CS 5
#define LCD_PIN_NUM_DC 16
#define LCD_PIN_NUM_RST 23
#define LCD_PIN_NUM_BCKL 4
#define LCD_PANEL esp_lcd_new_panel_st7789
#define LCD_HRES 135
#define LCD_VRES 240
#define LCD_COLOR_SPACE ESP_LCD_COLOR_SPACE_RGB
#define LCD_PIXEL_CLOCK_HZ (40 * 1000 * 1000)
#define LCD_GAP_X 40
#define LCD_GAP_Y 52
#define LCD_MIRROR_X false
#define LCD_MIRROR_Y true
#define LCD_INVERT_COLOR true
#define LCD_SWAP_XY true
using tft_bus_t = arduino::tft_spi_ex<LCD_SPI_HOST,LCD_PIN_NUM_CS,LCD_PIN_NUM_MOSI,-1,LCD_PIN_NUM_CLK,0,false>;
using panel_t = arduino::st7789<LCD_HRES,LCD_VRES, LCD_PIN_NUM_DC,LCD_PIN_NUM_RST,LCD_PIN_NUM_BCKL,tft_bus_t,0>;
static panel_t panel;
#endif // TTGO_T1
#ifdef ESP32S3_NEO
#include "neopixel_panel.hpp"
#include <rmt_led_strip.hpp>
#define LED
#if __has_include(<Arduino.h>)
// the type of neopixes. can be ws2812, sk6812 or apa106
#define LED_TYPE arduino::ws2812
#else
#define LED_TYPE espidf::ws2812
#endif
// The data pin for neopixel display
#define LED_PIN 18
// the physical horizontal resolution of the display
#define LED_HRES 8
// the physical vertical resolution of the display
#define LED_VRES 32
// 1 = swap x and y coordinates. width becomes height and vice versa
#define LED_SWAP_XY 0
// 1 = the LEDS are laid out in a single strip in series such that every other row's columns are inverted
#define LED_SNAKE_LAYOUT 1
// 1 = flip the x coordinates
#define LED_MIRROR_X 0
// 1 = flip the y coordinates
#define LED_MIRROR_Y 0
using panel_t = neopixel_panel;
LED_TYPE leds(LED_PIN,LED_HRES*LED_VRES);
panel_t panel(leds,LED_HRES,LED_SWAP_XY,LED_MIRROR_X,LED_MIRROR_Y,LED_SNAKE_LAYOUT);
#endif
#ifdef WIO_TERMINAL
#include <tft_spi.hpp>
#include <ili9341.hpp>
#define LCD
#define LCD_VRES 320
#define LCD_HRES 240
using bus_t = arduino::tft_spi_ex<3,LCD_SS_PIN,SPI_MODE0>;
using panel_t = arduino::ili9341<LCD_DC,LCD_RESET,LCD_BACKLIGHT,bus_t,3,true,400,200>;
static panel_t panel;
#endif // WIO_TERMINAL

#ifdef LCD
#ifndef LCD_WIDTH
#ifdef LCD_SWAP_XY
#if LCD_SWAP_XY
#define LCD_WIDTH LCD_VRES
#define LCD_HEIGHT LCD_HRES
#else
#define LCD_WIDTH LCD_HRES
#define LCD_HEIGHT LCD_VRES
#endif
#else
#define LCD_WIDTH LCD_HRES
#define LCD_HEIGHT LCD_VRES
#endif
#endif
#ifndef LCD_BIT_DEPTH
#define LCD_BIT_DEPTH 16
#endif
#ifndef LCD_X_ALIGN
#define LCD_X_ALIGN 1
#endif
#ifndef LCD_Y_ALIGN
#define LCD_Y_ALIGN 1
#endif
#ifndef LCD_FRAME_ADAPTER
#define LCD_FRAME_ADAPTER gfx::bitmap<gfx::rgb_pixel<LCD_BIT_DEPTH>>
#endif
#ifndef LCD_DC_BIT_OFFSET
#define LCD_DC_BIT_OFFSET 0
#endif
#endif
#ifdef LED
#if LED_SWAP_XY
#define LED_WIDTH LED_VRES
#define LED_HEIGHT LED_HRES
#else
#define LED_WIDTH LED_HRES
#define LED_HEIGHT LED_VRES
#endif
#define LED_COUNT (LED_WIDTH*LED_HEIGHT)
#endif
#endif // CONFIG_H