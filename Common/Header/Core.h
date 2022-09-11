#pragma once

#ifdef WIN32
#define WIN(exp) exp
#define LIN(exp)
using SockAddr_in = SOCKADDR_IN;
using Scocket = SOCKET;
#else
#define WIN(exp)
#define LIN(exp) exp

using SockAddr_in = struct sockaddr_in;
using Socket = int;
#endif