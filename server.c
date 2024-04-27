#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8000

int main(){
    int socket_fd , recv_socket_fd;
    socklen_t  clilen;
    char buffer[256];

    typedef struct {
    uint message_id;  // 16bit
    uint length;      // 16bit
} packet_header;

    // sockaddr_inという、構造体が提供されてる。
    /* /usr/include/netinet/in.h:
   struct in_addr {
      u_int32_t s_addr;
   };

   struct sockaddr_in {
      u_char  sin_len;    （このメンバは古いOSでは存在しない）
      u_char  sin_family;    （アドレスファミリ．今回はAF_INETで固定）
      u_short sin_port;    （ポート番号）
      struct  in_addr sin_addr;    （IPアドレス）
      char    sin_zero[8];    （無視してもよい．「詰め物」のようなもの）
   };
    */
    struct sockaddr_in serv_addr, cli_addr;
    int n;

// ソケットを作成する
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd< 0) {
        perror("ERROR opening socket");
        exit(1);
    }

memset ((char *)&serv_addr , 0 , sizeof(serv_addr));

// memset : メモリ領域(第1引数)の先頭から(第3引数)バイトを文字c(第2引数)に設定する関数。メモリ領域を'\0'に設定すれば、配列の初期化などに使用できる。

serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr=INADDR_ANY;
serv_addr.sin_port=htons(PORT);

// htons : ホストのバイト順の IP ポート番号をネットワーク バイト順の IP ポート番号に変換

if(bind(socket_fd , (struct sockaddr *)&serv_addr , sizeof(serv_addr)) < 0 ){
    printf("Error on binding\n");
    exit(1);
}

//  bind : 作成されたsocketにポート番号などをくっつける

/*sockfd（第１引数）
任意のソケットを示すファイルディスクリプター。

addr（第２引数）
ソケットに割り当てるアドレス。

addrlen（第３引数）
addrの指す構造体のサイズ。*/

listen(socket_fd , 5);
// 第二引数は、接続待ちの最大数です。

clilen = sizeof(cli_addr);
recv_socket_fd = accept(socket_fd, (struct sockaddr *)&cli_addr, &clilen);
    if (recv_socket_fd < 0) {
        printf("ERROR on accept");
        exit(1);
    }

/* accept()を実行すると、クライアントから通信接続要求が来るまでプログラムを停止し、接続後にプログラムを再開*/

char *p = malloc(1024);
if (p == NULL) {
        printf("メモリの割り当てに失敗しました。\n");
        return 1;
    }

n = recv(recv_socket_fd , p , 1024, 0);

// 第四引数は、受信オプションを指定します。0を指定すると、通常の受信になります。

if (n < 0) {
        printf("ERROR reading from socket");
        exit(1);
    }

packet_header *ph = (packet_header*) p;

printf("Message_ID from client: %u\n", ph->message_id);
printf("Message length from client: %u\n", ph->length);

char *q = ((char*)ph + sizeof(packet_header));

printf("%s\n", q);


// // データを送信する
//     n = send(recv_socket_fd, q, 1024, 0);
//     if (n < 0) {
//         printf("ERROR writing to socket");
//         exit(1);
//     }

//     close(recv_socket_fd);
//     close(socket_fd);

    return 0;
}