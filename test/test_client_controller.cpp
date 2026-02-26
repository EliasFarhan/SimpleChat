#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <span>
#include <string>
#include <vector>

#include "client_controller.h"
#include "client_model_interface.h"
#include "client_view_interface.h"

class MockClientModel : public ClientModelInterface {
 public:
  MOCK_METHOD(bool, Connect, (std::string_view host, unsigned short port),
              (override));
  MOCK_METHOD(bool, SendMessage, (std::string_view message), (override));
  MOCK_METHOD(void, PollMessages, (), (override));
  MOCK_METHOD(std::span<const std::string>, messages, (),
              (const, override));
  MOCK_METHOD(bool, IsConnected, (), (const, override));
};

class MockClientView : public ClientViewInterface {
 public:
  MOCK_METHOD(bool, Init, (), (override));
  MOCK_METHOD(void, Shutdown, (), (override));
  MOCK_METHOD(void, BeginFrame, (), (override));
  MOCK_METHOD(void, EndFrame, (), (override));
  MOCK_METHOD(bool, DrawConnectionPanel,
              (std::string & address, unsigned short& port), (override));
  MOCK_METHOD(bool, DrawChatPanel,
              (std::span<const std::string> messages, std::string& sendMessage),
              (override));
  MOCK_METHOD(bool, ShouldQuit, (), (const, override));
};

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

TEST(ClientControllerTest, InitFailureStopsImmediately) {
  MockClientModel model;
  auto view = std::make_unique<MockClientView>();
  auto* viewPtr = view.get();

  EXPECT_CALL(*viewPtr, Init()).WillOnce(Return(false));
  // Should NOT call Shutdown, ShouldQuit, etc.
  EXPECT_CALL(*viewPtr, Shutdown()).Times(0);
  EXPECT_CALL(*viewPtr, ShouldQuit()).Times(0);

  ClientController controller(model, std::move(view));
  controller.Run();
}

TEST(ClientControllerTest, ImmediateQuit) {
  MockClientModel model;
  auto view = std::make_unique<MockClientView>();
  auto* viewPtr = view.get();

  EXPECT_CALL(*viewPtr, Init()).WillOnce(Return(true));
  EXPECT_CALL(*viewPtr, ShouldQuit()).WillOnce(Return(true));
  EXPECT_CALL(*viewPtr, Shutdown()).Times(1);

  ClientController controller(model, std::move(view));
  controller.Run();
}

TEST(ClientControllerTest, ConnectionFlowConnectsModel) {
  MockClientModel model;
  auto view = std::make_unique<MockClientView>();
  auto* viewPtr = view.get();

  EXPECT_CALL(*viewPtr, Init()).WillOnce(Return(true));

  // First iteration: not connected, draw connection panel -> user clicks
  // connect Second iteration: quit
  EXPECT_CALL(*viewPtr, ShouldQuit())
      .WillOnce(Return(false))
      .WillOnce(Return(true));
  EXPECT_CALL(*viewPtr, BeginFrame()).Times(1);
  EXPECT_CALL(*viewPtr, EndFrame()).Times(1);

  EXPECT_CALL(model, IsConnected()).WillOnce(Return(false));
  EXPECT_CALL(*viewPtr, DrawConnectionPanel(_, _)).WillOnce(Return(true));
  EXPECT_CALL(model, Connect(_, _)).WillOnce(Return(true));

  EXPECT_CALL(*viewPtr, Shutdown()).Times(1);

  ClientController controller(model, std::move(view));
  controller.Run();
}

TEST(ClientControllerTest, ChatSendFlowSendsMessage) {
  MockClientModel model;
  auto view = std::make_unique<MockClientView>();
  auto* viewPtr = view.get();
  std::vector<std::string> messages;

  EXPECT_CALL(*viewPtr, Init()).WillOnce(Return(true));
  EXPECT_CALL(*viewPtr, ShouldQuit())
      .WillOnce(Return(false))
      .WillOnce(Return(true));
  EXPECT_CALL(*viewPtr, BeginFrame()).Times(1);
  EXPECT_CALL(*viewPtr, EndFrame()).Times(1);

  EXPECT_CALL(model, IsConnected()).WillOnce(Return(true));
  EXPECT_CALL(model, PollMessages()).Times(1);
  EXPECT_CALL(model, messages()).WillOnce(Return(messages));
  EXPECT_CALL(*viewPtr, DrawChatPanel(_, _)).WillOnce(Return(true));
  EXPECT_CALL(model, SendMessage(_)).WillOnce(Return(true));

  EXPECT_CALL(*viewPtr, Shutdown()).Times(1);

  ClientController controller(model, std::move(view));
  controller.Run();
}
