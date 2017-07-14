#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "dns_parse.h"

static char* handle_udp_packet(unsigned char* buffer, int offset);
static void parse_dns(unsigned char* buffer, int offset);
static char source[20], dest[20];

char *
handle_packet(unsigned char* buffer)
{
    int offset = 0;

    offset += ETH_HLEN;
    struct iphdr* iph = (struct iphdr*)(buffer+offset);
    long2ip(ntohl(iph->saddr), source);
    long2ip(ntohl(iph->daddr), dest);
    switch (iph->protocol) {
    
        case IPPROTO_ICMP:          /*ICMP Protocol*/
        case IPPROTO_IGMP:          /*IGMP Protocol*/
            break;
        case IPPROTO_TCP:           /*TCP Protocol*/
            break;
        case IPPROTO_UDP:           /*UDP Protocol*/
            offset += iph->ihl*4;
            handle_udp_packet(buffer, offset);
            break;
        default:                    /*Some Other Protocol like ARP etc.*/
            break;
    }
}


char *
handle_udp_packet(unsigned char* buffer, int offset)
{
    struct udphdr * uh = (struct udphdr *)(buffer + offset);
    int s_port=htons(uh->uh_sport);
    int d_port=htons(uh->uh_dport);

    offset += 8; /*udp header length*/
    if (s_port == 53 || d_port == 53) {
        parse_dns(buffer, offset);
    }
}

void
parse_dns(unsigned char* buffer, int offset)
{
    char buf[1024] = {0};
    char tmp[100] = {0};

    offset += 12;      /*DNS协议头长度*/
    char *curr = buffer + offset;
    while(*curr) {
        strncpy(tmp, curr + 1, *curr);
        strcat(tmp, ".");
        strcat(buf, tmp);
        memset(tmp, '\0', sizeof(tmp));
        offset += *curr + 1;
        curr += *curr + 1;
    }
    buf[strlen(buf)-1] = '\0';
    char *tim = current_time();
    printf("%s DNS lookup: [%s]=>[%s] target=%s\n", tim, source, dest, buf);
    free(tim);
}
