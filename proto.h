#ifndef __PROTO_H_
#define __PROTO_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#define DEF_CHKSUM 0XFFF
typedef int uint8_t ;
typedef enum opcode{
    GET=0 ,
    SET=1,
    UNDEFINED=-1,
} opcode;
 
 typedef enum type{
    INT=0,
    CHAR=1,
    STR=2,
    INVALID=-1,
 }type;
 struct msg_fmt{
    uint8_t SOF;
    opcode opc;
    type typ;
    uint8_t len;
    uint8_t val;
    uint16_t chksum;
    uint8_t id;
 };
#endif