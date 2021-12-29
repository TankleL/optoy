#include <stl-preview/proxy>

namespace optoy {
    namespace op {
        struct intersect : std::facade_operation<int(int, int)> {
            template<class T>
            static int apply(T&& operand, int arg0, int arg1) {
                return std::forward<T>(operand).intersect(arg0, arg1);
            }
        };
    }

    struct irtrace_target : std::facade<op::intersect> {};

    class irresolver {
    public:
        int resolve(std::proxy<irtrace_target> tracer, int a, int b) {
            return tracer.invoke<op::intersect>(a, b);
        }
    };
}

