#include <string_view>
namespace winrt::raydbg::shaders {
    constexpr inline std::string_view vlinepool_ps =
        "struct vs_out{"                                    "\n"
        "  float4 pos : SV_POSITION;"                       "\n"
        "};"                                                "\n"
        "float4 main(vs_out input) : SV_TARGET {"        "\n"
        "  return float4(1.0, 0.0, 1.0, 1.0);"               "\n"
        "}"                                                 "\n"
        ;
}
