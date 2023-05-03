#define speedL 5
#define IN1 7
#define IN2 8
#define IN3 10
#define IN4 9
#define speedR 6
#define threshold (400 + 19) / 2
#define baseSpeed 180
//#define loss_r 10
//#define loss_l 20
#define loss_r 115
#define loss_l 120
#define Kd 50
#define turning_speed 100

// speed 255 - kp 50 - turning 100 - two batteries
// speed 230 - kp 30 - turning 100 - two batteries

float error = 0;
int leftSpeed, rightSpeed, previousError=0;

void setup() {   
 pinMode(IN1, OUTPUT);   
 pinMode(IN2, OUTPUT);   
 pinMode(speedL, OUTPUT);   
 pinMode(IN3, OUTPUT);   
 pinMode(IN4, OUTPUT);   
 pinMode(speedR, OUTPUT);  
 DDRC &= ~(0x1F);
 setup_motors(1, 1);
}   

void brake() {
 digitalWrite(IN1, HIGH); // make left motor A brake
 digitalWrite(IN2, HIGH);
 digitalWrite(IN3, HIGH); // make right motor B brake
 digitalWrite(IN4, HIGH);
}

void setup_motors(int forward_a, int forward_b) {
 if (forward_a == 1) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
 } else {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
 }
 if (forward_b == 1) {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
 } else {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
 }
}

void change_speed(int leftSpeed, int rightSpeed) {
 analogWrite(speedR, rightSpeed); 
 analogWrite(speedL, leftSpeed); 
}

int readSensor(int sensor) {
 int reading = analogRead(sensor);
 if (reading < threshold) return 0;
 return 1;
}

void loop() { 
  linefollow();
 }  


 void linefollow(){
   int sensor1, sensor2, sensor3, sensor4, sensor5,D;
 sensor1 = readSensor(A2);
 sensor2 = readSensor(A1);
 sensor3 = readSensor(A0);
 //sensor4 = readSensor(A1);
 //sensor5 = readSensor(A0);
 if (sensor1 == 0 and sensor2 == 0 and sensor3 == 0 ){//and sensor4 == 1 and sensor5 == 1) {
  brake();
  if (error < 0) {
   change_speed(turning_speed, turning_speed);
   setup_motors(0, 1);
   }
  if (error > 0) {
   change_speed(turning_speed, turning_speed);
   setup_motors(1, 0);
    }
 } else {
  error = ((sensor1*-2) +(sensor3*2)); //+(sensor4*2) +(sensor5*4));  // 0 1/2 1 2
  error /= (sensor1 + sensor2 + sensor3 );//+ sensor4 + sensor5);
  leftSpeed = baseSpeed;
  rightSpeed = baseSpeed;
  D=error-previousError;
//  if(error != 0) {
//    leftSpeed += (error * Kp + D*Kd);
//    rightSpeed -= (error * Kp + D*Kd);
//  }
  if (error < 0) leftSpeed += (error * loss_l + D*Kd);
  if (error > 0) rightSpeed -= (error * loss_r + D*Kd);
  previousError = error;
  setup_motors(1, 1);
  change_speed(leftSpeed, rightSpeed);
 }
 }
 
