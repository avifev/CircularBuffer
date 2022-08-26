project(thread-safe-logger)
cmake_minimum_required(VERSION 3.10)

include_directories(include)

add_executable(main ../src/main.cpp ../src/client.cpp)
target_link_libraries(main pthread)