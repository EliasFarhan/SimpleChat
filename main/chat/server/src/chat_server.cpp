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

#include <print>
#include <ranges>
#include <set>

#include "const.h"

bool ChatServer::Start(uint16_t port) {
  // Non-blocking listener so that accept() returns immediately when no
  // new client is waiting.
  listener_.setBlocking(false);
  const auto listenerStatus = listener_.listen(port);
  if (listenerStatus != sf::Socket::Status::Done) {
    std::print(stderr, "Error while listening\n");
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
    socketSelector_.add(socket);
    sockets_.push_back(std::move(socket));
  }
}

void ChatServer::CleanDisconnected() {
  for (int64_t socket_idx = std::ssize(sockets_) - 1; socket_idx >= 0; --socket_idx) {
    auto& socket = sockets_[static_cast<size_t>(socket_idx)];
    // A local port of 0 means the OS has closed the socket.
    if (socket.getLocalPort() == 0) {
      RemoveSocketAt(socket_idx);
    }
  }
}

void ChatServer::HandleMessages() {
  // Wait up to 100 ms for any socket to become ready.  This small timeout
  // prevents the loop from busy-spinning while still being responsive.
  if (!socketSelector_.wait(sf::milliseconds(100))) {
    return;  // No socket had data within the timeout.
  }

  std::set<int64_t> socket_idx_to_remove;

  for (int64_t socket_idx = std::ssize(sockets_) - 1; socket_idx >= 0; --socket_idx) {
    auto& socket = sockets_[static_cast<size_t>(socket_idx)];
    // Only read from sockets that the selector flagged as ready.
    if (!socketSelector_.isReady(socket)) continue;

    std::string message;
    message.resize(kMaxMessageLength);
    size_t actualLength = 0;

    sf::Socket::Status receiveStatus = socket.receive(message.data(), kMaxMessageLength, actualLength);
    if (receiveStatus == sf::Socket::Status::Disconnected) {
      socket_idx_to_remove.insert(socket_idx);
      continue;
    }

    switch (receiveStatus) {
      case sf::Socket::Status::Done: {
        message.resize(actualLength);
        std::print("Message received: {}\n", message);

        // --- Broadcast: send the message to ALL connected clients. ---
        // For a game you would replace this with game logic (validate
        // the move, update state, send targeted responses, etc.).
        for (int64_t other_idx = std::ssize(sockets_) - 1; other_idx >= 0; --other_idx) {
          auto& otherSocket = sockets_[static_cast<size_t>(other_idx)];
          size_t totalSent = 0;
          while (totalSent < actualLength) {
            size_t sentDataCount = 0;
            const auto sendStatus = otherSocket.send(
                message.data() + totalSent, actualLength - totalSent,
                sentDataCount);
            if (sendStatus == sf::Socket::Status::Partial) {
              totalSent += sentDataCount;
              continue;
            }
            else if (sendStatus == sf::Socket::Status::Disconnected) {
              socket_idx_to_remove.insert(other_idx);
              break;
            }
            else {
              break;
            }
          }
        }
        break;
      }
      case sf::Socket::Status::Partial:
        std::print(stderr, "Partial received should not happen...\n");
        break;
      case sf::Socket::Status::Error:
        std::print(stderr, "Error receiving\n");
        break;
      case sf::Socket::Status::NotReady:
        std::print(stderr, "Not ready on received\n");
        break;
      case sf::Socket::Status::Disconnected:
        std::print(stderr, "Socket disconnected\n");
        break;
    }
  }
  for (long socket_idx : std::ranges::reverse_view(socket_idx_to_remove)) {
    sockets_.erase(sockets_.begin() + socket_idx);
  }
}
void ChatServer::RemoveSocketAt(int64_t index) {
  socketSelector_.remove(sockets_[static_cast<size_t>(index)]);
  sockets_.erase(sockets_.begin() + index);
}
