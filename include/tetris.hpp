#include <stddef.h>
#include <stdint.h>
#include <gfx.hpp>
#include <math.h>
using namespace data;

template<typename T>
using piece_palette_type = gfx::ega_palette<T>;
using piece_pixel_type = gfx::indexed_pixel<4>;
class piece {
    uint8_t m_data[2];
    piece_pixel_type m_color;
    gfx::size16 m_dimensions;
    gfx::point16 m_location;
    void do_copy(const piece& rhs);
public:
    piece(const piece& rhs);
    piece& operator=(const piece& rhs);
    piece();
    using data_type = gfx::bitmap<gfx::gsc_pixel<1>>;
    using const_data_type = gfx::const_bitmap<typename data_type::pixel_type>;
    
    gfx::point16 location() const;
    void location(gfx::point16 location);
    gfx::size16 dimensions() const;
    gfx::rect16 bounds() const;
    piece_pixel_type color() const;
    void color(piece_pixel_type color);
    bool hit_test(gfx::point16 location, bool local = false) const;
    const const_data_type source_bitmap() const;
    void rotate_left();
    void rotate_right();
    static bool create(const data_type& bmp, piece* out_result);
    static piece create(const size_t index);
    static piece create_L();
    static piece create_J();
    static piece create_T();
    static piece create_S();
    static piece create_Z();
    static piece create_O();
    static piece create_I();
};
namespace {
template<typename PixelType>
class board {
    uint8_t* m_data;
    gfx::size16 m_dimensions;
    
