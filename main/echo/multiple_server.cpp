#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <array>
#include <print>

#include "SFML/Network/SocketSelector.hpp"

constexpr size_t kMaxMsgLength = 100;
constexpr uint16_t kPort = 53000;

int main() {
  sf::TcpListener listener;
  listener.setBlocking(false);
  std::vector<sf::TcpSocket> clientSockets;
  sf::SocketSelector selector;
  // bind the listener to a port
  if (const auto status = listener.listen(kPort);
      status != sf::Socket::Status::Done) {
    std::print(stderr, "Error while listening status: {}\n",
               static_cast<int>(status));
    return EXIT_FAILURE;
  }

  while (true) {
    // accept a new connection
    {
      sf::TcpSocket newClient;
      newClient.setBlocking(false);
      if (listener.accept(newClient) == sf::Socket::Status::Done) {
        std::print("Client at {}:{} connected!\n",
                   newClient.getRemoteAddress()->toString(),
                   newClient.getRemotePort());
        newClient.setBlocking(true);
        selector.add(newClient);
        clientSockets.push_back(std::move(newClient));
      }
    }

    if (selector.wait(sf::milliseconds(100))) {
      for (int64_t i = std::ssize(clientSockets) - 1; i >= 0; --i) {
        auto& socket = clientSockets[i];
        if (selector.isReady(socket)) {
          // Receiving data
          std::array<char, kMaxMsgLength> receive_data{};
          size_t received;

          // TCP socket:
          if (const auto status =
                  socket.receive(receive_data.data(), kMaxMsgLength, received);
              status != sf::Socket::Status::Done) {
            if (status == sf::Socket::Status::Disconnected) {
              selector.remove(socket);
              clientSockets.erase(clientSockets.begin() + i);
            } else {
              std::print(stderr,
                         "Error while receiving data from {}:{} status: {}, "
                         "local port: {}\n",
                         socket.getRemoteAddress().has_value()
                             ? socket.getRemoteAddress()->toString()
                             : "disconnected",
                         socket.getRemotePort(), static_cast<int>(status),
                         socket.getLocalPort());
            }
            continue;
          }

          std::print("Received data: {}\n", receive_data.data());

          if (const auto status = socket.send(receive_data.data(), received);
              status != sf::Socket::Status::Done) {
            std::print(stderr, "Error while sending data: {}\n",
                       static_cast<int>(status));
          }
        }
      }
    }

    std::fflush(stdout);
  }
}