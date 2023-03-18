/*
 Sample Line Following Code for the Robojunkies LF-2 robot
*/

#define speedL 5
#define IN1 7
#define IN2 6
#define IN3 9
#define IN4 8
#define speedR 10

int P, D, I, previousError, PIDvalue, error;
int lsp, rsp;
int lfspeed = 70;

float Kp = 0;
float Kd = 0;
float Ki = 0;

void setup()
{
  Kp = 0.03;
  Ki = 0;
  Kd = 0;
  DDRC &= ~(0x1F);
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
  int error = (analogRead(18)+analogRead(17)) - (analogRead(15) + analogRead(14));

  P = error;
  I = I + error;
  D = error - previousError;

  PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
  // Serial.print("P = ");
  // Serial.println(P);
  // Serial.print("I = ");
  // Serial.println(I);
  // Serial.print("D = ");
  // Serial.println(D);
  // Serial.print("PID = ");
  // Serial.println(PIDvalue);
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

// void calibrate()
// {
//   for ( int i = 1; i < 6; i++)
//   {
//     minValues[i] = analogRead(i);
//     maxValues[i] = analogRead(i);
//   }
  
//   for (int i = 0; i < 3000; i++)
//   {
//     motor1.drive(50);
//     motor2.drive(-50);

//     for ( int i = 1; i < 6; i++)
//     {
//       if (analogRead(i) < minValues[i])
//       {
//         minValues[i] = analogRead(i);
//       }
//       if (analogRead(i) > maxValues[i])
//       {
//         maxValues[i] = analogRead(i);
//       }
//     }
//   }

//   for ( int i = 1; i < 6; i++)
//   {
//     threshold[i] = (minValues[i] + maxValues[i]) / 2;
//     Serial.print(threshold[i]);
//     Serial.print("   ");
//   }
//   Serial.println();
  
//   motor1.drive(0);
//   motor2.drive(0);
// }