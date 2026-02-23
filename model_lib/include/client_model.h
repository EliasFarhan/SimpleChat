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

#include "chat_client_interface.h"
#include "client_model_interface.h"

class ClientModel : public ClientModelInterface {
 public:
  explicit ClientModel(ChatClientInterface& client);

  /// Connect to the server at the given address and port.
  [[nodiscard]] bool Connect(std::string_view host,
                             unsigned short port) override;

  /// Send a chat message (or game action) to the server.
  [[nodiscard]] bool SendMessage(std::string_view message) override;

  /// Read all available messages from the network and store them.
  void PollMessages() override;

  /// Get the full list of received messages (read-only).
  [[nodiscard]] std::span<const std::string> GetMessages() const override;

  /// Check whether we are still connected to the server.
  [[nodiscard]] bool IsConnected() const override;

 private:
  ChatClientInterface& client_;  ///< Low-level network connection.
  std::vector<std::string> receivedMessages_;  ///< Chat history.
};

#endif  // CLIENT_MODEL_H_
