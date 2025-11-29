#ifndef STRUCT_IN_H
#define STRUCT_IN_H
#pragma once

#include <QObject>
#include <QVariant>
#include <QThread>
//=====================================================================
#include <pcap.h>
#include <net/ethernet.h>   // struct ether_header
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>


typedef struct st_packet{

    int num;
    QString src;
    QString dst;
    QString proto;
    int len;
    QString info;

} st_pkt;


#pragma pack(push, 1)

typedef struct e_H{
    u_int8_t  ether_dhost[ETH_ALEN];      // 6바이트: 목적지 MAC
    u_int8_t  ether_shost[ETH_ALEN];      // 6바이트: 출발지 MAC
    u_int16_t ether_type;                 // 2바이트: EtherType
}e_H;

typedef struct ip_H{

    uint8_t ihl:4, version:4;
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
}ip_H;

struct tcp_H{
    uint16_t source;
    uint16_t dest;
    uint32_t seq;
    uint32_t ack_seq;

#if __BYTE_ORDER == __LITTLE_ENDIAN
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
};
#pragma pack(pop)


#endif // STRUCT_IN_H
