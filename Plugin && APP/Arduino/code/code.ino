#include <avr/io.h>
#include<stdio.h>
#define F_CPU 16000000UL
#define UBRRVAL 51
#define BAUD 115200
#define MYUBRR (F_CPU/((16*BAUD)-1))

void usart_init(void);
void sendbyte(unsigned char);
void sendstr(unsigned char *);
unsigned char receivebyte(void);
void receivestr(unsigned char*);

void timer0_init();

unsigned char onmsg[] = "ON\n";  
unsigned char offmsg[] = "OFF\n";
unsigned char defaultmsg[] = "LED Status:\n";
unsigned char rxdata;

uint16_t ticks, time;


void setup() {
  // put your setup code here, to run once:
// //115200
  Serial.begin(115200); 
  pinMode(13,OUTPUT);
  timer0_init();

  //usart_init(); 

}

void loop(){
while(Serial.available()>0){
  rxdata=Serial.read();
  Serial.println(rxdata);
}
}


//  void  loop(){ 
// printf("HIGHgg");

//     // char c='c';
//     // Serial.println(c);
// //   DDRB |= (1<<PB0);
  
//   while(1)
//   {
//     rxdata = receivebyte();
 //     if(rxdata == "1")
//     {
//     //  Serial.println("ll");
      
//     }
//                                                                                                                                                                                        (rxdata);
//     // Serial.println(55);
//   } 
   
  
//  }
 
void usart_init(void){
  // UBRR0H= (unsigned char)(UBRRVAL>>8);   //high byte
  // UBRR0L=(unsigned char)UBRRVAL;          //low byte
   //UBRR0L=(uint8_t)MYUBRR;
  //UBRR0H=(uint8_t)(MYUBRR>>8);
  // UCSR0B |= (1<<TXEN0) | (1<<RXEN0);    //Enable Transmitter and Receiver
  // UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);  //Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size
   UBRR0L = 51;
  UBRR0H = 0;
  UCSR0B=0x18;
  UCSR0C=0x86;
}

void sendbyte(unsigned char MSG){
    while((UCSR0A&(1<<UDRE0)) == 0);     // Wait if a byte is being transmitted
    UDR0 = MSG; 
}

void sendstr(unsigned char *s){
  unsigned char i = 0;
  while(s[i] != '\0'){
   sendbyte(s[i]);
   i++;
   }
}

unsigned char receivebyte(void){
   //Serial.println("hhh");
  while(!(UCSR0A & (1<<RXC0)));
   return UDR0;
}

void timer0_init()
{
  TCCR2B |= (1 << CS20) | (1 << CS22);
  TCNT2 = 0;
  sei();
  TIMSK2 |= (1 << TOIE2);
}

ISR(TIMER2_OVF_vect)
{
  if (ticks == 244)
  {
    TCNT0 = 238;
    ticks = 0;
    time++;
    Serial.print("Time = ");
    Serial.println(time);
  }
  else ticks++;
}