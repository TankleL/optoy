#pragma once
#include "pch.h" 

namespace dx {
	class DeviceResources final
	{
	public:
		DeviceResources();
		void set_dpi(float dpi);
		void set_logical_size(winrt::Windows::Foundation::Size size);
		void validate_device();
		void handle_device_lost();
		void set_composition_scale(float scale_x, float scale_y);
		void set_swapchainpanel(winrt::Windows::UI::Xaml::Controls::SwapChainPanel panel);
		D3D11_VIEWPORT get_screen_viewport() const { return _screenviewport; };
		ID3D11DeviceContext4* get_d3dcontext() const { return _d3dcontext.get(); };
		ID3D11Device3* get_d3ddevice() const { return _d3ddevice.get(); }
		ID3D11RenderTargetView1* get_back_render_target_view() const { return _d3drtview.get(); };
		void present();

	private:
		void _reset_dires(); // create device independent resources
		void _reset_ddres(); // create device dependent resources
		void _reset_wsdres(); // reset window size dependent resources
		void _update_rtsize(); // update render target size

	private:
		winrt::com_ptr<ID3D11Device3> _d3ddevice;
		winrt::com_ptr<ID3D11DeviceContext4> _d3dcontext;
		winrt::com_ptr<ID3D11RenderTargetView1> _d3drtview;
		winrt::com_ptr<ID3D11DepthStencilView> _d3ddsview;
		winrt::com_ptr<IDXGISwapChain3> _swapchain;
		D3D11_VIEWPORT _screenviewport;
		winrt::Windows::UI::Xaml::Controls::SwapChainPanel _swapchain_panel;
		D3D_FEATURE_LEVEL _d3dfeaturelevel;
		winrt::Windows::Foundation::Size _d3drt_size;
		winrt::Windows::Foundation::Size _output_size;
		winrt::Windows::Foundation::Size _logical_size;
		float _dpi;
		float _composition_scale_x;
		float _composition_scale_y;
		float _effective_composition_scale_x;
		float _effective_composition_scale_y;
		float _effective_dpi;

	private:
		constexpr static inline float _dpi_threshold = 192.f;
		constexpr static inline float _width_threshold = 1920.f;
		constexpr static inline float _height_threshold = 1080.f;
	};
}


