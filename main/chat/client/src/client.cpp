#include <memory>

#include "chat_client.h"
#include "client_controller.h"
#include "client_model.h"
#include "client_view.h"

int main() {
  ChatClient chatClient;
  ClientModel model(chatClient);
  ClientController controller(model, std::make_unique<ClientView>());
  controller.Run();
}
