cmake_minimum_required(VERSION 3.16)
project(Mini_Webserver)
set(CMAKE_CXX_STANDARD 17)

if(WIN32)
set(COMPILE_OPTIONS /Gt /GT)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(InstallRequiredSystemLibraries)
set(SOCKET_SOURCE socket/socket_windows.cpp)
elseif(UNIX)
set(COMPILE_OPTIONS -Wall -Wextra -pendatic -Werror -fstack-protector)
set(CMAKE_BUILD_TYPE "Debug")
set(SOCKET_SOURCE socket/socket_linux.cpp)
endif()

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
configure_file("config_version.h.in"
${CMAKE_SOURCE_DIR}/src/config_version.h)

set(CMAKE_INSTALL_PREFIX .)

option(testing "Execute tests" OFF)

set(TESTS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/tests)

add_executable(tcp_echo_server_example src/examples/example.cpp)
target_include_directories(tcp_echo_server_example PRIVATE Logging/include)
target_link_libraries(tcp_echo_server_example logger)
install(TARGETS tcp_echo_server_example DESTINATION bin)

add_executable(queue_example src/examples/queue_example.cpp)
target_include_directories(queue_example PRIVATE Logging/include)
target_link_libraries(queue_example logger)
install(TARGETS queue_example DESTINATION bin)

add_subdirectory(Logging)
add_subdirectory(src)

if(testing)
add_subdirectory(tests)
endif()