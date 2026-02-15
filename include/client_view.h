#ifndef CLIENT_VIEW_H_
#define CLIENT_VIEW_H_

#include <span>
#include <string>

struct SDL_Window;
struct SDL_Renderer;

class ClientView {
 public:
  [[nodiscard]] bool Init();
  void Shutdown();
  void BeginFrame();
  void EndFrame();
  bool DrawConnectionPanel(std::string& address, unsigned short& port);
  bool DrawChatPanel(std::span<const std::string> messages,
                     std::string& sendMessage);
  [[nodiscard]] bool ShouldQuit() const;

 private:
  SDL_Window* window_ = nullptr;
  SDL_Renderer* renderer_ = nullptr;
  bool shouldQuit_ = false;
};

#endif  // CLIENT_VIEW_H_
