# Server networking library
add_library(server_lib STATIC
  src/chat_server.cpp
)
target_include_directories(server_lib PUBLIC include)
target_link_libraries(server_lib PUBLIC model_lib SFML::Network)
target_compile_options(server_lib PRIVATE ${PROJECT_WARNING_FLAGS})

add_executable(server src/server.cpp)
target_link_libraries(server PRIVATE server_lib)
target_compile_options(server PRIVATE ${PROJECT_WARNING_FLAGS})
