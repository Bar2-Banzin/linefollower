#include <avr/io.h>
#include <avr/interrupt.h>
#define speedL 10
#define IN1 7
#define IN2 6
#define IN3 9
#define IN4 8
#define speedR 11
#define pot A5

#define base_speed_PWM 0
#define base_speed_RPM 220

#define SET_BIT(reg,pin) (reg|=(1<<pin))
#define CLEAR_BIT(reg,pin) (reg&=~(1<<pin))
#define TOGGLE_BIT(reg,pin) (reg^=(1<<pin))

int rpm_speeds[2] = {0, 0};
int actual_speeds[2] = {0, 0};

//Encoder variables
int right_pulses = 0;
int left_pulses = 0;
int right_pulses_prev = 0;
int left_pulses_prev = 0;
long long start_time_l = 0;
long long start_time_r = 0;
int count = 1000;
long long current_time = 0;
long long dt = 0;
long long encoder_resolution = pow(10,3) * (60/20);

uint16_t left_revolutions;
uint16_t right_revolutions;


//Timer variables
int timer_iterations = 0;
int seconds = 0;

int motor_ctrl(uint8_t pin, int setPoint, int actualSpeed, float Kp = 0, float Kd = 0);

void setup()
{
  DDRC &= ~(0x3F);
  pinMode(speedL, OUTPUT);
  pinMode(speedR, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  analogWrite(speedL, 0);
  analogWrite(speedR, 0);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  // start_time_l = millis();
  start_time_r = millis();
  // INT0_Init();
  INT1_Init(); 
  Serial.begin(9600);
}

void loop()
{
  //some code
  uint16_t desiredRPM = simulate_setpoint(pot);
  getMotorSpeeds();
  int actualPWM = motor_ctrl(speedR, desiredRPM, actual_speeds[1], 0.8, 0);
  Serial.print(desiredRPM);
  Serial.print(",");
  Serial.print(actual_speeds[1]);
  Serial.print(",");
  Serial.println(actualPWM);
  // Serial.println(analogRead(A5));
}


int motor_ctrl(uint8_t pin, int setPoint, int actualSpeed, float Kp = 0, float Kd = 0)
{
  
  static int previousError = 0;
  int error = setPoint-actualSpeed; 
  
  int P = error;
  int D = error - previousError;

  int PIDvalue = (Kp * P) + (Kd * D);
  previousError = error;

  int out = base_speed_PWM + PIDvalue;
  if (out > 255) {
    out = 255;
  }
  if (out < 0) {
    out = 0;
  }
  analogWrite(pin, out);
  return out;
}

void getMotorSpeeds()
{ 
  // uint16_t rpm_left = left_revolutions / (float)seconds;
  uint16_t rpm_right = 3 * right_pulses;
  right_pulses = 0;

  // actual_speeds[0] = rpm_left;
  actual_speeds[1] = rpm_right;
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

uint16_t simulate_setpoint(uint8_t pin)
{
  uint16_t reading = analogRead(pin);
  reading = map(reading, 0, 1023, 0, 300);
  return reading;
}