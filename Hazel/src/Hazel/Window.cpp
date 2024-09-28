#include "Hzpch.h"
#include "Window.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Hazel {
    Window* Window::Create(const WindowProps& props) {
        std::cout << "Window.cpp: Window::Create(): Creating a WindowsWindow\n";
        return new WindowsWindow(props); // Create and return a new WindowsWindow
    }
}
