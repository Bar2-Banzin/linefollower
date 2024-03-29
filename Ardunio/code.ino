/*
 Sample Line Following Code for the Robojunkies LF-2 robot
*/

// Speed 100 , 150
// kp =0.055
// kd=0.2 --- 0.18  ---  0.15


// Speed 100 , 15
// kp =0.055
// kd=0.2 --- 0.18  ---  0.15

//Speed 120 - BATTERIES: 3.95 3.85
//kp =0.075 ---- 0.065
//kd = 0.3 --- 0.28   // 0.55

// speed = 140
// kp = 0.1
// kd = 0

///////////////////////// *********************************************** ////////////////////////////////
// Speed 180
// kp =0.073
// kd=0.18 

// Speed 100 , 150
// kp =0.055
// kd= 0.18


// Speed 200
// kp =0.093
// kd=0.2


///////////////////////// **********************3 batteries 11.3v************************* ////////////////////////////////

// Speed 100 
// kp =0.055
// kd= 0.18

// Speed 120 
// kp = 0.075
// kd= 0.18

// Speed 150 
// kp =0.13
// kd= 0.215

///////////////////////// **********************3 batteries 11.5v************************* ////////////////////////////////

// Speed 150 
// kp =0.085
// kd= 0.18

// Speed 120 
// kp = 0.075
// kd= 0.18


///////////////////////// **********************3 batteries 11.3v************************* ////////////////////////////////


// Speed 100 
// kp =0.06
// kd= 0.18

// Speed 120 
// kp = 0.085
// kd= 0.18

///////////////////////// **********************3 batteries 11.1v************************* ////////////////////////////////


// Speed 100 
// kp =0.07
// kd= 0.18

///////////////////////// **********************3 batteries 11.8v************************* ////////////////////////////////


// Speed 100 
// kp =0.055
// kd= 0.18


///////////////////////// ********************** SELKA ************************* ////////////////////////////////


// Speed 100 
// kp =0.075
// kd= 0.18

// Speed 120 
// kp =0.095
// kd= 0.18


//int lfspeed = 150;
//
//float Kp = 0.15;
//float Kd = 0.25;


///////////////////////// ********************** 2 Batteries ==> men el driver 7.8 ************************* //////////////////////////////// 


// Speed 200 
// kp = 0.11
// kd = 0.195

// Speed 180 
// kp = 0.095
// kd = 0.18


// Speed 150 
// kp = 0.065
// kd = 0.18

// Speed 220 
// kp = 0.14
// kd = 0.195

///////////////////////// ********************** 3 Batteries ==> men el driver 11.78 ************************* //////////////////////////////// 

//int lfspeed = 90;
//
//float Kp = 0.075;
//float Kd = 0.3;
///////////////////
//int lfspeed = 100;
//float Kp = 0.0585;
//float Kd = 0.185; 


////////////////////////////
//int lfspeed = 80;
//float Kp = 0.0575; 
//float Kd = 0.2; 

//int lfspeed = 100;
//float Kp = 0.0593; 
//float Kd = 0.22;
//lfspeed = 80; // in loop
//delay(25);



////////////////////////////////////////////////////////////integration with maazzeeedriver was 8.11 /////////////////////////////////////////////////////
//int lfspeed = 80;
//float Kp = 0.0575; 
//float Kd = 0.2; 
//tuen_speed = 80;


//int lfspeed = 100;
//float Kp = 0.059; 
//float Kd = 0.2;
//tuen_speed = 90;

//int lfspeed = 120;
//tune_speed=110;
//float Kp = 0.0605; 
//float Kd = 0.2;
//float Ki = 0;

//int lfspeed = 150;
//tune_speed = 130;
//float Kp = 0.0625; 
//float Kd = 0.2;
//float Ki = 0;


////////////////////////////////////////////////////////////integration with maazzeeedriver was 4.01 4.01 4.1 /////////////////////////////////////////////////////
//int lfspeed = 100;
//float Kp = 0.059; 
//float Kd = 0.2;
//tuen_speed = 90;



// tsleem day 
//int lfspeed = 120, 180;
//float Kp = 0.067; 
//float Kd = 0.2;
//int turn_speed = 100;

