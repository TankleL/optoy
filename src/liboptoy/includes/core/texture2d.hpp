#ifndef OPTOY_TEXTURE2D_HPP
#define OPTOY_TEXTURE2D_HPP

#include <vector>
#include "interfaces/iimage.hpp"

namespace optoy {
// >>> proxy info: texture2d_t meets the requirements of image2d::interface_t
class texture2d_t {
public:
    texture2d_t(image2d::coord_t width, image2d::coord_t height);
    const unsigned char* raw_data() const;
    image2d::coord_t width() const;
    image2d::coord_t height() const;
    const image2d::pixel_t& get(image2d::coord_t x, image2d::coord_t z) const;
    void set(image2d::coord_t x, image2d::coord_t z, const image2d::pixel_t& val);

private:
    image2d::coord_t _width, _height;
    std::vector<image2d::pixel_t> _buffer;
};

inline texture2d_t::texture2d_t(image2d::coord_t width, image2d::coord_t height)
    : _width(width)
    , _height(height) {
    _buffer.resize(width * height);
}

inline const image2d::pixel_t& texture2d_t::get(image2d::coord_t x, image2d::coord_t y) const {
    return _buffer[x + y * _width];
}

inline void texture2d_t::set(image2d::coord_t x, image2d::coord_t y, const image2d::pixel_t& val) {
    _buffer[x + y * _width] = val;
}

inline image2d::coord_t texture2d_t::width() const {
    return _width;
}

inline image2d::coord_t texture2d_t::height() const {
    return _height;
}

inline const unsigned char* texture2d_t::raw_data() const {
    auto* p = _buffer.data();
    return &p->r;
}

} // namespace optoy

#endif // !OPTOY_TEXTURE2D_HPP
