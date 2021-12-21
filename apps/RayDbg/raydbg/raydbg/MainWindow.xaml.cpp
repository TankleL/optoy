#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

namespace winrt::raydbg::implementation {
    MainWindow::MainWindow() {
        InitializeComponent();
        mainframe().Navigate(winrt::xaml_typename<MainPage>());
    }
}