    piece_palette_type<PixelType> m_palette;
public:
    using pixel_type = PixelType;
    using palette_type = piece_palette_type<PixelType>;
    using data_type = gfx::bitmap<piece_pixel_type,palette_type>;
    using const_data_type = gfx::const_bitmap<piece_pixel_type,palette_type>;
    board() : m_data(nullptr), m_dimensions(0,0) {

    }
    const piece_palette_type<PixelType>* palette() const {
        return &m_palette;
    }
    gfx::size16 dimensions() const {
        return m_dimensions;
    }
    const_data_type source_bitmap() const {
        return const_data_type(m_dimensions,m_data,&m_palette);
    }
    void dimensions(gfx::size16 dimensions) {
        if(dimensions.width==0||dimensions.height==0) {
            if(m_data!=nullptr) {
                free(m_data);
                m_data = nullptr;
            }
            m_dimensions = dimensions;
            return;
        }
        m_data = (uint8_t*)realloc(m_data,data_type::sizeof_buffer(dimensions));
        if(m_data==nullptr) {
            m_dimensions = gfx::size16::min();
            return;
        }
        data_type bmp(dimensions,m_data,&m_palette);
        bmp.clear(bmp.bounds());
        m_dimensions = dimensions;
    }
    bool hit_test(gfx::point16 location) const {
        const_data_type bmp(m_dimensions,m_data,&m_palette);
        typename const_data_type::pixel_type px;
        bmp.point(location,&px);
        return px.template channel<0>()!=0;
    }
    bool hit_test(const piece& p) const {
        auto ico = p.source_bitmap();
        gfx::point16 loc = p.location();
        for(int y = 0;y<ico.dimensions().height;++y) {
            for(int x = 0;x<ico.dimensions().width;++x) {
                gfx::point16 spt(x,y);
                gfx::point16 dpt = spt.offset((gfx::spoint16)loc);
                typename piece::data_type::pixel_type px;
                ico.point(spt,&px);
                if(px.template channel<0>()) {
                    if(hit_test(dpt)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    void clear() {
        data_type bmp(m_dimensions,m_data,&m_palette);
        bmp.clear(bmp.bounds());
    }
    void add_piece(const piece& p) {
        if(m_dimensions.width==0||m_dimensions.height==0 || m_data==nullptr) {
            return;
        }
        auto ico = p.source_bitmap();
        gfx::point16 loc = p.location();
        auto col = p.color();
        data_type bmp(m_dimensions,m_data,&m_palette);
        for(int y = 0;y<ico.dimensions().height;++y) {
            for(int x = 0;x<ico.dimensions().width;++x) {
                gfx::point16 spt(x,y);
                gfx::point16 dpt = spt.offset((gfx::spoint16)loc);
                typename piece::data_type::pixel_type px;
                ico.point(spt,&px);
                if(px.native_value) {
                    bmp.point(dpt,col);
                }
            }
        }
    }
    bool remove_next_full_row() {
        auto ico = source_bitmap();
        piece_pixel_type px;
        int yy=-1;
        for(int y = m_dimensions.height-1;y>=0;--y) {
            bool full = true;
            for(int x = 0;x<m_dimensions.width;++x) {
                gfx::point16 spt(x,y);
                ico.point(spt,&px);
                if(!px.template channel<0>()) {
                    full = false;
                    break;
                }
            }
            if(full) {
                yy=y;
                break;
            }
        }
        if(0>yy) {
            return false;
        }
        data_type bmp(m_dimensions,m_data,&m_palette);
        for(int y =yy;y>=0;--y) {
            if(y>0) {
                for(int x = 0;x<m_dimensions.width;++x) {
                    bmp.point(gfx::point16(x,y-1),&px);
                    bmp.point(gfx::point16(x,y),px);
                }
            } else {
                bmp.clear(gfx::rect16(0,y,m_dimensions.width-1,y));
            }
        }
        return true;
    }
};
template<typename Destination, typename PixelType>
void draw_square(Destination& destination, const gfx::rect16& bounds, PixelType col) {
    // get some x11 colors in HSL 24 bit
    using x11 = gfx::color<gfx::hsl_pixel<24>>;
    // get black and white and convert them to the target pixel type
    constexpr static const PixelType white = gfx::convert<gfx::hsl_pixel<24>,PixelType>(x11::white);
    constexpr static const PixelType black = gfx::convert<gfx::hsl_pixel<24>,PixelType>(x11::black);
    const gfx::rect16 b = bounds.normalize();
    // if our dimensions are small enough, just do a simple fill
    if((b.x2-b.x1+1)<3 || (b.y2-b.y1+1)<3) {
        destination.fill(b,col);
        return;
    }
    // otherwise, draw a 3d tile
    const gfx::rect16 rb = b.inflate(-1,-1);
    destination.fill(rb,col);
    // make the lighter color
    PixelType px2 = col.blend(white,0.5f);
    destination.fill(gfx::rect16(b.x1,b.y1,b.x2-1,b.y1),px2);
    destination.fill(gfx::rect16(b.x2,b.y1,b.x2,b.y2-1),px2);
    // make the darker color
    px2 = col.blend(black,0.5f);
    destination.fill(gfx::rect16(b.x2,b.y2,b.x1+1,b.y2),px2);
    destination.fill(gfx::rect16(b.x1,b.y2,b.x1,b.y1+1),px2);
}
}
template<typename PixelType>
class tetris {
public:
    using board_type = board<PixelType>;
private:
    constexpr static const uint32_t drop_ms = 50;
    board_type m_board;
    bool m_dropping;
    piece m_current;
    piece m_next;
    gfx::size16 m_dimensions;
    gfx::size16 m_square_dimensions;
    size_t m_rows_cleared;
    uint32_t m_advance_time;
    uint32_t m_advance_ts;
    uint32_t m_dropping_ts;
    bool m_running;
    bool m_dirty;
    static gfx::size16 square_dimensions_from_screen_dimensions(gfx::size16 screen_dimensions) {
        int w = screen_dimensions.width<screen_dimensions.height?screen_dimensions.width:screen_dimensions.height;
        int target = w/10;
        if(target==0) {
            target = 1;
        }
        return gfx::size16(target,target);
    }
    static gfx::size16 game_dimensions_from_screen_dimensions(gfx::size16 screen_dimensions) {
        int w = screen_dimensions.width<screen_dimensions.height?screen_dimensions.width:screen_dimensions.height;
        int target = w/10;
        if(target==0) {
            target = 1;
        }
        return gfx::size16(w/target,screen_dimensions.height/target);
    }
    bool advance() {
        if(!move_down()) {
            bool done = m_current.location().y==0;
            m_board.add_piece(m_current);
            m_dirty = true;
            next();
            return !done;
        }
        bool recomp_rows = false;
        while(m_board.remove_next_full_row()) {
             recomp_rows = true;
             ++m_rows_cleared;
        }
        if(recomp_rows) {
            int rc = m_rows_cleared>100?100:m_rows_cleared;
            if(rc==0) {
                rc=1;
            }
            m_advance_time = 10 * (100-rc);
        }
        return true;
    }
    void next() {
        m_current = m_next;
        m_dirty = true;
        m_next = piece::create(rand());
        int i=0;
        while(i==0 || i==7 || i==15) {
            i = rand()%16;
        }
        piece_pixel_type pxi(i);
        m_next.color(pxi);
        m_next.location(gfx::point16((m_board.dimensions().width-m_next.dimensions().width)/2,0));
    }
public:
    tetris() : m_dropping(false), m_advance_time(0),m_advance_ts(0),m_running(false),m_dirty(false) {
    }
    gfx::size16 dimensions() const { return m_dimensions; }
    void dimensions(gfx::size16 dimensions) { m_dimensions = dimensions; m_board.dimensions(game_dimensions_from_screen_dimensions(dimensions)); m_square_dimensions = square_dimensions_from_screen_dimensions(dimensions); restart(); }
    gfx::size16 board_dimensions() const {
        return m_board.dimensions();
    }
    void restart() {
        m_board.clear();
        // shuffle
        next();
        next();
        m_advance_time = 1000;
        m_advance_ts = millis();
        m_running = true;
        m_dirty = true;
        m_rows_cleared = 0;
    }
    uint32_t advance_time() const {
        return m_advance_time;
    }
    void advance_time(uint32_t advance_time_ms) {
        m_advance_time = advance_time_ms;
    }
    size_t rows_cleared() const {
        return m_rows_cleared;
    }
    piece& current_piece() {
        return m_next;
    }
    piece& next_piece() {
        return m_next;
    }
    bool running() const {
        return m_running;
    }
    void running(bool running) {
        if(running) {
            if(!m_running && m_advance_ts==0) {
                restart();
                return;
            }
        }
        m_running = running;
    }
    bool move_right() {
        const uint16_t w = m_board.dimensions().width;
        if(m_current.bounds().x2+1>=w) {
            return false;
        }
        m_current.location(m_current.location().offset(1,0));
        if(m_board.hit_test(m_current)) {
            // undo it
            m_current.location(m_current.location().offset(-1,0));
            return false;
        }
        m_dirty = true;
        return true;
    }
    bool move_left() {
        if(m_current.location().x==0) {
            return false;
        }
        m_current.location(m_current.location().offset(-1,0));
        if(m_board.hit_test(m_current)) {
            // undo it
            m_current.location(m_current.location().offset(1,0));
            return false;
        }
        m_dirty = true;
        return true;
    }
    bool move_down() {
        const uint16_t h = m_board.dimensions().height;
        if(m_current.bounds().y2+1>=h) {
            return false;
        }
        m_current.location(m_current.location().offset(0,1));
        if(m_board.hit_test(m_current)) {
            // undo it
            m_current.location(m_current.location().offset(0,-1));
            return false;
        }
        m_dirty = true;
        return true;
    }
    void drop() {
        m_dropping = true;
        m_dropping_ts = millis();
    }
    bool needs_draw() const {
        return m_dirty;
    }
    bool rotate_left() {
        m_current.rotate_left();
        if(m_board.hit_test(m_current)) {
            // undo it
            m_current.rotate_right();
            return false;
        }
        gfx::point16 loc=m_current.location();
        if(m_current.bounds().x2>=m_board.dimensions().width) {
            loc.x =m_board.dimensions().width-m_current.dimensions().width;
        }
        if(m_current.bounds().y2>=m_board.dimensions().height) {
            m_current.rotate_left();
            return false;
        }
        m_current.location(loc);
        m_dirty = true;
        return true;
    }
    bool rotate_right() {
        m_current.rotate_right();
        if(m_board.hit_test(m_current)) {
            // undo it
            m_current.rotate_left();
            return false;
        }
        gfx::point16 loc=m_current.location();
        if(m_current.bounds().x2>=m_board.dimensions().width) {
            loc.x =m_board.dimensions().width-m_current.dimensions().width;
        }
        if(m_current.bounds().y2>=m_board.dimensions().height) {
            m_current.rotate_right();
            return false;
        }
        m_current.location(loc);
        m_dirty = true;
        return true;
    }
    void update() {
        uint32_t ms = millis();
        if(m_running) {
            if(m_dropping && ms>m_dropping_ts+drop_ms) {
                m_dropping_ts = millis();
                if(!move_down()) {
                    m_dropping = false;
                    m_dropping_ts = 0;
                }
            }
            if(ms>m_advance_ts+m_advance_time) {
                m_advance_ts = ms;
                if(!advance()) {
                    m_running = false;
                }
            }
        } else {
            m_advance_ts = ms;
        }
    }
    template<typename Destination>
    void draw(Destination& destination, gfx::point16 location) {
        auto ico = m_current.source_bitmap();
        auto brd = m_board.source_bitmap();
        typename Destination::pixel_type ccol;
        m_board.palette()->map(m_current.color(),&ccol);
        for(int y = 0;y<dimensions().height;++y) {
            for(int x = 0;x<dimensions().width;++x) {
                const gfx::point16 bpt(x,y);
                const gfx::rect16 rsq(gfx::point16(x*m_square_dimensions.width,
                                                y*m_square_dimensions.height),
                                            m_square_dimensions);
                if(m_current.hit_test(bpt)) {
                    draw_square(destination,rsq,ccol);
                } else {
                    piece_pixel_type bcol;
                    brd.point(bpt,&bcol);
                    if(bcol==piece_pixel_type()) {
                        destination.fill(rsq,typename Destination::pixel_type());
                    } else {
                        typename Destination::pixel_type scol;
                        m_board.palette()->map(bcol,&scol);
                        draw_square(destination,rsq,scol);
                    }
                }
            }
        }
        m_dirty = false;
    }
};


