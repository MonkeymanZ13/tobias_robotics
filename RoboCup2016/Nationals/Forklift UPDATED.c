#pragma config(Sensor, S1,     gyro,           sensorEV3_Gyro)
#pragma config(Sensor, S4,     colour,         sensorEV3_Color, modeEV3Color_RGB_Raw)
#pragma config(Motor,  motorA,          left,          tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          right,         tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          zig,           tmotorEV3_Large, PIDControl, reversed, encoder)
#pragma config(Motor,  motorD,          forklift,      tmotorEV3_Large, PIDControl, encoder)

//VIEW THE README FILE FOR A DESCRIPTION OF WHAT THIS ROBOT DOES DURING THE PERFORMANCE

/* ABOVE CONFIGURATIONS:
		set up gyro sensor in port S1 called 'gyro' for more accurate turns
		set up colour sensor in port S4 called 'colour' for following lines and stopping at a specific colour
		set up left wheel in port A called 'left'
		set up right wheel in port B called 'right'
		set up motor controlling the concertina mechanism in port C called 'zig' to go CRAZY!
		set up motor controlling the forklift port D called 'crane' to pick up and drop off pallets
*/

int gyroValue; //declares integer variable 'gyroValue' to store the value the the gyro sensor is reading
int red, green, blue; //declares integer variables 'red', 'green' and 'blue' to store raw RGB values
string colourName; //declares string variable 'colourName' to store the colour that the colour sensor is on
string r = "Right"; //declares string variable 'r' to store the string "Right" for turning direction
string l = "Left"; //declares string variable 'l' to store the string "Left" for turning direction

//BELOW: functions that use the wheels and control movement

void off(int timeToWait=0) { //turns off both wheels and waits for 'timeToWait' milliseconds afterwards
	motor[left] = 0;
	motor[right] = 0;
	sleep(timeToWait);
}

void on(int powerLeft, int powerRight, int time=0, int timeToWait=0) { //sets left wheel to 'powerLeft'
	motor[left] = powerLeft;							 															 //and right wheel to 'powerRight'
	motor[right] = powerRight;																					 //then stops motors after 'time'
	if(time != 0) {																											 //milliseconds if specified
		sleep(time);
		off(timeToWait);
	}
}

void turn(string direction, int timeToWait=0, int turnValue=85) { //turns in the direction 'turn' to
	resetGyro(gyro);																					      //'turnValue' degrees and waits for
	sleep(500);																								      //'timeToWait' milliseconds afterwards
	if(direction == "Left") {
		on(-15,15);
		waitUntil(gyroValue < turnValue * -1);
		} else {
		on(15,-15);
		waitUntil(gyroValue > turnValue);
	}
	off(timeToWait);
}

void followLineToRed(int timeToWait=0) { //follows the black line until it reaches the red line and
	while(colourName != "Red") {					 //waits for 'timeToWait' milliseconds afterwards
		if(colourName == "Black") {
			on(25,5);
			} else {
			on(5,25);
		}
	}
	off(timeToWait);
}

//BELOW: functions that use the concertina mechanism

void zigUpDown(int timeUp) { //puts the motor 'zig' up, waits for 'timUp' milliseconds and then lowers it
	motor[zig] = 50;
	sleep(1000);
	motor[zig] = 0;
	sleep(timeUp);
	motor[zig] = -50;
	sleep(500);
	motor[zig] = 0;
}

void zigUp() { //turns on the motor 'zig' at 50% power
	motor[zig] = 50;
}

//BELOW: functions that control the forklift

void forkliftUp(int time, int power) { //turns on the motor 'forklift' for 'time' milliseconds at 'power'
	motor[forklift] = power;						 //power
	sleep(time);
	motor[forklift] = 0;
}

void forkliftDown(int time, int power) { //lowers the forklift for 'time' milliseconds at 'power' power
	motor[forklift] = power * -1;
	sleep(time);
	motor[forklift] = 0;
}

//BELOW: tasks that read and display the sensor values

