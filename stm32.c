//Commands called by ASCII

//hex=dec,cmd name, src, dest
//0x00=0 ,Ping ,check the subsystem status     ,Master,All   , {PING}->{ACK AFDEVSAT}or{NACK}
//0x02=2 ,ACK  ,Acknowledge reply              ,ALL   ,Master, {ACK}//not applicable
//0x03=3 ,NACK ,Not Acknowledge reply          ,ALL   ,Master, {NACK}//not applicable 
//0x04=4 ,GD   ,Get parameter data from device ,Master,All   , {GD EPS}->{EPS ACK GD SPC E6 IPC E6}or{NACK}
//0x05=4 ,PD   ,Put parameter data to device   ,Master,All   , {PD EPS SPC xxxxx yyyyy}->{EPS ACK PD xxxxx yyyyy}or{NACK}
//0x06=6 ,RD   ,Read data                      ,Master,All   , {RD EPS}->{EPS ACK RD MASTER}
//0x07=7 ,WD   ,Write data                     ,Master,All   , {WD EPS}->{EPS ACK WD MASTER}
//0x0b=11,SON  ,Switch ON subsystem            ,Master,EPS   , {SON PYLD}or{SON ADCS}or{SON GCS}
//0x0c=12,SOF  ,Switch OFF subsystem           ,Master,EPS   , {SOF PYLD}or{SOF ADCS}or{SOF GCS}
//0x15=21,SM   ,Set subsystem Mode of operation,Master,ALL   , {SM xxx x}->{EPS ACK SM xxx y MASTER} xxx=subsyt, y=mode
//0x16=22,GM   ,Get subsystem Mode of operation,Master,ALL   , {GM PYLD x}->{EPS ACK GM PYLD x MASTER},{GM ADCS x},{GM GCS x}
//0x17=23,GSC  ,Get Synch Counter value        ,Master,ALL   , {GSC}->{EPS ACK GSC EPS MASTER}->{}->{END}
//0x18=24,SSC  ,Set Synch Counter value        ,Master,ALL   , {SSC pppp}->{EPS ACK SSC pppp MASTER END} or {EPS NACK SSC pppp}
//0x1A=26,GFP  ,Get Function Parameter         ,Master,ALL   , {GFP EPS xxxxx}->{EPS ACK GFP MASTER yyy} or {NACK}
//0x1B=27,SFP  ,Set Function Parameter         ,Master,ALL   , {SFP EPS xxxxx yyy}->{ACK SFP xxxxx yyy} or {NACK}
//0x1C=28,FON  ,Function switch ON             ,Master,ALL   , {FON}  ->{ACK FON x}
//0x1D=29,FOF  ,Function switch OFF            ,Master,ALL   , {FOF}  ->{ACK FOF x}
//0x25=37,GOSTM,Get On line Subsystem TeleMetry,Master,ALL   , {GOSTM}->{ACK GOSTM PV1 yyyy PV2 yyyy BATV yyyy MODE y MASTER}
//0x31=49,KEN  ,Kill ENable                    ,GCS   ,EPS   , {KEN}  ->{ACK KEN} or{NACK}
//0x32=50,KDIS ,Kill DISable                   ,GCS   ,EPS   , {KDIS} ->{ACK KDIS}or{NACK}

/********************************************/
//OK
int symbol_pause_count=1000000;
int intermission_pause_count=1000000;

//OK
int zero= 48;int one=49; int two=50  ; int three=51; int four=52; int five=53;
int six=54; int seven=55; int eight=56; int nine=57;
int A=65 ;int B=66 ;int C=67 ;int D=68 ;int E=69 ;int F=70 ;int G=71 ;int H=72 ;int I=73 ;int J=74;
int K=75;int L=76;int M=77;int N=78;int O=79;int P=80;int Q=81;int R=82;int S=83;int T=84;
int U=85;int V=86;int W=87;int X=88;int Y=89;int Z=90;int space=32;

//OK
int receive_binary [400];
int receive_symbol [50 ];//50byte
int transmit_binary[400];
int transmit_symbol[50 ];//50byte


//OK
//initialize
for(int index=0;index<=399;index++){receive_binary [index]=0;}
for(int index=0;index<=49 ;index++){receive_symbol [index]=0;}
for(int index=0;index<=399;index++){transmit_binary[index]=0;}
for(int index=0;index<=49 ;index++){transmit_symbol[index]=0;}


