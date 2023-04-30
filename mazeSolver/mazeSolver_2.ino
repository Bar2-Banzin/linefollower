// #include<avr/io.h>

// #define Kp 0.05
// #define Kd 0.2
// #define Ki .001
// #define MaxSpeed 80
// #define BaseSpeed 80
// #define NUM_SENSORS 7
// #define TIMEOUT 2500
// #define EMITTER_PIN QTR_NO_EMITTER_PIN
// #define speedturn 255  // edit this to determine the speed during taking turns

// #define rightMotor1 9     // done
// #define rightMotor2 8     // done
// #define rightMotorPWM 3  // done
// #define leftMotor1 7      // done
// #define leftMotor2 6      // done
// #define leftMotorPWM 10    // done

// unsigned int line_position = 0;  // done

// unsigned int sensorValues[5];  // done

// #define motorPower A2
// #define threshold 700
// #define led 13
// int drivePastDelay = 100;


// char path[100] = "";
// unsigned char dir;
// unsigned char path_length = 0;

// void setup() {

//   pinMode(rightMotor1, OUTPUT);
//   pinMode(rightMotor2, OUTPUT);
//   pinMode(leftMotor1, OUTPUT);
//   pinMode(leftMotor2, OUTPUT);
//   pinMode(rightMotorPWM, OUTPUT);
//   pinMode(leftMotorPWM, OUTPUT);
//   pinMode(13, OUTPUT);
//   digitalWrite(rightMotor1, HIGH);
//   digitalWrite(rightMotor2, LOW);
//   digitalWrite(leftMotor1, HIGH);
//   digitalWrite(leftMotor2, LOW);
//   // DDRC = 0x00;
//   // sensors input
//   pinMode(A0, INPUT);
//   pinMode(A1, INPUT);
//   pinMode(A2, INPUT);
//   pinMode(A3, INPUT);
//   pinMode(A4, INPUT);

//   analogWrite(rightMotorPWM, BaseSpeed);
//   analogWrite(leftMotorPWM, BaseSpeed);
//   delay(3000);
// }

// void loop() {

//   MazeSolve();
// }

// void follow_line()  // follow the line
// {

//   digitalWrite(9, HIGH);
//   digitalWrite(8, LOW);
//   digitalWrite(7, HIGH);
//   digitalWrite(6, LOW);
//   digitalWrite(13, HIGH);  

//   int lastError = 0;

//   while (1) {

//     line_position = ~(PINC & 0x1F);

//     int error = line_position - 0b00100;
//     int error1 = error - lastError;

//     ////////option1////////
//     int error2 = (2.0 / 3.0) * error2 + error;
//     int motorSpeed = (Kp * error + Kd * error1 + Ki * error2) * 10;
//     /////////////////////////

//     int rightMotorSpeed = BaseSpeed - motorSpeed;
//     int leftMotorSpeed = BaseSpeed + motorSpeed;

//     if (rightMotorSpeed > MaxSpeed)
//       rightMotorSpeed = MaxSpeed;  // prevent the motor from going beyond max speed
//     if (leftMotorSpeed > MaxSpeed)
//       leftMotorSpeed = MaxSpeed;  // prevent the motor from going beyond max speed
//     if (rightMotorSpeed < 0)
//       rightMotorSpeed = 0;
//     if (leftMotorSpeed < 0)
//       leftMotorSpeed = 0;
//     analogWrite(rightMotorPWM, rightMotorSpeed);
//     analogWrite(leftMotorPWM, leftMotorSpeed);

//     lastError = error;

//     sensorValues[0] = ~digitalRead(A0);
//     sensorValues[1] = ~digitalRead(A1);
//     sensorValues[2] = ~digitalRead(A2);
//     sensorValues[3] = ~digitalRead(A3);
//     sensorValues[4] = ~digitalRead(A4);
//     if (sensorValues[0] == 1 || sensorValues[4] == 1)  // cross
//     {
//       digitalWrite(13,LOW);
//       analogWrite(rightMotorPWM, BaseSpeed);
//       analogWrite(leftMotorPWM, BaseSpeed);
//       return;
//     }
//     if (sensorValues[0] == 0 && sensorValues[1] == 0 && sensorValues[2] == 0 && sensorValues[3] == 0 && sensorValues[4] == 0 && sensorValues[5] == 0 && sensorValues[6] == 0) {

//       analogWrite(rightMotorPWM, BaseSpeed);
//       analogWrite(leftMotorPWM, BaseSpeed);
//       return;
//     }
//   }
// }
// void turn(char dir) {
//   switch (dir) {
//     case 'L':

