#include <memory>

#include "client_controller.h"
#include "client_view.h"

int main() {
  ClientController controller(std::make_unique<ClientView>());
  controller.Run();
}
