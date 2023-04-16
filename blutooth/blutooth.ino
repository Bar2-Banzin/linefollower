char data;
void setup() {
 // put your setup code here, to run once:
 Serial.begin(9600);

}
void loop() {
 // put your main code here, to run repeatedly:
 if(Serial.available()){
 data=Serial.read();
 Serial.println(data);
 if(data=='F'){
// forward   
}
 if(data=='B'){
// backward   
}
 if(data=='L'){
// left   
}

 if(data=='R'){
// right   
}
 else{
 // default s
 }
 }

}
