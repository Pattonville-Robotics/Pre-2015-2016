#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     rightArmMotor, tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     rightMotor,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     leftMotor,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     leftArmMotor,  tmotorTetrix, PIDControl, encoder)
#pragma config(Servo,  srvo_S1_C3_1,    lockServo,            tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int motorPower = 50;
int master;
int slave;
int delta;
bool locked = false;

void initializeRobot()
{
	motor[leftArmMotor] = 0;
	motor[rightArmMotor] = 0;
	clearDebugStream();
	nMotorEncoder[leftArmMotor] = 0;
	nMotorEncoder[rightArmMotor] = 0;
}


task main()
{


	initializeRobot();
	while (true)
	{
		master = nMotorEncoder[leftArmMotor];
		slave = nMotorEncoder[rightArmMotor];
		delta = master - slave;

		clearDebugStream();
		//writeDebugStreamLine("LEFT ARM ENCODER: %d    RIGHT ARM ENCODER: %d" , nMotorEncoder[leftArmMotor], nMotorEncoder[rightArmMotor]);
		writeDebugStreamLine("Added Power: %d" , delta);
		displayTextLine(5, "LPOW: %d", motor[leftArmMotor]);
		displayTextLine(6, "RPOW: %d", motor[rightArmMotor]);

		if(nNxtButtonPressed == 2)
		{
				motor[rightArmMotor] = - motorPower + (delta / 5);
				motor[leftArmMotor] = - motorPower;

			//displayTextLine(3, "Lowering Arm");
		}
		else if(nNxtButtonPressed == 1)
		{
				motor[rightArmMotor] = motorPower + (delta / 5);
				motor[leftArmMotor] = motorPower;

			//displayTextLine(3, "Raising Arm");
		}
		else if(nNxtButtonPressed == 3 && locked == false)
		{
			servo[lockServo] = 128;
			locked = true;
		}
		else if (nNxtButtonPressed == 3 && locked == true)
		{
			servo[lockServo] = 0;
			locked = false;
		}
		else
		{
			motor[leftArmMotor] = 0;
			motor[rightArmMotor] = 0;
			displayTextLine(3, "LEFT: %d" , nMotorEncoder[leftArmMotor]);
			displayTextLine(4, "RIGHT: %d" , nMotorEncoder[rightArmMotor]);
			servo[lockServo] = 128;
		}
	}
}