task readSensors() { //constantly reads the values of the gyro and colour sensors and assigns them to their
	repeat(forever) {  //respective variables ('red', 'green', 'blue' and 'colourName' for colour sensor,
		getColorRGB(colour, red, green, blue); //'gyroValue' for the gyro sensor)
		if(red > 10 && green < 6 && blue < 6) {
			colourName = "Red";
			} else if(red < 10 && green < 10 && blue < 10) {
			colourName = "Black";
			} else if(red > 15 && green > 15 && blue > 15) {
			colourName = "Silver";
			} else {
			colourName = "Unknown";
		}
		gyroValue = getGyroHeading(gyro);
	}
}

task displaySensors() { //constantly displays the values of the two sensors and what colour the colour
	repeat(forever) {     //sensor is currently reading
		displayBigTextLine(1, "R				 	G					B");
		displayBigTextLine(4, "%d			  %d			 %d", red, green, blue);
		displayBigTextLine(7, "				 %s", colourName);
		displayBigTextLine(10, "       %d", gyroValue);
	}
}

task crazy() { //waits for the boing in the music and then the concertina mechanism goes up on the first
	clearTimer(T1); //boing, then it goes up and down at random times throughout the backwards part of the
	sleep(64324);  //music, and there is one final 'boing' at the end where it goes up as well
	zigUpDown(1500);
	while(time1[T1] < 102162) {
		sleep(random(4000) + 3000);
		zigUpDown(random(1000) + 1000);
	}
	while(time1[T1] < 111962) {}
	zigUp();
}

task main() {
	sleep(4843); //waits 4.843 seconds while introduction plays in music
	startTask(crazy); //starts task 'crazy' (see above)
	startTask(readSensors); //starts task 'readSensors' (see above)
	startTask(displaySensors); //starts task 'displaySensors' (see above)

	//resets the gyro sensor so 0 is straight forward
	sleep(500);
	resetGyro(gyro);
	sleep(500);

	//moving towards pallet
	on(45,45,3500,500);
	turn(l, 500);
	on(20,20,2000,500);

	//picking up first pallet
	forkliftUp(1000, 30);
	on(-25,-25,1500,500);

	//moving towards drop-off
	turn(l, 500);
	on(20,20,6000,500);
	turn(r, 500);
	on(25,25,11000,500);

	//dropping off first pallet
	forkliftDown(1000, 30);
	on(-20,-20,2000,500);

	//moving towards second pallet
	turn(r,100,175);
	on(30,30,7750,500);
	turn(l,500/*,80*/);
	on(30,30);
	waitUntil(colourName == "Red");
	off(500);
	turn(l,500,80);
	on(20,20,2000,500);

	//picking up second pallet
	forkliftUp(1000,30);
	on(-25,-25,1500,500);

	//moving towards drop-off
	turn(l, 0, 82);
	on(25,25,3800,500);
	turn(r, 500);

	//forklift arm going crazy
	repeat(3) {
		forkliftUp(750,60);
		forkliftDown(750,60);
	}
	forkliftUp(750, 60);

	//crazy zig zag driving
	on(40,20,2000);
	on(20,40,2000);
	on(40,20,1000);
	on(20,40,3000);
	on(40,20,3500);
	on(-40,-20,2000,1000);

	//going backwards onto black line
	on(-40,-40);
	forkliftDown(1000,60);
	waitUntil(colourName == "Black");
	off(500);
	on(30,30);
	waitUntil(colourName == "Silver");

	//crazy spins
	on(50,-50);
	repeat(2) {
		waitUntil(colourName == "Silver");
		waitUntil(colourName == "Black");
	}
	waitUntil(colourName == "Silver");
	on(-50,50);
	repeat(2) {
		waitUntil(colourName == "Black");
		waitUntil(colourName == "Silver");
	}
	waitUntil(colourName == "Black");
	off(500);

	//lining up with black line
	on(20,20);
	waitUntil(colourName == "Silver");
	on(-20,20);
	waitUntil(colourName == "Black");
	waitUntil(colourName == "Silver");
	on(20,-20);
	waitUntil(colourName == "Black");
	off(500);

	//following the black line and knocking over the stacked crates
	followLineToRed(500);
	forkliftUp(500,50);
	on(100,100,1000);
	sleep(5000);
}
