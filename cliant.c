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

    typedef struct {
    uint message_id;  // 16bit
    uint length;      // 16bit
} packet_header;


    /* gethostbynemaeの返却値の構造体は以下の構造
    struct hostent {
	   char  *h_name;             ホストの正式名称 
	   char  **h_aliases;         別名リスト 
	   int   h_addrtype;          ストアドレスのタイプ (AF_INET6 など)
	   int   h_length;            アドレスの長さ
	   char  **h_addr_list;       NULL で終わるアドレスのリスト
    };
*/

    if (argc < 2) {
        fprintf(stderr, "usage %s <message>\n", argv[0]);
        exit(0);
    }

    socket_fd = socket(AF_INET , SOCK_STREAM , 0);

    if(socket_fd <0){
        printf("ERROR opening socket");
        exit(1);
    }
    strcpy(argv[1],"133.92.147.249");
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
    // メモリの割り当て
    char *p = malloc(1024);
    if (p == NULL) {
        printf("メモリの割り当てに失敗しました。\n");
        return 1;
    }

    // packet_header 構造体へのポインタを作成
    packet_header *ph = (packet_header*) p;

   
    ph->message_id = 1; 
    ph->length = strlen(argv[0]);

    // 残りの領域に、メッセージを入れる
    // phは構造体型なので、charにキャスト
    // そのあとに、構造体分の大きさをインクリメントする
    char *q = ((char*)ph + sizeof(packet_header));

    strcpy(q ,argv[2]);
    n = send(socket_fd, &ph, sizeof(ph), 0);
    if (n < 0) {
        printf("ERROR writing to socket");
        exit(1);
    }

// データを受信する
    // char buffer[1024];
    // memset(buffer , 0 , 1023);
    // n = recv(socket_fd , buffer , 1023 , 0);
    // if (n < 0) {
    //     printf("ERROR reading from socket");
    //     exit(1);
    // }

    // printf("Message from server: %s\n", buffer);

    // close(socket_fd);

    // return 0;

}