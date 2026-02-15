/**
 * @file client.cpp
 * @brief Entry point for the chat client (GUI application).
 *
 * All the work is delegated to ClientController::Run(), which owns the
 * main loop, the MVC components, and the SDL/ImGui lifecycle.
 * This file is intentionally minimal -- the MVC pattern keeps each
 * responsibility in its own class.
 */

#include "client_controller.h"

int main() {
  ClientController controller;
  controller.Run();
}
