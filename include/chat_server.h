/**
 * @file chat_server.h
 * @brief TCP chat server that accepts multiple clients and broadcasts messages.
 *
 * Architecture overview:
 *  1. A **TcpListener** listens for incoming connections on a given port.
 *  2. Each accepted client gets its own **TcpSocket** stored in a vector.
 *  3. A **SocketSelector** efficiently monitors all connected sockets so we
 *     only try to read from sockets that actually have data ready.
 *  4. When a message arrives from any client, the server **broadcasts** it
 *     to every connected client (including the sender).
 *
 * For a turn-based game you would replace the broadcast with game-specific
 * logic: validate the move, update the game state, then send the new state
 * (or a delta) to all players.
 */

#ifndef CHAT_SERVER_H_
#define CHAT_SERVER_H_

#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <optional>
#include <vector>

class ChatServer {
 public:
  /**
   * @brief Start listening for incoming connections on @p port.
   * @return true if the listener was set up successfully.
   */
  [[nodiscard]] bool Start(unsigned short port);

  /**
   * @brief Run one server tick: clean up, accept new clients, relay messages.
   *
   * This is called in an infinite loop from main().  In a game you would
   * also update the game simulation here.
   */
  void Update();

 private:
  /// Accept any pending client connections from the listener.
  void AcceptNewConnections();

  /// Remove sockets that have been disconnected since the last tick.
  void CleanDisconnected();

  /// Read incoming data from ready sockets and broadcast to all clients.
  void HandleMessages();

  void RemoveSocketAt(int64_t index);

  sf::TcpListener listener_;  ///< Listens for new TCP connections.
  sf::SocketSelector socketSelector_;  ///< Watches multiple sockets for readiness.

  /**
   * Connected client sockets.
   * Slots may be nullopt after a client disconnects; they get reused by
   * the next connecting client.
   */
  std::vector<sf::TcpSocket> sockets_;
};

#endif  // CHAT_SERVER_H_
