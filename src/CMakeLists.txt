add_library(socketlib SHARED
socket/socket_linux.cpp)
target_include_directories(socketlib PRIVATE socket)

target_include_directories(queue_example PRIVATE thread_pool)


target_link_libraries(example socketlib)
target_include_directories(example PRIVATE socket)
