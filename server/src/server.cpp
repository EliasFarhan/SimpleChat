/**
 * @file server.cpp
 * @brief Entry point for the chat server.
 *
 * The server is a headless (no GUI) process.  It simply:
 *  1. Creates a ChatServer and starts listening on PORT_NUMBER.
 *  2. Runs an infinite loop calling Update(), which accepts new clients,
 *     cleans up disconnected ones, and relays messages.
 *
 * To run: launch this executable first, then start one or more clients.
 */

#include <iostream>

#include "chat_server.h"
#include "const.h"

int main() {
  ChatServer server;
  if (!server.Start(PORT_NUMBER)) {
    return EXIT_FAILURE;
  }
  // Server main loop -- runs forever until the process is killed (Ctrl+C).
  while (true) {
    server.Update();
  }
}
