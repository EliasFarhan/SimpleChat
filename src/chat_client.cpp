#include "chat_client.h"

#include <SFML/Network/IpAddress.hpp>
#include <iostream>

#include "const.h"

bool ChatClient::Connect(const std::string& host, unsigned short port) {
  auto address = sf::IpAddress::resolve(host);
  if (!address) {
    std::cerr << "Failed to resolve address: " << host << "\n";
    return false;
  }

  socket_.setBlocking(true);
  const auto connectionStatus = socket_.connect(address.value(), port);
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

bool ChatClient::Send(const std::string& message) {
  std::size_t dataSent;
  sf::TcpSocket::Status sendStatus;
  do {
    sendStatus = socket_.send(message.data(), MAX_MESSAGE_LENGTH, dataSent);
  } while (sendStatus == sf::Socket::Status::Partial);
  return sendStatus == sf::Socket::Status::Done;
}

std::optional<std::string> ChatClient::Receive() {
  std::string message;
  message.resize(MAX_MESSAGE_LENGTH, 0);
  std::size_t actuallyReceived;
  const auto receivedStatus =
      socket_.receive(message.data(), MAX_MESSAGE_LENGTH, actuallyReceived);
  if (receivedStatus == sf::Socket::Status::Done) {
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
