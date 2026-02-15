#ifndef CLIENT_CONTROLLER_H_
#define CLIENT_CONTROLLER_H_

#include <memory>

#include "client_model.h"
#include "client_view_interface.h"
#include "const.h"

class ClientController {
 public:
  explicit ClientController(std::unique_ptr<ClientViewInterface> view);

  void Run();

 private:
  ClientModel model_;
  std::unique_ptr<ClientViewInterface> view_;

  std::string serverAddress_ = "localhost";
  unsigned short portNumber_ = PORT_NUMBER;
  std::string sendMessage_;
};

#endif  // CLIENT_CONTROLLER_H_