//       digitalWrite(rightMotor2, LOW);
//       digitalWrite(leftMotor2, LOW);
//       digitalWrite(rightMotor1, HIGH);
//       digitalWrite(leftMotor1, HIGH);

//       analogWrite(rightMotorPWM, speedturn);
//       analogWrite(leftMotorPWM, speedturn);

//       line_position = ~(PINC & 0x1F);
//       sensorValues[0] = digitalRead(A0);
//       while (sensorValues[0] == 0) {
//         line_position = ~(PINC & 0x1F);
//         sensorValues[0] = digitalRead(A0);
//       }
//       // 0 1 2 3 4 5 6
//       line_position = ~(PINC & 0x1F);
//       sensorValues[0] = digitalRead(A0);
//       sensorValues[1] = digitalRead(A1);
//       sensorValues[2] = digitalRead(A2);
//       sensorValues[3] = digitalRead(A3);
//       sensorValues[4] = digitalRead(A4);

//       while (sensorValues[1] == 0 || sensorValues[2] == 0)  // wait for outer most sensor to find the line
//       {
//         line_position = ~(PINC & 0x1F);
//         sensorValues[0] = digitalRead(A0);
//         sensorValues[1] = digitalRead(A1);
//         sensorValues[2] = digitalRead(A2);
//         sensorValues[3] = digitalRead(A3);
//         sensorValues[4] = digitalRead(A4);
//       }

//       digitalWrite(leftMotor1, LOW);
//       digitalWrite(rightMotor1, HIGH);
//       digitalWrite(leftMotor2, HIGH);
//       digitalWrite(rightMotor2, HIGH);

//       analogWrite(rightMotorPWM, 255);
//       break;

//     // Turn right 90deg
//     case 'R':
//       digitalWrite(rightMotor1, LOW);
//       digitalWrite(rightMotor2, HIGH);
//       digitalWrite(leftMotor1, LOW);
//       digitalWrite(leftMotor2, HIGH);

//       analogWrite(rightMotorPWM, speedturn);
//       analogWrite(leftMotorPWM, speedturn);
//       line_position = ~(PINC & 0x1F);
//       sensorValues[0] = digitalRead(A0);
//       sensorValues[1] = digitalRead(A1);
//       sensorValues[2] = digitalRead(A2);
//       sensorValues[3] = digitalRead(A3);
//       sensorValues[4] = digitalRead(A4);
//       while (sensorValues[2] == 0 || sensorValues[3] == 0 || sensorValues[4] == 0)  // wait for outer most sensor to find the line
//       {
//         line_position = ~(PINC & 0x1F);
//         sensorValues[0] = digitalRead(A0);
//         sensorValues[1] = digitalRead(A1);
//         sensorValues[2] = digitalRead(A2);
//         sensorValues[3] = digitalRead(A3);
//         sensorValues[4] = digitalRead(A4);
//       }

//       digitalWrite(rightMotor1, LOW);
//       digitalWrite(rightMotor2, HIGH);
//       digitalWrite(leftMotor1, LOW);
//       digitalWrite(leftMotor2, HIGH);

//       analogWrite(leftMotorPWM, 255);
//       break;

//     // Turn right 180deg to go back
//     case 'B':
//       digitalWrite(rightMotor1, LOW);
//       digitalWrite(rightMotor2, HIGH);
//       digitalWrite(leftMotor1, LOW);
//       digitalWrite(leftMotor2, HIGH);

//       analogWrite(rightMotorPWM, speedturn);
//       analogWrite(leftMotorPWM, speedturn);
//       line_position = ~(PINC & 0x1F);
//       sensorValues[0] = digitalRead(A0);
//       sensorValues[1] = digitalRead(A1);
//       sensorValues[2] = digitalRead(A2);
//       sensorValues[3] = digitalRead(A3);
//       sensorValues[4] = digitalRead(A4);
//       ////////////////////////////////////////////////////////////////////////////////////
//       while (sensorValues[2] == 0)  // wait for outer most sensor to find the line
//       {
//         line_position = ~(PINC & 0x1F);
//         sensorValues[0] = digitalRead(A0);
//         sensorValues[1] = digitalRead(A1);
//         sensorValues[2] = digitalRead(A2);
//         sensorValues[3] = digitalRead(A3);
//         sensorValues[4] = digitalRead(A4);
//       }
//       digitalWrite(rightMotor1, LOW);
//       digitalWrite(rightMotor2, HIGH);
//       digitalWrite(leftMotor1, LOW);
//       digitalWrite(leftMotor2, HIGH);

