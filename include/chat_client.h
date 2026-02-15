/**
 * @file chat_client.h
 * @brief Low-level TCP client that wraps an SFML TcpSocket.
 *
 * This class handles the raw networking: connecting to a server, sending
 * bytes, and receiving bytes.  It does NOT know anything about the UI;
 * the MVC layer (ClientModel) uses it to talk to the server.
 *
 * Key networking concepts used here:
 *  - **Blocking vs non-blocking sockets**: The socket is set to blocking
 *    only during the initial connect() call so we wait until the connection
 *    is established.  After that it is switched to non-blocking so that
 *    Receive() returns immediately when there is no data, avoiding freezing
 *    the UI.
 *  - **std::optional for Receive()**: Because the socket is non-blocking,
 *    a receive call may have nothing to return.  We use std::optional to
 *    express "maybe a message, maybe nothing".
 */

#ifndef CHAT_CLIENT_H_
#define CHAT_CLIENT_H_

#include <SFML/Network/TcpSocket.hpp>
#include <optional>
#include <string>
#include <string_view>

/// Simple enum to track whether we are currently connected to a server.
enum class ConnectionStatus { NOT_CONNECTED, CONNECTED };

class ChatClient {
 public:
  /**
   * @brief Resolve the host name and open a TCP connection to the server.
   * @return true on success, false on failure (address not found, refused, etc.)
   */
  [[nodiscard]] bool Connect(std::string_view host, unsigned short port);

  /**
   * @brief Send a text message to the server (up to MAX_MESSAGE_LENGTH bytes).
   * @return true if the whole message was sent successfully.
   */
  [[nodiscard]] bool Send(std::string_view message);

  /**
   * @brief Try to receive a message from the server (non-blocking).
   * @return The received string, or std::nullopt if nothing is available yet.
   */
  [[nodiscard]] std::optional<std::string> Receive();

  /// @return true if we believe the connection is still alive.
  [[nodiscard]] bool IsConnected() const;

  /// Gracefully close the connection.
  void Disconnect();

 private:
  sf::TcpSocket socket_;  ///< The underlying SFML TCP socket.
  ConnectionStatus status_ = ConnectionStatus::NOT_CONNECTED;
};

#endif  // CHAT_CLIENT_H_
