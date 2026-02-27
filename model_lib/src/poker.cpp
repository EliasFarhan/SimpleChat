//
// Created by efarhan on 2/27/26.
//

#include "poker.h"

namespace poker {

Poker::Poker() {
  available_cards_.reserve(52);
  for (int color = 0; color < static_cast<int>(CardColor::kInvalid); color++) {
    for (int type = 0; type < static_cast<int>(CardType::kInvalid); type++) {
      available_cards_.emplace_back(static_cast<CardType>(type), static_cast<CardColor>(color));
    }
  }
}
PlayerIndex Poker::AddNewPlayer() {
  PlayerIndex new_player_index{std::ssize(players_)};
  players_.emplace_back();
  return new_player_index;
}
void Poker::StartGame() {

}
}  // namespace poker