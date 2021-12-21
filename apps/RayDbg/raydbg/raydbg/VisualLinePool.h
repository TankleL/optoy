#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "Shaders/vlinepool_vs.hlsl.h"
#include "Shaders/vlinepool_ps.hlsl.h"
#include "GraphicsEntities.h"
#include "GraphicsAlgo.h"
#include "GraphicsRenderThread.h"

namespace winrt::raydbg {
    class VisualLinePool {
    private:
        struct vertex_t {
            float x, y, z, w;
        };

    public:
        void device_reset(dx::DeviceResources& dxres);
        void draw(dx::DeviceResources& dxres) const;
        template<class L> requires
            std::is_same_v<L, std::vector<entities::line_segment_t<float> const&>> ||
            std::is_same_v<L, std::vector<entities::line_segment_t<float>&&>>
            void set_lines(L && lines, GraphicsRenderThread & graphics) {
            graphics.dispatch([this, l = std::forward<L>(lines)](auto& g) {
                std::vector<vertex_t> vertices;
                vertices.resize(l.size() * 2);
                size_t i = 0;
                for (const auto& line : l) {
                    const auto& t = galgo::end_points(line);
                    const auto& p0 = std::get<0>(t);
                    const auto& p1 = std::get<1>(t);
                    vertices[i]     = vertex_t{ p0.x, p0.y, p0.z, 1.f };
                    vertices[i + 1] = vertex_t{ p1.x, p1.y, p1.z, 1.f };
                    i += 2;
                }
                auto* ctx = graphics.get_device_resources().get_d3dcontext;
                D3D11_MAPPED_SUBRESOURCE mapped;
                ctx->Map(_mainvb.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
                std::memcpy(mapped.pData, vertices.data(), vertices.size());
                ctx->Unmap(_mainvb.get(), 0);
                _vertex_count = vertices.size();
            });
        }

    private:
        constexpr const static inline uint32_t _default_vertex_num = 128;

    private:
        winrt::com_ptr<ID3D11VertexShader> _mainvs;
        winrt::com_ptr<ID3D11PixelShader> _mainps;
        winrt::com_ptr<ID3D11Buffer> _mainvb;
        winrt::com_ptr<ID3D11InputLayout> _input_layout;
        size_t _vertex_count{ 0 };
    };

    inline void VisualLinePool::device_reset(dx::DeviceResources& dxres) {
        auto* device = dxres.get_d3ddevice();

        // create a vertex shader
        auto vsc= dx::compile_shader(
            shaders::vlinepool_vs,
            "main_vs",
            "main",
            "vs_5_0");
        dx::throw_if_failed(device->CreateVertexShader(vsc->GetBufferPointer(), vsc->GetBufferSize(), nullptr, _mainvs.put()));

        // create a pixel shader
        auto psc = dx::compile_shader(
            shaders::vlinepool_ps,
            "main_ps",
            "main",
            "ps_5_0");
        dx::throw_if_failed(device->CreatePixelShader(psc->GetBufferPointer(), psc->GetBufferSize(), nullptr, _mainps.put()));

        // set up the input layout
        D3D11_INPUT_ELEMENT_DESC input_ele_desc[] = {
            { "POS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
          /*
          { "COL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
          { "NOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
          { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
          */
        };
        dx::throw_if_failed(device->CreateInputLayout(
            input_ele_desc,
            ARRAYSIZE(input_ele_desc),
            vsc->GetBufferPointer(),
            vsc->GetBufferSize(),
            _input_layout.put()));

        // create default vertex buffer
        vertex_t vertices[_default_vertex_num] = { 0 };
        D3D11_BUFFER_DESC vb_desc{ 0 };
        vb_desc.ByteWidth = sizeof(vertices);
        vb_desc.Usage = D3D11_USAGE_DEFAULT;
        vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA vb_data{ 0 };
        vb_data.pSysMem = vertices;
        dx::throw_if_failed(device->CreateBuffer(&vb_desc, &vb_data, _mainvb.put()));
    }

    inline void VisualLinePool::draw(dx::DeviceResources& dxres) const {
        if (_vertex_count > 0) {
            auto* ctx = dxres.get_d3dcontext();
            ctx->IASetInputLayout(_input_layout.get());
            ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
            auto main_vb_ptr = _mainvb.get();
            UINT main_vb_stride = sizeof(vertex_t);
            UINT main_vb_offset = 0;
            ctx->IASetVertexBuffers(0, 1, &main_vb_ptr, &main_vb_stride, &main_vb_offset);
            ctx->VSSetShader(_mainvs.get(), nullptr, 0);
            ctx->PSSetShader(_mainps.get(), nullptr, 0);
            ctx->Draw(static_cast<UINT>(_vertex_count), 0);
        }
    }
}
