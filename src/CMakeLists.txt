add_library(socketlib SHARED
${SOCKET_SOURCE})
target_include_directories(socketlib PRIVATE socket)

if(WIN32)
target_link_libraries(socketlib wsock32 ws2_32)
endif()
install(TARGETS socketlib DESTINATION bin)

target_include_directories(queue_example PRIVATE thread_pool)
install(TARGETS queue_example DESTINATION bin)

target_link_libraries(tcp_echo_server_example socketlib)
target_include_directories(tcp_echo_server_example PRIVATE socket)

