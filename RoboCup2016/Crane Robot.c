#pragma config(Sensor, S1,     colour,         sensorEV3_Color, modeEV3Color_RGB_Raw)
#pragma config(Motor,  motorA,          right,         tmotorEV3_Large, PIDControl, reversed, encoder)
#pragma config(Motor,  motorB,          left,          tmotorEV3_Large, PIDControl, reversed, encoder)
#pragma config(Motor,  motorC,          claw,          tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          crane,         tmotorEV3_Large, PIDControl, encoder)

long red, green, blue;
string colourName;
bool crazy = false;

//moving functions

void on(int powerLeft, int powerRight) {
	motor[left] = powerLeft;
	motor[right] = powerRight;
}

void off(int timeToWait=0) {
	motor[left] = 0;
	motor[right] = 0;
	sleep(timeToWait);
}

void followLineToRed(int timeToWait=0) {
	while(colourName != "Red") {
		if(colourName == "Black") {
			on(35,15);
			} else{
			on(15,35);
		}
	}
	off(timeToWait);
}

void followLine(int time, int timeToWait=0) {
	clearTimer(T1);
	while(time1[T1] < time) {
		if(colourName == "Black") {
			on(35,15);
			} else{
			on(15,35);
		}
	}
	off(timeToWait);
}

//claw functions

void stopClaw() {
	motor[claw] = 0;
}

void openClaw(int timeToWait=0) {
	motor[claw] = 100;
	sleep(3000);
	stopClaw();
	sleep(timeToWait);
}

void closeClaw(int timeToWait=0) {
	motor[claw] = -100;
	sleep(4000);
	stopClaw();
	sleep(timeToWait);
}

//crane functions

void stopCrane() {
	motor[crane] = 0;
}

void craneUp(int timeToWait=0) {
	motor[crane] = -30;
	sleep(1700);
	stopCrane();
	sleep(timeToWait);
}

void craneDown(int timeToWait=0) {
	motor[crane] = 30;
	sleep(1700);
	stopCrane();
	sleep(timeToWait);
}


//colour tasks
task readColour() {
	while(true) {
		getColorRGB(colour, red, green, blue);
		if(red > 15 && green < 10 && blue < 10) {
			colourName = "Red";
			} else if(red < 10 && green < 10 && blue < 10) {
			colourName = "Black";
			} else if(red > 20 && green > 20) {
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
	sleep(64524);
	crazy = true;
}

task main()
{
	startTask(readColour);
	startTask(displayColour);
	startTask(boing);

	craneUp();
	followLineToRed(500);
	on(30,-30);
	while(colourName != "Silver") {}
	while(colourName != "Red") {}
	off(500);
	openClaw(500);
	craneDown(500);
	closeClaw(500);
	craneUp(500);
	on(-30,30);
	while(colourName != "Silver") {}
	while(colourName != "Black" && colourName != "Red") {}
	off(500);

	followLine(1000);
	followLineToRed();
	on(30,30);
	while(colourName != "Silver") {}
	off(500);
	on(30,-30);
	while(colourName != "Black") {}
	while(colourName != "Silver") {}
	off(500);
	followLineToRed(500);

	on(-20,20);
	sleep(1000);
	off(500);

	craneDown(500);
	openClaw();
	closeClaw(500);
	craneUp(500);

	on(-30,-30);
	while(colourName != "Black") {}
	off(500);
	on(30,-30);
	while(colourName != "Silver") {}
	off(500);

	while(crazy == false) {
		if(colourName == "Black") {
			on(35,15);
			} else{
			on(15,35);
		}
	}
	off(500);

	repeat(2) {
		on(-50,-50);
		sleep(1000);
		on(50,50);
		sleep(1000);
	}
	on(-30,30);
	while(colourName != "Black") {}
	repeat(2) {
		while(colourName != "Silver") {}
		while(colourName != "Black") {}
	}
	off(500);

	followLineToRed();
	on(30,30);
	while(colourName != "Silver") {}
	off(500);
	on(30,-30);
	while(colourName != "Black") {}
	while(colourName != "Silver") {}
	off(500);
	followLineToRed(500);

	on(-30,30);
	while(colourName != "Silver") {}
	while(colourName != "Red") {}
	off(500);
	on(50,50);
	while(colourName != "Silver") {}
	while(colourName != "Black") {}
	on(-50,-50);
	motor[claw] = 100;
	sleep(3000);
	motor[claw] = 0;
	while(colourName != "Red") {}
	off(500);

	on(-30,30);
	while(colourName != "Black") {}
	on(30,30);
	while(colourName != "Black") {}
	while(colourName != "Silver") {}
	on(30,-30);
	while(colourName != "Black") {}
	while(colourName != "Silver") {}

	followLineToRed(500);
	motor[crane] = 60;
	sleep(750);
}
