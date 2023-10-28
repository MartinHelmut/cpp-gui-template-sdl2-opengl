#include "Window.hpp"

#include <SDL_video.h>
#include <glad/glad.h>

#include "Core/DPIHandler.hpp"
#include "Core/Debug/Instrumentor.hpp"
#include "Core/Log.hpp"

namespace App {

Window::Window(const Settings& settings) {
  APP_PROFILE_FUNCTION();

  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  constexpr auto window_flags{static_cast<SDL_WindowFlags>(
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI)};
  constexpr int window_center_flag{SDL_WINDOWPOS_CENTERED};
  const WindowSize size{DPIHandler::get_dpi_aware_window_size(settings)};

  m_window = SDL_CreateWindow(settings.title.c_str(),
      window_center_flag,
      window_center_flag,
      size.width,
      size.height,
      window_flags);

  // NOLINTNEXTLINE
  m_gl_context = SDL_GL_CreateContext(m_window);
  if (m_gl_context == nullptr) {
    APP_ERROR("Could not create SDL OpenGL context.");
    return;
  }

  gladLoadGLLoader(SDL_GL_GetProcAddress);
  SDL_GL_MakeCurrent(m_window, m_gl_context);
  SDL_GL_SetSwapInterval(1);  // Enable vsync
}

Window::~Window() {
  APP_PROFILE_FUNCTION();

  SDL_GL_DeleteContext(m_gl_context);
  SDL_DestroyWindow(m_window);
}

SDL_Window* Window::get_native_window() const {
  return m_window;
}

SDL_GLContext Window::get_native_context() const {
  return m_gl_context;
}

}  // namespace App
