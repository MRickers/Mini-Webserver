cmake_minimum_required(VERSION 3.16)
project(Mini_Webserver)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

if(WIN32)
# https://docs.microsoft.com/de-de/cpp/build/reference/compiler-options-listed-alphabetically?view=msvc-160
set(COMPILE_OPTIONS /GT /Gs /GS)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(InstallRequiredSystemLibraries)
add_definitions(-DNOMINMAX) # to use std::max
set(SOCKET_SOURCE socket/socket_windows.cpp)

elseif(UNIX)
set(COMPILE_OPTIONS -Wall -Wextra -pendatic -Werror -fstack-protector)
set(SOCKET_SOURCE socket/socket_linux.cpp)
endif()

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
configure_file("config_version.h.in"
${CMAKE_SOURCE_DIR}/src/config_version.h)

option(testing "Execute tests" OFF)

set(TESTS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/tests)

add_executable(tcp_echo_server_example src/examples/tcp_echo_server_example.cpp)
target_include_directories(tcp_echo_server_example PRIVATE Logging/include)
target_link_libraries(tcp_echo_server_example logger)
install(TARGETS tcp_echo_server_example DESTINATION bin)

add_executable(queue_example src/examples/queue_example.cpp)
target_include_directories(queue_example PRIVATE Logging/include)
target_link_libraries(queue_example logger)
install(TARGETS queue_example DESTINATION bin)

add_executable(thread_pool_example src/examples/thread_pool_example.cpp)
target_include_directories(thread_pool_example PRIVATE Logging/include)
target_link_libraries(thread_pool_example logger)
install(TARGETS thread_pool_example DESTINATION bin)

add_executable(webserver_example src/examples/webserver_example.cpp)
install(TARGETS webserver_example DESTINATION bin)

add_executable(event_server_example  
src/examples/event_server_example.cpp)
install(TARGETS event_server_example DESTINATION bin)

add_subdirectory(Logging)
add_subdirectory(src)

if(testing)
add_subdirectory(tests)
endif()