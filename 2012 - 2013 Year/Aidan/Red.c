#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Hubs,  S3, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,                    sensorI2CMuxController)
#pragma config(Sensor, S2,     irSensor,            sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S3,     ,                    sensorI2CMuxController)
#pragma config(Sensor, S4,     Gyro,                sensorI2CHiTechnicGyro)
#pragma config(Motor,  mtr_S1_C1_1,     slideR,        tmotorNormal, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     rightmotor,    tmotorNormal, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     slideL,        tmotorNormal, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     sweeper,       tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     leftmotor,     tmotorNormal, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     MotorG,        tmotorNormal, openLoop)
#pragma config(Servo,  srvo_S3_C1_1,    servo1,               tServoStandard)
#pragma config(Servo,  srvo_S3_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_3,    Claw,                 tServoStandard)
#pragma config(Servo,  srvo_S3_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S3_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "HTIRS2-driver.h"
#include "HTGYRO-driver.h"

float happy_angle=0;
const int FORWARD =0;
const int BACKWARD =1;
const int TURNLEFT =2;
const int TURNRIGHT =3;

void start_motor (int speed,int direction)
{
	int motor_speed;
	int max_time=abs(speed)*10;
	time1[T1]=0;
	while (time1[T1]<max_time)
	{
		motor_speed=time1[T1]/10;
		switch (direction)
		{
		case FORWARD:
			motor[rightmotor]=-motor_speed;
			motor[leftmotor]=-motor_speed;
			break;
		case BACKWARD:
			motor[rightmotor]=motor_speed;
			motor[leftmotor]=motor_speed;
			break;
		case TURNLEFT:
			motor[rightmotor]=-motor_speed;
			motor[leftmotor]=motor_speed;
			break;
		case TURNRIGHT:
			motor[rightmotor]=motor_speed;
			motor[leftmotor]=-motor_speed;
			break;
		}
	}
}

const int slide_speed=80;

task update_gyro ()
{
	HTGYROstartCal(Gyro);
	while(true)
	{
		float angle=HTGYROreadRot(Gyro)/100.0;
		happy_angle=happy_angle+angle;
		wait1Msec (10);
	}
}

task main ()
{

	waitForStart();   // wait for start of tele-op phase
	StartTask (update_gyro);
	nMotorEncoder[leftmotor] = 0;
	nMotorEncoder[rightmotor] = 0;
	servo[Claw]=110;
	start_motor (25, BACKWARD);
	wait10Msec(200);
	start_motor (25, FORWARD);
	wait10Msec(60);
	motor[leftmotor]=0;
	motor[rightmotor]=0;
	wait10Msec(100);
	while (nMotorEncoder[slideL]<18000)
	{
		motor[slideL]=slide_speed;
		motor[slideR]=slide_speed;
	}
	motor[slideL]=0;
	motor[slideR]=0;

	//start_motor (40, TURNRIGHT); //not reading gyro while running start_motor (0.4 seconds).

	while (happy_angle>-90) //ramp up to 40, skip to 100 and run until 40 degrees. TURNED TOO FAR
	{
		nxtDisplayTextLine (3,"%f",happy_angle);
		motor[leftmotor]=-70;
		motor[rightmotor]=70;
	}
	motor[leftmotor]=0;
	motor[rightmotor]=0;
	wait10Msec(100);
	servo[Claw]=50;
	motor[leftmotor]=60;
	motor[rightmotor]=60;
	wait10Msec(80);
	motor[leftmotor]=0;
	motor[rightmotor]=0;
	wait10Msec(100);
	while (happy_angle>-95) //ramp up to 40, skip to 100 and run until 40 degrees.
	{
		nxtDisplayTextLine (3,"%f",happy_angle);
		motor[leftmotor]=-50;
		motor[rightmotor]=50;
	}
	motor[leftmotor]=0;
	motor[rightmotor]=0;
	motor[leftmotor]=100;
	motor[rightmotor]=100;
	wait10Msec(260);
	motor[leftmotor]=0;
	motor[rightmotor]=0;
	wait10Msec(300);
	while (nMotorEncoder[slideL]>0)
	{
		motor[slideL]=-slide_speed;
		motor[slideR]=-slide_speed;
	}
	motor[slideL]=0;
	motor[slideR]=0;
}
