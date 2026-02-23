# Client library (view + networking)
add_library(client_lib STATIC
  src/client_view.cpp
  src/chat_client.cpp
)
target_include_directories(client_lib PUBLIC include)
target_include_directories(client_lib SYSTEM PUBLIC
  ${CMAKE_SOURCE_DIR}/externals/SDL/include
)
target_link_libraries(client_lib PUBLIC model_lib SFML::Network imgui::imgui SDL3::SDL3)
target_compile_options(client_lib PRIVATE ${PROJECT_WARNING_FLAGS})

# Client executable
add_executable(chat_client src/client.cpp)
target_link_libraries(chat_client PRIVATE client_lib)
target_compile_options(chat_client PRIVATE ${PROJECT_WARNING_FLAGS})
