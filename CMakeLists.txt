cmake_minimum_required(VERSION 3.10)
project(SimpleChat)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(externals/)

# Warning flags (scoped to our targets only, not externals)
if(MSVC)
  set(PROJECT_WARNING_FLAGS
    /W4
    /WX
    /permissive-
    /w14242 /w14254 /w14263 /w14265 /w14287
    /we4289 /w14296 /w14311
    /w14545 /w14546 /w14547 /w14549 /w14555
    /w14619 /w14640 /w14826
    /w14905 /w14906 /w14928
  )
else()
  set(PROJECT_WARNING_FLAGS
    -Wall
    -Wextra
    -Werror
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat=2
    -Wimplicit-fallthrough
  )
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    list(APPEND PROJECT_WARNING_FLAGS
      -Wmisleading-indentation
      -Wduplicated-cond
      -Wduplicated-branches
      -Wlogical-op
      -Wuseless-cast
    )
  endif()
endif()

# Common networking library
add_library(common_lib STATIC
  src/chat_client.cpp
  src/chat_server.cpp
)
target_include_directories(common_lib PUBLIC include)
target_include_directories(common_lib SYSTEM PUBLIC externals/SFML/include)
target_link_libraries(common_lib PUBLIC sfml-network)
target_compile_options(common_lib PRIVATE ${PROJECT_WARNING_FLAGS})

# Treat external include dirs as system to suppress warnings in their headers
get_target_property(IMGUI_INCLUDE_DIRS imgui-sdl INTERFACE_INCLUDE_DIRECTORIES)
set_target_properties(imgui-sdl PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${IMGUI_INCLUDE_DIRS}")

# Client MVC library
add_library(client_lib STATIC
  src/client_model.cpp
  src/client_view.cpp
  src/client_controller.cpp
)
target_include_directories(client_lib PUBLIC include)
target_include_directories(client_lib SYSTEM PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/externals/SDL/include
)
target_link_libraries(client_lib PUBLIC common_lib imgui-sdl SDL3::SDL3)
target_compile_options(client_lib PRIVATE ${PROJECT_WARNING_FLAGS})

# Server executable
add_executable(server main/server.cpp)
target_include_directories(server PRIVATE include)
target_link_libraries(server PRIVATE common_lib)
target_compile_options(server PRIVATE ${PROJECT_WARNING_FLAGS})

# Client executable
add_executable(client main/client.cpp)
target_include_directories(client PRIVATE include)
target_link_libraries(client PRIVATE client_lib)
target_compile_options(client PRIVATE ${PROJECT_WARNING_FLAGS})
