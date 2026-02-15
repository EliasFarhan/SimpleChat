#include "client_model.h"

bool ClientModel::Connect(std::string_view host, unsigned short port) {
  return client_.Connect(host, port);
}

bool ClientModel::SendMessage(std::string_view message) {
  return client_.Send(message);
}

void ClientModel::PollMessages() {
  while (auto msg = client_.Receive()) {
    receivedMessages_.push_back(std::move(*msg));
  }
}

const std::vector<std::string>& ClientModel::GetMessages() const {
  return receivedMessages_;
}

bool ClientModel::IsConnected() const { return client_.IsConnected(); }
