add_library(socketlib SHARED
${SOCKET_SOURCE})
target_include_directories(socketlib PRIVATE socket Logging/include)
target_link_libraries(socketlib logger)

if(WIN32)
target_link_libraries(socketlib wsock32 ws2_32)
endif()
install(TARGETS socketlib DESTINATION bin)

target_include_directories(queue_example PRIVATE thread_pool)

target_link_libraries(tcp_echo_server_example socketlib)
target_include_directories(tcp_echo_server_example PRIVATE socket)

target_sources(thread_pool_example PRIVATE thread_pool/thread_pool.cpp)
target_include_directories(thread_pool_example PRIVATE thread_pool)

target_sources(webserver_example PRIVATE 
webserver/ld_webserver.cpp
thread_pool/thread_pool.cpp
utils/ld_timer.cpp)
target_include_directories(webserver_example PRIVATE webserver thread_pool socket utils)
target_link_libraries(webserver_example socketlib)
