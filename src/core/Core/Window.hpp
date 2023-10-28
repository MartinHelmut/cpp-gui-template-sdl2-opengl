#pragma once

#include <SDL.h>

#include <string>

namespace App {

class Window {
 public:
  struct Settings {
    std::string title;
    int width{1280};
    int height{720};
  };

  explicit Window(const Settings& settings);
  ~Window();

  Window(const Window&) = delete;
  Window(Window&&) = delete;
  Window& operator=(Window other) = delete;
  Window& operator=(Window&& other) = delete;

  [[nodiscard]] SDL_Window* get_native_window() const;
  [[nodiscard]] SDL_GLContext get_native_context() const;

 private:
  SDL_Window* m_window{nullptr};
  SDL_GLContext m_gl_context{nullptr};
};

}  // namespace App
