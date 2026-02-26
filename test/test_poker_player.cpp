//
// Created by unite on 26.02.2026.
//


#include <gtest/gtest.h>

#include "poker_player.h"

TEST(PokerCard, GiveCardsShouldPutStored) {
  poker::PokerPlayer player;
  EXPECT_TRUE(player.cards().empty());

  static constexpr poker::Card c1{.type = poker::CardType::k6, .color = poker::CardColor::kHeart};
  static constexpr poker::Card c2{.type = poker::CardType::k7, .color = poker::CardColor::kHeart};

  player.GiveCard(c1);
  player.GiveCard(c2);

  ASSERT_EQ(player.cards().size(), 2);
  EXPECT_EQ(player.cards()[0], c1);
  EXPECT_EQ(player.cards()[1], c2);

}

TEST(PokerCard, ShouldNotGiveMoreThanTwoCards) {
  poker::PokerPlayer player;
  EXPECT_TRUE(player.cards().empty());

  static constexpr poker::Card c1{.type = poker::CardType::k6, .color = poker::CardColor::kHeart};
  static constexpr poker::Card c2{.type = poker::CardType::k7, .color = poker::CardColor::kHeart};
  static constexpr poker::Card c3{.type = poker::CardType::k8, .color = poker::CardColor::kHeart};

  poker::PlayerStatus status = player.GiveCard(c1);
  EXPECT_EQ(status, poker::PlayerStatus::VALID);
  status = player.GiveCard(c2);
  EXPECT_EQ(status, poker::PlayerStatus::VALID);
  status = player.GiveCard(c3);
  EXPECT_EQ(status, poker::PlayerStatus::TOO_MANY_CARDS);

  ASSERT_EQ(player.cards().size(), 2);
  EXPECT_EQ(player.cards()[0], c1);
  EXPECT_EQ(player.cards()[1], c2);

}

TEST(PokerCard, CantGiveTheSameCard) {
  poker::PokerPlayer player;
  EXPECT_TRUE(player.cards().empty());

  static constexpr poker::Card c1{.type = poker::CardType::k6, .color = poker::CardColor::kHeart};
  static constexpr poker::Card c2{.type = poker::CardType::k6, .color = poker::CardColor::kHeart};

  poker::PlayerStatus status = player.GiveCard(c1);
  EXPECT_EQ(status, poker::PlayerStatus::VALID);
  status = player.GiveCard(c2);
  EXPECT_EQ(status, poker::PlayerStatus::SAME_CARD);

  ASSERT_EQ(player.cards().size(), 1);
  EXPECT_EQ(player.cards()[0], c1);

}