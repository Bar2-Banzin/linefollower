#define speedL 10
#define IN1 9
#define IN2 8
#define IN3 7
#define IN4 6
#define speedR 5 

#define max_speed 60


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
  right
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
  if((prev == right || prev == left) && reading == still);
  else if(prev != reading) move_car(reading);
  //delay(20);
  prev = reading;
}



void move_motor(char pin1, char pin2, motor_dir dir)
{
  //delay(10);
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
    break;
    case backward:
    move_motor(IN1, IN2, m_backward);
    move_motor(IN3, IN4, m_backward);
    break;
    case left:
    move_motor(IN1, IN2, m_forward);
    move_motor(IN3, IN4, m_backward);
    break;
    case right:
    move_motor(IN1, IN2, m_backward);
    move_motor(IN3, IN4, m_forward);
    break;
  }
}

car_dir read_line()
{
  uint8_t reading = PINC & 0x1F;      //Capture the sensor reading
  reading = (~reading)&0x1F;
  Serial.println(reading);
  switch(reading)
  {
    case 0b00000:
    return still;
    case 0b00001:
    return right;
    case 0b00010:
    return right;
    case 0b00011:
    return right;
    case 0b00100:
    return forward;
    case 0b00101:
    return forward;
    case 0b00110:
    return forward;
    case 0b00111:
    return right;
    case 0b01000:
    return left;
    case 0b01001: //////////////
    return left;    
    case 0b01010: //////////////////////////
    return right;
    case 0b01011:
    return right;
    case 0b01100:
    return forward;
    case 0b01101:
    return forward;
    case 0b01110:
    return forward;
    case 0b01111:
    return right;
    case 0b10000:
    return left;
    case 0b10001: ////////////////////////////
    return left;
    case 0b10010://///////////////
    return right;
    case 0b10011:
    return right;
    case 0b10100:
    return left;
    case 0b10101:
    return forward;
    case 0b10110:
    return forward;
    case 0b10111:
    return right;
    case 0b11000:
    return left;
    case 0b11001:
    return left;
    case 0b11010:
    return left;
    case 0b11011:////////////////////////////////
    return right;
    case 0b11100:
    return left;
    case 0b11101:
    return left;
    case 0b11110:
    return left;
    case 0b11111:
    return still;
    default :
    return still;

    }
  // char middle = ~reading & 0x04;   //The reading has zero in the middle
  // if(middle == 4) return forward;

  // char l = (reading >> 3);
  // char r = reading & 0x03;

  // if(l > r) return right;
  // if(r > l) return left;
  // return still;
}