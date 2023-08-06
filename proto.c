#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<math.h>
#include "proto.h"
uint8_t create_msg(struct msg_fmt*msg){
     msg=malloc(sizeof(struct msg_fmt*)); // assign memory for each data
    if(msg==NULL){
        return 0;
    }
    return 1;
}

uint8_t check_msg_exists(struct msg_fmt* msg,char packet_recieve[]){
    if(msg->id==(int)(packet_recieve[2]*10+packet_recieve[3])){       //check if the msg aready exists
        return 1;
    }
    return 0;
}
//set functions
void set_id(struct msg_fmt *msg,char packet[]){ 
    msg->id=(uint8_t)(packet[2]*10+packet[3]);//2 bit ID, If more bits are there, we should run a for loop
}
void set_type(struct msg_fmt *msg,char packet[]){
    msg->typ=packet[4]*10+packet[5]; // 2 bit type
}
int set_len(struct msg_fmt *msg ,char packet[]){
    msg->len=packet[6]*100+packet[7]*10+packet[8]; // 3 bit length
    return msg->len;
}

void set_value(struct msg_fmt *msg ,char packet[],int len){
    msg->val=0;
    for(int i=0;i<len;i++) //set data according to the length found in previous function call
    {
        msg->val=msg->val+pow(2,len-i)*packet[i];    //iterating over 'len' bit of payload
    }
}

//get functions
void get_type(struct msg_fmt *msg,char str[]){
    str[4]=(msg->typ)/10;  // 3rd and 4th bit of packet is for type of payload
    str[5]=(msg->typ)%10;
}

void get_len(struct msg_fmt *msg,char str[]){
    int temp=msg->len;     //7th, 8th and 9th bit of packet is for length of payload
    str[8]=temp%10;   
    temp=temp/10;
    str[7]=temp%10;
    str[6]=temp/10;
}

void get_value(struct msg_fmt *msg,char str[]){
    int length=str[6]*100+str[7]*10+str[8];   //calculate length of payload
    int temp=msg->val;
    for(int i=0;i<length;i++){  //get payload
        str[9+length-i]=temp%10;
        temp=temp/10;
    }
}
void set_existing_data(struct msg_fmt* msg,char packet_recieve[]){
                set_type(msg,packet_recieve);
                int len=set_len(msg,packet_recieve);
                set_value(msg,packet_recieve,len);
}


void set_new_data(struct msg_fmt* msg,char packet_recieve[]){
                if(create_msg(msg)){// create a new msg if it doesnt exists
                    set_id(msg,packet_recieve);
                    set_type(msg,packet_recieve);
                    int len=set_len(msg,packet_recieve);
                set_value(msg,packet_recieve,len);
        }
        }
int is_memory_empty(struct msg_fmt* msg,char packet_recieve[]){
if(msg==NULL){
    return 0;
}
        return 1;
}
void get_data(struct msg_fmt*msg,char packet_recieve[],char str[]) {
    for(int i=0;i<4;i++){
        if(check_msg_exists(msg,packet_recieve)){
            str[2]=packet_recieve[2];// we can have loop if more bytes are there
            str[3]=packet_recieve[3];
            get_type(msg,str);//append tye here
            get_len(msg,str); //append length here
            get_value(msg,str);//append value here
            break;
        }
    }
}
//main fnction
int main(void){
    struct msg_fmt *msg[4];
     //length is taken as 4, because ID is 2 bit identifier so 4 value of ID are possible
    char packet[]="110010100101110" ;//1st bit 1 is sof, 2nd bit 1 is opcode SET, 3rd and 4th bit 00 is identifier ID,5th and 6th bit 10 is data typ int here,7,8 and 9th bit 100 is data length  bit, 10,11,12th bit 1011 is payload, last 2 bits are checksum
    int found=0;

  if(packet[1]=='1'){ //set frame recieved
  for(int i=0;i<4;i++){
    // check the entire msg array if the msg ID exists, if yes then modify the existing msg
    if(check_msg_exists(msg[i],packet)){
        found=1;
        printf("found");
    set_existing_data(msg[i],packet);
    break;
    }}
    if(found==0){
       for(int i=0;i<4;i++){ 
        if(is_memory_empty(msg[i],packet)){
            set_new_data(msg[i],packet);
           found=1;
           printf("found");
            break;
    }}}
    if(found==0){
        printf("there is no empty space");
    }}
else //get frame recieved// Now check for ID
{
    if(packet[1]=='0'){
    for(int i=0;i<4;i++){ //check entire msg[]
    if(msg[i]->id==packet[2]*10+packet[3])
    {
        char str[20];
        str[0]=1;
        str[1]=1;
        get_data(msg[i],packet,str); 
        break;// msg ID exists so break
    }
    }}}
    printf("found is %d",found);
    return 0;
}


