/*
 Receiver Code  
Arduino SCK MISO  MOSI  SS
Uno      13  12    11  10
Nano     13  12    11  10              

*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = A0, en = A1, d4 = A2, d5 = A3, d6 = A4, d7 = A5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

int x;

#define motor1a 2
#define motor1b 3

#define motor2a 4
#define motor2b 5

#define buzzer 6

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  pinMode(motor1a, OUTPUT);
  pinMode(motor1b, OUTPUT);
  pinMode(motor2a, OUTPUT);
  pinMode(motor2b, OUTPUT);

  pinMode(buzzer, OUTPUT);

  digitalWrite(motor1a, HIGH);
  digitalWrite(motor1b, HIGH);
  
  digitalWrite(motor2a, LOW);
  digitalWrite(motor2b, LOW);
}

void loop() {
  
  rf();
   if (Serial.available()> 0 )
  {
  
  x = Serial.read();
switch(x){
case 'F' : forward();
break;
case 'B' : back();
break; 
case 'R' : right();
break;
case 'L' : left();
break; 
case 'S' : stoop();
break;
default: stoop();
}}}
  
void rf(){
   
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
   

      
      int x1 = atoi(text);
      Serial.println(x1); //shows: 12
      //----------------------------
      memset(text, 0x30, 3);  //replace by 0s
      int x2 = atoi(text);
      Serial.println(x2);  //shows: 34
      //-----------------------------
      
    lcd.setCursor(0,0);
    lcd.print("H=");
    lcd.setCursor(2,0);
    lcd.print(x2);
    lcd.setCursor(4,0);
    lcd.print("%");
    
    lcd.setCursor(6,0);
    lcd.print(" T=");
    lcd.setCursor(9,0);
    lcd.print(x1);
    lcd.setCursor(11,0);
    lcd.print((char)223);
    lcd.setCursor(12,0);
    lcd.print("c");
    
    lcd.setCursor(0,1);
    lcd.print("RPM=");
    lcd.setCursor(3,1);
    lcd.print("0");
    lcd.setCursor(7,1);

    delay(500);
    lcd.clear();
    if (x1 >= 45){ // temp
       digitalWrite(buzzer, HIGH);
       delay(500);
    }
    else{
       digitalWrite(buzzer, LOW);
    }
  }
}
void forward(){
  digitalWrite(motor1a, LOW);
  digitalWrite(motor1b, LOW);
  
  digitalWrite(motor2a, HIGH);
  digitalWrite(motor2b, LOW);
  
}

void back(){
  digitalWrite(motor1a, LOW);
  digitalWrite(motor1b, HIGH);
  
  digitalWrite(motor2a, LOW);
  digitalWrite(motor2b, HIGH);
  
}

void right(){
  digitalWrite(motor1a, LOW);
  digitalWrite(motor1b, LOW);

  digitalWrite(motor2a, LOW);
  digitalWrite(motor2b, HIGH);
  
}

void left(){
  digitalWrite(motor1a, HIGH);
  digitalWrite(motor1b, HIGH);
  
  digitalWrite(motor2a, HIGH);
  digitalWrite(motor2b, LOW);
  
}
void stoop(){
  digitalWrite(motor1a, HIGH);
  digitalWrite(motor1b, HIGH);
  
  digitalWrite(motor2a, LOW);
  digitalWrite(motor2b, LOW);
}
