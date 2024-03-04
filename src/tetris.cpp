#include "tetris.hpp"

using namespace gfx;
using namespace data;
static const typename piece::data_type::pixel_type piece_set(true,1.0f);
void piece::do_copy(const piece& rhs) {
    memcpy(m_data,rhs.m_data,sizeof(m_data));
    m_dimensions = rhs.m_dimensions;
    m_location = rhs.m_location;
    m_color = rhs.m_color;
}
piece::piece(const piece& rhs) {
    do_copy(rhs);
}
piece& piece::operator=(const piece& rhs) {
    do_copy(rhs);
    return *this;
}
piece::piece() {
    m_dimensions = size16::min();
    m_location = point16::zero();
    m_color = piece_pixel_type();
    memset(m_data,0,sizeof(m_data));
}
point16 piece::location() const {
    return m_location;
}
void piece::location(point16 location) {
    m_location = location;
}
size16 piece::dimensions() const {
    return m_dimensions;
}
rect16 piece::bounds() const {
    return rect16(m_location,m_dimensions);
}
bool piece::hit_test(point16 location, bool local) const {
    if(!local) {
        location.offset_inplace(-m_location.x,-m_location.y);
    }
    if(!m_dimensions.bounds().intersects(location)) {
        return false;
    }
    typename data_type::pixel_type px;
    data_type::point(m_dimensions,m_data,location,&px);
    return px.native_value!=0;
}
const piece::const_data_type piece::source_bitmap() const {
    const_data_type result(m_dimensions,m_data);
    return result;
}
piece_pixel_type piece::color() const {
    return m_color;
}
void piece::color(piece_pixel_type color) {
    m_color = color;
}
bool piece::create(const data_type& bmp, piece* out_result) {
    if(out_result==nullptr) {
        return false;
    }
    if(bmp.dimensions().area()>8*sizeof(m_data)) {
        return false;
    }
    memcpy(out_result->m_data,bmp.begin(),data_type::sizeof_buffer(bmp.dimensions()));
    out_result->m_dimensions = bmp.dimensions();
    out_result->m_location = point16::zero();
    out_result->m_color = piece_pixel_type();
    return true;
}
piece piece::create(size_t index) {
    switch(index%7) {
        case 1:
            return create_J();
        case 2:
            return create_T();
        case 3:
            return create_S();
        case 4:
            return create_Z();
        case 5:
            return create_O();
        case 6:
            return create_I();
        default: // 0
            return create_L();
    }
}

piece piece::create_L() {
    piece result;
    result.m_location = point16::zero();
    result.m_dimensions = {2,3};
    data_type bmp(result.m_dimensions,result.m_data);
    bmp.clear(bmp.bounds());
    bmp.point({1,0},piece_set);
    bmp.point({1,1},piece_set);
    bmp.point({0,2},piece_set);
    bmp.point({1,2},piece_set);
    return result;
}
piece piece::create_J() {
    piece result;
    result.m_location = point16::zero();
    result.m_dimensions = {2,3};
    data_type bmp(result.m_dimensions,result.m_data);
    bmp.clear(bmp.bounds());
    bmp.point({0,0},piece_set);
    bmp.point({0,1},piece_set);
    bmp.point({0,2},piece_set);
    bmp.point({1,2},piece_set);
    return result;
}

piece piece::create_T() {
    piece result;
    result.m_location = point16::zero();
    result.m_dimensions = {3,2};
    data_type bmp(result.m_dimensions,result.m_data);
    bmp.clear(bmp.bounds());
    bmp.point({0,0},piece_set);
    bmp.point({1,0},piece_set);
    bmp.point({2,0},piece_set);
    bmp.point({1,1},piece_set);
    return result;
}
piece piece::create_S() {
    piece result;
    result.m_location = point16::zero();
    result.m_dimensions = {3,2};
    data_type bmp(result.m_dimensions,result.m_data);
    bmp.clear(bmp.bounds());
    bmp.point({1,0},piece_set);
    bmp.point({2,0},piece_set);
    bmp.point({0,1},piece_set);
    bmp.point({1,1},piece_set);
    return result;
}
piece piece::create_Z() {
    piece result;
    result.m_location = point16::zero();
    result.m_dimensions = {3,2};
    data_type bmp(result.m_dimensions,result.m_data);
    bmp.clear(bmp.bounds());
    bmp.point({0,0},piece_set);
    bmp.point({1,0},piece_set);
    bmp.point({1,1},piece_set);
    bmp.point({2,1},piece_set);
    return result;
}
piece piece::create_O() {
    piece result;
    result.m_location = point16::zero();
    result.m_dimensions = {2,2};
    data_type bmp(result.m_dimensions,result.m_data);
    bmp.fill(bmp.bounds(),piece_set);
    return result;
}
piece piece::create_I() {
    piece result;
    result.m_location = point16::zero();
    result.m_dimensions = {1,4};
    data_type bmp(result.m_dimensions,result.m_data);
    bmp.fill(bmp.bounds(),piece_set);
    return result;
}

void piece::rotate_right() {
    rotate_left();
    rotate_left();
    rotate_left();
}

void piece::rotate_left() {
    uint8_t data[2];
    size16 dim = size16(m_dimensions.height,m_dimensions.width);
    data_type bmp(dim,data);
    for(int sx = 0;sx<m_dimensions.width;++sx) {
        for(int sy = 0;sy<m_dimensions.height;++sy) {
            const point16 spt(sx,sy);
            const point16 dpt(sy,m_dimensions.width-sx-1);
            auto px = piece_set;
            data_type::point(m_dimensions,m_data,spt,&px);
            bmp.point(dpt,px);
        }
    }
    m_dimensions = bmp.dimensions();
    memcpy(m_data,data,sizeof(m_data));
}

