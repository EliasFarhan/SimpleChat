#ifndef CHAT_SERVER_H_
#define CHAT_SERVER_H_

#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <memory>
#include <vector>

class ChatServer {
 public:
  [[nodiscard]] bool Start(unsigned short port);
  void Update();

 private:
  void AcceptNewConnections();
  void CleanDisconnected();
  void HandleMessages();

  sf::TcpListener listener_;
  sf::SocketSelector socketSelector_;
  std::vector<std::unique_ptr<sf::TcpSocket>> sockets_;
};

#endif  // CHAT_SERVER_H_
