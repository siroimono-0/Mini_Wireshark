#ifndef STRUCT_IN_H
#define STRUCT_IN_H
#pragma once

#include <QObject>
#include <QVariant>
#include <QThread>
//=====================================================================
#include "PlatformCompat.h"


typedef struct st_packet{

    int num;
    QString src;
    QString dst;
    QString proto;
    int len;
    QString info;

} st_pkt;

Q_DECLARE_METATYPE(st_pkt)


#pragma pack(push, 1)
typedef struct e_H{
    uint8_t  ether_dhost[ETH_ALEN];      // 6諛붿씠?? 紐⑹쟻吏 MAC
    uint8_t  ether_shost[ETH_ALEN];      // 6諛붿씠?? 異쒕컻吏 MAC
    uint16_t ether_type;                 // 2諛붿씠?? EtherType
}e_H;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct ip_H{
#if MINI_WIRESHARK_LITTLE_ENDIAN
    uint8_t ihl:4;
    uint8_t version:4;
#else
    uint8_t version:4;
    uint8_t ihl:4;
#endif
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
} ip_H;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct tcp_H{
    uint16_t source;
    uint16_t dest;
    uint32_t seq;
    uint32_t ack_seq;

#if MINI_WIRESHARK_LITTLE_ENDIAN
    uint16_t res1:4,
        doff:4,
        fin:1,
        syn:1,
        rst:1,
        psh:1,
        ack:1,
        urg:1,
        ece:1,
        cwr:1;
#else
    uint16_t doff:4,
        res1:4,
        cwr:1,
        ece:1,
        urg:1,
        ack:1,
        psh:1,
        rst:1,
        syn:1,
        fin:1;
#endif

    uint16_t window;
    uint16_t check;
    uint16_t urg_ptr;
}tcp_H;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct udp_H {
    uint16_t source;   // Source Port
    uint16_t dest;     // Destination Port
    uint16_t len;      // UDP header + data ?꾩껜 湲몄씠
    uint16_t check;    // Checksum
}udp_H;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct icmp_H {
    uint8_t  type;      // 硫붿떆吏 ???

    uint8_t  code;      // 遺媛 肄붾뱶(?몃? ?먯씤)

    uint16_t checksum;  // ICMP checksum

    union {
        struct {
            uint16_t id;
            uint16_t sequence;
        } echo;                 // ICMP Echo (ping)

        uint32_t   gateway;     // Redirect 硫붿떆吏?먯꽌 ?ъ슜
        struct {
            uint16_t __unused;
            uint16_t mtu;
        } frag;                 // Fragmentation needed
    } un;
}icmp_H;
#pragma pack(pop)


#endif // STRUCT_IN_H

