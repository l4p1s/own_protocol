#ifndef STRUCT_TYPE_H
#define STRUCT_TYPE_H

#define Hide "hide"
#define None "none"

typedef struct {
    uint message_id;    // 16bit
    uint length;        // 16bit
    uint message_num;
    char message_type[5]; // 文字列 "hide" のための領域を確保
} packet_header;

#endif /* STRUCT_TYPE_H */
