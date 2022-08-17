/*
 * Copyright (c) 2022 Martin Helmut Fieber <info@martin-fieber.se>
 */

#include "Application.hpp"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl.h>
#include <glad/glad.h>
#include <imgui.h>

#include "Core/Instrumentor.hpp"

namespace App {

Application::Application(const std::string& title) {
  APP_PROFILE_FUNCTION();

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
    APP_ERROR("Error: %s\n", SDL_GetError());
    m_exit_status = ExitStatus::FAILURE;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  m_window = std::make_shared<Window>(Window::Settings{title});

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  set_theme();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForOpenGL(m_window->get_native_window(), m_window->get_native_context());
  ImGui_ImplOpenGL3_Init("#version 410 core");
}

Application::~Application() {
  APP_PROFILE_FUNCTION();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_Quit();
}

ExitStatus App::Application::run() {
  APP_PROFILE_FUNCTION();

  if (m_exit_status == ExitStatus::FAILURE) {
    return m_exit_status;
  }

  m_state.running = true;

  const ImGuiIO& io{ImGui::GetIO()};

  while (m_state.running) {
    APP_PROFILE_SCOPE("MainLoop");

    SDL_Event event{};
    while (SDL_PollEvent(&event) != 0) {
      APP_PROFILE_SCOPE("EventPolling");

      ImGui_ImplSDL2_ProcessEvent(&event);

      if (event.type == SDL_QUIT) {
        stop();
      }

      if (event.type == SDL_WINDOWEVENT &&
          event.window.windowID == SDL_GetWindowID(m_window->get_native_window())) {
        on_event(event.window);
      }
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (!m_state.minimized) {
      ImGui::DockSpaceOverViewport();

      if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
          if (ImGui::MenuItem("Exit", "Cmd+Q")) {
            stop();
          }
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
          ImGui::MenuItem("Some Panel", nullptr, &m_state.show_some_panel);
          ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
      }
    }

    // Whatever GUI to implement here ...
    if (m_state.show_some_panel) {
      ImGui::Begin("Some panel", &m_state.show_some_panel);
      // NOLINTNEXTLINE
      ImGui::Text("Hello World");
      ImGui::End();
    }

    // Rendering
    ImGui::Render();
    glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
    glClearColor(0.5F, 0.5F, 0.5F, 1.00F);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
      SDL_Window* backup_current_window{SDL_GL_GetCurrentWindow()};
      SDL_GLContext backup_current_context{SDL_GL_GetCurrentContext()};
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }

    SDL_GL_SwapWindow(m_window->get_native_window());
  }

  return m_exit_status;
}

void App::Application::stop() {
  m_state.running = false;
}

void Application::on_event(const SDL_WindowEvent& event) {
  APP_PROFILE_FUNCTION();

  switch (event.event) {
    case SDL_WINDOWEVENT_CLOSE:
      return on_close();
    case SDL_WINDOWEVENT_MINIMIZED:
      return on_minimize();
    case SDL_WINDOWEVENT_SHOWN:
      return on_shown();
    case SDL_WINDOWEVENT_RESIZED:
      return on_resize(event);
  }
}

// Can be static, but will serve as an example call, so ignore.
// NOLINTNEXTLINE
void Application::on_resize([[maybe_unused]] const SDL_WindowEvent& event) {
  APP_DEBUG("RESIZE {} {}", event.data1, event.data2);
}

void Application::on_minimize() {
  m_state.minimized = true;
}

void Application::on_shown() {
  m_state.minimized = false;
}

void Application::on_close() {
  stop();
}

void Application::set_theme() const {
  APP_PROFILE_FUNCTION();

  ImGuiIO& io{ImGui::GetIO()};
  ImGuiStyle& style{ImGui::GetStyle()};

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable |
                    ImGuiConfigFlags_ViewportsEnable;

  // ImGui font
  const float font_scaling_factor{m_window->get_scale()};
  const float font_size{18.0F * font_scaling_factor};

  io.Fonts->AddFontFromFileTTF("assets/fonts/Manrope/Manrope-Regular.ttf", font_size);
  io.FontDefault =
      io.Fonts->AddFontFromFileTTF("assets/fonts/Manrope/Manrope-Regular.ttf", font_size);
  io.FontGlobalScale = 1.0F / font_scaling_factor;

  style.WindowRounding = 5.3F;
  style.GrabRounding = style.FrameRounding = 2.3F;
  style.ScrollbarRounding = 5.0F;
  style.FrameBorderSize = 1.0F;
  style.ItemSpacing.y = 6.5F;

  if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
    style.WindowRounding = 0.0F;
    style.Colors[ImGuiCol_WindowBg].w = 1.0F;
  }

  // Theme setup ...
  // auto* colors{static_cast<ImVec4*>(style.Colors)};
  // colors[ImGuiCol_Text] = {0.73333335F, 0.73333335F, 0.73333335F, 1.00F};
  // ...
}

}  // namespace App
