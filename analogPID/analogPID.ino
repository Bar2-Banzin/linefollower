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
int lfspeed = 80;
int count = 0;
// int lfspeed = 100;

float Kp = 0;
float Kd = 0;

int c = 1, h = 1, s = 1;

void setup()
{
  Kp = 0.04;
  Kd = 0.06;
  DDRC &= ~(0x3F);
  previousError = 0;
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
  Serial.begin(9600);
}


void loop()
{
  //int x = analogRead(16);
  // Serial.print("Mid sensor: ");
  // Serial.println(x);
  // Serial.print("A5: ");
  // Serial.println(analogRead(A5));
  // Serial.print("A0: ");
  // Serial.println(analogRead(A0));
  //  Serial.print("A1: ");
  // Serial.println(analogRead(A1));
  //  Serial.print("A2: ");
  // Serial.println(analogRead(A2));
  //  Serial.print("A3: ");
  // Serial.println(analogRead(A3));
  //  Serial.print("A4: ");
  // Serial.println(analogRead(A4));
  // delay(1000);
  linefollow();
}

void linefollow()
{
  // int error = (analogRead(18)+analogRead(17)) - (analogRead(15) + analogRead(14));
  // int central_error = analogRead(A3) - analogRead(A2) ;
  int half_error = analogRead(A4) - analogRead(A1) ;
  int side_error = analogRead(A5) - analogRead(A0) ; 
  int error = h*half_error + s*side_error; 
  // Serial.print("Central = ");
  // Serial.println(central_error);
  
  
  P = error;
  D = error - previousError;

  PIDvalue = (Kp * P) + (Kd * D);

  // delay(3000); 
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
  if(count == 2000){
  Serial.print("PID = ");
  Serial.println(PIDvalue); 
  Serial.print("lsp = ");
  Serial.println(lsp);    
  Serial.print("rsp = ");
  Serial.println(rsp);
   
  Serial.print("Half = ");
  Serial.println(half_error);    
  Serial.print("Side = ");
  Serial.println(side_error);
  // delay(2000); 
  count = 0;
  }
  count++;
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