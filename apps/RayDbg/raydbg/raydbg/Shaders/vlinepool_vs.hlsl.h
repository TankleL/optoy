#include <string_view>
namespace winrt::raydbg::shaders {
    constexpr inline std::string_view vlinepool_vs =
        "struct vs_in {"                    "\n"
        "  float4 pos : POS;"               "\n"
        "};"                                "\n"
        "struct vs_out{"                    "\n"
        "  float4 pos : SV_POSITION;"       "\n"
        "};"                                "\n"
        "vs_out main(vs_in input) {"        "\n"
        "  vs_out output = (vs_out)0;"      "\n"
        "  output.pos = input.pos;"         "\n"
        "  return output;"                  "\n"
        "}";
}
