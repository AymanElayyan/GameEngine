#include "Hzpch.h"
#include "Window.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Hazel {
    Window* Window::Create(const WindowProps& props) {
        return new WindowsWindow(props); 
    }
}
