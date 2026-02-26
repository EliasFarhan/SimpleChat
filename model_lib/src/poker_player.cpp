//
// Created by unite on 26.02.2026.
//

#include "poker_player.h"

#include <algorithm>

poker::PlayerStatus poker::PokerPlayer::GiveCard(const Card& new_card) {
  if (cardCount_ >= kMaxPlayerCardCount) {
    return PlayerStatus::TOO_MANY_CARDS;
  }
  const auto cards = this->cards();

  if (std::ranges::any_of(cards, [&new_card](const Card& card) {return card == new_card;})) {
    return PlayerStatus::SAME_CARD;
  }

  for (const auto& card : cards) {
    if (card == new_card) {
      return PlayerStatus::SAME_CARD;
    }
  }

  cards_[cardCount_] = new_card;
  cardCount_++;
  return PlayerStatus::VALID;
}
std::span<const poker::Card> poker::PokerPlayer::cards() const {
  return std::span<const poker::Card>{cards_.data(), cardCount_};
}
