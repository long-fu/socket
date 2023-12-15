cmake_minimum_required(VERSION 3.0.0)
project(server VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(server server.cpp)
target_link_libraries(server PUBLIC pthread)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
