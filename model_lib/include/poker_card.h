//
// Created by unite on 26.02.2026.
//

#ifndef SIMPLECHAT_POKER_CARD_H
#define SIMPLECHAT_POKER_CARD_H

namespace poker {
enum class CardType {
  k2,
  k3,
  k4,
  k5,
  k6,
  k7,
  k8,
  k9,
  k10,
  kJack,
  kQueen,
  kKing,
  kAce,
  kInvalid
};

enum class CardColor {
  kClub,
  kHeart,
  kSpades,
  kDiamonds,
  kInvalid
};

struct Card {
  CardType type = CardType::kInvalid;
  CardColor color = CardColor::kInvalid;

  constexpr bool operator==(const Card &other) const {
    return type == other.type && color == other.color;
  }
};
}
#endif  // SIMPLECHAT_POKER_CARD_H
