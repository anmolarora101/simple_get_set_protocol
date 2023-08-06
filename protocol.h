#ifndef __PROTO_H_
#define __PROTO_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define DEF_CHKSUM  0xFFFF


typedef enum opcode {
    GET = 0,
    SET = 1,
    UNDEFINED = -1
} opcode;

typedef enum type {
    INT  = 0,
    CHAR = 1,
    STR  = 2,
    INVALID = -1
} type;

typedef struct message_fields {
    type typ;
    uint8_t len;
    uint8_t *val;    
} msg_fields;

typedef struct message_format {
    type typ;
    uint8_t len;
    uint8_t val;
} msg_fmt;


#endif
