//
// Created by unite on 26.02.2026.
//

#ifndef SIMPLECHAT_POKER_PLAYER_H
#define SIMPLECHAT_POKER_PLAYER_H
#include <array>
#include <span>
#include <cstdint>

#include "poker_card.h"

namespace poker {

enum class PlayerStatus {
  VALID,
  TOO_MANY_CARDS,
  SAME_CARD, //when trying to give exactly the same card to the player
};

class PokerPlayer {
public:
  PlayerStatus GiveCard(const Card &new_card);
  [[nodiscard]] std::span<const Card> cards() const;
private:
  static constexpr size_t kMaxPlayerCardCount = 2;
  std::array<Card, kMaxPlayerCardCount> cards_{};
  size_t cardCount_ = 0;

};
}
#endif  // SIMPLECHAT_POKER_PLAYER_H
