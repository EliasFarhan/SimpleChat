add_library(model_lib STATIC
    src/client_model.cpp
    src/client_controller.cpp
    src/poker_player.cpp
    src/poker.cpp
)
target_include_directories(model_lib PUBLIC include)
target_compile_options(model_lib PRIVATE ${PROJECT_WARNING_FLAGS})
