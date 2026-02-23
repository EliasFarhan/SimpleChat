#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "chat_client_interface.h"
#include "client_model.h"

class MockChatClient : public ChatClientInterface {
 public:
  MOCK_METHOD(bool, Connect, (std::string_view host, unsigned short port),
              (override));
  MOCK_METHOD(bool, Send, (std::string_view message), (override));
  MOCK_METHOD(std::optional<std::string>, Receive, (), (override));
  MOCK_METHOD(bool, IsConnected, (), (const, override));
  MOCK_METHOD(void, Disconnect, (), (override));
};

using ::testing::Return;

static constexpr size_t kMessageCount = 2u;

TEST(ClientModelTest, ConnectDelegatesToClient) {
  static constexpr auto kHost = "localhost";
  static constexpr unsigned short kPort = 4533;

  MockChatClient client;
  ClientModel model(client);

  EXPECT_CALL(client, Connect(kHost, kPort)).WillOnce(Return(true));
  EXPECT_TRUE(model.Connect(kHost, kPort));
}

TEST(ClientModelTest, ConnectFailurePropagated) {
  static constexpr auto kHost = "badhost";
  static constexpr unsigned short kPort = 9999;

  MockChatClient client;
  ClientModel model(client);

  EXPECT_CALL(client, Connect(kHost, kPort)).WillOnce(Return(false));
  EXPECT_FALSE(model.Connect(kHost, kPort));
}

TEST(ClientModelTest, SendMessageDelegatesToClient) {
  static constexpr auto kMessage = "hello";

  MockChatClient client;
  ClientModel model(client);

  EXPECT_CALL(client, Send(kMessage)).WillOnce(Return(true));
  EXPECT_TRUE(model.SendMessage(kMessage));
}

TEST(ClientModelTest, PollMessagesAccumulatesMessages) {
  static constexpr auto kMsg1 = "msg1";
  static constexpr auto kMsg2 = "msg2";

  MockChatClient client;
  ClientModel model(client);

  EXPECT_CALL(client, Receive())
      .WillOnce(Return(std::optional<std::string>(kMsg1)))
      .WillOnce(Return(std::optional<std::string>(kMsg2)))
      .WillOnce(Return(std::nullopt));

  model.PollMessages();

  const auto& msgs = model.GetMessages();
  ASSERT_EQ(msgs.size(), kMessageCount);
  EXPECT_EQ(msgs[0], kMsg1);
  EXPECT_EQ(msgs[1], kMsg2);
}

TEST(ClientModelTest, PollMessagesAppendsAcrossCalls) {
  static constexpr auto kMsg1 = "first";
  static constexpr auto kMsg2 = "second";

  MockChatClient client;
  ClientModel model(client);

  EXPECT_CALL(client, Receive())
      .WillOnce(Return(std::optional<std::string>(kMsg1)))
      .WillOnce(Return(std::nullopt))
      .WillOnce(Return(std::optional<std::string>(kMsg2)))
      .WillOnce(Return(std::nullopt));

  model.PollMessages();
  model.PollMessages();

  const auto& msgs = model.GetMessages();
  ASSERT_EQ(msgs.size(), kMessageCount);
  EXPECT_EQ(msgs[0], kMsg1);
  EXPECT_EQ(msgs[1], kMsg2);
}

TEST(ClientModelTest, IsConnectedDelegatesToClient) {
  MockChatClient client;
  ClientModel model(client);

  EXPECT_CALL(client, IsConnected()).WillOnce(Return(true));
  EXPECT_TRUE(model.IsConnected());

  EXPECT_CALL(client, IsConnected()).WillOnce(Return(false));
  EXPECT_FALSE(model.IsConnected());
}
