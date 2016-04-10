cmake_minimum_required(VERSION 3.1)
project(brainfuck-bot)

find_package(Threads)

add_executable(bot main.cpp simulator.cpp)
target_link_libraries(bot TgBot boost_system ssl crypto ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(bot PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)
