#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint message_id;  // 16bit
    uint length;      // 16bit
} packet_header;

int main(int argc, char *argv[]) {
    // 引数の数をチェックする
    if (argc != 1) {
        printf("Usage: %s <message_id> <message>\n", argv[0]);
        return 1;
    }

    // メモリを割り当てる
    char *p = malloc(1024);
    if (p == NULL) {
        printf("メモリの割り当てに失敗しました。\n");
        return 1;
    }

    // packet_header 構造体へのポインタを作成する
    packet_header *ph = p;

    // メッセージIDを設定する
    ph->message_id = 1 // 第1引数からメッセージIDを取得

    // メッセージの長さを設定する
    ph->length = strlen(argv[1]);   // 第2引数の文字列の長さを取得

    // メッセージIDを表示する
    printf("Message ID: %u\n", ph->message_id);
    printf("Message Length: %u\n", ph->length);

    // メモリを解放する
    free(p);

    return 0;
}
