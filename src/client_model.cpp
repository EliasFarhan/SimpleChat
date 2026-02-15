#include "client_model.h"

void ClientModel::Connect(const std::string& host, unsigned short port) {
  client_.Connect(host, port);
}

void ClientModel::SendMessage(const std::string& message) {
  client_.Send(message);
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