//OK
int bit_transmit(int bit){//bit_transmit
if(bit==1){
  /*review TX VALUE 1*/;
  //for(int x=0;x<255;x++){for(int x=0;x<255;x++){for(int x=0;x<255;x++){}}}
  for(int i=0;i<=symbol_pause_count;i++){}//for
}//if 1
if(bit==0){
  /*review TX VALUE 0*/;
  for(int i=0;i<=symbol_pause_count;i++){};
}//if 0
/*review TX VALUE INTERMISSION*/;
for(int i=0;i<=intermission_pause_count;i++){};//for intermission
return 0;
}//bit_transmit


//OK
int byte_transmit(int mybytenumber){
for(int leftshift=7;leftshift>=0;leftshift--){
bit_transmit( (int) (mybytenumber&(1<<leftshift)) );
}//for
return 0;
}//byte_transmit

//OK
int receive_binary_to_receive_symbol(){
for(int symbol_index=0;symbol_index<=49;symbol_index++){
  int sum=0;
  for(int leftshift=7;leftshift>=0;leftshift--){sum=sum+(receive_binary[symbol*8]*((int)(1<<7)));}//for
  receive_symbol[symbol_index]=sum;
}//for
return 0;
}//receive_binary_to_receive_symbol


//OK
int nack_response(){
if(/*review TX ACTIVE*/){//if
 int response_array[]={N,A,C,K};
 for(int symbol_index=0;symbol_index<=3;symbol_index++){//for
   byte_transmit(response_array[symbol_index]);
 }//for
}//if
return 0;
}//nack_response


//OK
int ping_check(){//ping response
if(
(receive_symbol[0]==P    )&&(receive_symbol[1]==I)&&(receive_symbol[2]==N)&&(receive_symbol[3]==G)&&
(receive_symbol[4]==space)&&(receive_symbol[5]==E)&&(receive_symbol[6]==P)&&(receive_symbol[7]==S)&&
/*review TX ACTIVE*/
){//ping detected
int response_array[]={E,P,S,space,A,C,K,space,P,I,N,G,space,A,F,D,E,V,S,A,T,space,M,A,S,T,E,R};
for(int index=0;index<=27;index++){//for
byte_transmit((response_array[index]));
}//for
}//ping detected
else{nack_response();}
return 0;
}//ping check



int gd_check(){
if(
(receive_symbol[0]==G    )&&(receive_symbol[1]==D)&&(receive_symbol[2]==space)&&(receive_symbol[3]==E)&&
(receive_symbol[4]==P)&&(receive_symbol[5]==S)&&/*review TX ACTIVE*/
){//gd detected
int response_array[31]={E,P,S,space,A,C,K,space,G,D,space,S,P,C,space,E,6,space,I,P,C,space,E,6,space,M,A,S,T,E,R};
for(int symbol_index=0;symbol_index<=30;symbol_index++){//for
byte_transmit((response_array[symbol_index]));
}//for
}//gd detected
else{if(/*review TX ACTIVE*/){nack_response();}}
return 0;
}


/**HMMMM
int pd_check(){
if(
(receive_symbol[0]==P)&&(receive_symbol[1]==D)&&(receive_symbol[2]==space)&&(receive_symbol[3]==E)&&
(receive_symbol[4]==P)&&(receive_symbol[5]==S)&&(receive_symbol[6]==space)&&/*review TX ACTIVE*/
){//pd detected
receive_symbol[6];receive_symbol[6];receive_symbol[6];receive_symbol[6];receive_symbol[6];receive_symbol[6];
int response_array[31]={E,P,S,space,A,C,K,space,P,D,space,S,P,C,space,E,6,space,I,P,C,space,E,6,space,M,A,S,T,E,R};
for(int symbol_index=0;symbol_index<=30;symbol_index++){//for
byte_transmit((response_array[symbol_index]));
}//for
}//pd detected
else{if(/*review TX ACTIVE*/){nack_response();}}
return 0;
}
HMM
***********/




int ken_check(){
if(
(receive_symbol[0]==K)&&(receive_symbol[1]==E)&&(receive_symbol[2]==N)&&(receive_symbol[3]==S)&&/*review TX ACTIVE*/
){//kdis detected
/*review KDIS*/
int response_array[12]={E,P,S,space,A,C,K,space,K,D,I,S};
for(int symbol_index=0;symbol_index<=11;symbol_index++){//for
byte_transmit((response_array[symbol_index]));
}//for
}//kdis detected
else{if(/*review TX ACTIVE*/){nack_response();}}
return 0;
}





