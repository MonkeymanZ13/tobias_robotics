#pragma config(Sensor, S1,     colour,         sensorEV3_Color, modeEV3Color_RGB_Raw)
#pragma config(Motor,  motorA,          right,         tmotorEV3_Large, PIDControl, reversed, encoder)
#pragma config(Motor,  motorB,          left,          tmotorEV3_Large, PIDControl, reversed, encoder)
#pragma config(Motor,  motorC,          claw,          tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          crane,         tmotorEV3_Large, PIDControl, encoder)

//VIEW THE README FILE FOR A DESCRIPTION OF WHAT THIS ROBOT DOES DURING THE PERFORMANCE

/* ABOVE CONFIGURATIONS:
		set up colour sensor in S1 called 'colour' for following lines
		set up right wheel in port A called 'right'
		set up left wheel in port B called 'left'
		set up motor controlling opening and closing the claw in port C called 'claw' to pick up chocolates
		set up motor moving the crane arm up and down in port D called 'crane' to move the claw higher and lower

*/

int red, green, blue; //declares integer variables 'red', 'green' and 'blue' to store raw RGB values
string colourName; //declares string variable 'colourName' to store the colour that the colour sensor is on

//BELOW: functions that use the wheels and control movement

void off(int timeToWait=0) { //turns off both wheels and waits for 'timeToWait' milliseconds afterwards
	motor[left] = 0;
	motor[right] = 0;
	sleep(timeToWait);
}

void on(int powerLeft, int powerRight, int time=0, int timeToWait=0) { //sets left wheel to
	motor[left] = powerLeft;							 															 //'powerLeft' and right
	motor[right] = powerRight;																					 //wheel to 'powerRight'
	if(time != 0) {																											 //then stops motors after
		sleep(time);																											 //'time' milliseconds if
		off(timeToWait);																									 // specified
	}
}


void followLineToRed(int timeToWait=0) { //follows the black line until it reaches the red line and
	while(colourName != "Red") {					 //waits for 'timeToWait' milliseconds afterwards
		if(colourName == "Black") {
			on(35,15);
			} else {
			on(15,35);
		}
	}
	off(timeToWait);
}

void followLine(int time, int timeToWait=0) { //follows the black line for 'time' milliseconds and
	while(time1[T1] < time) {										//waits for 'timeToWait' milliseconds afterwards
		if(colourName == "Black") {
			on(35,15);
			} else {
			on(15,35);
		}
	}
}

//BELOW: functions control opening and closing the claw

void stopClaw() { //turns off the motor 'claw'
	motor[claw] = 0;
}

void openClaw(int timeToWait=0) { //opens the claws and waits for 'timeToWait' milliseconds
	motor[claw] = 100;							//afterwards
	sleep(3000);
	stopClaw();
	sleep(timeToWait);
}

void closeClaw(int timeToWait=0) { //closes the claws and waits for 'timeToWait' milliseconds
	motor[claw] = -100;							 //afterwards
	sleep(4000);
	stopClaw();
	sleep(timeToWait);
}

//BELOW: functions that control moving the crane arm up and down

void stopCrane() { //turns off the motor 'crane'
	motor[crane] = 0;
}

void craneUp(int timeToWait=0) { //raises the crane arm and waits for 'timeToWait' milliseconds
	motor[crane] = -30;						 //afterwards
	sleep(1700);
	stopCrane();
	sleep(timeToWait);
}

void craneDown(int timeToWait=0) { //lowers the crane arm and waits for 'timeToWait' milliseconds
	motor[crane] = 30;							 //afterwards
	sleep(1700);
	stopCrane();
	sleep(timeToWait);
}

//BELOW: functions that use more than one motor

void pickUpChocolate() { //picks up a chocolate
	openClaw(500);
	craneDown(500);
	closeClaw(500);
	craneUp(500);
}

void dropChocolate() { //drops off a chocolate
	craneDown(500);
	openClaw(500);
	closeClaw(500);
	craneUp(500);
}


//BELOW: tasks that use the colour sensor

task readColour() { //constantly reads the raw RGB values, sets them to 'red', 'green' and 'blue'
	repeat(forever) {	//and then uses this information to set 'colourName' to either "Red", "Black",
		getColorRGB(colour, red, green, blue); //"Silver", or "Unknown"
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

task displayColour() { //constantly displays the raw RGB values on the screen and the colour that it is on
	repeat(forever) {		//(the variable 'colourName')
		displayBigTextLine(1, "R				 	G					B");
		displayBigTextLine(4, "%d			  %d			 %d", red, green, blue);
		displayBigTextLine(7, "				 %s", colourName);
	}
}

task main() {
	startTask(readColour); //starts task readColour (see above)
	startTask(displayColour); //starts task displayColour (see above)
	clearTimer(T1); //resets timer 'T1' to 0 milliseconds and starts timing

	craneUp(); //needs to start with crane arm up

	//picking up chocolate
	followLineToRed(500);
	on(30,-30);
	waitUntil(colourName == "Silver");
	waitUntil(colourName == "Red");
	off(500);
	pickUpChocolate();

	//getting onto ramp
	on(-30,30);
	waitUntil(colourName == "Silver");
	waitUntil(colourName != "Silver");
	off(500);
	on(30,30,1000);
	followLineToRed();
	on(30,30);
	waitUntil(colourName == "Silver");
	off(500);
	on(30,-30);
	waitUntil(colourName == "Black");
	waitUntil(colourName == "Silver");
	off(500);

	//moving up ramp
	followLineToRed(500);
	on(-20,20,1000,500);

	//dropping off chocolate
	dropChocolate();

	//getting back onto ramp
	on(-30,-30);
	waitUntil(colourName == "Black");
	off(500);
	on(30,-30);
	waitUntil(colourName == "Silver");
	off(500);

	//follow line down ramp until 'boing' in music (i.e. 64.524 seconds through)
	followLine(64524, 500);

	//going CRAZY!
	repeat(2) {
		on(-50,-50, 1000);
		on(50,50, 1000);
	}

	//spin on ramp
	on(-30,30);
	waitUntil(colourName == "Black");
	repeat(2) {
		waitUntil(colourName == "Silver");
		waitUntil(colourName == "Black");
	}
	off(500);

	//getting off ramp
	followLineToRed();
	on(30,30);
	waitUntil(colourName == "Silver");
	off(500);
	on(30,-30);
	waitUntil(colourName == "Black");
	waitUntil(colourName == "Silver");
	off(500);
	followLineToRed(500);

	//go backwards up the board
	on(-30,30);
	waitUntil(colourName == "Silver");
	waitUntil(colourName == "Red");
	off(500);
	on(50,50);
	waitUntil(colourName == "Silver");
	waitUntil(colourName == "Black");
	on(-50,-50);
	motor[claw] = 100;
	sleep(3000);
	motor[claw] = 0;
	waitUntil(colourName == "Red");
	off(500);

	//follow last line up to box of chocolates
	on(-30,30);
	waitUntil(colourName == "Black");
	on(30,30);
	waitUntil(colourName == "Black");
	waitUntil(colourName == "Silver");
	on(30,-30);
	waitUntil(colourName == "Black");
	waitUntil(colourName == "Silver");
	followLineToRed(500);

	//drop arm into chocolates!
	motor[crane] = 60;
	sleep(750);
}
