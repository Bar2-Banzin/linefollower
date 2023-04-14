#include <avr/io.h>
#include <avr/interrupt.h>
#define speedL 10
#define IN1 7
#define IN2 6
#define IN3 9
#define IN4 8
#define speedR 11

#define base_speed_RPM 150
#define kp_line 0.5
#define kd_line 0

#define SET_BIT(reg,pin) (reg|=(1<<pin))
#define CLEAR_BIT(reg,pin) (reg&=~(1<<pin))
#define TOGGLE_BIT(reg,pin) (reg^=(1<<pin))

int c = 1, h = 1, s = 1;
int rpm_speeds[2] = {0, 0};
int actual_speeds[2] = {0, 0};

int count_print = 0;


// long printing_count = 0;

//Encoder variables
int right_pulses = 0;
int left_pulses = 0;
// int right_pulses_prev = 0;
// int left_pulses_prev = 0;
// long long start_time_l = 0;
// long long start_time_r = 0;
// int count = 1000;
// long long current_time = 0;
// long long encoder_resolution = pow(10,3) * (60/20);
long long encoder_resolution = (60/20);

// int count_print = 0;

//Timer variables
int timer_iterations = 0;

//int motor_ctrl(uint8_t pin, int setPoint, int actualSpeed, float Kp = 0, float Kd = 0);

