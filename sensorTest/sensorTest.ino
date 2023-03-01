void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  DDRC &= ~(0x1F);
  int reading = analogRead(14);      //Capture the sensor reading
  Serial.print("Right most: ");
  Serial.println(reading);
  delay(1000);
  reading = analogRead(15);      //Capture the sensor reading
  Serial.print("Right: ");
  Serial.println(reading);
  delay(1000);
  reading = analogRead(16);      //Capture the sensor reading
  Serial.print("Middle: ");
  Serial.println(reading);
  delay(1000);
  reading = analogRead(17);      //Capture the sensor reading
  Serial.print("Left: ");
  Serial.println(reading);
  delay(1000);
  reading = analogRead(18);      //Capture the sensor reading
  Serial.print("Left most: ");
  Serial.println(reading);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  // int reading = analogRead(14);      //Capture the sensor reading
  // Serial.println("Right most: ");
  // Serial.print(reading);
  // delay(1000);
  // reading = analogRead(15);      //Capture the sensor reading
  // Serial.println("Right: ");
  // Serial.print(reading);
  // delay(1000);
  // reading = analogRead(16);      //Capture the sensor reading
  // Serial.println("Middle: ");
  // Serial.print(reading);
  // delay(1000);
  // reading = analogRead(17);      //Capture the sensor reading
  // Serial.println("Left: ");
  // Serial.print(reading);
  // delay(1000);
  // reading = analogRead(18);      //Capture the sensor reading
  // Serial.println("Left most: ");
  // Serial.print(reading);
  // delay(1000);
}
