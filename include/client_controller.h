/**
 * @file client_controller.h
 * @brief The **Controller** in the client's MVC pattern.
 *
 * The Controller is the glue between the Model (data + networking) and the
 * View (rendering + input).  Its Run() method contains the main loop:
 *
 *  1. Begin a new frame (poll OS events).
 *  2. If not connected  --> draw the connection panel and attempt to connect.
 *     If connected      --> poll incoming messages, draw the chat panel,
 *                           and send messages when the user clicks "Send".
 *  3. End the frame (render + present).
 *
 * For a game you would add your game-loop logic here (e.g. handle turns,
 * process server responses, transition between game screens, etc.).
 */

#ifndef CLIENT_CONTROLLER_H_
#define CLIENT_CONTROLLER_H_

#include "client_model.h"
#include "client_view.h"
#include "const.h"

class ClientController {
 public:
  /// Enter the main loop; returns when the user closes the window.
  void Run();

 private:
  ClientModel model_;   ///< Owns the network connection and received data.
  ClientView view_;     ///< Owns the SDL window and ImGui rendering.

  std::string serverAddress_ = "localhost";  ///< Default server address.
  unsigned short portNumber_ = PORT_NUMBER;  ///< Default port from const.h.
  std::string sendMessage_;  ///< Text currently typed in the "Send" field.
};

#endif  // CLIENT_CONTROLLER_H_
