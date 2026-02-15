#ifndef CLIENT_VIEW_H_
#define CLIENT_VIEW_H_

#include <string>
#include <vector>

struct SDL_Window;
struct SDL_Renderer;

struct ConnectAction {
  std::string host;
  unsigned short port;
};

struct SendAction {
  std::string message;
};

class ClientView {
 public:
  bool Init();
  void Shutdown();
  void BeginFrame();
  void EndFrame();
  bool DrawConnectionPanel(std::string& address, short& port);
  bool DrawChatPanel(const std::vector<std::string>& messages,
                     std::string& sendMessage);
  bool ShouldQuit() const;

 private:
  SDL_Window* window_ = nullptr;
  SDL_Renderer* renderer_ = nullptr;
  bool shouldQuit_ = false;
};

#endif  // CLIENT_VIEW_H_
