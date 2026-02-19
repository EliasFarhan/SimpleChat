add_executable(server src/server.cpp)
target_link_libraries(server PRIVATE common_lib)
target_compile_options(server PRIVATE ${PROJECT_WARNING_FLAGS})
