#ifndef OPTOY_IIMAGE_HPP
#define OPTOY_IIMAGE_HPP

#include <stl-preview/proxy>

namespace optoy {
namespace image2d {
using coord_t = uint32_t;
using channel_t = unsigned char;

struct pixel_t {
    channel_t r, g, b, a;
};

struct width_f : std::facade_operation<
    coord_t(),
    [](const auto& self) { return self.width(); }> {};

struct height_f : std::facade_operation<
    coord_t(),
    [](const auto& self) { return self.height(); }> {};

struct rawdata_f : std::facade_operation<
    const channel_t*(),
    [](const auto& self) { return self.raw_data(); }> {};

struct pixel_get_f : std::facade_operation<
    const pixel_t&(coord_t, coord_t),
    [](const auto& self, coord_t x, coord_t y) { return self.get(x, y); }> {};

struct pixel_set_f : std::facade_operation<
    void(coord_t, coord_t, const pixel_t&),
    [](auto& self, coord_t x, coord_t y, const pixel_t& p) { return self.set(x, y, p); }> {};

struct interface_t : std::facade<
    width_f,
    height_f,
    rawdata_f,
    pixel_get_f,
    pixel_set_f>{};

} // namespace image2d
} // namespace optoy

#endif // !OPTOY_IIMAGE_HPP
