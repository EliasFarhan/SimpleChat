#include "client_controller.h"

#include <iostream>

void ClientController::Run() {
  if (!view_.Init()) {
    return;
  }

  while (!view_.ShouldQuit()) {
    view_.BeginFrame();
    if (!model_.IsConnected()) {
      if (view_.DrawConnectionPanel(serverAddress_, portNumber_)) {
        if (!model_.Connect(serverAddress_, portNumber_)) {
          std::cerr << "Failed to connect to " << serverAddress_ << ":"
                    << portNumber_ << "\n";
        }
      }
    } else {
      model_.PollMessages();
      if (view_.DrawChatPanel(model_.GetMessages(), sendMessage_)) {
        if (!model_.SendMessage(sendMessage_)) {
          std::cerr << "Failed to send message\n";
        }
      }
    }
    view_.EndFrame();
  }

  view_.Shutdown();
}