void setup()
{
  DDRC &= ~(0x3F);
  pinMode(speedL, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(speedR, OUTPUT);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(speedL, 0);
  analogWrite(speedR, 0);
  Serial.begin(9600);
  INT0_Init();
  INT1_Init(); 
  timer2_init(); 
}

int motor_ctrl_r(uint8_t pin, int setPoint, int actualSpeed, float Kp = 0, float Ki = 0, float Kd = 0)
{
  static int I = 0;
  static int previousError = 0;
  static int prev_set_point = 0;
  int error = setPoint-actualSpeed; 
  
  int P = error;
  I += error;
  int D = error - previousError;

  int PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
  
  previousError = error;
  prev_set_point = setPoint;
  int out = PIDvalue;
  
  if (out > 255) {
    out = 255;
  }
  if (out < 0) {
    out = 0;
  }
  if(out >= 10)
  analogWrite(pin, out);
  return out; 
}


int motor_ctrl_l (uint8_t pin, int setPoint, int actualSpeed, float Kp = 0, float Ki = 0, float Kd = 0)
{
  static int I = 0;
  static int previousError = 0;
  static int prev_set_point = 0;
  int error = setPoint-actualSpeed; 
  
  int P = error;
  I += error;
  int D = error - previousError;

  int PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
  
  previousError = error;
  prev_set_point = setPoint;
  int out = PIDvalue;
  
  if (out > 255) {
    out = 255;
  }
  if (out < 0) {
    out = 0;
  }
  if(out >= 10)
  analogWrite(pin, out);
  return out; 
}



void loop()
{
//  Serial.print("Sensor_1 = ");
//  Serial.println(analogRead(A1));
  linefollow(kp_line, kd_line);

  getMotorSpeeds();
  int actualPWM_r = motor_ctrl_r(speedR, rpm_speeds[1], actual_speeds[1], 0.5, 0.01, 0.5);
  int actualPWM_l = motor_ctrl_l(speedL, rpm_speeds[0], actual_speeds[0], 0.5, 0.01 , 0.5);
//  Serial.print(rpm_speeds[0]);
//  Serial.print(",");
//  Serial.print(rpm_speeds[1]);
//  Serial.print(",");
//  Serial.print(actual_speeds[0]);
//  Serial.print(",");
//  Serial.println(actual_speeds[1]);

//  if(printing_count == 1000)
//  {
//      
//    Serial.print("L = ");
//    Serial.println(sppedy_l);
//    Serial.print("R = ");
//    Serial.println(sppedy_r);
//    printing_count = 0;
//  }
//  
//  printing_count++;

  // Serial.print(actual_speeds[0]);
  // Serial.print(" - ");
  // Serial.println(actual_speeds[1]);
}

void linefollow(float Kp, float Kd)
{
  
  static int previousError = 0;
  int half_error = analogRead(A4) - analogRead(A1) ;
  int side_error = analogRead(A3) - analogRead(A0) ; 
  int error = h*half_error + s*side_error; 
  
  int P = error;
  int D = error - previousError;

  int PIDvalue = (Kp * P) + (Kd * D);
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
  if(count_print == 2000){
//  Serial.print("PID = ");
//  Serial.println(PIDvalue); 
  Serial.print("half = ");
  Serial.println(half_error);    
  Serial.print("side = ");
  Serial.println(side_error);
   
  Serial.print("Sensor_0 = ");
  Serial.println(analogRead(A0));
  Serial.print("Sensor_1 = ");
  Serial.println(analogRead(A1));
  Serial.print("Sensor_2 = ");
  Serial.println(analogRead(A2));
//  Serial.print("Sensor_3 = ");
//  Serial.println(analogRead(A3));
  Serial.print("Sensor_4 = ");
  Serial.println(analogRead(A4)); 
  Serial.print("Sensor_5 = ");
  Serial.println(analogRead(A5));   
  // delay(2000); 
  count_print = 0;
  }
  count_print++;

  rpm_speeds[0] = lsp;
  rpm_speeds[1] = rsp;
}


// void getMotorSpeeds()
// {
//   current_time = millis();

//   long rpm_right = ((right_pulses - right_pulses_prev) * encoder_resolution) /(float)(current_time - start_time_r);
//   long rpm_left = ((left_pulses - left_pulses_prev) * encoder_resolution) /(float)(current_time - start_time_l);

//   if((left_pulses - left_pulses_prev) >= count){
//     left_pulses_prev = left_pulses;
//     start_time_l = millis();
//   }

//   if((right_pulses - right_pulses_prev) >= count){
//     right_pulses_prev = right_pulses;
//     start_time_r = millis();
//   }

//   actual_speeds[0] = rpm_left;
//   actual_speeds[1] = rpm_right;
// }

void getMotorSpeeds()
{ 
  if(timer_iterations >= 20)
  {
    timer_iterations = 0;
    TCNT2 = 132;
    long rpm_left = (left_pulses * encoder_resolution);
    long rpm_right = (right_pulses * encoder_resolution);
    left_pulses = 0;
    right_pulses = 0;
    actual_speeds[0] = rpm_left;
    actual_speeds[1] = rpm_right;
  }
}

ISR(TIMER2_OVF_vect){
  timer_iterations++;
}

ISR(INT0_vect) {
  left_pulses += 1;
}

ISR(INT1_vect) {
  right_pulses += 1;
}

void INT0_Init (void) {
  SREG &= ~(1<<7);
  DDRD &= (~(1<<PD2));
  EIMSK |= (1<<INT0);
  EICRA |= (1<<ISC00) | (1<<ISC01);
  SREG |= (1<<7);
}

void INT1_Init (void) {
  SREG &= ~(1<<7);
  DDRD &= (~(1<<PD3));
  EIMSK |= (1<<INT1);
  EICRA |= (1<<ISC10) | (1<<ISC11);
  SREG |= (1<<7);
}


void timer2_init(){
  // TIMER 2 NORMAL MODE
  CLEAR_BIT(SREG,7);
  CLEAR_BIT(TCCR2B,6);
  CLEAR_BIT(TCCR2B,3);
  TCCR2B |= 0x07;     // prescaler
  CLEAR_BIT(TCCR2B,4);
  CLEAR_BIT(TCCR2B,5);
  // initialize counter
  TIMSK2 |= (1<<TOIE2);
  TCNT2 = 132;
  SET_BIT(SREG,7);
}
