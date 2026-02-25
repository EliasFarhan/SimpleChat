#ifndef CLIENT_MODEL_INTERFACE_H_
#define CLIENT_MODEL_INTERFACE_H_

#include <string>
#include <string_view>
#include <span>
#include <cstdint>

class ClientModelInterface {
 public:
  virtual ~ClientModelInterface() = default;

  [[nodiscard]] virtual bool Connect(std::string_view host,
                                     uint16_t port) = 0;
  [[nodiscard]] virtual bool SendMessage(std::string_view message) = 0;
  virtual void PollMessages() = 0;
  [[nodiscard]] virtual std::span<const std::string> messages() const = 0;
  [[nodiscard]] virtual bool IsConnected() const = 0;
};

#endif  // CLIENT_MODEL_INTERFACE_H_
