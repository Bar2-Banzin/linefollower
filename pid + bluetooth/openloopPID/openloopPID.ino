/*
 Sample Line Following Code for the Robojunkies LF-2 robot
*/

// Speed 100
//  kp =0.055
// kd=0.18  ---  0.15

//Speed 120 - BATTERIES: 3.95 3.85
//kp =0.075 ---- 0.065
//kd = 0.3 --- 0.28

// speed = 140
// kp = 0.1
// kd = 0


// speed =120  - BATTERIES: total 7.2
//  Kp = 0.075;
//  Kd = 0.37;
//  Ki = 0.001;



// #define speedL 10
// #define IN1 7
// #define IN2 6
// #define IN3 9
// #define IN4 8
// #define speedR 11

#define speedL 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 10
#define speedR 6

int P, D, previousError, PIDvalue, error;
int lsp, rsp;
int lfspeed = 120;

float Kp = 0;
float Kd = 0;
float Ki = 0;

void setup()
{
  Kp = 0.075;
  Kd = 0.45;
  Ki = 0.001;
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
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
//   Serial.begin(9600);
}


void loop()
{
  //int x = analogRead(16);
  // Serial.print("Mid sensor: ");
  // Serial.println(x);
  linefollow();
}

void linefollow()
{
  int error = (2*analogRead(A0)+analogRead(A1)) - (2*analogRead(A4) + analogRead(A3));
  if(error > 900) {
    error = 900;
  }
//   Serial.println(error);
//   delay(1000);
  static int I = 0;
  I += error;
  P = error;
  D = error - previousError;

//  PIDvalue = (Kp * P) + (Kd * D);
  PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
  previousError = error;

  lsp = lfspeed + PIDvalue;
  rsp = lfspeed - PIDvalue;

  if (lsp > 255) lsp = 255;
  if (lsp < 0)   lsp = 0;
  if (rsp > 255) rsp = 255;
  // if (rsp < 80)  rsp = 70;
  if (rsp < 0)   rsp = 0;
  analogWrite(speedL, lsp);
  analogWrite(speedR, rsp);
}
