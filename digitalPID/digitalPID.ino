/*
 Sample Line Following Code for the Robojunkies LF-2 robot
*/

// Speed 100
// kp =0.055
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

// int P, D, previousError, PIDvalue, error;
int lsp, rsp;
int baseSpeed = 240;

float line = 0;
int line0, line1, line2, line3, line4, no_line, speed_a, speed_b;
int threshold = (400 + 19) / 2;
int l_loss = 200, r_loss = 200, turning_speed = 255;

void setup()
{
  DDRC &= ~(0x1F);
  pinMode(speedL, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(speedR, OUTPUT);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(speedL, baseSpeed);
  analogWrite(speedR, baseSpeed);
  // Serial.begin(9600);
}


void loop()
{
  linefollow();
}

int read_eye(int eye) {
 int line = analogRead(eye);
 if (line < threshold) return 0;
 return 1;
}

void linefollow()
{
  line0 = read_eye(A0);
  line1 = read_eye(A1);
  line2 = read_eye(A2);
  line3 = read_eye(A3);
  line4 = read_eye(A4);
  
  line = (4*line0) + (2*line1) + (-2*line3) + (-4*line4);
  line /= (line0 + line1 + line2 + line3 + line4);

  lsp = baseSpeed;
  rsp = baseSpeed;
  if(line < 0) lsp += (line * l_loss);
  if(line > 0) rsp += (-1 * line * r_loss);

  if (lsp > 255) lsp = 255;
  if (lsp < 0)   lsp = 0;
  if (rsp > 255) rsp = 255;
  if (rsp < 0)   rsp = 0;
  analogWrite(speedL, lsp);
  analogWrite(speedR, rsp);
}
