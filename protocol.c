#include "protocol.h"
#include<math.h>
#include<string.h>
 msg_fmt *msg = NULL;

/* setters prototypes */
void set_opcode(uint8_t);
void set_type(uint8_t type){
    msg->typ=type;
}
void set_length(uint8_t length){
    msg->len=length;
}
void set_value(uint8_t data ){
     msg->val=data;
}
void set_chksum(uint16_t);
 void convert2bin(int arr[],int dec,int n,int start){
    int temparr[100];
    int i=0;
    int count=0;
while(dec>0)  
{    
temparr[count]=dec%2;    
dec=dec/2;  
count++; 
}  
int remaining=n-count;
for(int i=0;i<remaining;i++){// append the 0s in front to make complete byte
    arr[i+start]=0;
}
for(int i=count-1;i>-1;i--){// append the reverse of temporary array recieved during dec to binary converion
    arr[start+n-i-1]=temparr[i];
}   
 }
/* getters prototypes */
uint8_t get_opcode(void);
void get_type(int send_packet2[]){
    convert2bin(send_packet2,msg->typ,2,2);   // here 2 indicates that type field is of length 2, so we want 2 bit binary number

}
void get_length(int send_packet2[]){
    convert2bin(send_packet2,msg->len,3,4);   // here 3 indicates that length field is of length 3, so we want 3 bit binary number

}
void get_value(int send_packet2[]){    
   convert2bin(send_packet2,msg->val,8*(msg->len),7);   // here n=8 indicates that data field is of length n, so we want 8 bit binary number. same function can be used if type is int or string

}
void initials(int send_packet2[],int n){
    send_packet2[0]=1;
    send_packet2[1]=1;
}
uint16_t get_chksum(void);

/* Helper functions */
uint8_t create_msg();
uint8_t calculate_chksum();

/* APIs */
uint8_t frame_packet();
uint8_t parse_packet();
uint8_t create_msg() {
    if( msg==NULL) {
        msg = (msg_fmt *)malloc(sizeof(msg_fmt));   
        return (1);
    } else {
        // msg already created - singleton pattern
        return (0);
    }
}
// SOF =1
//followed by OPCODE 1 or 0 SET o GET
//followed by 2 bit type, 01 in case of CHAR
//Followed by 3 bit for length field, 001 in case of char
//followed by 8 bit of data in case of CHAR (8*length)
//checksum implementation is pending
// Data length is calculated on the basis of type of data
//t stands for true , its binary is 01110100   (LED STATUs ON)
//f stands for false, its binay is 01100110     (LED STATUs OFF)
// typ is 01 for char
//3 bits reserved for length (maximum 7 byte of data) here we are ssending 1 CHAR=1 byte=8bits So len field is 001
// Temporarily silence the linker. The protocol files are supposed to get integrated with app file

int main() { //packet is input  to test operation SET and packet2 is input to test GET operation. 
//int packet[100]={1,1,0,1,0,0,1,0,1,1,1,0,1,0,0}; //send t   0,1,1,1,0,1,0,0  TRUE      Taken binary ASCII values of t and f
int packet[100]={1,1,0,1,0,0,1,0,1,1,0,0,1,1,0}; //send f      0,1,1,0,0,1,1,0    FALSE
int packet2[100]={1,0,};  //get the data

//to SET the msg
    if(packet[0]==1){
        printf("valid Start of frame, SET data\n");
        if(packet[1]==SET){// set frame recieved
        //check type
        int typ=packet[2]*2+packet[3];
        if(typ==CHAR){ //if character is recieved
            printf("Character data recieved\n");
            uint8_t length=packet[4]*4+packet[5]*2+packet[6];     //binary to decimal conversion
            uint8_t data=0;
            int count=7;
            for(int i=7;i<15;i++){   //save 8 bit of payload into variable data
                data=data+pow(2,count)*packet[i];
                count--;
            }
            create_msg();
            set_type(typ);
            set_length(length);
            set_value(data);
            printf("msg typ is %d\n",msg->typ);
            printf("msg length is %d\n",msg->len);
            printf("msg data is %d\n",msg->val);
    } }}
    if(packet[0]==1){   //valid SOF recieved
        if(packet2[1]==GET){
            printf("valid Start of frame, GET data\n");
            int send_packet2[100]={0};
            initials(send_packet2,100);
            get_type(send_packet2);
            get_length(send_packet2);
            get_value(send_packet2);   //if length  is 1, then 8 bit of data is expected, 
            int size=7+8*msg->len;
            printf("send this data\n");
           
            for (int i=0;i<size;i++){
                printf("%d",send_packet2[i]);
            }
    }
    }
    if(packet[0]==0 &&packet2[0]==0){
        printf("invalid Start of frame. Dontt do anything");
    }
     return 0;
}





