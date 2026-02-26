enable_testing()

add_executable(unit_tests
  test_client_model.cpp
  test_client_controller.cpp
        test_poker_player.cpp
)
target_link_libraries(unit_tests PRIVATE model_lib GTest::gmock_main)
target_compile_options(unit_tests PRIVATE ${PROJECT_WARNING_FLAGS})

include(GoogleTest)
gtest_discover_tests(unit_tests DISCOVERY_MODE PRE_TEST)
