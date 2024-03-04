#pragma once
#include <stddef.h>
#include <stdint.h>
#include <gfx_core.hpp>
#include <gfx_pixel.hpp>
#include <gfx_positioning.hpp>
#include <rmt_led_strip.hpp>

class neopixel_panel {
    bool m_swap_xy;
    bool m_mirror_x;
    bool m_mirror_y;
    bool m_snake_layout;
    uint16_t m_width;
    int m_suspend_count;
#ifdef ARDUINO
    using led_strip_type = arduino::led_strip;
    
#else
    using led_strip_type = espidf::led_strip;
#endif
    led_strip_type* m_leds;
    size_t compute_index(gfx::point16 location) const {
        int x = location.x;
        int y = location.y;
        if(m_swap_xy) {
            int tmp = x;
            x = y;
            y = tmp;
        }
        if(m_snake_layout && m_swap_xy==(y&1)) x = m_width-x-1;
        if(m_swap_xy) {
            if(m_mirror_y) {
                x = m_width-x-1;
            }
            if(m_mirror_x) {
                y = (m_leds->length()/m_width)-y-1;
            }
        } else {
            if(m_mirror_y) {
                y = (m_leds->length()/m_width)-y-1;
            }
            if(m_mirror_x) {
                x = m_width-x-1;
            }
        }
        
        
        return y*m_width+x;
    }
public:
    using pixel_type = gfx::rgb_pixel<24>;
    using caps = gfx::gfx_caps<false,false,false,false,true,true,false>;
    neopixel_panel(led_strip_type& leds,uint16_t width=0, bool swap_xy = false, bool mirror_x = false, bool mirror_y = false, bool snake_layout = false) : m_swap_xy(swap_xy), m_mirror_x(mirror_x), m_mirror_y(mirror_y), m_snake_layout(snake_layout), m_width(width==0?(uint16_t)leds.length():width), m_suspend_count(0),m_leds(&leds) {
    }
    bool initialize() {
        return m_leds->initialize();
    }
    bool initialized() const {
        return m_leds->initialized();
    }
    gfx::size16 dimensions() const {
        return m_swap_xy?gfx::size16(m_leds->length()/m_width,m_width):gfx::size16(m_width,m_leds->length()/m_width);
    }
    gfx::rect16 bounds() const {
        return dimensions().bounds();
    }
    gfx::gfx_result point(gfx::point16 location, pixel_type col) {
        if(!initialize()) {
            return gfx::gfx_result::device_error;
        }
        if(!this->bounds().intersects(location)) {
            return gfx::gfx_result::success;
        }
        const auto r = col.channel<0>();
        const auto g = col.channel<1>();
        const auto b = col.channel<2>();
        //const auto w = col.channel<3>();
        m_leds->color(compute_index(location),r,g,b);
        if(!m_suspend_count) {
            m_leds->update();
        }
        return gfx::gfx_result::success;
    }
    gfx::gfx_result point(gfx::point16 location, pixel_type* out_col) const {
        if(!initialized()) {
            return gfx::gfx_result::invalid_state;
        }
        if(!this->bounds().intersects(location)) {
            *out_col = pixel_type();
            return gfx::gfx_result::success;
        }
        const uint32_t col = m_leds->color(compute_index(location));
        // (w << 24) | (r << 16) | (g << 8) | b;
        //const uint8_t w = (col >> 24)&0xFF;
        const uint8_t r = (col >> 16)&0xFF;
        const uint8_t g = (col >> 8)&0xFF;
        const uint8_t b = col&0xFF;
        *out_col = pixel_type(r,g,b);
        return gfx::gfx_result::success;
    }
    gfx::gfx_result fill(const gfx::rect16& bounds,pixel_type col) {
        if(!initialize()) {
            return gfx::gfx_result::device_error;
        }
        if(!bounds.intersects(this->bounds())) {
            return gfx::gfx_result::success;
        }
        const gfx::rect16 bnds = bounds.crop(bounds.normalize());
        const auto r = col.channel<0>();
        const auto g = col.channel<1>();
        const auto b = col.channel<2>();
        //const auto w = col.channel<3>();
        for(int y = bnds.y1;y<=bnds.y2;++y) {
            for(int x = bnds.x1;x<=bnds.x2;++x) {
                m_leds->color(compute_index(gfx::point16(x,y)),r,g,b);
            }
        }
        if(!m_suspend_count) {
            m_leds->update();
        } 
        return gfx::gfx_result::success;
    }
    gfx::gfx_result clear(const gfx::rect16& bounds) {
        return fill(bounds,pixel_type());
    }
    gfx::gfx_result suspend() {
        if(!initialize()) {
            return gfx::gfx_result::device_error;
        }
        ++m_suspend_count;
        return gfx::gfx_result::success;
    }
    gfx::gfx_result resume(bool force = false) {
        if(!initialize()) {
            return gfx::gfx_result::device_error;
        }
        if(m_suspend_count==1 || force) {
            m_suspend_count=0;
            m_leds->update();
        } else if(m_suspend_count>0) {
            --m_suspend_count;
        }
        return gfx::gfx_result::success;
    }
};