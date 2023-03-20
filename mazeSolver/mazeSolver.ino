/*
 Sample Line Following Code for the Robojunkies LF-2 robot
*/
#include <util/delay.h>
#define speedL 5
#define IN1 9
#define IN2 8
#define IN3 7
#define IN4 6
#define speedR 10
#define L 3
#define R 13
#define F 2

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
    move_motor(IN1, IN2, m_forward);
    move_motor(IN3, IN4, m_backward);
    break;
  case right:
    move_motor(IN1, IN2, m_backward);
    move_motor(IN3, IN4, m_forward);
    break;
  }
}
void linefollow()
{
  // if(analogRead(16)==0){
  //    analogWrite(speedL, 180);
  // analogWrite(speedR, 180);
    
  // }
while(1){   
  uint8_t reading = PINC & 0x1F;
   // Capture the sensor reading
  reading = (~reading) & 0x1F;
  int error = ((reading & 0x18)>>3) - (reading&0x03);

  P = error;

if(P>0){//droit 11  00
rsp=180;
lsp=0;
} else if(P<0){//gauche
  rsp=0;
lsp=180;
} else if(digitalRead(16)==0){
   digitalWrite(13,HIGH); 
  lsp=100;
rsp=100;
break ;
}else {
   lsp=100;
rsp=100;
}
  // D = error - previousError;

  // PIDvalue = (Kp * P) + (Kd * D);
  // previousError = error;

  // lsp = lfspeed + PIDvalue;
  // rsp = lfspeed - PIDvalue;

  // if (lsp > 255) {
  //   lsp = 255;
  // }
  // if (lsp < 0) {
  //   lsp = 0;
  // }
  // if (rsp > 255) {
  //   rsp = 255;
  // }
  // if (rsp < 0) {
  //   rsp = 0;
  // }
  analogWrite(speedL, lsp);
  analogWrite(speedR, rsp);
}
}
void move_left(){
  
  _delay_ms(100);
 
  move_car(left);

 // _delay_ms(100);
  
  while(digitalRead(A3)==1){
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



car_dir read_line()
{
  char reading = PINC & 0x1F; // Capture the sensor reading

  char middle = ~reading & 0x04; // The reading has zero in the middle

  char l = (reading >> 3);
  char r = reading & 0x03;
  Serial.println("In readline");

  if (l > r)
    return right;
  if (r > l)
    return left;
  if (middle == 4)
    return forward;

  return still;
}

void Cases()
{
  uint8_t reading = PINC & 0x1F; // Capture the sensor reading
  reading = (~reading) & 0x1F;

  // delay(50);
  // To go left 11000
  if (((reading)&0x18) == 0x18){
    move_left();
      // digitalWrite(13,HIGH);
  // _delay_ms(1000);
  }

  else if ((reading & 0x03) == 0x03){
    _delay_ms(50);
     reading = PINC & 0x1F; // Capture the sensor reading
     reading = (~reading) & 0x1F;   
     if ((reading) == 0x00){
       move_right();
    } else{
       move_forward();
    }
  
  }  else if(reading  == 0x00){
    U_turn();
    
   }else{
    // digitalWrite(13,LOW);
    move_forward();
  }
}

void setup()
{
  Kp = 0.00001;
  // Kd = 0.040;
  previousError = 0;  
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(speedL, OUTPUT);
  pinMode(speedR, OUTPUT);
  pinMode(L, OUTPUT);
  pinMode(R, OUTPUT);
  pinMode(F, OUTPUT);
  DDRC &= ~(0x1F);
  analogWrite(speedL, 255);
  analogWrite(speedR, 255);
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