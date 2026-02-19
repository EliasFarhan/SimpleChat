/**
 * @file client_view.cpp
 * @brief Implementation of the client View (rendering + input).
 *
 * This file sets up SDL3 and Dear ImGui, and provides two GUI panels:
 *  - DrawConnectionPanel(): shown when not yet connected to a server.
 *  - DrawChatPanel(): shown once connected; displays messages and a send box.
 *
 * ImGui uses an "immediate mode" paradigm: every frame you describe the
 * entire UI, and ImGui figures out what changed.  Buttons return true on
 * the frame they are clicked, input fields modify a string reference
 * directly, etc.
 */

#include "client_view.h"

#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <misc/cpp/imgui_stdlib.h>

#include <print>
#include <span>

bool ClientView::Init() {
  // --- SDL initialisation ---
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::print(stderr, "Failed to init SDL: {}\n", SDL_GetError());
    return false;
  }
  window_ =
      SDL_CreateWindow("Simple Chat", 1280, 720, SDL_WINDOW_RESIZABLE);
  if (window_ == nullptr) {
    std::print(stderr, "Failed to create window: {}\n", SDL_GetError());
    return false;
  }
  renderer_ = SDL_CreateRenderer(window_, nullptr);
  if (renderer_ == nullptr) {
    std::print(stderr, "Failed to create renderer: {}\n", SDL_GetError());
    return false;
  }
  // Enable VSync so we don't render faster than the monitor refresh rate.
  SDL_SetRenderVSync(renderer_, 1);

  // --- ImGui initialisation ---
  // CreateContext() must be called before any other ImGui function.
  ImGui::CreateContext();
  // Hook ImGui into SDL so it can receive keyboard/mouse events.
  ImGui_ImplSDL3_InitForSDLRenderer(window_, renderer_);
  // Hook ImGui into the SDL renderer so it can draw its widgets.
  ImGui_ImplSDLRenderer3_Init(renderer_);
  return true;
}

void ClientView::Shutdown() {
  // Tear down in reverse order of initialisation.
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

void ClientView::BeginFrame() {
  // --- Process OS events (keyboard, mouse, window close, etc.) ---
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    // Forward every event to ImGui so it can track focus, hover, clicks, etc.
    ImGui_ImplSDL3_ProcessEvent(&event);
    if (event.type == SDL_EVENT_QUIT) {
      shouldQuit_ = true;
    }
  }

  // --- Start a new ImGui frame ---
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  // Make the ImGui window fill the entire SDL window.
  int w = 0, h = 0;
  SDL_GetWindowSize(window_, &w, &h);
  ImGui::SetNextWindowSize(
      {static_cast<float>(w), static_cast<float>(h)}, ImGuiCond_Always);
  ImGui::SetNextWindowPos({0.0f, 0.0f}, ImGuiCond_Always);
  ImGui::Begin("Simple Chat", nullptr, ImGuiWindowFlags_NoTitleBar);
}

void ClientView::EndFrame() {
  ImGui::End();  // Close the ImGui window opened in BeginFrame().

  // Clear the background, render ImGui draw data, and present to screen.
  SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
  SDL_RenderClear(renderer_);
  ImGui::Render();
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer_);
  SDL_RenderPresent(renderer_);
}

bool ClientView::DrawConnectionPanel(std::string& address,
                                      unsigned short& port) {
  // Text input for the server address.
  ImGui::InputText("Host Address", &address);
  ImGui::SameLine();

  // Integer input for the port, clamped to valid TCP port range.
  int portInt = port;
  if (ImGui::InputInt("Port", &portInt, 0, 0)) {
    if (portInt > 0 && portInt <= 65535) {
      port = static_cast<unsigned short>(portInt);
    }
  }

  // Returns true on the frame the user clicks "Connect".
  return ImGui::Button("Connect");
}

bool ClientView::DrawChatPanel(std::span<const std::string> messages,
                               std::string& sendMessage) {
  // Text input for composing a message.
  ImGui::InputText("Message", &sendMessage);
  bool send = ImGui::Button("Send");

  // Display every received message as a line of text.
  for (const auto& message : messages) {
    ImGui::TextUnformatted(message.c_str());
  }
  return send;
}

bool ClientView::ShouldQuit() const { return shouldQuit_; }
