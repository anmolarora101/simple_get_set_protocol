#include<stdio.h>
#include<conio.h>
#include<string.h>
#include "proto.h"
//set functions
void set_id(struct msg_fmt *msg,char* packet){ 
    msg->id=packet[2]*10+packet[3];//2 bit ID, If more bits are there, we should run a for loop
    return 0;
}
void set_typ(struct msg_fmt *msg,char* packet){
    msg->typ=packet[4]*10+packet[5]; // 2 bit type
    return 0;
}
int set_len(struct msg_fmt *msg ,char* packet){
    msg->len=packet[6]*100+packet[7]*10+packet[8]; // 3 bit length
    return msg->len;
}

void set_value(struct msg_fmt *msg ,char* packet,int len){
    msg->val=0;
    for(int i=0;i<len;i++) //set data according to the length found in previous function call
    {
        msg->val=msg->val+pow(2,len-i)*packet[i];    //iterating over 'len' bit of payload
    }
    return 0;
}
void set_chksum(char* packet);// not implemented yet
//get functions
char* get_typ(struct msg_fmt *msg,char* data){
    data[4]=(msg->typ)/10;  // 3rd and 4th bit of packet is for type of payload
    data[5]=(msg->typ)%10;
    return data;
}

char* get_len(struct msg_fmt *msg,char* data){
    int temp=msg->len;     //7th, 8th and 9th bit of packet is for length of payload
    data[8]=temp%10;   
    temp=temp/10;
    data[7]=temp%10;
    data[6]=temp/10;
    return data;
}

char* get_value(struct msg_fmt *msg,char* data){
    int length=data[6]*100+data[7]*10+data[8];   //calculate length of payload
    int temp=msg->val;
    for(int i=0;i<length;i++){  //get payload
        data[9+length-i]=temp%10;
        temp=temp/10;
    }
    return data;
}


//main fnction
int main(void){
    struct msg_fmt *msg[4]; //length is taken as 4, because ID is 2 bit identifier so 4 value of ID are possible
    char packet[]="110010100101110" ;//1st bit 1 is sof, 2nd bit 1 is opcode SET, 3rd and 4th bit 00 is identifier ID,5th and 6th bit 10 is data typ int here,7,8 and 9th bit 100 is data length  bit, 10,11,12th bit 1011 is payload, last 2 bits are checksum
if(packet[1]==1){ //set frame recieved
    set_data(msg,packet);
}
else //get frame recieved// Now check for ID
{
    for(int i=0;i<4;i++){ //check entire msg[]
    if(msg[i]->id==packet[2]*10+packet[3])
    {
        char str[20];
        char*data=&str;
        str[0]=1;
        str[1]=1;
       char* data=get_data(msg[i],packet,data); 
        break;// msg ID exists so break
    }
    }}
    return 0;
}

uint8_t create_msg(struct msg_fmt*msg){
     msg=malloc(sizeof(struct msg_fmt)); // assign memory for each data
    if(msg==NULL){
        return 0;
    }
    return 1;
}

uint8_t check_msg_exists(struct msg_fmt* msg,char* packet_recieve){
    if(msg->id==(packet_recieve[2]*10+packet_recieve[3])){       //check if the msg aready exists
        return 1;
    }
    return 0;
}
int set_data(struct msg_fmt* msg,char* packet_recieve){
    static int i=0;
    if(NULL==packet_recieve){// if no packet recieved
        return -1;
    }
    else{
        for(int i=0;i<4;i++){// check the entire msg array if the msg ID exists, if yes then modify the existing msg
            if(check_msg_exists(&msg[i],packet_recieve)){
                set_type(&msg[i],packet_recieve);
                int len=set_len(&msg[i],packet_recieve);
                set_value(&msg[i],packet_recieve,len);
                break;
            }
            else{
                if(create_msg(&msg[i])){// create a new msg if it doesnt exists
                    set_id(&msg[i],packet_recieve);
                    set_typ(&msg[i],packet_recieve);
                    int len=set_len(&msg[i],packet_recieve);
                set_value(&msg[i],packet_recieve,len);
                if(calculate_chksum())
                {
                    set_chksum(&msg[i]); //cheksum is yet to implement
                }else{
                    set_chksum(DEF_CHKSUM);
            }
            i++;
        }
        }
    }
}
return 0;
}
char* get_data(struct msg_fmt*msg,char* packet_recieve,char*data) {
    for(int i=0;i<4;i++){
        if(check_msg_exist(&msg[i],packet_recieve)){
            char*data[2]=packet_recieve[2];// we can have loop if more bytes are there
            char*data[3]=packet_recieve[3];
            char*data[]=get_type(&msg[i],&data);//append tye here
            char*data[]=get_len(&msg[i],&data); //append length here
            char*data[]=get_value(&msg[i],&data);//append value here
            break;
        }
    }
    return data;
}