#ifndef CLIENT_VIEW_INTERFACE_H_
#define CLIENT_VIEW_INTERFACE_H_

#include <span>
#include <string>

class ClientViewInterface {
 public:
  virtual ~ClientViewInterface() = default;

  [[nodiscard]] virtual bool Init() = 0;
  virtual void Shutdown() = 0;
  virtual void BeginFrame() = 0;
  virtual void EndFrame() = 0;
  virtual bool DrawConnectionPanel(std::string& address,
                                   unsigned short& port) = 0;
  virtual bool DrawChatPanel(std::span<const std::string> messages,
                             std::string& sendMessage) = 0;
  [[nodiscard]] virtual bool ShouldQuit() const = 0;
};

#endif  // CLIENT_VIEW_INTERFACE_H_
