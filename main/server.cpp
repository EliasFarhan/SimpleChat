#include <iostream>

#include "chat_server.h"
#include "const.h"

int main() {
  ChatServer server;
  if (!server.Start(PORT_NUMBER)) {
    return EXIT_FAILURE;
  }
  while (true) {
    server.Update();
  }
}
