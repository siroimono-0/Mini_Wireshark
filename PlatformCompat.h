#ifndef PLATFORMCOMPAT_H
#define PLATFORMCOMPAT_H

#include <cstdint>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <pcap/pcap.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pcap.h>
#endif

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

#if defined(_WIN32)
#define MINI_WIRESHARK_LITTLE_ENDIAN 1
#elif defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && \
    (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define MINI_WIRESHARK_LITTLE_ENDIAN 1
#elif defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && \
    (__BYTE_ORDER == __LITTLE_ENDIAN)
#define MINI_WIRESHARK_LITTLE_ENDIAN 1
#else
#define MINI_WIRESHARK_LITTLE_ENDIAN 0
#endif

#endif // PLATFORMCOMPAT_H
