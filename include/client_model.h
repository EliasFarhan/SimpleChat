/**
 * @file client_model.h
 * @brief The **Model** in the client's MVC (Model-View-Controller) pattern.
 *
 * The Model owns the application data and business logic.  Here it:
 *  - Wraps the low-level ChatClient (networking).
 *  - Stores all received chat messages in a vector.
 *  - Exposes a simple interface that the Controller can call without
 *    knowing any networking details.
 *
 * For a game you would extend this class with game state (board, scores,
 * current turn, etc.) and replace chat messages with game events.
 */

#ifndef CLIENT_MODEL_H_
#define CLIENT_MODEL_H_

#include <string>
#include <string_view>
#include <vector>

#include "chat_client.h"

class ClientModel {
 public:
  /// Connect to the server at the given address and port.
  [[nodiscard]] bool Connect(std::string_view host, unsigned short port);

  /// Send a chat message (or game action) to the server.
  [[nodiscard]] bool SendMessage(std::string_view message);

  /// Read all available messages from the network and store them.
  void PollMessages();

  /// Get the full list of received messages (read-only).
  [[nodiscard]] const std::vector<std::string>& GetMessages() const;

  /// Check whether we are still connected to the server.
  [[nodiscard]] bool IsConnected() const;

 private:
  ChatClient client_;  ///< Low-level network connection.
  std::vector<std::string> receivedMessages_;  ///< Chat history.
};

#endif  // CLIENT_MODEL_H_
