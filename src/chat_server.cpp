/**
 * @file chat_server.cpp
 * @brief Implementation of the TCP chat server.
 *
 * The server follows a simple loop each tick:
 *  1. CleanDisconnected() -- remove dead sockets.
 *  2. AcceptNewConnections() -- accept any pending clients.
 *  3. HandleMessages() -- wait (briefly) for incoming data, then broadcast.
 */

#include "chat_server.h"

#include <iostream>
#include <ranges>

#include "const.h"

bool ChatServer::Start(unsigned short port) {
  // Non-blocking listener so that accept() returns immediately when no
  // new client is waiting.
  listener_.setBlocking(false);
  const auto listenerStatus = listener_.listen(port);
  if (listenerStatus != sf::Socket::Status::Done) {
    std::cerr << "Error while listening\n";
    return false;
  }
  return true;
}

void ChatServer::Update() {
  CleanDisconnected();
  AcceptNewConnections();
  HandleMessages();
}

void ChatServer::AcceptNewConnections() {
  // Try to accept a new client.  Because the listener is non-blocking this
  // returns immediately with a status other than Done when nobody is waiting.
  sf::TcpSocket socket;
  socket.setBlocking(false);
  const auto newSocketStatus = listener_.accept(socket);
  if (newSocketStatus == sf::Socket::Status::Done) {
    auto newSocket = std::make_unique<sf::TcpSocket>(std::move(socket));
    // Register the new socket with the selector so HandleMessages() can
    // efficiently check if it has data ready.
    socketSelector_.add(*newSocket);

    // Try to reuse a nullptr slot left by a disconnected client,
    // otherwise append at the end.
    auto it = std::ranges::find_if(
        sockets_, [](const auto& s) { return s == nullptr; });
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
    // A local port of 0 means the OS has closed the socket.
    if (socket->getLocalPort() == 0) {
      socket = nullptr;
    }
  }
}

void ChatServer::HandleMessages() {
  // Wait up to 100 ms for any socket to become ready.  This small timeout
  // prevents the loop from busy-spinning while still being responsive.
  if (!socketSelector_.wait(sf::milliseconds(100))) {
    return;  // No socket had data within the timeout.
  }

  for (auto& socket : sockets_) {
    if (socket == nullptr) continue;
    // Only read from sockets that the selector flagged as ready.
    if (!socketSelector_.isReady(*socket)) continue;

    std::string message;
    message.resize(MAX_MESSAGE_LENGTH);
    std::size_t actualLength = 0;

    const auto receiveStatus =
        socket->receive(message.data(), MAX_MESSAGE_LENGTH, actualLength);
    switch (receiveStatus) {
      case sf::Socket::Status::Done: {
        message.resize(actualLength);
        std::cout << "Message received: " << message << "\n";

        // --- Broadcast: send the message to ALL connected clients. ---
        // For a game you would replace this with game logic (validate
        // the move, update state, send targeted responses, etc.).
        for (auto& otherSocket : sockets_) {
          if (otherSocket == nullptr) continue;
          std::size_t totalSent = 0;
          while (totalSent < actualLength) {
            std::size_t sentDataCount = 0;
            const auto sendStatus = otherSocket->send(
                message.data() + totalSent, actualLength - totalSent,
                sentDataCount);
            if (sendStatus == sf::Socket::Status::Partial) {
              totalSent += sentDataCount;
              continue;
            }
            break;
          }
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
      case sf::Socket::Status::Disconnected:
        std::cerr << "Socket disconnected\n";
        break;
    }
  }
}
