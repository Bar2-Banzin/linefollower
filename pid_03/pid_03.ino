#define ENA 6  //right motor
#define motorInput1 8
#define motorInput2 7
#define motorInput3 10
#define motorInput4 9
#define ENB 5  //left motor

#define numsensors 5 //define number of sensors you are using
#define SET_POINT 2000


float P = 0 , I = 0 , D = 0, error = 0, LP = 0;

float kp = 0.1;  //Modify these values depending on your robot
float ki = 2;      //Modify these values depending on your robot
float kd = 0;      //Modify these values depending on your robot
int r_spd, l_spd, max_spd = 150;  //Set max_spd depending on your speed of motors

long Saverage;
int Ssum;
int pos;
long sensor[] = {0, 0, 0, 0, 0};
int i;
int p[] = {0, 0, 0, 0};

void read_sensor();
void pid_calc();
void calc_turn();
void drive(int, int);
int set_motors(int , int );
void forward();
void left();
void right();
void backward();
void stopg();
void readline();
int mod(int);

void setup()
{
  // Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);
  pinMode(motorInput3, OUTPUT);
  pinMode(motorInput4, OUTPUT);
}

void loop()
{
  read_sensor();
  pid_calc();
  calc_turn();
  drive(r_spd, l_spd);

}

void read_sensor()
{
  Saverage = 0;
  Ssum = 0;
  for (int i = 0; i < numsensors; i++)
  { sensor[i] = analogRead(i);
    Saverage += sensor[i] * (i) * 1000;
    Ssum += int(sensor[i]);
  }
  pos = int(Saverage / Ssum);
  // Serial.println(pos);
}

//Replace the Set-point value by positioning the robot on the line in pid_calc func

void pid_calc()
{ pos = int(Saverage / Ssum);
  P = pos - SET_POINT; //Find the postion and replace the value
  I = I + P;
  D = P - LP;
  LP = P;
  error = int(P * kp + I * ki + D * kd);
}

void calc_turn()
{
  error = int(constrain(error, -200, 200));
  // Serial.println(error);

  if (error < 0) //reduce right turn values for right turn
  {

    r_spd = max_spd + error;
    l_spd = max_spd;
  }

  else //reduce left turn values for left turn
  {
    r_spd = max_spd;
    l_spd = max_spd - error;
  }

}

void drive(int r_spd, int l_spd)
{
  analogWrite(ENB, mod(r_spd) );
  analogWrite(ENA, mod(l_spd) );
  forward();
}

void readline()
{
  p[0] = analogRead(A0);
  p[1] = analogRead(A1);
  p[2] = analogRead(A2);
  p[3] = analogRead(A3);
  for (i = 0; i < 4; i++)
  {
    if (p[i] > 500)
    {
      p[i] = 1;
    }
    else
    {
      p[i] = 0;
    }
  }
}

void forward() {
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);

  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);
}

void left() {
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);

  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, LOW);
}

void left90() {
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);

  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
}

void right() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, LOW);

  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);
}

void right90() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);

  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);
}

void backward() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);

  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
}

void stopg() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, LOW);

  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, LOW);
}



int mod(int v)
{

  if (v < 0)

    return -1 * v;

  else if (v > 0)

    return v;

}