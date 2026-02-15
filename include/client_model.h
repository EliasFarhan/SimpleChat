#ifndef CLIENT_MODEL_H_
#define CLIENT_MODEL_H_

#include <string>
#include <string_view>
#include <vector>

#include "chat_client.h"

class ClientModel {
 public:
  [[nodiscard]] bool Connect(std::string_view host, unsigned short port);
  [[nodiscard]] bool SendMessage(std::string_view message);
  void PollMessages();
  [[nodiscard]] const std::vector<std::string>& GetMessages() const;
  [[nodiscard]] bool IsConnected() const;

 private:
  ChatClient client_;
  std::vector<std::string> receivedMessages_;
};

#endif  // CLIENT_MODEL_H_
