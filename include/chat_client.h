#ifndef CHAT_CLIENT_H_
#define CHAT_CLIENT_H_

#include <SFML/Network/TcpSocket.hpp>
#include <optional>
#include <string>

enum class ConnectionStatus { NOT_CONNECTED, CONNECTED };

class ChatClient {
 public:
  bool Connect(const std::string& host, unsigned short port);
  bool Send(const std::string& message);
  std::optional<std::string> Receive();
  bool IsConnected() const;
  void Disconnect();

 private:
  sf::TcpSocket socket_;
  ConnectionStatus status_ = ConnectionStatus::NOT_CONNECTED;
};

#endif  // CHAT_CLIENT_H_
