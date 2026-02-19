//
// Created by unite on 19.02.2026.
//
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <cstdlib>

constexpr size_t kMaxMsgLength = 100;
constexpr uint16_t kPort = 53000;
#include <array>
#include <iostream>
#include <print>

int main() {
  // accept a new connection
  sf::TcpSocket socket;
  sf::Socket::Status status = socket.connect({127, 0, 0, 1}, kPort);
  if (status != sf::Socket::Status::Done) {
    std::print(stderr, "Error while connecting: {}\n",
               static_cast<int>(status));
    return EXIT_FAILURE;
  }

  std::print("Write message without space:\n");
  std::string send_data(kMaxMsgLength, 0);
  std::cin >> send_data;
  // After filling the data
  status =
      socket.send(send_data.data(), std::min(send_data.size(), kMaxMsgLength));
  if (status != sf::Socket::Status::Done) {
    std::print(stderr, "Error while sending data: {}\n",
               static_cast<int>(status));
    return EXIT_FAILURE;
  }

  std::array<char, kMaxMsgLength> receive_data{};
  size_t received;

  // TCP socket:
  status = socket.receive(receive_data.data(), 100, received);
  if (status != sf::Socket::Status::Done) {
    std::print(stderr, "Error while receiving data: {}\n",
               static_cast<int>(status));
    return EXIT_FAILURE;
  }
  std::print("Received: {}", receive_data.data());
  return EXIT_SUCCESS;
}