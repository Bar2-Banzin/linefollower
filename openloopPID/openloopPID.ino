/*
 Sample Line Following Code for the Robojunkies LF-2 robot
*/

#define speedL 10
#define IN1 7
#define IN2 6
#define IN3 9
#define IN4 8
#define speedR 11

int P, D, previousError, PIDvalue, error;
int lsp, rsp;
int lfspeed = 100;

float Kp = 0;
float Kd = 0;

void setup()
{
  Kp = 0.055;
  Kd = 0;
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
  int error = (analogRead(A0)+analogRead(A1)) - (analogRead(A4) + analogRead(A3));

  P = error;
  D = error - previousError;

  PIDvalue = (Kp * P) + (Kd * D);
  previousError = error;

  lsp = lfspeed + PIDvalue;
  rsp = lfspeed - PIDvalue;

  if (lsp > 255) {
    lsp = 255;
  }
  if (lsp < 0) {
    lsp = 0;
  }
  if (rsp > 255) {
    rsp = 255;
  }
  if (rsp < 0) {
    rsp = 0;
  }
  analogWrite(speedL, lsp);
  analogWrite(speedR, rsp);
}