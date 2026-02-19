//
// Created by unite on 19.02.2026.
//
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <array>
#include <print>

constexpr size_t kMaxMsgLength = 100;
constexpr uint16_t kPort = 53000;

int main() {
  sf::TcpListener listener;

  // bind the listener to a port
  if (const auto status = listener.listen(kPort);
      status != sf::Socket::Status::Done) {
    std::print(stderr, "Error while listening status: {}\n",
               static_cast<int>(status));
    return EXIT_FAILURE;
  }

  // accept a new connection
  sf::TcpSocket client;
  if (const auto status = listener.accept(client);
      status != sf::Socket::Status::Done) {
    std::print(stderr, "Error while accepting connection: {}\n",
               static_cast<int>(status));
    return EXIT_FAILURE;
  }
  std::print("Client at {}:{} connected!\n",
             client.getRemoteAddress()->toString(), client.getRemotePort());
  // Receiving data
  std::array<char, kMaxMsgLength> receive_data{};
  size_t received;

  // TCP socket:
  if (const auto status =
          client.receive(receive_data.data(), kMaxMsgLength, received);
      status != sf::Socket::Status::Done) {
    std::print(stderr, "Error while receiving data: {}\n",
               static_cast<int>(status));
    return EXIT_FAILURE;
  }

  std::println("Received data: {}\n", receive_data.data());

  if (const auto status = client.send(receive_data.data(), received);
      status != sf::Socket::Status::Done) {
    std::print(stderr, "Error while sending data: {}\n",
               static_cast<int>(status));
    return EXIT_FAILURE;
  }
  std::println("Resent back. Bye!\n");
}