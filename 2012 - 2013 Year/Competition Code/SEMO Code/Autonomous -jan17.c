#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S2,     ,               sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     rightmotor,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     leftmotor,     tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     rightslidemotor, tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     leftslidemotor, tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S1_C3_1,    bucketservo,          tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include <JoystickDriver.c>
#include <hitechnic-sensormux.h>
#include <hitechnic-irseeker-v2.h>
#include <hitechnic-gyro.h>
#include <hitechnic-touchmux.h>
#include <lego-light.h>
//These are the drivers required for handling


const tMUXSensor LightSensor = msensor_S2_2;
const tMUXSensor gyroSensor = msensor_S2_1;
const tMUXSensor irseeker = msensor_S2_3;
//These three lines declare the sensors that we are running through the sensor multiplexor

const int leftColumn = 0;
const int midColumn = 1;
const int rightColumn = 2;

float happy_angle=0;

task update_gyroSensor()
{
	HTGYROstartCal(gyroSensor);
	while(true)
	{
		float angle=HTGYROreadRot(gyroSensor)/100.0;
		happy_angle=happy_angle+angle;
		nxtDisplayTextLine(7, "%d", happy_angle);
		wait1Msec (10);
	}
}

int readIR()
{
	int IRValue = HTIRS2readACDir(irseeker);
	return IRValue;
}

void driveStraight (int motorSpeed, int angle)//Drives straight at an angle.
{
	motor[leftmotor] = motorSpeed;
	motor[rightmotor] = motorSpeed;
	//if (happy_angle < 3 && happy_angle > -3)//while straight go straight
	//{
	//	nxtDisplayTextLine(6, "Going Straight");
	//	motor[rightmotor]=motorSpeed;
	//	motor[leftmotor]=motorSpeed;
	//}
	if (happy_angle<0)// while rightmotor go leftmotor
	{
		nxtDisplayTextLine(6, "Going rightmotor");
		motor[rightmotor]=motorSpeed;
		motor[leftmotor]=motorSpeed;
	}
	else if (happy_angle>0)// while leftmotor go rightmotor
	{
		nxtDisplayTextLine(6, "Going leftmotor");

		motor[rightmotor]=motorSpeed * 2;
		motor[leftmotor]=motorSpeed * 0.15;
	}
}

void raiseBucket() //This function allows us to raise the linear slides in order to score.
{
	motor[leftslidemotor]=100;
	motor[rightslidemotor]=100;
	wait10Msec(75);
	motor[leftslidemotor]=0;
	motor[rightslidemotor]=0;
}

void lowerBucket()
{
	motor[leftslidemotor]=-100;
	motor[rightslidemotor]=-100;
	wait10Msec(75);
	motor[leftslidemotor]=0;
	motor[rightslidemotor]=0;
};

void goStraightForTime(int time, int speed)//This function allows locomotion over a predefined period (per the parameters) and stop. We create a timer in order to measure the time elapsed between the beginning and end of execution of this function.
{
	time1[T1] = 0;//Creating the aforementioned timer
	while (time1[T1] < time)//While loop saying that while the value of the timer is less than the value of the parametrically set stop time.
	{
		driveStraight (speed, 0);//Drivers forward with no angle change (the null value is set in the parameters)
	}
}

void turnToAngle(int motorSpeedTurn, int motorSpeedForward, int angle)
{
	bool turningRight = false;
	if(happy_angle < angle)
	{
		turningRight = true;
	}
	if(turningRight == true)
	{
		while(happy_angle < angle)
		{
			motor[leftmotor] = motorSpeedForward + motorSpeedTurn;
			motor[rightmotor] = motorSpeedForward - motorSpeedTurn;
		}
	}
	else //Turning left
	{
		while(happy_angle > angle)
		{
			motor[leftmotor] = motorSpeedForward - motorSpeedTurn;
			motor[rightmotor] = motorSpeedForward + motorSpeedTurn;
		}
	}
}

int turnUntilIRNot(int irValue, int motorSpeedTurn)
{
	while(readIR() == irValue)
	{
		motor[leftmotor] = motorSpeedTurn;
		motor[rightmotor] = -motorSpeedTurn;
	}
	return happy_angle;
}



int findLine()
{
	servo[servo1] = 0;
	int angle = 0;
	int initValue = 0;
	int raw = 0;
	bool isFinished = false;
	int i = 0;
	LSsetActive(LightSensor);
	wait10Msec(100);
	initValue = LSvalNorm(LightSensor);
	nxtDisplayBigTextLine(2, "%d", initValue);
	raw = initValue;
	for(i = 0; abs(raw-initValue)<=4 && i < 255; i++)
	{
		raw = LSvalNorm(LightSensor);
		nxtDisplayBigTextLine(4, "%d", raw);
		if(abs(raw - initValue)> 4)
		{
			isFinished = true;
		}
		wait10Msec(1);
		servo[servo1] = i;
	}
	angle = i * 180/255;
	int distance = 40*cosDegrees(angle);
	return distance;
};



bool isCenter;
bool isLeft;



void alignWithColumn(int value)
{
	if(value == leftColumn)
	{
		isCenter = true;//Fix these
		isLeft = false;
	}
	else if(value == midColumn)
	{
		isCenter = false;
		isLeft = true;
	}
	else if(value == rightColumn)
	{
		isCenter = false;
		isLeft = false;
	}
	else
	{
		motor[rightmotor] = 0;
		motor[leftmotor] = 0;
		isCenter = true;
		isLeft = true;
	}

	//Is for following through with the previously found values.
	if(isCenter == true && isLeft == false)
	{
		goStraightForTime(3000, 60);
		isLeft = true;
	}
	else if(isCenter == false && isLeft == true)
	{
		goStraightForTime(2000, 60);
		turnToAngle(40, 0, -90);
		goStraightForTime(1000, 40);
		turnToAngle(40, 0, 0);
		isLeft = true;
		isCenter = true;
	}
	else if(isCenter==false && isLeft == false)
	{
		goStraightForTime(2000, 60);
		turnToAngle(40, 0, 90);
		goStraightForTime(1000, 40);
		turnToAngle(40, 0, 0);
		isLeft = true;
		isCenter = true;

	}
	else
	{
		motor[rightmotor] = 0;
		motor[leftmotor] = 0;
	}
}

void determineColumn()
{
	int columnAngle = turnUntilIRNot(5, 50);
	if(columnAngle < 24)
	{
		alignWithColumn(leftColumn);
	}
	else if(columnAngle < 36)
	{
		alignWithColumn(midColumn);
	}
	else
	{
		alignWithColumn(rightColumn);
	}
};
//1000ms on bucket is 65mm

task main()//Main task. Execution starts here.
{
	waitForStart();//Waiting for the start command from FCS, as sanctioned by the Game Manual
	//raiseBucket();//Raise the bucket to the height of the first row of pegs on the scoring goal (for value, see the "raiseBucket" void function)

	//int IRState; //Gets value from the IR
	//StartTask (Play)
	StartTask (update_gyroSensor);
	wait10Msec(350);

	//turnToAngle (20, 0, -90);
	//StopAllTasks ();

	goStraightForTime(3500, 64);
	goStraightForTime(500, 30);
	goStraightForTime(500, 20);
	//lowerBucket();

}
