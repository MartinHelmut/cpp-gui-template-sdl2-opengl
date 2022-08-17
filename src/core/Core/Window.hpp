/*
 * Copyright (c) 2022 Martin Helmut Fieber <info@martin-fieber.se>
 */

#pragma once

#include <SDL.h>

#include <string>

namespace App {

class Window {
 public:
  struct Settings {
    std::string title;
    const int width{1280};
    const int height{720};
  };

  explicit Window(const Settings& settings);
  ~Window();

  Window(const Window&) = delete;
  Window(Window&&) = delete;
  Window& operator=(Window other) = delete;
  Window& operator=(Window&& other) = delete;

  [[nodiscard]] float get_scale() const;

  [[nodiscard]] SDL_Window* get_native_window() const;
  [[nodiscard]] SDL_GLContext get_native_context() const;

 private:
  SDL_Window* m_window;
  SDL_GLContext m_gl_context;
};

}  // namespace App
