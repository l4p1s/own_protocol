#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8000


int main(int argc , char *argv[]){
    int socket_fd , n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    /* gethostbynemaeの返却値の構造体は以下の構造
    struct hostent {
	   char  *h_name;             ホストの正式名称 
	   char  **h_aliases;         別名リスト 
	   int   h_addrtype;          ストアドレスのタイプ (AF_INET6 など)
	   int   h_length;            アドレスの長さ
	   char  **h_addr_list;       NULL で終わるアドレスのリスト
    };
*/
    char buffer[256];

    if (argc < 3) {
        fprintf(stderr, "usage %s hostname message\n", argv[0]);
        exit(0);
    }

    socket_fd = socket(AF_INET , SOCK_STREAM , 0);

    if(socket_fd <0){
        printf("ERROR opening socket");
        exit(1);
    }

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    memset((char *)&serv_addr , 0 , sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // hostによって、IPアドレスが変わるのでmemcpyする
    memcpy((char *)&serv_addr.sin_addr.s_addr , (char *)server->h_addr , server->h_length);
    serv_addr.sin_port=htons(PORT);

//  サーバに接続する

if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("ERROR connecting");
        exit(1);
    }


// データを送信する
    memset(buffer, 0, 256);
    strcpy(buffer, argv[2]);
    n = send(socket_fd, buffer, strlen(buffer), 0);
    if (n < 0) {
        printf("ERROR writing to socket");
        exit(1);
    }

// データを受信する
    memset(buffer , 0 , 255);
    n = recv(socket_fd , buffer , 255 , 0);
    if (n < 0) {
        printf("ERROR reading from socket");
        exit(1);
    }

    printf("Message from server: %s\n", buffer);

    close(socket_fd);

    return 0;

}