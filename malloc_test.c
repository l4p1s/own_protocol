#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint message_id;  // 16bit
    uint length;      // 16bit
} packet_header;

int main(int argc, char *argv[]) {

    if(argc < 2){
        printf("Usage: %s <message> \n", argv[0]);
        exit(1);
    }

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

    char *r = q;

    // インクリメントしたポインタpにargv[0]をコピーする
    strcpy(q , argv[0]);

    // 次のメッセージを入れるために、ポインタをインクリメントする
    q += strlen(argv[0]);

    strcpy(q , argv[1]);

    printf("Message ID: %u\n", ph->message_id);
    printf("Message Length: %u\n", ph->length);
    printf("%s", r);

    // メモリを解放する
    free(p);

    return 0;
}
