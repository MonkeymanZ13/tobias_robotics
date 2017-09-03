#pragma config(Sensor, S1,     gyro,           sensorEV3_Gyro)
#pragma config(Sensor, S4,     colour,         sensorEV3_Color, modeEV3Color_RGB_Raw)
#pragma config(Motor,  motorA,          left,          tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          right,         tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          zig,           tmotorEV3_Large, PIDControl, reversed, encoder)
#pragma config(Motor,  motorD,          forklift,      tmotorEV3_Large, PIDControl, encoder)

int gyroValue;
long red, green, blue;
string _right = "Right";
string _left = "Left";
string colourName;

//zig functions
void zigUpDown(int timeUp) {
	motor[zig] = 50;
	sleep(1000);
	motor[zig] = 0;
	sleep(timeUp);
	motor[zig] = -50;
	sleep(500);
	motor[zig] = 0;
}

void zigUp() {
	motor[zig] = 50;
}

//forklift functions
void forkliftUp(int time, int power) {
	motor[forklift] = power;
	sleep(time);
	motor[forklift] = 0;
}

void forkliftDown(int time, int power) {
	motor[forklift] = power * -1;
	sleep(time);
	motor[forklift] = 0;
}

//motor functions
void on(int powerLeft, int powerRight) {
	motor[left] = powerLeft;
	motor[right] = powerRight;
}

void off(int timeToWait=0) {
	motor[left] = 0;
	motor[right] = 0;
	sleep(timeToWait);
}

void turn(string turn, int timeToWait=0, int turnValue=85) {
	resetGyro(gyro);
	sleep(500);
	if(turn == "Left") {
		on(-15,15);
		while(gyroValue > turnValue * -1) {}
		} else {
		on(15,-15);
		while(gyroValue < turnValue) {}
	}
	off(timeToWait);
}

void halfTurn(int timeToWait=0) {
	resetGyro(gyro);
	sleep(500);
	on(15,-15);
	while(gyroValue < 175) {}
	off(timeToWait);
}

void followLineToRed(int timeToWait) {
	while(colourName != "Red") {
		if(colourName == "Black") {
			on(25,10);
			} else{
			on(10,25);
		}
	}
	off(timeToWait);
}

//gyro tasks

task getGyro() {
	while(true) {
		gyroValue = getGyroDegrees(gyro);
	}
}

task displayGyro() {
	while(true) {
		displayBigTextLine(10, "       %d", gyroValue);
	}
}

//colour tasks
task readColour() {
	while(true) {
		getColorRGB(colour, red, green, blue);
		if(red > 10 && green < 6 && blue < 6) {
			colourName = "Red";
			} else if(red < 10 && green < 10 && blue < 10) {
			colourName = "Black";
			} else if(red > 15 && green > 15 && blue > 15) {
			colourName = "Silver";
			} else {
			colourName = "Unknown";
		}
	}
}

task displayColour() {
	while(true) {
		displayBigTextLine(1, "R				 	G					B");
		displayBigTextLine(4, "%d			  %d			 %d", red, green, blue);
		displayBigTextLine(7, "				 %s", colourName);
	}
}

task boing() {
	clearTimer(T1);
	sleep(64324);
	zigUpDown(1500);
	while(time1[T1] < 102162) {
		sleep(random(4000) + 3000);
		zigUpDown(random(1000) + 1000);
	}
	while(time1[T1] < 111962) {}
	zigUp();
}

task main()
{
	sleep(4843);
	startTask(getGyro);
	startTask(displayGyro);
	startTask(boing);
	startTask(readColour);
	startTask(displayColour);

	sleep(500);
	resetGyro(gyro);
	sleep(500);

	on(45,45);
	sleep(3500);
	off(500);

	turn(_left, 500);

	on(20,20);
	sleep(2000);
	off(500);

	//first pallet up
	forkliftUp(1000, 30);
	on(-25,-25);
	sleep(1500);
	off(500);
	turn(_left, 500);
	on(20,20);
	sleep(6000);
	off(500);

	turn(_right, 500);

	on(25,25);
	sleep(11000);
	off(500);

	//first pallet down
	forkliftDown(1000, 30);

	on(-20,-20);
	sleep(2000);
	off(500);
	halfTurn(500);
	on(30,30);
	sleep(7750);
	off(500);

	turn(_left, 500, 80);
	on(30,30);
	while(colourName != "Red") {}
	off(500);
	turn(_left, 500, 82);

	on(20,20);
	sleep(2000);
	off(500);

	//pick up second pallet
	forkliftUp(1000, 30);
	on(-25,-25);
	sleep(1500);
	off(500);
	turn(_left, 0, 82);
	on(25,25);
	sleep(3800);
	off(500);

	turn(_right, 500);
	repeat(3) {
		forkliftUp(750,60);
		forkliftDown(750,60);
	}
	forkliftUp(750, 60);

	//crazy zig zag driving
	on(40,20);
	sleep(2000);
	on(20,40);
	sleep(2000);
	on(40,20);
	sleep(1000);
	on(20,40);
	sleep(3000);
	on(40,20);
	sleep(3000);

	sleep(500);
	on(-40,-20);
	sleep(2000);
	off(1000);
	on(-40,-40);
	forkliftDown(1000,60);
	while(colourName != "Black") {}
	off(500);
	on(30,30);
	while(colourName != "Silver") {}

	//crazy spins
	on(50,-50);
	repeat(2) {
		while(colourName != "Silver") {}
		while(colourName != "Black") {}
	}
	while(colourName != "Silver") {}
	on(-50,50);
	repeat(2) {
		while(colourName != "Black") {}
		while(colourName != "Silver") {}
	}
	while(colourName != "Black") {}
	on(30,30);
	while(colourName != "Silver") {}
	on(-30,30);
	while(colourName != "Black") {}
	off(500);

	followLineToRed(500);
	forkliftUp(250,60);
	on(75,75);
	sleep(1000);
}
