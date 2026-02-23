/**
 * @file const.h
 * @brief Shared constants used by both the server and the client.
 *
 * This header is included on both sides so that the client and the server
 * agree on the same network parameters.  When you adapt the project for
 * your game you can add your own shared constants here (e.g. max player
 * count, tick rate, game-specific limits, etc.).
 */

#ifndef SIMPLE_CHAT_CONST_H
#define SIMPLE_CHAT_CONST_H

#include <cstdint>

/// Maximum number of bytes in a single chat message (or game packet).
inline constexpr std::size_t MAX_MESSAGE_LENGTH = 150;

/// TCP port the server listens on and the client connects to.
/// Make sure this port is not already in use on your machine.
inline constexpr std::uint16_t PORT_NUMBER = 4533;

#endif  // SIMPLE_CHAT_CONST_H
