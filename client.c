#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "type_h.h"


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

    if (argc < 2) {
        fprintf(stderr, "usage %s <-option> <message>\n", argv[0]);
        exit(0);
    }

    socket_fd = socket(AF_INET , SOCK_STREAM , 0);

    if(socket_fd <0){
        printf("ERROR opening socket");
        exit(1);
    }
    server = gethostbyname("133.92.147.249");
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
    
    int total_length =0;
    // 送るメッセージすべての長さ
    for(int i=2 ; i < argc ; i++){
        total_length+=strlen(argv[i]);
    }
    ph->length = total_length;
    // 送るメッセージの個数
    ph->message_num = argc-1;

    // 送るメッセージの形式
    if(strcmp(argv[1] , "-hide") == 0){
        strcpy(ph->message_type , Hide);
    }else{
        if(strchr(argv[1] , '-')){
            strcpy(ph->message_type , None);
        }else{
            fprintf(stderr, "usage %s <-option> <message>\n", argv[0]);
            exit(0);
        }
    }

    // 残りの領域に、メッセージを入れる
    // phは構造体型なので、charにキャスト
    // そのあとに、構造体分の大きさをインクリメントする
    char *q = ((char*)ph + sizeof(packet_header));

    for(int i=2 ; i < argc ; i++){
        strcpy(q , argv[i]);
        q+=strlen(argv[i]);
    }

    *q++ = '\0';

    n = send(socket_fd, ph, 1024, 0);
    if (n < 0) {
        printf("ERROR writing to socket");
        exit(1);
    }

    free(p);

    close(socket_fd);

    return 0;

}