#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>


#ifdef _WIN32
    #include <winsock2.h>
#include "Packets/Serverbound/Handshake/HandshakePacket.h"

#endif

#ifdef __APPLE__
    #include <netinet/in.h>
#endif


int main() {


#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(1,1), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        exit(1);
    }
    SOCKET sockD = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif
#ifdef __APPLE__
    int sockD = socket( PF_LOCAL, SOCK_STREAM, 0);
#endif
    char address[] = "localhost";
    HandshakePacket* header = header_new(address, strlen(address), 25565);

    struct sockaddr_in server_address;
//    uint32_t ip_address = 2130706433; // 127.0.0.1 as an integer
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(25565);
    server_address.sin_family = AF_INET;

    printf("Trying to connect to port %d.\n", server_address.sin_port);
    int connection_status = connect(sockD, (const struct sockaddr *) &server_address, sizeof server_address);

    if (connection_status == -1) {
        printf("Fehler!");
        return -1;
    }
    printf("Connected succesfully!\n");

    char emptySize = 1;
    char emptyPacket = 0x00;
    char ping = 0x01;
    char* test = calloc(8, sizeof(char));
    NetworkBuffer* buffer = get_ptr_buffer(header);
    buffer_send_packet(buffer, sockD);
    buffer_free(buffer);

    send(sockD, &emptySize, 1, 0);
    send(sockD, &emptyPacket, 1, 0);
    send(sockD, &ping, 1, 0);
    send(sockD, test, sizeof(long), 0);

    NetworkBuffer* answer = buffer_new();
    int packet_length = varint_receive(sockD);
    printf("Packet length: %d\n", packet_length);
    int packet_id = varint_receive(sockD);
    printf("Packet id: %d\n", packet_id);
    buffer_read_string(answer, sockD);
    buffer_print_string(answer);
    buffer_free(answer);

    close(connection_status);

    return 0;
}
