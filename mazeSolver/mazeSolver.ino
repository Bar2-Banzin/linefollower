/*
 Sample Line Following Code for the Robojunkies LF-2 robot
*/
#include <util/delay.h>
#define speedL 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 10
#define speedR 6
// #define L 3
// #define R 13
// #define F 2

int P, D, previousError, PIDvalue, error;
int lsp, rsp;
int lfspeed = 200;

float Kp = 0;
float Kd = 0;
enum motor_dir
{
  m_still = 0,
  m_forward = 1,
  m_backward = 2
};

enum car_dir
{
  still,
  forward,
  backward,
  left,
  right
};

// void rotate_90(car_dir dir)
// {
//   uint8_t firstPin, secondPin;
//   uint8_t first = 0, second = 0, prevF = 0, prevS = 0;
//   switch(car_dir)
//   {
//     case left:
//     firstPin = 17;
//     secondPin = 16;
//     break;
//     case right:
//     firstPin = 16;
//     secondPin = 17;
//     break;
//     default:
//     move_car(still);
//     return;
//   }

//   move_car(dir);
//   while(!(first == 0 && prevF == 1))
//   {
//     prevF = first;
//     first = digitalRead(firstPin);
//   }
  
//   while(!(second == 0 && prevS == 1))
//   {
//     prevS = second;
//     second = digitalRead(secondPin);
//   }

//   move_car(still);
// }

void move_motor(char pin1, char pin2, motor_dir dir)
{
  // delay(10);
  switch (dir)
  {
  case m_still:
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    break;
  case m_forward:
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
    break;
  case m_backward:
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    break;
  }
}


void move_car(car_dir dir)
{
  switch (dir)
  {
  case still:
    move_motor(IN1, IN2, m_still);
    move_motor(IN3, IN4, m_still);
    break;
  case forward:
    move_motor(IN1, IN2, m_forward);
    move_motor(IN3, IN4, m_forward);
    break;
  case backward:
    move_motor(IN1, IN2, m_backward);
    move_motor(IN3, IN4, m_backward);
    break;
  case left:
    move_motor(IN1, IN2, m_backward);
    move_motor(IN3, IN4, m_forward);
    break;
  case right:
    move_motor(IN1, IN2, m_backward);
    move_motor(IN3, IN4, m_forward);
    break;
  }
}
// void linefollow()
// {
//   // if(analogRead(16)==0){
//   //    analogWrite(speedL, 180);
//   // analogWrite(speedR, 180);
    
//   // }
// while(1){   
//   uint8_t reading = PINC & 0x1F;
//    // Capture the sensor reading
//   reading = (~reading) & 0x1F;
//   int error = ((reading & 0x18)>>3) - (reading&0x03);

//   P = error;

// if(P>0){//droit 11  00
// rsp=180;
// lsp=0;
// } else if(P<0){//gauche
//   rsp=0;
// lsp=180;
// } else if(digitalRead(16)==0){
//    digitalWrite(13,HIGH); 
//   lsp=100;
// rsp=100;
// break ;
// }else {
//    lsp=100;
// rsp=100;
// }
//   // D = error - previousError;

//   // PIDvalue = (Kp * P) + (Kd * D);
//   // previousError = error;

//   // lsp = lfspeed + PIDvalue;
//   // rsp = lfspeed - PIDvalue;

//   // if (lsp > 255) {
//   //   lsp = 255;
//   // }
//   // if (lsp < 0) {
//   //   lsp = 0;
//   // }
//   // if (rsp > 255) {
//   //   rsp = 255;
//   // }
//   // if (rsp < 0) {
//   //   rsp = 0;
//   // }
//   analogWrite(speedL, lsp);
//   analogWrite(speedR, rsp);
// }
// }
void move_left(){
  
  // _delay_ms(50);
 
  move_car(left);

  _delay_ms(250);
  
  while(digitalRead(A2)==0){
    move_car(left);
  }
  _delay_ms(50);
  move_car(still);
 
  
}

void move_right(){
    // _delay_ms(95);
 
  move_car(right);

 // _delay_ms(100);
  
  while(digitalRead(A3)==1){
    move_car(right);
  }
  _delay_ms(50);
  move_car(still);


}
void move_forward(){
  move_car(forward);
  // _delay_ms(1000);

}
void U_turn(){
  // digitalWrite(L,LOW);
  // digitalWrite(R,LOW);
  // digitalWrite(F,LOW);
  // move_car(still);
  // _delay_ms(1000);
  move_car(still);
}



// car_dir read_line()
// {
//   char reading = PINC & 0x1F; // Capture the sensor reading

//   char middle = ~reading & 0x04; // The reading has zero in the middle

//   char l = (reading >> 3);
//   char r = reading & 0x03;
//   Serial.println("In readline");

//   if (l > r)
//     return right;
//   if (r > l)
//     return left;
//   if (middle == 4)
//     return forward;

//   return still;
// }

void Cases()
{
  // uint8_t reading = PINC & 0x1F; // Capture the sensor reading
  // reading = (~reading) & 0x1F;

  int s1 = digitalRead(A0);
  int s2 = digitalRead(A1);
  int s3 = digitalRead(A2);
  int s4 = digitalRead(A3);
  int s5 = digitalRead(A4);

  // delay(50);
  // To go left 11000
  if ((s1 == 0) && (s2 == 0) ){
    move_left();
      // digitalWrite(13,HIGH);
  // _delay_ms(1000);
  }

  else if ((s4 == 0) && (s5 == 0) ){
    _delay_ms(50);
   s1 = digitalRead(A0);
   s2 = digitalRead(A1);
   s3 = digitalRead(A2);
   s4 = digitalRead(A3);
   s5 = digitalRead(A4); 
     if ((s1 == 1) && (s2 == 1) && (s4 == 1) && (s5 == 1) && (s3 == 1)){
       move_right();
    } else{
       move_forward();
    }
  
  }  else if((s1 == 1) && (s2 == 1) && (s4 == 1) && (s5 == 1) && (s3 == 1)){
    U_turn();
    
   }else{
    // digitalWrite(13,LOW);
    move_forward();
  }
}

void setup()
{
  // Kp = 0.00001;
  // Kd = 0.040;
  previousError = 0;  
  // Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(speedL, OUTPUT);
  pinMode(speedR, OUTPUT);
  // pinMode(L, OUTPUT);
  // pinMode(R, OUTPUT);
  // pinMode(F, OUTPUT);
  DDRC &= ~(0x1F);
  analogWrite(speedL, 100);
  analogWrite(speedR, 100);
  move_car(forward); 
  // digitalWrite(L,HIGH);
  // digitalWrite(F,HIGH);
  // digitalWrite(R,HIGH); 
  // move_car(still);
  //delay(2000);
}

void loop()
{
  Cases();
  // Serial.println(~(PINC & 0x1F) & 0x1F);
}