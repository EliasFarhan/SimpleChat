#include "chat_client.h"

#include <SFML/Network/IpAddress.hpp>
#include <algorithm>
#include <iostream>

#include "const.h"

bool ChatClient::Connect(std::string_view host, unsigned short port) {
  auto address = sf::IpAddress::resolve(std::string(host));
  if (!address) {
    std::cerr << "Failed to resolve address: " << host << "\n";
    return false;
  }

  socket_.setBlocking(true);
  const auto connectionStatus = socket_.connect(*address, port);
  socket_.setBlocking(false);

  switch (connectionStatus) {
    case sf::Socket::Status::Done:
      status_ = ConnectionStatus::CONNECTED;
      return true;
    case sf::Socket::Status::NotReady:
      std::cerr << "Socket not ready\n";
      break;
    case sf::Socket::Status::Partial:
      std::cerr << "Partial\n";
      break;
    case sf::Socket::Status::Disconnected:
      std::cerr << "Socket disconnected\n";
      break;
    case sf::Socket::Status::Error:
      std::cerr << "Socket error\n";
      break;
  }
  return false;
}

bool ChatClient::Send(std::string_view message) {
  const auto sendSize = std::min(message.size(), MAX_MESSAGE_LENGTH);
  std::size_t totalSent = 0;
  while (totalSent < sendSize) {
    std::size_t dataSent = 0;
    const auto sendStatus =
        socket_.send(message.data() + totalSent, sendSize - totalSent, dataSent);
    if (sendStatus == sf::Socket::Status::Partial) {
      totalSent += dataSent;
      continue;
    }
    if (sendStatus == sf::Socket::Status::Done) {
      return true;
    }
    return false;
  }
  return true;
}

std::optional<std::string> ChatClient::Receive() {
  std::string message;
  message.resize(MAX_MESSAGE_LENGTH);
  std::size_t actuallyReceived = 0;
  const auto receivedStatus =
      socket_.receive(message.data(), MAX_MESSAGE_LENGTH, actuallyReceived);
  if (receivedStatus == sf::Socket::Status::Done) {
    message.resize(actuallyReceived);
    return message;
  }
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
