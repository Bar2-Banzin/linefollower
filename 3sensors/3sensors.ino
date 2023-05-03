/*
 Sample Line Following Code for the Robojunkies LF-2 robot
*/

// Speed 100
// kp =0.055
// kd=0.2 --- 0.18  ---  0.15

//Speed 120 - BATTERIES: 3.95 3.85
//kp =0.075 ---- 0.065
//kd = 0.3 --- 0.28   // 0.55

// speed = 140
// kp = 0.1
// kd = 0


// speed =120  - BATTERIES: total 7.2
//  Kp = 0.075;
//  Kd = 0.37;
//  Ki = 0.001;

#define speedL 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 10
#define speedR 5

//#define turning_speed 80

int P, D, previousError, PIDvalue, error;
int lsp, rsp;
int lfspeed = 150;

//float Kp = 0.08;
//float Kd = 0.0;
//float Ki = 0;
float Kp = 40;
float Kd = 80;
float Ki = 0;
char data;

void setup()
{
  DDRC &= ~(0x1F);
  previousError = 0;
  pinMode(speedL, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(speedR, OUTPUT);
  
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); // 2 motords forward
  Serial.begin(9600);
}


void loop()
{

  
 linefollow();
}

void brake() {
 digitalWrite(IN1, HIGH); // make left motor A brake
 digitalWrite(IN2, HIGH);
 digitalWrite(IN3, HIGH); // make right motor B brake
 digitalWrite(IN4, HIGH);
}

void setup_motors(int forward_a, int forward_b) {
 if (forward_a == 1) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
 } else {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
 }
 if (forward_b == 1) {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
 } else {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
 }
}

void change_speed(int leftSpeed, int rightSpeed) {
 analogWrite(speedR, rightSpeed); 
 analogWrite(speedL, leftSpeed); 
}

void linefollow()
{

  int sensor1 = digitalRead(A2);
  int sensor2 = !digitalRead(A1);
  int sensor3 = digitalRead(A0);
//  int sensor4 = analogRead(A3);
//  int sensor5 = analogRead(A4);
  
  // if (sensor1 >= 500 and sensor2 >= 500 and sensor3 >= 500 and sensor4 >= 500 and sensor5 >= 500) {
  // brake();
  // if (error < 0) {
  //  change_speed(turning_speed, turning_speed);
  //  setup_motors(0, 1);
  //  }
  // if (error > 0) {
  //  change_speed(turning_speed, turning_speed);
  //  setup_motors(1, 0);
  //   }
  // } else {

  int error = (sensor1) - (sensor3);
  if(error<0){
    sensor2 = sensor2*-1;
  }
  error += sensor2;
  error = error; // * 600
  if(error > 900) {
    error = 900;
  }
  else if(error<15&&error>-15){ 
    if(Serial.available()){
      data=Serial.read();
      if(data == '1')  lfspeed = 200;
      else lfspeed = 100;
    }
  }
  static int I = 0;
  I += error;
  P = error;
  D = error - previousError;

//  PIDvalue = (Kp * P) + (Kd * D);
  PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
  previousError = error;

  lsp = lfspeed - PIDvalue;
  rsp = lfspeed + PIDvalue;

  if (lsp > 255) lsp = 255;
  if (lsp < 0)   lsp = 0;
  if (rsp > 255) rsp = 255;
  if (rsp < 0)   rsp = 0;
  analogWrite(speedL, lsp);
  analogWrite(speedR, rsp);
  // }
}
