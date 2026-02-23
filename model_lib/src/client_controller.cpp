#include "client_controller.h"

#include <print>
#include <utility>

ClientController::ClientController(ClientModelInterface& model,
                                   std::unique_ptr<ClientViewInterface> view)
    : model_(model), view_(std::move(view)) {}

void ClientController::Run() {
  if (!view_->Init()) {
    return;
  }

  while (!view_->ShouldQuit()) {
    view_->BeginFrame();

    if (!model_.IsConnected()) {
      if (view_->DrawConnectionPanel(serverAddress_, portNumber_)) {
        if (!model_.Connect(serverAddress_, portNumber_)) {
          std::print(stderr, "Failed to connect to {}:{}\n", serverAddress_, portNumber_);
        }
      }
    } else {
      model_.PollMessages();
      if (view_->DrawChatPanel(model_.GetMessages(), sendMessage_)) {
        if (!model_.SendMessage(sendMessage_)) {
          std::print(stderr, "Failed to send message\n");
        }
      }
    }

    view_->EndFrame();
  }

  view_->Shutdown();
}
