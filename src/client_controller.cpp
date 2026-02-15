/**
 * @file client_controller.cpp
 * @brief Implementation of the client Controller (main loop + glue logic).
 *
 * The Controller orchestrates the client application:
 *  - Initialises the View (SDL + ImGui).
 *  - Each frame, decides which screen to show based on the connection state.
 *  - Forwards user actions (connect, send) to the Model.
 *  - Shuts down the View when the user closes the window.
 */

#include "client_controller.h"

#include <iostream>

void ClientController::Run() {
  // Initialise SDL and ImGui.  If this fails there is nothing we can do.
  if (!view_.Init()) {
    return;
  }

  // ---------- Main loop ----------
  // Runs once per frame until the user closes the window.
  while (!view_.ShouldQuit()) {
    view_.BeginFrame();

    if (!model_.IsConnected()) {
      // --- Connection screen ---
      // Draw address/port inputs and a Connect button.
      // When the button is pressed, attempt to connect to the server.
      if (view_.DrawConnectionPanel(serverAddress_, portNumber_)) {
        if (!model_.Connect(serverAddress_, portNumber_)) {
          std::cerr << "Failed to connect to " << serverAddress_ << ":"
                    << portNumber_ << "\n";
        }
      }
    } else {
      // --- Chat screen ---
      // 1. Poll any new messages that arrived from the server.
      model_.PollMessages();
      // 2. Draw the message list and the send box.
      //    If the user clicks "Send", forward the message to the server.
      if (view_.DrawChatPanel(model_.GetMessages(), sendMessage_)) {
        if (!model_.SendMessage(sendMessage_)) {
          std::cerr << "Failed to send message\n";
        }
      }
    }

    view_.EndFrame();
  }

  // Clean up SDL and ImGui resources.
  view_.Shutdown();
}
