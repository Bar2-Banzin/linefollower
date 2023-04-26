/*
 Sample Line Following Code for the Robojunkies LF-2 robot
*/
#include <util/delay.h>
#define speedL 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 10
#define speedR 6
// #define L 3
// #define R 13
// #define F 2
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Timers
unsigned long timer = 0;
float timeStep = 0.01;

// Pitch, Roll and Yaw values
float pitch = 0;
float roll = 0;
float yaw = 0;
int P, D, previousError, PIDvalue, error;
int lsp, rsp;
int lfspeed = 200;

float Kp = 0;
float Kd = 0;
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

// void rotate_90(car_dir dir)
// {
//   uint8_t firstPin, secondPin;
//   uint8_t first = 0, second = 0, prevF = 0, prevS = 0;
//   switch(car_dir)
//   {
//     case left:
//     firstPin = 17;
//     secondPin = 16;
//     break;
//     case right:
//     firstPin = 16;
//     secondPin = 17;
//     break;
//     default:
//     move_car(still);
//     return;
//   }

//   move_car(dir);
//   while(!(first == 0 && prevF == 1))
//   {
//     prevF = first;
//     first = digitalRead(firstPin);
//   }
  
//   while(!(second == 0 && prevS == 1))
//   {
//     prevS = second;
//     second = digitalRead(secondPin);
//   }

//   move_car(still);
// }

void move_motor(char pin1, char pin2, motor_dir dir)
{
  // delay(10);
  switch (dir)
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
    digitalWrite(pin2, LOW);
    break;
  }
}


void move_car(car_dir dir)
{
  switch (dir)
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
void linefollow()
{
  // if(analogRead(16)==0){
  //    analogWrite(speedL, 180);
  // analogWrite(speedR, 180);
    
  // }
while(1){   
  uint8_t reading = PINC & 0x1F;
   // Capture the sensor reading
  reading = (~reading) & 0x1F;
  int error = ((reading & 0x18)>>3) - (reading&0x03);

  P = error;

if(P>0){//droit 11  00
rsp=180;
lsp=0;
} else if(P<0){//gauche
  rsp=0;
lsp=180;
} else if(digitalRead(16)==0){
   digitalWrite(13,HIGH); 
  lsp=100;
rsp=100;
break ;
}else {
   lsp=100;
rsp=100;
}
  // D = error - previousError;

  // PIDvalue = (Kp * P) + (Kd * D);
  // previousError = error;

  // lsp = lfspeed + PIDvalue;
  // rsp = lfspeed - PIDvalue;

  // if (lsp > 255) {
  //   lsp = 255;
  // }
  // if (lsp < 0) {
  //   lsp = 0;
  // }
  // if (rsp > 255) {
  //   rsp = 255;
  // }
  // if (rsp < 0) {
  //   rsp = 0;
  // }
  analogWrite(speedL, lsp);
  analogWrite(speedR, rsp);
}
}
void move_left(){
  
  _delay_ms(100);
 
  move_car(left);

 // _delay_ms(100);
  


  start_turn();
  // while(digitalRead(A3)==1){
  //   move_car(left);
  // }
  // _delay_ms(50);
  

  move_car(still);
 
  
}

void move_right(){
    // _delay_ms(95);
 
  move_car(right);

 // _delay_ms(100);
  start_turn();

  // while(digitalRead(A3)==1){
  //   move_car(right);
  // }
  // _delay_ms(50);
  move_car(forward);


}
void move_forward(){
  move_car(forward);
  // _delay_ms(1000);

}
void U_turn(){
  // digitalWrite(L,LOW);
  // digitalWrite(R,LOW);
  // digitalWrite(F,LOW);
  // move_car(still);
  // _delay_ms(1000);
  move_car(still);
}



car_dir read_line()
{
  char reading = PINC & 0x1F; // Capture the sensor reading

  char middle = ~reading & 0x04; // The reading has zero in the middle

  char l = (reading >> 3);
  char r = reading & 0x03;
  Serial.println("In readline");

  if (l > r)
    return right;
  if (r > l)
    return left;
  if (middle == 4)
    return forward;

  return still;
}

void Cases()
{
  uint8_t reading = PINC & 0x0F; // Capture the sensor reading
  reading = (~reading) & 0x0F;
  uint8_t read_sensor= digitalRead(3);   
  // delay(50);
  // To go left 11000
  if (((reading)&0x03) == 0x03){
    move_right();
      // digitalWrite(13,HIGH);
  // _delay_ms(1000);
  }

  else if ((reading & 0x08) == 0x08 && ((~read_sensor) == 0x01)){
    _delay_ms(50);
     reading = PINC & 0x0F; // Capture the sensor reading
     reading = (~reading) & 0x0F;  
     read_sensor= digitalRead(3);   
     if ((reading) == 0x00 && ((~read_sensor) == 0x00)){
       move_left();
    } else{
       move_forward();
    }
  
  }  else if(reading  == 0x00 && ((~read_sensor) == 0x00)){
    U_turn();
    
   }else{
    // digitalWrite(13,LOW);
    move_forward();
  }
}

void setup()
{
  Kp = 0.00001;
  // Kd = 0.040;
  previousError = 0;  
  // Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(speedL, OUTPUT);
  pinMode(speedR, OUTPUT);
  // pinMode(L, OUTPUT);
  // pinMode(R, OUTPUT);
  // pinMode(F, OUTPUT);
  pinMode(12,OUTPUT);
  DDRC &= ~(0x0F);
  pinMode(3, INPUT);
  analogWrite(speedL, 100);
  analogWrite(speedR, 100);
  move_car(forward);
  Serial.begin(115200);

  // Initialize MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  
  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  mpu.calibrateGyro();

  // Set threshold sensivty. Default 3.
  // If you don't want use threshold, comment this line or set 0.
  mpu.setThreshold(3); 
  // digitalWrite(L,HIGH);
  // digitalWrite(F,HIGH);
  // digitalWrite(12,HIGH); 
  // move_car(still);
  //delay(2000);
}

void start_turn(){
   Vector norm = mpu.readNormalizeGyro();

  // Calculate Pitch, Roll and Yaw
  pitch = pitch + norm.YAxis * timeStep;
  float intial_position = roll + norm.XAxis * timeStep;
  yaw = yaw + norm.ZAxis * timeStep;

  
  while(1){
    digitalWrite(12,HIGH);

    timer = millis();

    // Read normalized values
    Vector norm = mpu.readNormalizeGyro();

    // Calculate Pitch, Roll and Yaw
    pitch = pitch + norm.YAxis * timeStep;
    roll = roll + norm.XAxis * timeStep;
    yaw = yaw + norm.ZAxis * timeStep;
      Serial.print("differnece ");
      Serial.println(abs(roll-intial_position));
      Serial.print("ROLL=");
      Serial.print(roll);
      Serial.print("__");
      Serial.print(pitch);
      Serial.print("__");
      Serial.println(yaw);
      Serial.print("intial_position");
      Serial.println(intial_position);
    

    if(abs(roll-intial_position)>30){
      move_car(still);
        digitalWrite(12,LOW);
        delay(10000);
      break;
    }
  // Wait to full timeStep period
  delay((timeStep*1000) - (millis() - timer));
  }
}
void loop()
{
  Cases();
  // Serial.println(~(PINC & 0x1F) & 0x1F);
}