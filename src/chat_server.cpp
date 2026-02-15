#include "chat_server.h"

#include <iostream>
#include <ranges>

#include "const.h"

bool ChatServer::Start(unsigned short port) {
  listener_.setBlocking(false);
  const auto listenerStatus = listener_.listen(port);
  if (listenerStatus != sf::Socket::Status::Done) {
    std::cerr << "Error while listening\n";
    return false;
  }
  sockets_.reserve(15);
  return true;
}

void ChatServer::Update() {
  CleanDisconnected();
  AcceptNewConnections();
  HandleMessages();
}

void ChatServer::AcceptNewConnections() {
  sf::TcpSocket socket;
  socket.setBlocking(false);
  const auto newSocketStatus = listener_.accept(socket);
  if (newSocketStatus == sf::Socket::Status::Done) {
    auto newSocket = std::make_unique<sf::TcpSocket>(std::move(socket));
    socketSelector_.add(*newSocket);
    auto it = std::ranges::find_if(
        sockets_, [](auto& s) { return s == nullptr; });
    if (it != sockets_.end()) {
      *it = std::move(newSocket);
    } else {
      sockets_.push_back(std::move(newSocket));
    }
  }
}

void ChatServer::CleanDisconnected() {
  for (auto& socket : sockets_) {
    if (socket == nullptr) continue;
    if (socket->getLocalPort() == 0) {
      socket = nullptr;
    }
  }
}

void ChatServer::HandleMessages() {
  if (!socketSelector_.wait(sf::milliseconds(100))) {
    return;
  }
  for (auto& socket : sockets_) {
    if (socket == nullptr) continue;
    if (!socketSelector_.isReady(*socket)) continue;

    std::string message;
    message.resize(MAX_MESSAGE_LENGTH, 0);
    std::size_t actualLength;

    sf::TcpSocket::Status receiveStatus =
        socket->receive(message.data(), MAX_MESSAGE_LENGTH, actualLength);
    switch (receiveStatus) {
      case sf::Socket::Status::Done: {
        std::cout << "Message received: " << message << std::endl;
        for (auto& otherSocket : sockets_) {
          if (otherSocket == nullptr) continue;
          std::size_t sentDataCount;
          sf::TcpSocket::Status sendStatus;
          do {
            sendStatus = otherSocket->send(message.data(), actualLength,
                                           sentDataCount);
          } while (sendStatus == sf::Socket::Status::Partial);
        }
        break;
      }
      case sf::Socket::Status::Partial:
        std::cerr << "Partial received...\n";
        break;
      case sf::Socket::Status::Error:
        std::cerr << "Error receiving\n";
        break;
      case sf::Socket::Status::NotReady:
        std::cerr << "Not ready on received\n";
        break;
      default:
        break;
    }
  }
}
