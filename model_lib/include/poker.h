//
// Created by efarhan on 2/27/26.
//

#ifndef SIMPLECHAT_POKER_H
#define SIMPLECHAT_POKER_H

#include <expected>
#include <span>
#include <vector>

#include "poker_player.h"

namespace poker {

class PlayerIndex {
 public:
  static constexpr auto kInvalidIndex = -1;

  explicit PlayerIndex(int64_t index) : index_(index) {}

  [[nodiscard]] constexpr size_t index() const noexcept {
    return static_cast<size_t>(index_);
  }

 private:
  int64_t index_ = kInvalidIndex;
};

enum class GameStatus { VALID, TOO_MANY_ROUNDS };

class Poker {
 public:
  Poker();
  [[nodiscard]] constexpr std::span<const Card> available_cards()
      const noexcept {
    return available_cards_;
  }

  [[nodiscard]] PlayerIndex AddNewPlayer();

  [[nodiscard]] constexpr std::span<const Card> cards_on_the_board()
      const noexcept {
    return {available_cards_.data(), cards_count_on_the_board_};
  }

  void StartGame();

  [[nodiscard]] const PokerPlayer& GetPlayerAt(PlayerIndex player_index) const {
    return players_.at(player_index.index());
  }

 private:
  std::vector<PokerPlayer> players_;
  std::vector<Card> available_cards_;
  std::array<Card, 5> cards_on_the_board_;
  size_t cards_count_on_the_board_ = 0;
};
}  // namespace poker
#endif  // SIMPLECHAT_POKER_H
