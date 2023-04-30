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

int P, D, previousError, PIDvalue, error;
int lsp, rsp;
int baseSpeed = 120;

float Kp = 0.055;
float Kd = 0.18;
char data;

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

void rotate_90(car_dir dir)
{
  uint8_t turningSpeed = 100;
  uint8_t pin, pin_2;
  delay(100);
  move_car(still);
  delay(500);
  switch(dir)
  {
    case left:
    pin = A0;
    pin_2 = A2;
    break;
    case right:
    pin = A4;
    pin_2 = A2;
    break;
    default:
    move_car(still);
    return;
  }
  analogWrite(speedR, turningSpeed);
  analogWrite(speedL, turningSpeed);
  move_car(dir);
  while(digitalRead(pin) == 1);
  while(digitalRead(pin_2) == 1);
  move_car(still);
  
}

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
    digitalWrite(pin2, HIGH);
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
    move_motor(IN1, IN2, m_forward);
    move_motor(IN3, IN4, m_backward);
    break;
  }
}

void Cases()
{

  int s1 = digitalRead(A0);
  int s2 = digitalRead(A1);
  int s3 = digitalRead(A2);
  int s4 = digitalRead(A3);
  int s5 = digitalRead(A4);

  if (s1 == 0 && s2 == 0 && s3 == 0){
    rotate_90(left);
  }
  else if (s4 == 0 && s5 == 0 && s3 == 0){
    //_delay_ms(100);

    while(digitalRead(A4)==0);

  //  s1 = digitalRead(A0);
  //  s2 = digitalRead(A1);
    s3 = digitalRead(A2);
  //  s4 = digitalRead(A3);
  //  s5 = digitalRead(A4);
   


    if ((1)){
       rotate_90(right);
    } else{
       move_car(forward);
    }
  
  } else if((s1 == 1) && (s2 == 1) && (s4 == 1) && (s5 == 1) && (s3 == 1)){
    // U_turn();
    
  }else{
    move_car(forward);
    linefollow();
  }
}

void linefollow()
{

  int sensor1 = analogRead(A0);
  int sensor2 = analogRead(A1);
  int sensor3 = analogRead(A2);
  int sensor4 = analogRead(A3);
  int sensor5 = analogRead(A4);
  

  int error = (sensor1+sensor2) - (sensor5 + sensor4);
  if(error > 900) {
    error = 900;
  }

  P = error;
  D = error - previousError;


  PIDvalue = (Kp * P)  + (Kd * D);
  previousError = error;

  lsp = baseSpeed + PIDvalue;
  rsp = baseSpeed - PIDvalue;

  if (lsp > 255) lsp = 255;
  if (lsp < 0)   lsp = 0;
  if (rsp > 255) rsp = 255;
  if (rsp < 0)   rsp = 0;
  analogWrite(speedL, lsp);
  analogWrite(speedR, rsp);
  // }
}

void setup()
{
  previousError = 0;  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(speedL, OUTPUT);
  pinMode(speedR, OUTPUT);
  DDRC &= ~(0x1F);
  analogWrite(speedL, baseSpeed);
  analogWrite(speedR, baseSpeed);
  move_car(forward); 
  Serial.begin(9600);
}

void loop()
{
  Cases();
}