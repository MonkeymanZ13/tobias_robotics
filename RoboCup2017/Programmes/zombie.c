#pragma config(Sensor, S1,     touch,          sensorEV3_Touch)
#pragma config(Sensor, S4,     ir,             sensorEV3_IRSensor, modeEV3IR_Seeker)
#pragma config(Motor,  motorA,          shooter,       tmotorEV3_Medium, PIDControl, encoder)
#pragma config(Motor,  motorB,          left,          tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          right,         tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          axe,           tmotorEV3_Medium, PIDControl, encoder)
//motors & sensors setup

int irDir=0; //variable to store the value of direction which the ir sensor detects the beacon
int axeSwingDuration = 3000; //amount of time to swing the axe for

//moving functions
void on(int leftSpeed, int rightSpeed, int timeToWait=0) { //turn on motors at specified speed
	motor[left] = leftSpeed;
	motor[right] = rightSpeed;
	sleep(timeToWait);
}

void off(int timeToWait=0) { //turn off both motors
	motor[left] = 0;
	motor[right] = 0;
	sleep(timeToWait);
}

void drive(int leftSpeed, int rightSpeed, int duration, int timeToWait=0) { //turn on motors for a certain length of time
	on(leftSpeed,rightSpeed,duration);
	off(timeToWait);
}

void skate(int leftSpeed, int rightSpeed, int repeats, int timeToWait=0) { //skate forward
	repeat(repeats) {
		drive(leftSpeed,rightSpeed,1000);
		drive(rightSpeed,leftSpeed,1000);
	}
	sleep(timeToWait);
}

//axe functions
void axeSwing(int duration, int timeToWait=0) { //swing axe up and down for certain length of time
	clearTimer(T1);
	while(time1[T1] < duration) {
		motor[axe] = 100;
		sleep(520);
		motor[axe] = -100;
		sleep(500);
	}
	motor[axe] = 0;
	sleep(timeToWait);
}

//shoot
void shoot(int timeToWait=0) { //shoot a ball
	motor[shooter] = 75;
	sleep(1000);
	motor[shooter] = 0;
	sleep(timeToWait);
}

void follow(int duration, int timeToWait=0) { //follow the ir beacon (cart) for a certain length of time
	clearTimer(T1);
	while(time1[T1] < duration) {
		if(irDir>=-5 && irDir<=5) {
			on(40,40);
		} else if(irDir>5) {
			on(40,-40);
		} else if(irDir<-5) {
			on(-40,40);
		}
	}
	off(timeToWait);
}

task stopButton() { //press the touch sensor to stop the programme
	while(getTouchValue(touch) == 0) {}
	while(getTouchValue(touch) == 1) {}
	stopAllTasks();
}

task swingAxe() { //swing axe at same time as doing other stuff
	axeSwing(axeSwingDuration);
}

task irRead() { //read and display value that ir sensor is reading
	while(true) {
		irDir = SensorValue[ir];
		displayBigTextLine(3, "Dir: %d", irDir);
	}
}

task main() {
	sleep(24750);
	//sleep(2000);
	startTask(irRead);
	startTask(stopButton);

	//come out of coffin
	drive(50,50,2000);
	axeSwingDuration = 3000; startTask(swingAxe);
	follow(2000);
	drive(30,-30,500,500);
	shoot();

	//follow to ramp
	drive(-30,30,500,500);
	drive(40,40,2000);
	drive(-40,40,500,1000);

	follow(5000,5000);
	drive(40,40,3000,500);
	drive(40,-40,750,4500);

	shoot(4000);

	//crazy spin
	startTask(swingAxe);
	drive(-50,50,3000,500);

	//go down board
	drive(40,-40,2500);
	skate(50,35,2,2000);
	drive(-40,40,2000);

	skate(20,10,2);
	skate(-20,-10,2);
	sleep(1500);

	skate(20,10,2);
	drive(30,-30,800);
	follow(10000);

	axeSwingDuration = 10000; startTask(swingAxe);
	follow(5000);
	clearTimer(T1);

	//turn to face cart for 2 sec
	while(time1[T1] < 2000) {
		if(irDir<-5) {
			on(-30,30);
		} else if(irDir>5) {
			on(30,-30);
		} else {
			off();
		}
	}

	skate(-20,-10,2);
	on(-30,30);
	while(irDir<-5||irDir>5) {}

	off();
	shoot();
	follow(5000);
	axeSwingDuration = 5000; startTask(swingAxe);
}