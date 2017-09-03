#include <Servo.h>

Servo servo; //servo control pin (orange wire) on edge of board

const int LEDS = 17; //pin to turn on lights

const int trig = 16, echo = 19;
const float unit_division_factor = 58;
unsigned long distance = 0;

void openDoor(int timeToWait=0) { //open coffin door
	for(int x=0; x<=100; x++) {
		servo.write(x);
		delay(13);
	}
	delay(timeToWait);
}

void closeDoor(int timeToWait=0) { //close coffin door
	for(int x=100; x>=0; x--) {
		servo.write(x);
		delay(13);
	}
	delay(timeToWait);
}

void ledFlash(int timeToWait) {
        digitalWrite(LEDS, HIGH);
        delay(150);
        digitalWrite(LEDS, LOW);
        delay(150);
        digitalWrite(LEDS, HIGH);
        delay(timeToWait);
        digitalWrite(LEDS, LOW);
}

void setup() { //set up LEDs and servo motor
	servo.attach(8);
	pinMode(LEDS, OUTPUT); digitalWrite(LEDS, LOW);
	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);
	Serial.begin(9600);
	closeDoor();

	//first two openings to be in time with music
	delay(5000);
	openDoor(1000); Serial.println("open 1");
	ledFlash(3000);
	closeDoor(); Serial.println("close 1");

	delay(23000); //wait until cart comes up to coffin
	openDoor(1000); Serial.println("open 2");
	ledFlash(3000);
	closeDoor(); Serial.println("close 2");	
	
	Serial.println("Starting loop");	
}

void loop() {
        distance = 0;
	while(distance > 30 || distance == 0) { //wait until something comes near the ultrasonic sensor (e.g. my hand)
		digitalWrite(trig, LOW);
                delayMicroseconds(10);
		digitalWrite(trig, HIGH);
		distance = pulseIn(echo, HIGH, 20000) / unit_division_factor;  //read from ultrasonic sensor
		Serial.print("Distance: ");
		Serial.println(distance);
		delay(50);
	}
	Serial.println("Object Detected");
	openDoor(1000); Serial.println("open");
	ledFlash(3000);
	closeDoor(); Serial.println("close");	
}
