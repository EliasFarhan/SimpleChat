#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <cstdlib>

#include <array>
#include <iostream>
#include <print>

#include "const.h"

int main() {
  // accept a new connection
  sf::TcpSocket socket;
  sf::Socket::Status status = socket.connect({127, 0, 0, 1}, kPortNumber);
  if (status != sf::Socket::Status::Done) {
    std::print(stderr, "Error while connecting: {}\n",
               static_cast<int>(status));
    return EXIT_FAILURE;
  }

  std::print("Write message without space:\n");
  std::string send_data(kMaxMessageLength, 0);
  std::cin >> send_data;
  // After filling the data
  status =
      socket.send(send_data.data(), std::min(send_data.size(), kMaxMessageLength));
  if (status != sf::Socket::Status::Done) {
    std::print(stderr, "Error while sending data: {}\n",
               static_cast<int>(status));
    return EXIT_FAILURE;
  }

  std::array<char, kMaxMessageLength> receive_data{};
  size_t received_byte_count;

  // TCP socket:
  status = socket.receive(receive_data.data(), kMaxMessageLength, received_byte_count);
  if (status != sf::Socket::Status::Done) {
    std::print(stderr, "Error while receiving data: {}\n",
               static_cast<int>(status));
    return EXIT_FAILURE;
  }
  std::print("Received: {}", std::string_view(receive_data.data(), received_byte_count));
  return EXIT_SUCCESS;
}
