const int MOTOR_1_A = 2, MOTOR_1_B = 3; //variables to store the pins for each motor
const int MOTOR_2_A = 6, MOTOR_2_B = 9;
const int MOTOR_3_A = 10, MOTOR_3_B = 11;
const int CAROUSEL_A = 12, CAROUSEL_B = 13;

void off(int motor) { //make a specific motor turn off
	if(motor == 1) { //spider 1
		digitalWrite(MOTOR_1_A, LOW);
		digitalWrite(MOTOR_1_B, LOW);
		Serial.println("one off");
	} else if(motor == 2) { //spider 2
		digitalWrite(MOTOR_2_A, LOW);
		digitalWrite(MOTOR_2_B, LOW);
		Serial.println("two off");
	} else if(motor == 3) { //spider 3
		digitalWrite(MOTOR_3_A, LOW);
		digitalWrite(MOTOR_3_B, LOW);
		Serial.println("three off");
	} else if(motor == 4) { //carousel
		digitalWrite(CAROUSEL_A, LOW);
		digitalWrite(CAROUSEL_B, LOW);
		Serial.println("carousel off");
	}
}

void up(int motor) { //make a specific spider start going up
	if(motor == 1) {
		digitalWrite(MOTOR_1_A, LOW);
		digitalWrite(MOTOR_1_B, HIGH);
		Serial.println("one up");
	} else if(motor == 2) {
		digitalWrite(MOTOR_2_A, LOW);
		digitalWrite(MOTOR_2_B, HIGH);
		Serial.println("two up");
	} else if(motor == 3) {
		digitalWrite(MOTOR_3_A, LOW);
		digitalWrite(MOTOR_3_B, HIGH);
		Serial.println("three up");
	}
}

void down(int motor) { //make a specific spider start going down
	if(motor == 1) {
		digitalWrite(MOTOR_1_A, HIGH);
		digitalWrite(MOTOR_1_B, LOW);
		Serial.println("one down");
	} else if(motor == 2) {
		digitalWrite(MOTOR_2_A, HIGH);
		digitalWrite(MOTOR_2_B, LOW);
		Serial.println("two down");
	} else if(motor == 3) {
		digitalWrite(MOTOR_3_A, HIGH);
		digitalWrite(MOTOR_3_B, LOW);
		Serial.println("three down");
	}
}

void spin(int dir) { //spin the carousel
	if(dir == 0) {
		digitalWrite(CAROUSEL_A, HIGH);
		digitalWrite(CAROUSEL_B, LOW);
		Serial.println("spin left"); //NOTE: NEED TO CHECK
	} else if(dir == 1) {
		digitalWrite(CAROUSEL_A, LOW);
		digitalWrite(CAROUSEL_B, HIGH);
		Serial.println("spin right");
	}
}

void setup() {
	//set up motors
	pinMode(MOTOR_1_A, OUTPUT); pinMode(MOTOR_1_B, OUTPUT);
	pinMode(MOTOR_2_A, OUTPUT); pinMode(MOTOR_2_B, OUTPUT);
	pinMode(MOTOR_3_A, OUTPUT); pinMode(MOTOR_3_B, OUTPUT);
	pinMode(CAROUSEL_A, OUTPUT); pinMode(CAROUSEL_B, OUTPUT);
	//set up serial for debugging
	Serial.begin(9600);
	Serial.println("Setup");

	Serial.println("Start");

  //main programme
    off(1); off(2); off(3); off(4);
	up(1); up(3); spin(0); delay(10000);
	off(1); off(3); off(4); delay(500);
	down(1); down(3); up(2); delay(10000);

	off(1); off(3); delay(6000); off(2); delay(500);
	down(2); spin(1); delay(6000);
	up(1); delay(5000); off(4); delay(5000);

	off(2); delay(6000); spin(0);
	off(1); delay(1000);
	down(1); up(3); delay(6000); off(4); delay(10000);

	off(1); off(4); down(3); delay(8000); spin(1); delay(8000);
	off(3); off(4); delay(1000);
	up(2); spin(0); delay(3000); off(4); delay(1000); spin(1); delay(2000);

	up(3); delay(1000); off(4); delay(1000); spin(0); delay(3000); off(4); delay(1000);
	up(1); spin(1); delay(3000); off(4); delay(1000);
	off(2); spin(0); delay(3000); off(4); delay(1000); spin(1); delay(2000);

	off(3); delay(1000); off(4); delay(1000); spin(0); delay(3000);
	off(1); off(4);

	Serial.println("end");
}

void loop() {}
