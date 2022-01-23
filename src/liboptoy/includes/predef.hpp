#ifndef OPTOY_PREDEF_HPP
#define OPTOY_PREDEF_HPP

#include <stdexcept>
#include <stl-preview/proxy>

namespace optoy {
struct not_implemented_error : std::exception {
    not_implemented_error() : std::exception("not implemented.") {}
};

#define NOIMPL_ERROR {throw not_implemented_error();}
}

#endif // !OPTOY_PREDEF_HPP