int kdis_check(){
if(
(receive_symbol[0]==K)&&(receive_symbol[1]==D)&&(receive_symbol[2]==I)&&(receive_symbol[3]==S)&&/*review TX ACTIVE*/
){//kdis detected
/*review KDIS*/
int response_array[12]={E,P,S,space,A,C,K,space,K,D,I,S};
for(int symbol_index=0;symbol_index<=11;symbol_index++){//for
byte_transmit((response_array[symbol_index]));
}//for
}//kdis detected
else{if(/*review TX ACTIVE*/){nack_response();}}
return 0;
}











/*******************************************/

//EPS  - battery voltage, panel voltage, messages
//OBC  - messages
//ADCS - on,off,messages
//STRUCTURE
//PAYLOAD - on,off,high,moderate,low,alarm
//COMM/GCS

int one=101; int two=102  ; int three=103; int four=104; int five=105;
int six=106; int seven=107; int eight=108; int nine=109;
int A=1 ;int B=2 ;int C=3 ;int D=4 ;int E=5 ;int F=6 ;int G=7 ;int H=8 ;int I=9 ;int J=10;
int K=11;int L=12;int M=13;int N=14;int O=15;int P=16;int Q=17;int R=18;int S=19;int T=20;
int U=21;int V=22;int W=23;int X=24;int Y=25;int Z=26;int space=0; 



int receive_binary [160];
int receive_symbol [20 ];//20byte
int transmit_binary[160];
int transmit_symbol[20 ];//20byte
//initialize
for(int index=0;index<=159;index++){receive_binary [index]=0;}
for(int index=0;index<=19 ;index++){receive_symbol [index]=0;}
for(int index=0;index<=159;index++){transmit_binary[index]=0;}
for(int index=0;index<=19 ;index++){transmit_symbol[index]=0;}

int payload_status=0;
int adcs_status=0;
int obc_status=0;
int comm_status=0;
int eps_status=0;

int statusframebitnumber=0;

int transmit_bit_position=0;


int executeframe(){
//action
if((receive_symbol[0]==F )&&(receive_symbol[1]==O)&&(receive_symbol[2]==N )&&(receive_symbol[3]== )){
//action&change status
}
if((receive_symbol[0]== )&&(receive_symbol[1]== )&&(receive_symbol[2]== )&&(receive_symbol[3]== )){
//action&change status
}
if((receive_symbol[0]== )&&(receive_symbol[1]== )&&(receive_symbol[2]== )&&(receive_symbol[3]== )){
//action&change status
}
return 0;
}//executeframe

int transmitstatusbit(int transmission_bit_position){
//response
int response[20]={,,,,,,,,,,,,,,,,,,,};
if(payload_status==){response[]=;response[]=;}
if(payload_status==){response[]=;response[]=;}
if(payload_status==){response[]=;response[]=;}
if(   adcs_status==){response[]=;response[]=;}
if(   adcs_status==){response[]=;response[]=;}
if(   adcs_status==){response[]=;response[]=;}
if(   comm_status==){response[]=;response[]=;}
if(   comm_status==){response[]=;response[]=;}
if(   comm_status==){response[]=;response[]=;}
if(    obc_status==){response[]=;response[]=;}
if(    obc_status==){response[]=;response[]=;}
if(    obc_status==){response[]=;response[]=;}
if(    eps_status==){response[]=;response[]=;}
if(    eps_status==){response[]=;response[]=;}
if(    eps_status==){response[]=;response[]=;}
int result=((response[(transmission_bit_position//8)])&(1<<(7-(transmission_bit_position%8))));
TXA(result);TXB(!result);TXC(result);TXD(!result);
return 0;
}//transmitstatusbit

int shiftleftstore(int value){
for(int index=0;index<=158;index++){receive[index]=receive[index+1];};
receive[159]=value;
return 0;
}//shiftleftstore

int main(){///main
while(1){//while
//receive raw binary
int skip=0;
if(skip==0,RXA()==, RXB()==, RXC()==){shiftleftstore(0);executeframe();while(RXA()==, RXB()==, RXC()==);skip=1;}
if(skip==0,RXA()==, RXB()==, RXC()==){shiftleftstore(1);executeframe();while(RXA()==, RXB()==, RXC()==);skip=1;}
if(skip==0,RXA()==, RXB()==, RXC()==){                                 while(RXA()==, RXB()==, RXC()==);skip=1;}/*intermission*/
//transmit raw binary
if( transmit_bit_position   ==320){transmit_bit_position=0;}
if((transmit_bit_position%2)==0  ){transmitstatusbit(transmit_bit_position/2);/*actual*/}else{TXA();TXB();TXC();TXD();/*intermission*/}
transmit_bit_position=transmit_bit_position+1;
}//while
}//main

