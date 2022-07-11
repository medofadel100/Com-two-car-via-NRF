/*
*    Transmitter Code
*/
#include <dht11.h>
#define DHT11PIN 9

dht11 DHT11;

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";
char thChar[32] = "";
String thString = "";


int sensor = 10;
unsigned long start_time = 0;
unsigned long end_time = 0;
int steps=0;
float steps_old=0;
float temp=0;
float rps=0;



#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

//motor
#define motor1a A0
#define motor1b A1

#define motor2a A2
#define motor2b A3



void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
  pinMode(sensor,INPUT_PULLUP);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(motor1a, OUTPUT);
  pinMode(motor1b, OUTPUT);
  pinMode(motor2a, OUTPUT);
  pinMode(motor2b, OUTPUT);
}

void loop() {
  //speeed();
  Serial.println();

  int chk = DHT11.read(DHT11PIN);

  Serial.print("Humidity (%): ");
  int h=DHT11.humidity;
  Serial.println(h);

  Serial.print("Temperature (C): ");
  int t=DHT11.temperature;
  Serial.println(t);
  
  thString = String(t) +","+ String(h)+","+String(rps);
  thString.toCharArray(thChar, 12);
  radio.write(&thChar, sizeof(thChar));
  delay(1000);
  avoid();
}

void speeed(){
 start_time=millis();
 end_time=start_time+1000;
 while(millis()<end_time)
 {
   if(digitalRead(sensor))
   {
    steps=steps+1; 
    while(digitalRead(sensor));
   }

 }
    temp=steps-steps_old;
    steps_old=steps;
    rps=(temp/20);
   
    Serial.println(rps);
    
}


void ultrasonic(){
    // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}
void avoid(){
  ultrasonic();
  if (distance > 1 && distance< 20){
        stoop();
        delay(1000);
        righ(); 
        delay(1000);
  }
  else {
    forward();
  }
}

void forward(){
  digitalWrite(motor1a, HIGH);
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

void righ(){
  digitalWrite(motor1a, HIGH);
  digitalWrite(motor1b, LOW);

  digitalWrite(motor2a, LOW);
  digitalWrite(motor2b, LOW);
  
}

void left(){
  digitalWrite(motor1a, HIGH);
  digitalWrite(motor1b, LOW);
  
  digitalWrite(motor2a, LOW);
  digitalWrite(motor2b, HIGH);
  
}
void stoop(){
  digitalWrite(motor1a, LOW);
  digitalWrite(motor1b, LOW);
  
  digitalWrite(motor2a, LOW);
  digitalWrite(motor2b, LOW);
}
