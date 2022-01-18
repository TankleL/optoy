#ifndef OPTOY_PREDEF_HPP
#define OPTOY_PREDEF_HPP

#include <stdexcept>
#include <stl-preview/proxy>

namespace optoy {
struct not_implemented_error : std::exception {
    not_implemented_error() : std::exception("not implemented.") {}
};
}

#endif // !OPTOY_PREDEF_HPP
