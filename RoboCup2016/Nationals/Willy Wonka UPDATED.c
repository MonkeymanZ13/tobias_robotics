#pragma config(Sensor, S4,     touch,          sensorTouch)
#pragma config(Motor,  motorA,          arms,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          hat,           tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          lights,        tmotorNXT, PIDControl, encoder)

//VIEW THE README FILE FOR A DESCRIPTION OF WHAT THIS ROBOT DOES DURING THE PERFORMANCE

/* ABOVE CONFIGURATIONS:
		set up touch sensor in port S4 called 'touch' to use a button to turn on and off the lights
		set up the motor controlling the arms in port A called 'arms'
		set up motor controlling spinning the hat in port B called 'hat'
		set up motor controlling the piston mechanism for turning on and off the lights in port C called 'lights'
*/

int posNeg[2] = {-1, 1}; //declares integer array containing the numbers 1 and -1 (used later on with randomness)

//BELOW: functions that control the arms

void offArms() { //stops the arms from moving
	motor[arms] = 0;
}

void moveArms(int power=50, int timeMoving=0) { //turns on the arms at 'power' power and, if specified, stops them
	motor[arms] = power;													//after 'timeMoving' milliseconds
	if(timeMoving != 0) {
		sleep(timeMoving);
		offArms();
	}
}

//BELOW: functions that control the hat

void offHat() { //stops the hat from spinning
	motor[hat] = 0;
}

void spinHat(int power=50, int timeSpinning=0) {//starts spinning the hat at 'power' power and, if specified, stops
	motor[hat] = power;														//it after 'timeSpinning' milliseconds
	if(timeSpinning != 0) {
		sleep(timeSpinning);
		offHat();
	}
}

//BELOW: tasks that will be called in task main

task onOffLights() { //constantly waits until the button (touch sensor) is pressed and turns on the lights, then
	repeat(forever) {	 //waits for it to be pressed again and turns the lights off
		waitUntil(SensorValue[touch] == 1);
		waitUntil(SensorValue[touch] == 0);
		motor[lights] = -80;
		sleep(190);
		motor[lights] = 0;
		waitUntil(SensorValue[touch] == 1);
		waitUntil(SensorValue[touch] == 0);
		motor[lights] = 80;
		sleep(190);
		motor[lights] = 0;
	}
}

task randomHat() { //when he goes crazy, Willy Wonka's hat spins at random times and random speeds
	while(time1[T1] < 110298) {
		spinHat((random(50)+50)*posNeg[random(1)],random(2000)); //using the array 'posNeg' ensures that it can be a
		sleep(random(500));																			 //positive OR a negative number (at random) so it can
	}																													 //spin clockwise or anticlockwise
	waitUntil(time1[T1] > 110798);
	spinHat(100,1500);
}

task randomArms() { //when he goes crazy, Willy Wonka's arms move at random times and random speeds
	while(time1[T1] < 109798) {
		moveArms((random(50)+10)*posNeg[random(1)],random(2000)); //'posNeg' is used here in the same way as above
		sleep(random(1000));
	}
	waitUntil(time1[T1] > 110798);
	moveArms(70,1500);
}

task main() {
	sleep(4843); //waits 4.843 seconds while introduction plays in music
	clearTimer(T1); //resets the timer 'T1' to 0 and starts timing
	startTask(onOffLights); //starts task 'onOffLights' so I can turn the lights on and off at any time

	//Willy Wonka dancing and 'conducting' his factory
	waitUntil(time1[T1] > 389);
	moveArms(65,550);
	spinHat(60,550);
	moveArms(100);
	spinHat(60);
	waitUntil(time1[T1] > 2039);
	offArms();
	offHat();

	waitUntil(time1[T1] > 6016);
	spinHat(70,1200);
	spinHat(-70,1200);
	waitUntil(time1[T1] > 9983);
	moveArms(65,800);

	waitUntil(time1[T1] > 14213);
	moveArms(70);
	spinHat(100);
	sleep(2000);
	offArms();
	offHat();
	waitUntil(time1[T1] > 18316);
	spinHat(70,1200);
	spinHat(-70,1200);
	spinHat(70,1200);

	waitUntil(time1[T1] > 22365);
	moveArms(76,1000);
	waitUntil(time1[T1] > 24099);
	spinHat(-70,1200);
	spinHat(70,1200);

	waitUntil(time1[T1] > 27620);
	moveArms(70,500);
	spinHat(100,1000);
	waitUntil(time1[T1] > 32085);
	moveArms(70,500);

	waitUntil(time1[T1] > 36000);
	spinHat(100,500);
	spinHat(-100,500);
	spinHat(100,500);
	spinHat(-100,500);
	waitUntil(time1[T1] > 40467);
	spinHat(100,1200);

	waitUntil(time1[T1] > 44337);
	spinHat(70);
	moveArms(70,300);
	sleep(500);
	spinHat(-70,800);
	waitUntil(time1[T1] > 46202);
	spinHat(40);

	waitUntil(time1[T1] > 47022);
	spinHat(80);
	waitUntil(time1[T1] > 47375);
	spinHat(-40);

	waitUntil(time1[T1] > 48192);
	spinHat(-80);
	waitUntil(time1[T1] > 48369);
	offHat();

	waitUntil(time1[T1] > 50000);
	spinHat(40);
	waitUntil(time1[T1] > 51244);
	spinHat(80);

	waitUntil(time1[T1] > 51000);
	spinHat(-40);
	waitUntil(time1[T1] > 52138);
	spinHat(-80);

	waitUntil(time1[T1] > 54684);
	repeat(9) {
		moveArms(55,250);
		sleep(325);
		moveArms(-55,250);
		sleep(325);
	}
	offHat();

	//going CRAZY!
	waitUntil(time1[T1] > 64513);
	startTask(randomHat);
	startTask(randomArms);
	waitUntil(time1[T1] > 112798);
}
