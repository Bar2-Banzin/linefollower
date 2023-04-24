// speed = 150  - BATTERIES: 8.08
// l_loss = 115   r_loss = 115


#define speedL 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 10
#define speedR 6

// int P, D, previousError, PIDvalue, error;
int lsp, rsp;
int baseSpeed = 150;

float line = 0;
int line0, line1, line2, line3, line4, no_line, speed_a, speed_b;
int threshold = 209;
int l_loss = 100, r_loss = 100, turning_speed = 100;

void setup()
{
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
  line0 = digitalRead(A0);
  line1 = digitalRead(A1);
  line2 = digitalRead(A2);
  line3 = digitalRead(A3);
  line4 = digitalRead(A4);
  
  
  line = (4*line0) + (2*line1) + (-2*line3) + (-4*line4);
  line /= (line0 + line1 + line2 + line3 + line4);

  lsp = baseSpeed;
  rsp = baseSpeed;
  if(line < 0)
  {
    lsp += (line * l_loss);
    // rsp += 20;
  }
  if(line > 0) 
  {
    rsp += (-1 * line * r_loss);
    // lsp += 20;
  }

  if (lsp > 255) lsp = 255;
  if (lsp < 0)   lsp = 0;
  if (rsp > 255) rsp = 255;
  if (rsp < 0)   rsp = 0;
  analogWrite(speedL, lsp);
  analogWrite(speedR, rsp);
}