//int lfspeed = 150;
//float Kp = 0.082; 
//float Kd = 0.2;








// mine

//int lfspeed = 120; // no delay with bluetooth bas tunning
//float Kp = 0.06; 
//float Kd = 0.23;
//lfspeed = 100; // in loop

//int lfspeed = 140;  // no bluetooth
//float Kp = 0.064; 
//float Kd = 0.23;
//delay(25);


#define speedL 5
#define IN1 7
#define IN2 8
#define IN3 10
#define IN4 9 
#define speedR 6

//#define turning_speed 80

int P, D, previousError, PIDvalue, error;
int lsp, rsp;      
int lfspeed = 140;
float Kp = 0.083; 
float Kd = 0.23;
float Ki = 0;
char data;


void setup()
{
  DDRC &= ~(0x1F);
  previousError = 0;
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
  Serial.begin(9600);
}


void loop()
{
  linefollow();
}

void linefollow()
{

  int sensor1 = analogRead(A0);
  int sensor2 = analogRead(A1);
  int sensor3 = analogRead(A2);
  int sensor4 = analogRead(A3);
  int sensor5 = analogRead(A4);
  while(analogRead(A0) >= 200 && analogRead(A1) >= 200 && analogRead(A2) >= 200 && analogRead(A3) >= 200 && analogRead(A4) >= 200) {
    int turn_speed = 120;
    error = previousError;
    P = error;
    D = error - previousError;
  
    PIDvalue = (Kp * P) + (Kd * D);
    previousError = error;
  
    lsp = turn_speed + PIDvalue;
    rsp = turn_speed - PIDvalue;
  
    if (lsp > 255) lsp = 255;
    if (lsp < 0)   lsp = 0;
    if (rsp > 255) rsp = 255;
    if (rsp < 0)   rsp = 0;
    analogWrite(speedL, lsp);
    analogWrite(speedR, rsp);
  }
//    Serial.flush();
  // if (sensor1 >= 500 and sensor2 >= 500 and sensor3 >= 500 and sensor4 >= 500 and sensor5 >= 500) {
  // brake();
  // if (error < 0) {
  //  change_speed(turning_speed, turning_speed);
  //  setup_motors(0, 1);
  //  }
  // if (error > 0) {
  //  change_speed(turning_speed, turning_speed);
  //  setup_motors(1, 0);
  //   }
  // } else {

  error = (sensor1+sensor2) - (sensor5 + sensor4);
  if(error < 0){
    sensor3 *= -1;
  }
  error += sensor3;
  if(error > 900) {
    error = 900;
  }
//  else if(error<15&&error>-15){ 
//    if(Serial.available()){
//      data=Serial.read();
//      if(data == '1') lfspeed = 150;
//      else lfspeed = 120;
//    }
//  }
  
//  Serial.println(error);
  static int I = 0;
  I += error;
  P = error; 
  D = error - previousError;


  PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
  previousError = error;

  lsp = lfspeed + PIDvalue;
  rsp = lfspeed - PIDvalue;

  if (lsp > 255) lsp = 255;
  if (lsp < 0)   lsp = 0;
  if (rsp > 255) rsp = 255;
  if (rsp < 0)   rsp = 0;
  analogWrite(speedL, lsp);
  analogWrite(speedR, rsp);
  delay(25);
}






//void brake() {
// digitalWrite(IN1, HIGH); // make left motor A brake
// digitalWrite(IN2, HIGH);
// digitalWrite(IN3, HIGH); // make right motor B brake
// digitalWrite(IN4, HIGH);
//}

//void setup_motors(int forward_a, int forward_b) {
// if (forward_a == 1) {
//  digitalWrite(IN1, LOW);
//  digitalWrite(IN2, HIGH);
// } else {
//  digitalWrite(IN1, LOW);
//  digitalWrite(IN2, LOW);
// }
// if (forward_b == 1) {
//  digitalWrite(IN3, HIGH);
//  digitalWrite(IN4, LOW);
// } else {
//  digitalWrite(IN3, LOW);
//  digitalWrite(IN4, LOW);
// }
//}

//void change_speed(int leftSpeed, int rightSpeed) {
// analogWrite(speedR, rightSpeed); 
// analogWrite(speedL, leftSpeed); 
//}
