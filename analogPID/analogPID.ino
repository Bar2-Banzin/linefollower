/*
 Sample Line Following Code for the Robojunkies LF-2 robot
*/

#define speedL 10
#define IN1 7
#define IN2 6
#define IN3 9
#define IN4 8
#define speedR 11

int c = 1, h = 1, s = 1;
int rpm_speeds[2] = {0, 0};
int actual_speeds[2] = {0, 0};

void setup()
{
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
  linefollow(0.04, 0.06);
  motor_ctrl(speedL, rpm_speeds[0], actual_speeds[0], 0.8, 0);
  motor_ctrl(speedR, rpm_speeds[1], actual_speeds[1], 0.6, 0);
}

void linefollow(float Kp, float Kd)
{
  static int previousError = 0;
  int half_error = analogRead(A4) - analogRead(A1) ;
  int side_error = analogRead(A5) - analogRead(A0) ; 
  int error = h*half_error + s*side_error; 
  
  int P = error;
  int D = error - previousError;

  float PIDvalue = (Kp * P) + (Kd * D);
  previousError = error;

  int lsp = base_speed_RPM + PIDvalue;
  int rsp = base_speed_RPM - PIDvalue;

  if (lsp > 300) {
    lsp = 300;
  }
  if (lsp < 0) {
    lsp = 0;
  }
  if (rsp > 300) {
    rsp = 300;
  }
  if (rsp < 0) {
    rsp = 0;
  }
  // if(count == 2000){
  // Serial.print("PID = ");
  // Serial.println(PIDvalue); 
  // Serial.print("lsp = ");
  // Serial.println(lsp);    
  // Serial.print("rsp = ");
  // Serial.println(rsp);
   
  // Serial.print("Half = ");
  // Serial.println(half_error);    
  // Serial.print("Side = ");
  // Serial.println(side_error);
  // delay(2000); 
  // count = 0;
  // }
  // count++;

  rpm_speeds[0] = lsp;
  rpm_speeds[1] = rsp;
}



void motor_ctrl(uint8_t pin, int setPoint, int actualSpeed, float Kp, float Kd)
{
  static int previousError = 0;
  int error = setPoint-actualSpeed; 
  
  int P = error;
  int D = error - previousError;

  float PIDvalue = (Kp * P) + (Kd * D);
  previousError = error;

  int out = base_speed_PWM + PIDvalue;

  if (out > 255) {
    out = 255;
  }
  if (out < 0) {
    out = 0;
  }
  analogWrite(pin, out);
}
