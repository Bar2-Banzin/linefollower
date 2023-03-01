/*
 * File name: PID_LF_example
 * 
 * Hardware requirements: an Arduino Pro Mini
 *                        a QTR-8RC Reflectance Sensor Array
 *                        a DRV8835 Dual Motor Driver Carrier 
 *                        
 * Description: The basic PID control system implemented with 
 *              the line follower with the specified hardware. 
 *              The robot can follow a black line on a white surface 
 *              (or vice versa). 
 * Related Document: See the written documentation or the LF video from
 *                   Bot Reboot.
 *                   
 * Author: Bot Reboot
 */

#include "QTRSensors.h" //Make sure to install the library

/*************************************************************************
* Sensor Array object initialisation 
*************************************************************************/
QTRSensors qtr;
const uint8_t SensorCount = 5;
uint16_t sensorValues[SensorCount];

/*************************************************************************
* PID control system variables 
*************************************************************************/
float Kp = 0; //related to the proportional control term; 
              //change the value by trial-and-error (ex: 0.07).
float Ki = 0; //related to the integral control term; 
              //change the value by trial-and-error (ex: 0.0008).
float Kd = 0; //related to the derivative control term; 
              //change the value by trial-and-error (ex: 0.6).
int P;
int I;
int D;

/*************************************************************************
* Global variables
*************************************************************************/
int lastError = 0;
boolean onoff = false;

/*************************************************************************
* Motor speed variables (choose between 0 - no speed, and 255 - maximum speed)
*************************************************************************/
const uint8_t maxspeeda = 130;
const uint8_t maxspeedb = 130;
const uint8_t basespeeda = 80;
const uint8_t basespeedb = 80;

/*************************************************************************
* DRV8835 GPIO pins declaration
*************************************************************************/
int mode = 8;
int aphase = 9;
int aenbl = 6;
int bphase = 5;
int benbl = 3;

/*************************************************************************
* Buttons pins declaration
*************************************************************************/
int buttoncalibrate = 17; //or pin A3
int buttonstart = 2;

/*************************************************************************
* Function Name: setup
**************************************************************************
* Summary:
* This is the setup function for the Arduino board. It first sets up the 
* pins for the sensor array and the motor driver. Then the user needs to 
* slide the sensors across the line for 10 seconds as they need to be 
* calibrated. 
* 
* Parameters:
*  none
* 
* Returns:
*  none
*************************************************************************/
void setup() {
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){10, 11, 12, 14, 15, 16, 18, 19}, SensorCount);
  qtr.setEmitterPin(7);//LEDON PIN

  pinMode(mode, OUTPUT);
  pinMode(aphase, OUTPUT);
  pinMode(aenbl, OUTPUT);
  pinMode(bphase, OUTPUT);
  pinMode(benbl, OUTPUT);
  digitalWrite(mode, HIGH); //one of the two control interfaces 
                            //(simplified drive/brake operation)
  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);

  boolean Ok = false;
  while (Ok == false) { // the main function won't start until the robot is calibrated
    if(digitalRead(buttoncalibrate) == HIGH) {
      calibration(); //calibrate the robot for 10 seconds
      Ok = true;
    }
  }
  forward_brake(0, 0); //stop the motors
}

/*************************************************************************
* Function Name: calibration
**************************************************************************
* Summary:
* This is the calibration function for the QTR-8RC Reflectance Sensor Array. 
* The function calls the method 'qtr.calibrate()' offered by the imported 
* library. For approx. 10 seconds, each of the 8 sensors will calibrate with
* readings from the track. 
* 
* Parameters:
*  none
* 
* Returns:
*  none
*************************************************************************/
void calibration() {
  digitalWrite(LED_BUILTIN, HIGH);
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW);
}

/*************************************************************************
* Function Name: loop
**************************************************************************
* Summary:
* This is the main function of this application. When the start button is
* pressed, the robot will toggle between following the track and stopping.
* When following the track, the function calls the PID control method. 
* 
* Parameters:
*  none
* 
* Returns:
*  none
*************************************************************************/
void loop() {
  if(digitalRead(buttonstart) == HIGH) {
    onoff =! onoff;
    if(onoff = true) {
      delay(1000);//a delay when the robot starts
    }
    else {
      delay(50);
    }
  }
  if (onoff == true) {
    PID_control();
  }
  else {
    forward_brake(0,0); //stop the motors
  }
}

/*************************************************************************
* Function Name: forward_brake
**************************************************************************
* Summary:
* This is the control interface function of the motor driver. As shown in
* the Pololu's documentation of the DRV8835 motor driver, when the MODE is 
* equal to 1 (the pin is set to output HIGH), the robot will go forward at
* the given speed specified by the parameters. The phase pins control the
* direction of the spin, and the enbl pins control the speed of the motor.
* 
* A warning though, depending on the wiring, you might need to change the 
* aphase and bphase from LOW to HIGH, in order for the robot to spin forward. 
* 
* Parameters:
*  int posa: int value from 0 to 255; controls the speed of the motor A.
*  int posb: int value from 0 to 255; controls the speed of the motor B.
* 
* Returns:
*  none
*************************************************************************/
void forward_brake(int posa, int posb) {
  //set the appropriate values for aphase and bphase so that the robot goes straight
  digitalWrite(aphase, LOW);
  digitalWrite(bphase, LOW);
  analogWrite(aenbl, posa);
  analogWrite(benbl, posb);
}


