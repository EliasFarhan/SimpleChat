#ifndef CHAT_CLIENT_H_
#define CHAT_CLIENT_H_

#include <SFML/Network/TcpSocket.hpp>
#include <optional>
#include <string>
#include <string_view>

enum class ConnectionStatus { NOT_CONNECTED, CONNECTED };

class ChatClient {
 public:
  [[nodiscard]] bool Connect(std::string_view host, unsigned short port);
  [[nodiscard]] bool Send(std::string_view message);
  [[nodiscard]] std::optional<std::string> Receive();
  [[nodiscard]] bool IsConnected() const;
  void Disconnect();

 private:
  sf::TcpSocket socket_;
  ConnectionStatus status_ = ConnectionStatus::NOT_CONNECTED;
};

#endif  // CHAT_CLIENT_H_
