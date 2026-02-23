#ifndef CHAT_CLIENT_INTERFACE_H_
#define CHAT_CLIENT_INTERFACE_H_

#include <optional>
#include <string>
#include <string_view>

class ChatClientInterface {
 public:
  virtual ~ChatClientInterface() = default;

  [[nodiscard]] virtual bool Connect(std::string_view host,
                                     unsigned short port) = 0;
  [[nodiscard]] virtual bool Send(std::string_view message) = 0;
  [[nodiscard]] virtual std::optional<std::string> Receive() = 0;
  [[nodiscard]] virtual bool IsConnected() const = 0;
  virtual void Disconnect() = 0;
};

#endif  // CHAT_CLIENT_INTERFACE_H_
