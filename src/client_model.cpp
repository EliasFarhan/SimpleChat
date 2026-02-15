/**
 * @file client_model.cpp
 * @brief Implementation of the client Model (data layer).
 *
 * Every method here is a thin wrapper around ChatClient, plus storage for
 * the received messages.  The Model isolates the Controller from low-level
 * networking details.
 */

#include "client_model.h"

bool ClientModel::Connect(std::string_view host, unsigned short port) {
  return client_.Connect(host, port);
}

bool ClientModel::SendMessage(std::string_view message) {
  return client_.Send(message);
}

void ClientModel::PollMessages() {
  // The socket is non-blocking, so Receive() returns std::nullopt when
  // there is nothing to read.  We loop until the socket has no more data.
  while (auto msg = client_.Receive()) {
    receivedMessages_.push_back(std::move(*msg));
  }
}

const std::vector<std::string>& ClientModel::GetMessages() const {
  return receivedMessages_;
}

bool ClientModel::IsConnected() const { return client_.IsConnected(); }
