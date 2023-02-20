#define speedL 10
#define IN1 9
#define IN2 8
#define IN3 7
#define IN4 6
#define speedR 5 

#define max_speed 80

char reading = 0, prev = 0;

enum motor_dir
{
  m_still = 0,
  m_forward = 1,
  m_backward = 2
};

enum car_dir
{
  still =     0,
  forward =     0b11011,
  forward_2 =   0b10001,
  forward_3 =   0b10000,
  forward_4 =   0b00001,
  backward =  0b11111,
  left_1 = 0b01111,
  left_2 = 0b00111,
  left_3 = 0b10011,
  left_4 = 0b10111,
  left_5 = 0b00011,
  right_1 =     0b11110,
  right_2 =     0b11100,
  right_3 =     0b11001,
  right_4 =     0b11101,
  right_5 =     0b11000
};

void move_motor(char pin1, char pin2, motor_dir dir);
void move_car(car_dir dir);

void setup()
{
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  DDRC &= ~(0x1F);
  analogWrite(speedL,80);
  analogWrite(speedR,80);
  move_car(still);
  delay(2000);
}

void loop()
{
  //Just a test
  char reading = PINC & 0x1F;
  if(prev != reading) 
  {
    move_car(still);
    delay(50);
  }
  move_car(reading);
  delay(20);
  prev = reading;
}



void move_motor(char pin1, char pin2, motor_dir dir)
{
  switch(dir)
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
  switch(dir)
  {
    case still:
    move_motor(IN1, IN2, m_still);
    move_motor(IN3, IN4, m_still);
    break;
    case forward:
    case forward_2:
    case forward_3:
    case forward_4:
    move_motor(IN1, IN2, m_forward);
    move_motor(IN3, IN4, m_forward);
    break;
    case backward:
    move_motor(IN1, IN2, m_backward);
    move_motor(IN3, IN4, m_backward);
    break;
    case left_1:
    case left_2:
    case left_3:
    case left_4:
    case left_5:
    move_motor(IN1, IN2, m_forward);
    move_motor(IN3, IN4, m_still);
    break;
    case right_1:
    case right_2:
    case right_3:
    case right_4:
    case right_5:
    move_motor(IN1, IN2, m_still);
    move_motor(IN3, IN4, m_forward);
    break;
  }
}