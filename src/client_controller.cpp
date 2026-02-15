#include "client_controller.h"

#include "const.h"

void ClientController::Run() {
  view_.Init();
  serverAddress_.resize(50, 0);
  sendMessage_.resize(MAX_MESSAGE_LENGTH, 0);

  while (!view_.ShouldQuit()) {
    view_.BeginFrame();
    if (!model_.IsConnected()) {
      if (view_.DrawConnectionPanel(serverAddress_, portNumber_)) {
        model_.Connect(serverAddress_,
                       static_cast<unsigned short>(portNumber_));
      }
    } else {
      model_.PollMessages();
      if (view_.DrawChatPanel(model_.GetMessages(), sendMessage_)) {
        model_.SendMessage(sendMessage_);
      }
    }
    view_.EndFrame();
  }

  view_.Shutdown();
}
