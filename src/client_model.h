#ifndef CLIENT_MODEL_H_
#define CLIENT_MODEL_H_

#include <string>
#include <vector>

#include "chat_client.h"

class ClientModel {
 public:
  void Connect(const std::string& host, unsigned short port);
  void SendMessage(const std::string& message);
  void PollMessages();
  const std::vector<std::string>& GetMessages() const;
  bool IsConnected() const;

 private:
  ChatClient client_;
  std::vector<std::string> receivedMessages_;
};

#endif  // CLIENT_MODEL_H_
