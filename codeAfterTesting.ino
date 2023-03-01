#define speedL 10
#define IN1 9
#define IN2 8
#define IN3 7
#define IN4 6
#define speedR 5 

#define max_drift 200
#define max_speed 150


enum motor_dir
{
  m_still = 0,
  m_forward = 1,
  m_backward = 2
};

enum car_dir
{
  still,
  forward,
  backward,
  left,
  right,
};
car_dir reading = still, prev = still;

void move_motor(char pin1, char pin2, motor_dir dir);
void move_car(car_dir dir);
car_dir read_line();

void setup()
{
  Serial.begin(9600);
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
  reading = read_line();
  if(prev != reading) move_car(reading);
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
    move_motor(IN1, IN2, m_forward);
    move_motor(IN3, IN4, m_forward);
    analogWrite(speedL,max_speed);
    analogWrite(speedR,max_speed);
    break;
    case backward:
    move_motor(IN1, IN2, m_backward);
    move_motor(IN3, IN4, m_backward);
    analogWrite(speedL,max_speed);
    analogWrite(speedR,max_speed);
    break;
    case left:
    move_motor(IN1, IN2, m_forward);
    move_motor(IN3, IN4, m_backward);
    analogWrite(speedL,max_drift);
    analogWrite(speedR,max_drift);
    break;
    case right:
    move_motor(IN1, IN2, m_backward);
    move_motor(IN3, IN4, m_forward);
    analogWrite(speedL,max_drift);
    analogWrite(speedR,max_drift);
    break; 
  }
}

car_dir read_line()
{
  char reading = PINC & 0x1F;      //Capture the sensor reading

  char middle = ~reading & 0x04;   //The reading has zero in the middle
  

  char l = (reading >> 3);
  char r = reading & 0x03;

  if(l > r) return right;
  if(r > l) return left;
  if(middle == 4) return forward;
  return still;
}
