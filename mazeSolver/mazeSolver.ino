/*
 Sample Line Following Code for the Robojunkies LF-2 robot
*/

#define speedL 10
#define IN1 9
#define IN2 8
#define IN3 7
#define IN4 6
#define speedR 5 


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

void Cases(){
    uint8_t reading = PINC & 0x1F;      //Capture the sensor reading
  reading = (~reading)&0x1F;  // uint8_t reading = PINC & 0x1F;      //Capture the sensor reading
  

  if(reading=(~reading)&0x19){
    move_car(left);
  }else if(reading==0x07){    
    move_car(still);
    if(reading=(~reading)&0x00)){
      move_car(right);      
    }else{
      move_car(forward);
    }
  }
 
      
  }
}