/*
 * Copyright (c) 2022 Martin Helmut Fieber <info@martin-fieber.se>
 */

#pragma once

#include <SDL.h>

#include <memory>
#include <string>
#include <vector>

#include "Core/Window.hpp"

namespace App {

enum class ExitStatus : int { SUCCESS = 0, FAILURE = 1 };

class Application {
 public:
  explicit Application(const std::string& title);
  ~Application();

  Application(const Application&) = delete;
  Application(Application&&) = delete;
  Application& operator=(Application other) = delete;
  Application& operator=(Application&& other) = delete;

  ExitStatus run();
  void stop();

  void on_event(const SDL_WindowEvent& event);
  void on_resize(const SDL_WindowEvent& event);
  void on_minimize();
  void on_shown();
  void on_close();

 private:
  struct State {
    bool running{false};
    bool minimized{false};
    bool show_some_panel{true};
  };

  void set_theme() const;

  ExitStatus m_exit_status{ExitStatus::SUCCESS};
  std::shared_ptr<Window> m_window{nullptr};
  State m_state{};
};

}  // namespace App
