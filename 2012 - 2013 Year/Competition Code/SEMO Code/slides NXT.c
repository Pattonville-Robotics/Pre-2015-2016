#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
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

void lift()
{
	if(nNxtButtonPressed == kLeftButton)
	{
		motor[leftslidemotor] = -50;
		motor[rightslidemotor] = -50;
	}
	else if(nNxtButtonPressed == kRightButton)
	{
		motor[leftslidemotor] = 50;
		motor[rightslidemotor]= 50;
	}
	else
	{
		motor[leftslidemotor]=0;
		motor[rightslidemotor]=0;
	}
}

task main()
{
	while (true)
	{
		lift();
	}
}
