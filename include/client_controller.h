#ifndef CLIENT_CONTROLLER_H_
#define CLIENT_CONTROLLER_H_

#include "client_model.h"
#include "client_view.h"
#include "const.h"

class ClientController {
 public:
  void Run();

 private:
  ClientModel model_;
  ClientView view_;
  std::string serverAddress_ = "localhost";
  unsigned short portNumber_ = PORT_NUMBER;
  std::string sendMessage_;
};

#endif  // CLIENT_CONTROLLER_H_
