#include <avr/io.h>
#include <avr/interrupt.h>

#define encoder 2
#define encoder 3

#define speedL 10
#define IN1 7
#define IN2 6
#define IN3 9
#define IN4 8
#define speedR 11

#define speedy 100

//long printing_count = 0;

int right_pulses = 0;
int left_pulses = 0;

int right_pulses_prev = 0;
int left_pulses_prev = 0;

long long start_time_l = 0;
long long start_time_r = 0;

int count = 1000;

long long current_time = 0;
long long dt = 0;

long rpm_left = 0;
long rpm_right = 0;

long long encoder_resolution = pow(10,3) * (60/20);

ISR(INT0_vect) {
  left_pulses += 1;
}

ISR(INT1_vect) {
  right_pulses += 1;
}

void INT0_Init (void) {
  SREG &= ~(1<<7);
  DDRD &= (~(1<<PD2));
  EIMSK |= (1<<INT0);
  EICRA |= (1<<ISC00) | (1<<ISC01);
  SREG |= (1<<7);
}

void INT1_Init (void) {
  SREG &= ~(1<<7);
  DDRD &= (~(1<<PD3));
  EIMSK |= (1<<INT1);
  EICRA |= (1<<ISC10) | (1<<ISC11);
  SREG |= (1<<7);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(speedL, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(speedR, OUTPUT);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(speedL, speedy);
  analogWrite(speedR, speedy);
  start_time_l = millis();
  start_time_r = millis();
  INT0_Init();
  INT1_Init();  
}

void loop() {
  // put your main code here, to run repeatedly:
  current_time = millis();
  dt = current_time - start_time_r;

  rpm_right = ((right_pulses - right_pulses_prev) * encoder_resolution) /(float)dt;
  rpm_left = ((left_pulses - left_pulses_prev) * encoder_resolution) /(float)dt;

  if((left_pulses - left_pulses_prev) >= count){
    left_pulses_prev = left_pulses;
    start_time_l = millis();
  }

  if((right_pulses - right_pulses_prev) >= count){
    right_pulses_prev = right_pulses;
    start_time_r = millis();
  }
  
//  if(printing_count == 10000)
//  {
//    Serial.print("l: ");
//    Serial.println(rpm_left);
//    Serial.print("r: ");
//    Serial.println(rpm_right);
//    printing_count = 0;
//  }
//  
//  printing_count++;
}
