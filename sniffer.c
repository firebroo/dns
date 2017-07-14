#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include "config.h"
#include "common.h"
#include "dns_parse.h"

int
main(int argc, char *argv[])
{
    int             sock_raw;
    int              data_size;
    unsigned int     saddr_size;
    struct sockaddr  saddr;
    unsigned char   *buffer;
    char            *data;

    buffer = (unsigned char *)malloc(PKTBUFSIZE);

    /*接受所有的ip数据帧*/
    sock_raw = socket(PF_PACKET , SOCK_RAW , htons(ETH_P_IP));
    if(sock_raw < 0) {
        die("Socket Error");
    }

    saddr_size = sizeof(saddr);
    for(;;) {
        memset(buffer, '\0', BUFFER_SIZE);

        data_size = recvfrom(sock_raw , buffer , BUFFER_SIZE , 0 , &saddr , &saddr_size);
        if(data_size < 0) {
            die("Recvfrom error , failed to get packets");
        }
        handle_packet(buffer);
    }

    free(buffer);
    close(sock_raw);
    return 0;
}
