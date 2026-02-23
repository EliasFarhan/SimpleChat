/**
 * @file chat_client.cpp
 * @brief Implementation of the low-level TCP client.
 */

#include "chat_client.h"

#include <SFML/Network/IpAddress.hpp>
#include <algorithm>
#include <print>

#include "const.h"

bool ChatClient::Connect(std::string_view host, unsigned short port) {
  // Resolve the human-readable host name (e.g. "localhost") to an IP address.
  auto address = sf::IpAddress::resolve(std::string(host));
  if (!address) {
    std::print(stderr, "Failed to resolve address: {}\n", host);
    return false;
  }

  // Use blocking mode for the connection attempt so we wait for the result.
  socket_.setBlocking(true);
  const auto connectionStatus = socket_.connect(*address, port);
  // Switch back to non-blocking so that Receive() won't freeze the UI.
  socket_.setBlocking(false);

  switch (connectionStatus) {
    case sf::Socket::Status::Done:
      status_ = ConnectionStatus::CONNECTED;
      return true;
    case sf::Socket::Status::NotReady:
      std::print(stderr, "Socket not ready\n");
      break;
    case sf::Socket::Status::Partial:
      std::print(stderr, "Partial\n");
      break;
    case sf::Socket::Status::Disconnected:
      std::print(stderr, "Socket disconnected\n");
      break;
    case sf::Socket::Status::Error:
      std::print(stderr, "Socket error\n");
      break;
  }
  return false;
}

bool ChatClient::Send(std::string_view message) {
  // Clamp the message to MAX_MESSAGE_LENGTH to avoid buffer overflows.
  const auto sendSize = std::min(message.size(), MAX_MESSAGE_LENGTH);

  // TCP may not send all bytes in one call (especially for large messages).
  // We loop, advancing a cursor, until everything has been sent.
  std::size_t totalSent = 0;
  while (totalSent < sendSize) {
    std::size_t dataSent = 0;
    const auto sendStatus =
        socket_.send(message.data() + totalSent, sendSize - totalSent, dataSent);
    if (sendStatus == sf::Socket::Status::Partial) {
      // Only part of the data was sent -- advance the cursor and retry.
      totalSent += dataSent;
      continue;
    }
    if (sendStatus == sf::Socket::Status::Done) {
      return true;
    }
    // Any other status (Error, Disconnected, ...) is a failure.
    return false;
  }
  return true;
}

std::optional<std::string> ChatClient::Receive() {
  // Prepare a buffer large enough for one message.
  std::string message;
  message.resize(MAX_MESSAGE_LENGTH);
  std::size_t actuallyReceived = 0;

  // Non-blocking receive: returns immediately even if no data is available.
  const auto receivedStatus =
      socket_.receive(message.data(), MAX_MESSAGE_LENGTH, actuallyReceived);
  if (receivedStatus == sf::Socket::Status::Done) {
    // Shrink the string to the actual number of bytes received.
    message.resize(actuallyReceived);
    return message;
  }

  // If the local port dropped to 0 the OS has closed the socket -- mark
  // ourselves as disconnected so the Controller can react.
  if (socket_.getLocalPort() == 0) {
    status_ = ConnectionStatus::NOT_CONNECTED;
  }
  return std::nullopt;
}

bool ChatClient::IsConnected() const {
  return status_ == ConnectionStatus::CONNECTED;
}

void ChatClient::Disconnect() {
  socket_.disconnect();
  status_ = ConnectionStatus::NOT_CONNECTED;
}