//       analogWrite(leftMotorPWM, 255);
//       break;

//     case 'S':

//       break;
//   }
// }

// // This function decides which way to turn during the learning phase of
// // maze solving.  It uses the variables found_left, found_straight, and
// // found_right, which indicate whether there is an exit in each of the
// // three directions, applying the "left hand on the wall" strategy.
// char select_turn(unsigned char found_left, unsigned char found_straight, unsigned char found_right) {

//   if (found_left)
//     return 'L';
//   else if (found_straight)
//     return 'S';
//   else if (found_right)
//     return 'R';
//   else
//     return 'B';
// }

// void simplify_path() {

//   if (path_length < 3 || path[path_length - 2] != 'B')
//     return;

//   int total_angle = 0;
//   int i;
//   for (i = 1; i <= 3; i++) {
//     switch (path[path_length - i]) {
//       case 'R':
//         total_angle += 90;
//         break;
//       case 'L':
//         total_angle += 270;
//         break;
//       case 'B':
//         total_angle += 180;
//         break;
//     }
//   }

//   total_angle = total_angle % 360;

//   switch (total_angle) {
//     case 0:
//       path[path_length - 3] = 'S';
//       break;
//     case 90:
//       path[path_length - 3] = 'R';
//       break;
//     case 180:
//       path[path_length - 3] = 'B';
//       break;
//     case 270:
//       path[path_length - 3] = 'L';
//       break;
//   }
//   path_length -= 2;
// }

// void MazeSolve() {
//   while (1) {
//     follow_line();
//     digitalWrite(13,LOW);
//     unsigned char found_left = 0;
//     unsigned char found_straight = 0;
//     unsigned char found_right = 0;
//     sensorValues[0] = ~digitalRead(A0);
//     sensorValues[1] = ~digitalRead(A1);
//     sensorValues[2] = ~digitalRead(A2);
//     sensorValues[3] = ~digitalRead(A3);
//     sensorValues[4] = ~digitalRead(A4);

//     if (sensorValues[0] == 1)
//       found_left = 1;
//     else if (sensorValues[4] == 1) {

//       for (int i = 0; i < 100; i++) {
//         sensorValues[0] = ~digitalRead(A0);

//         if (sensorValues[0] == 1) {
//           found_left = 1;
//           delay(310);
//           goto bailout;
//         }
//       }

//       found_right = 1;
//     }

//     if (found_left == 1) {

//       delay(320);  // TIME  BEFORE TAKING LEFT TURN DURING SEARCH RUN
//     }

// bailout:
//     sensorValues[0] = ~digitalRead(A0);
//     sensorValues[1] = ~digitalRead(A1);
//     sensorValues[2] = ~digitalRead(A2);
//     sensorValues[3] = ~digitalRead(A3);
//     sensorValues[4] = ~digitalRead(A4);
//     if (sensorValues[2] == 1 || sensorValues[1] == 1 || sensorValues[3] == 1)
//       found_straight = 1;

//     // qtrrc.readLine(sensorValues);
//     // if (sensorValues[0] > threshold && sensorValues[1] > threshold && sensorValues[2] > threshold && sensorValues[3] > threshold && sensorValues[4] > threshold && sensorValues[5] > threshold && sensorValues[6] > threshold)
//     //     break;
//     unsigned char dir = select_turn(found_left, found_straight, found_right);

//     turn(dir);

//     path[path_length] = dir;
//     path_length++;

//     simplify_path();
//   }

//   // while (1)
//   // {

//   //     analogWrite(rightMotorPWM, 0);
//   //     analogWrite(leftMotorPWM, 0);
//   //     for (int i = 0; i < 10; i++)
//   //     {
//   //         digitalLow(led);
//   //         delay(200);
//   //         digitalHigh(led);
//   //         delay(200);
//   //     }

//   //     for (int i = 0; i < path_length; i++)
//   //     {

//   //         follow_line();
//   //         if (path[i] == 'S')
//   //         {
//   //             delay(130); // TIME  THE BOT TRAVELS STRAIGHT ON AN INTERSECTION DURING ACTUAL RUN
//   //         }
//   //         else if (path[i] == 'R')
//   //         {
//   //             delay(120); // TIME  BEFORE TAKING RIGHT TURN DURING ACTUAL  RUN
//   //         }
//   //         else
//   //         {

//   //             delay(120); // TIME  BEFORE TAKING LEFT TURN DURING ACTUAL RUN
//   //         }

//   //         turn(path[i]);
//   //     }

//   //     follow_line();
//   // }
// }