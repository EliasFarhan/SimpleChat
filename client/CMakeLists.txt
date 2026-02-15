# Client view library (SDL/ImGui concrete implementation)
add_library(client_lib STATIC
  src/client_view.cpp
)
target_include_directories(client_lib PUBLIC include)
target_include_directories(client_lib SYSTEM PUBLIC
  ${CMAKE_SOURCE_DIR}/externals/SDL/include
)
target_link_libraries(client_lib PUBLIC common_lib imgui-sdl SDL3::SDL3)
target_compile_options(client_lib PRIVATE ${PROJECT_WARNING_FLAGS})

# Client executable
add_executable(client src/client.cpp)
target_link_libraries(client PRIVATE client_lib)
target_compile_options(client PRIVATE ${PROJECT_WARNING_FLAGS})
