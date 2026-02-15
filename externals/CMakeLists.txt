add_subdirectory(SFML)
add_subdirectory(SDL)

file(GLOB imgui_src imgui/*.cpp)
add_library(imgui-sdl ${imgui_src} imgui/backends/imgui_impl_sdl3.cpp imgui/backends/imgui_impl_sdlrenderer3.cpp)
target_include_directories(imgui-sdl PUBLIC imgui imgui/backends)
target_link_libraries(imgui-sdl PUBLIC SDL3::SDL3)