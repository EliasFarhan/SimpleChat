cmake_minimum_required(VERSION 3.10)
project(SimpleChat)

set(CMAKE_CXX_STANDARD 23)


add_subdirectory(externals/)

add_executable(server main/server.cpp)
target_include_directories(server PRIVATE externals/SFML/include include)
target_link_libraries(server PRIVATE sfml-network)

add_executable(client main/client.cpp)
target_include_directories(client PRIVATE externals/SFML/include include)
target_link_libraries(client PRIVATE sfml-network imgui-sdl SDL3::SDL3)