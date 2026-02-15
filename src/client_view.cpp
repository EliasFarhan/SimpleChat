#include "client_view.h"

#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <misc/cpp/imgui_stdlib.h>

#include <iostream>
#include <span>

bool ClientView::Init() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "Failed to init SDL: " << SDL_GetError() << "\n";
    return false;
  }
  window_ =
      SDL_CreateWindow("Simple Chat", 1280, 720, SDL_WINDOW_RESIZABLE);
  if (window_ == nullptr) {
    std::cerr << "Failed to create window: " << SDL_GetError() << "\n";
    return false;
  }
  renderer_ = SDL_CreateRenderer(window_, nullptr);
  if (renderer_ == nullptr) {
    std::cerr << "Failed to create renderer: " << SDL_GetError() << "\n";
    return false;
  }
  SDL_SetRenderVSync(renderer_, 1);

  ImGui::CreateContext();
  ImGui_ImplSDL3_InitForSDLRenderer(window_, renderer_);
  ImGui_ImplSDLRenderer3_Init(renderer_);
  return true;
}

void ClientView::Shutdown() {
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

void ClientView::BeginFrame() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL3_ProcessEvent(&event);
    if (event.type == SDL_EVENT_QUIT) {
      shouldQuit_ = true;
    }
  }
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  int w = 0, h = 0;
  SDL_GetWindowSize(window_, &w, &h);
  ImGui::SetNextWindowSize(
      {static_cast<float>(w), static_cast<float>(h)}, ImGuiCond_Always);
  ImGui::SetNextWindowPos({0.0f, 0.0f}, ImGuiCond_Always);
  ImGui::Begin("Simple Chat", nullptr, ImGuiWindowFlags_NoTitleBar);
}

void ClientView::EndFrame() {
  ImGui::End();
  SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
  SDL_RenderClear(renderer_);
  ImGui::Render();
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer_);
  SDL_RenderPresent(renderer_);
}

bool ClientView::DrawConnectionPanel(std::string& address,
                                      unsigned short& port) {
  ImGui::InputText("Host Address", &address);
  ImGui::SameLine();
  int portInt = port;
  if (ImGui::InputInt("Port", &portInt, 0, 0)) {
    if (portInt > 0 && portInt <= 65535) {
      port = static_cast<unsigned short>(portInt);
    }
  }
  return ImGui::Button("Connect");
}

bool ClientView::DrawChatPanel(std::span<const std::string> messages,
                               std::string& sendMessage) {
  ImGui::InputText("Message", &sendMessage);
  bool send = ImGui::Button("Send");
  for (const auto& message : messages) {
    ImGui::TextUnformatted(message.c_str());
  }
  return send;
}

bool ClientView::ShouldQuit() const { return shouldQuit_; }
