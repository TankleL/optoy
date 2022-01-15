#ifndef OPTOY_PREDEF_HPP
#define OPTOY_PREDEF_HPP

#include <stl-preview/proxy>

namespace optoy {

template<class F>
using interface_t = std::proxy<F>;

}

#endif // !OPTOY_PREDEF_HPP
