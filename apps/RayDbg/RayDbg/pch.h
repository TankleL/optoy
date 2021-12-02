#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <exception>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>

// Use the C++ standard templated min/max
#define NOMINMAX

#include <d3d11_3.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <windows.h>
#include <unknwn.h>
#include <restrictederrorinfo.h>
#include <hstring.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Controls.Primitives.h>
#include <winrt/Windows.UI.Xaml.Data.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Windows.UI.Xaml.Markup.h>
#include <winrt/Windows.UI.Xaml.Navigation.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif
