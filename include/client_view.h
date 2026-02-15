/**
 * @file client_view.h
 * @brief The **View** in the client's MVC pattern -- everything related to
 *        rendering and user input.
 *
 * Uses SDL3 for window management and ImGui for the immediate-mode GUI.
 * The View has NO knowledge of networking or application logic; it simply:
 *  - Draws widgets (text fields, buttons, message list).
 *  - Returns user actions (button clicked, text entered) to the Controller.
 *
 * For a game you would add your game rendering here (sprites, board, etc.)
 * while keeping the game logic in the Model.
 */

#ifndef CLIENT_VIEW_H_
#define CLIENT_VIEW_H_

#include <span>
#include <string>

// Forward declarations so we don't need to include the full SDL headers here.
struct SDL_Window;
struct SDL_Renderer;

class ClientView {
 public:
  /// Create the SDL window, renderer, and ImGui context.
  [[nodiscard]] bool Init();

  /// Tear down ImGui + SDL resources in the reverse order of Init().
  void Shutdown();

  /// Start a new frame: poll OS events, begin ImGui frame, set up main window.
  void BeginFrame();

  /// Finish the frame: render ImGui draw data and present to screen.
  void EndFrame();

  /**
   * @brief Draw the "connect to server" panel (shown before connection).
   * @param address [in/out] The server address text field.
   * @param port    [in/out] The port number input field.
   * @return true if the user clicked the "Connect" button this frame.
   */
  bool DrawConnectionPanel(std::string& address, unsigned short& port);

  /**
   * @brief Draw the chat panel (shown after connection).
   * @param messages    The list of received messages to display.
   * @param sendMessage [in/out] The text field for composing a new message.
   * @return true if the user clicked the "Send" button this frame.
   */
  bool DrawChatPanel(std::span<const std::string> messages,
                     std::string& sendMessage);

  /// @return true if the user closed the window (SDL_EVENT_QUIT received).
  [[nodiscard]] bool ShouldQuit() const;

 private:
  SDL_Window* window_ = nullptr;
  SDL_Renderer* renderer_ = nullptr;
  bool shouldQuit_ = false;  ///< Set to true when the window close event fires.
};

#endif  // CLIENT_VIEW_H_
