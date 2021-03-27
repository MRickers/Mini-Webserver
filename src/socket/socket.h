#ifndef _SOCKET_HPP
#define _SOCKET_HPP

#ifdef __linux__
#include "socket_linux.h"
#elif _WIN32
#include "socket_windows.h"
#endif




#endif // _SOCKET_HPP