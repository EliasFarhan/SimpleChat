//
// Created by efarhan on 2/27/26.
//

#include <gtest/gtest.h>

#include "poker.h"

TEST(Poker, ConstructorSetupCards) {
  poker::Poker poker;
  EXPECT_EQ(poker.available_cards().size(), 52);
  for (const auto& card : poker.available_cards()) {
    EXPECT_FALSE(card.type == poker::CardType::kInvalid);
    EXPECT_FALSE(card.color == poker::CardColor::kInvalid);
  }
}

TEST(Poker, SimpleDuel) {
  poker::Poker poker;
  const auto p1 = poker.AddNewPlayer();
  const auto p2 = poker.AddNewPlayer();
  poker.StartGame();

  const auto p1_cards = poker.GetPlayerAt(p1).cards();
  EXPECT_EQ(p1_cards.size(), 2);
  for (const auto& card : p1_cards) {
    EXPECT_FALSE(card.type == poker::CardType::kInvalid);
    EXPECT_FALSE(card.color == poker::CardColor::kInvalid);
  }
  const auto p2_cards = poker.GetPlayerAt(p2).cards();
  EXPECT_EQ(p2_cards.size(), 2);

  for (const auto& card : p1_cards) {
    EXPECT_FALSE(card.type == poker::CardType::kInvalid);
    EXPECT_FALSE(card.color == poker::CardColor::kInvalid);
  }

  for (const auto& c1 : p1_cards) {
    for (const auto& c2 : p2_cards) {
      EXPECT_FALSE(c1 == c2);
    }
  }

}