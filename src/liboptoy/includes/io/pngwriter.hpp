#ifndef OPTOY_PNGWRITER_HPP
#define OPTOY_PNGWRITER_HPP

#include <stdexcept>
#include <lodepng.h>
#include "interfaces/iimage.hpp"

namespace optoy {
struct png_writer_t {
    static void write_file(const std::string& filename, std::proxy<image2d::interface_t> image);
};

struct png_writer_error_t : std::runtime_error {
    png_writer_error_t() : std::runtime_error("png_writer_error") {}
};

inline void png_writer_t::write_file(const std::string& filename, std::proxy<image2d::interface_t> image) {
    if (lodepng::encode(
        filename,
        image.invoke<image2d::rawdata_f>(),
        image.invoke<image2d::width_f>(),
        image.invoke<image2d::height_f>())) {
        throw png_writer_error_t();
    }
}
} // namespace optoy

#endif // !OPTOY_PNGWRITER_HPP
