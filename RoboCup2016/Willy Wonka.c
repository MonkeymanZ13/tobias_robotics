#pragma config(Sensor, S4,     touch,          sensorTouch)
#pragma config(Motor,  motorA,          arms,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          hat,           tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          lights,        tmotorNXT, PIDControl, encoder)

int posNeg[2] = {-1, 1};

void offArms() {
	motor[arms] = 0;
}

void offHat() {
	motor[hat] = 0;
}

void arm(int power=50, int timeToWait=0) {
	motor[arms] = power;
	if(timeToWait != 0) {
		sleep(timeToWait);
		offArms();
	}
}

void hatt(int power=50, int timeToWait=0) {
	motor[hat] = power;
	if(timeToWait != 0) {
		sleep(timeToWait);
		offHat();
	}
}

task light() {
	while(true) {
		while(SensorValue[touch] == 0) {}
		while(SensorValue[touch] == 1) {}
		motor[lights] = -80;
		sleep(190);
		motor[lights] = 0;
		while(SensorValue[touch] == 0) {}
		while(SensorValue[touch] == 1) {}
		motor[lights] = 80;
		sleep(190);
		motor[lights] = 0;
	}
}

task randomHat() {
	while(time1[T1] < 110298) {
		hatt((random(50)+50)*posNeg[random(1)],random(2000));
		sleep(random(500));
	}
	while(time1[T1] < 110798) {}
	hatt(100,1500);
}

task randomArms() {
	while(time1[T1] < 109798) {
		arm((random(50)+25)*posNeg[random(1)],random(2000));
		sleep(random(1000));
	}
	while(time1[T1] < 110798) {}
	arm(100,1500);
}

task main() {
	sleep(4843);
	clearTimer(T1);
	startTask(light);
	while(time1[T1] < 389) {};
	arm(65,550);
	hatt(60,550);
	arm(100);
	hatt(60);
	while(time1[T1] < 2039) {}
	offArms();
	offHat();
	while(time1[T1] < 6016) {}
	hatt(70,1200);
	hatt(-70,1200);
	while(time1[T1] < 9983) {}
	arm(65,800);
	while(time1[T1] < 14213) {}
	arm(70);
	hatt(100);
	sleep(2000);
	offArms();
	offHat();
	while(time1[T1] < 18316) {}
	hatt(70,1200);
	hatt(-70,1200);
	hatt(70,1200);
	while(time1[T1] < 22365) {}
	arm(80,1000);
	while(time1[T1] < 24099) {}
	hatt(-70,1200);
	hatt(70,1200);
	while(time1[T1] < 27620) {}
	arm(75,500);
	hatt(100,1000);
	while(time1[T1] < 32085) {}
	arm(75,500);
	while(time1[T1] < 36000) {}
	hatt(100,500);
	hatt(-100,500);
	hatt(100,500);
	hatt(-100,500);
	while(time1[T1] < 40467) {}
	hatt(100,1200);
	while(time1[T1] < 44337) {}
	hatt(70);
	arm(75,300);
	sleep(500);
	hatt(-70,800);
	while(time1[T1] < 46202) {}
	hatt(40);
	while(time1[T1] < 47022) {}
	hatt(80);
	while(time1[T1] < 47375) {}
	hatt(-40);
	while(time1[T1] < 48192) {}
	hatt(-80);
	while(time1[T1] < 48369) {}
	offHat();
	while(time1[T1] < 50000) {}
	hatt(40);
	while(time1[T1] < 51244) {}
	hatt(80);
	while(time1[T1] < 51000) {}
	hatt(-40);
	while(time1[T1] < 52138) {}
	hatt(-80);
	while(time1[T1] < 54684) {}
	repeat(9) {
		arm(70,250);
		sleep(325);
		arm(-70,250);
		sleep(325);
	}
	offHat();
	while(time1[T1] < 64513) {}
	nVolume = 4;
	playSoundFile("ohno.rso");
	startTask(randomHat);
	startTask(randomArms);
	while(time1[T1] < 112798) {}
}
