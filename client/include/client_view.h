#ifndef CLIENT_VIEW_H_
#define CLIENT_VIEW_H_

#include <span>
#include <string>

#include "client_view_interface.h"

struct SDL_Window;
struct SDL_Renderer;

class ClientView : public ClientViewInterface {
 public:
  [[nodiscard]] bool Init() override;
  void Shutdown() override;
  void BeginFrame() override;
  void EndFrame() override;
  bool DrawConnectionPanel(std::string& address, unsigned short& port) override;
  bool DrawChatPanel(std::span<const std::string> messages,
                     std::string& sendMessage) override;
  [[nodiscard]] bool ShouldQuit() const override;

 private:
  SDL_Window* window_ = nullptr;
  SDL_Renderer* renderer_ = nullptr;
  bool shouldQuit_ = false;
};

#endif  // CLIENT_VIEW_H_
