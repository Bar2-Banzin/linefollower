/*
 Sample Line Following Code for the Robojunkies LF-2 robot
*/
#include <util/delay.h>
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
    digitalWrite(pin2, HIGH);
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
  uint8_t reading = PINC & 0x1F; // Capture the sensor reading
  reading = (~reading) & 0x1F;
  Serial.print("AT THE BEGINNING"); // uint8_t reading = PINC & 0x1F;      //Capture the sensor reading
  Serial.println(reading);

  // To go left 11000
  if (((reading)&0x18) == 0x18)
  {
    move_car(forward);

    _delay_ms(50);
    
move_car(left);
    _delay_ms(500);

    // if (((reading)&0b00100) == 0b00100)
    // { // 10000
    //   // while ((~PINC & 0x10) != 0x10)
    //   // {
    //   //   // digitalWrite(13,HIGH);
    //   //   move_car(left);
    //   // }


    // }
    // while ((~PINC & 0x04) != 0b00100)
    // {
    //   // digitalWrite(13,HIGH);
    //   move_car(left);
    // }

    digitalWrite(13, LOW);
    // Either forward or right
  }

  else if ((reading & 0x03) == 0x03)
  { // there is right
    digitalWrite(13, LOW);
    move_car(forward);
    _delay_ms(100);
    reading = PINC & 0x1F; // Capture the sensor reading

    reading = (~reading) & 0x1F;
    // Go right
    Serial.println(reading);

    if ((reading) == 0x00)
    {
      Serial.print("r");
      Serial.println(reading);

      move_car(right);
      while (((~PINC & 0x04)) != 0b00100)
      {
        Serial.println('in the loop');
        move_car(right);
      }
    } /*go forward*/
    else
    {
      // move_car(right);
      // while((PINC & 0x1F) != 0b11011){
      // digitalWrite(13,HIGH);
      digitalWrite(13, HIGH);
      move_car(read_line());
      Serial.println("Outside readline");

      // }
    }
    // Go forward
  }
  else
  {
    digitalWrite(13, HIGH);
    move_car(read_line());
    Serial.println("Outside readline");
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(13, OUTPUT);
  DDRC &= ~(0x1F);
  analogWrite(speedL, 70);
  analogWrite(speedR, 70);
  move_car(still);
  // delay(2000);
}

void loop()
{
  Cases();
  // Just a test
  //  reading = read_line();
  // if(prev != reading)
  //  move_car(left);
  //  delay(2000);
  //  move_car(still);
  //  delay(500);
  //  move_car(right);
  //  delay(2000);
  //  move_car(still);
  //  delay(500);

  // prev = reading;
}


// /*
//  Sample Line Following Code for the Robojunkies LF-2 robot
// */

// #define speedL 10
// #define IN1 9
// #define IN2 8
// #define IN3 7
// #define IN4 6
// #define speedR 5


// enum motor_dir {
//   m_still = 0,
//   m_forward = 1,
//   m_backward = 2
// };

// enum car_dir {
//   still,
//   forward,
//   backward,
//   left,
//   right
// };

// void move_motor(char pin1, char pin2, motor_dir dir) {
//   //delay(10);
//   switch (dir) {
//     case m_still:
//       digitalWrite(pin1, LOW);
//       digitalWrite(pin2, LOW);
//       break;
//     case m_forward:
//       digitalWrite(pin1, HIGH);
//       digitalWrite(pin2, LOW);
//       break;
//     case m_backward:
//       digitalWrite(pin1, LOW);
//       digitalWrite(pin2, HIGH);
//       break;
//   }
// }

// void move_car(car_dir dir) {
//   switch (dir) {
//     case still:
//       move_motor(IN1, IN2, m_still);
//       move_motor(IN3, IN4, m_still);
//       break;
//     case forward:
//       move_motor(IN1, IN2, m_forward);
//       move_motor(IN3, IN4, m_forward);
//       break;
//     case backward:
//       move_motor(IN1, IN2, m_backward);
//       move_motor(IN3, IN4, m_backward);
//       break;
//     case left:
//       move_motor(IN1, IN2, m_forward);
//       move_motor(IN3, IN4, m_backward);
//       break;
//     case right:
//       move_motor(IN1, IN2, m_backward);
//       move_motor(IN3, IN4, m_forward);
//       break;
//   }
// }


// car_dir read_line() {
//   char reading = PINC & 0x1F;  //Capture the sensor reading

//   char middle = ~reading & 0x04;  //The reading has zero in the middle

//   char l = (reading >> 3);
//   char r = reading & 0x03;



//   if (l > r) return right;
//   if (r > l) return left;
//   if (middle == 4) return forward;

//   return still;
// }

// void Cases() {
//   uint8_t reading = PINC & 0x1F;  //Capture the sensor reading
//   reading = (~reading) & 0x1F;    // uint8_t reading = PINC & 0x1F;      //Capture the sensor reading

//   // To go left
//   if (((reading)&0x18) == 0x18) {
//     // digitalWrite(13,HIGH);
//     while ((PINC & 0x1F) != 0b11011) {
//       // digitalWrite(13,HIGH);
//       move_car(left);
//     }
//     digitalWrite(13, LOW);
//     //Either forward or right
//   } else if (reading == 0x07) {
//       digitalWrite(13, LOW);
//     move_car(still);
//     reading = PINC & 0x1F;  //Capture the sensor reading
//     reading = (~reading) & 0x1F;
//     //Go right

//     if ((reading) == 0x00) {
//       while ((~(PINC & 0x04)) != 0b00100) {
//         move_car(right);
//       }
//     }
//     //Go forward
//     else {
//       // move_car(right);
//       // while((PINC & 0x1F) != 0b11011){
//       // digitalWrite(13,HIGH);
//       digitalWrite(13, HIGH);
//       move_car(read_line());
//       // }
//     }
//     //Go forward
//   } else {
//     digitalWrite(13, HIGH);
//     move_car(read_line());
//   }
// }



// void setup() {
//   Serial.begin(9600);
//   pinMode(IN1, OUTPUT);
//   pinMode(IN2, OUTPUT);
//   pinMode(IN3, OUTPUT);
//   pinMode(IN4, OUTPUT);
//   pinMode(13, OUTPUT);
//   DDRC &= ~(0x1F);
//   analogWrite(speedL, 70);
//   analogWrite(speedR, 70);
//   move_car(still);
//   delay(2000);
// }

// void loop() {
//   Cases();
//   //Just a test
//   // reading = read_line();
//   //if(prev != reading)
//   // move_car(left);
//   // delay(2000);
//   // move_car(still);
//   // delay(500);
//   // move_car(right);
//   // delay(2000);
//   // move_car(still);
//   // delay(500);

//   //prev = reading;
// }
