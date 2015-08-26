#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Motor,  mtr_S1_C1_1,     rightArmMotor, tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     rightMotor,    tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     leftMotor,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     leftArmMotor,  tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S1_C3_1,    lockServo,            tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
        __________________________________________________________________________________
        ||                                                                              ||
        ||                    Program created by FTC Team 2866*   			            		||
        ||                                                                              ||
        ||            *This Teleop Program was created in collaboration with a          ||
        ||               programmer from the FTC Team 7856: Mitchell Skaggs             ||
        ||                                                                              ||
        ||              Through Gracious Professionalism our team was able to           ||
        ||              work with another programmer and create an extremely            ||
        ||              elegant teleop program that makes use of the polar              ||
        ||              coordinates system and trigonometry in a simple manner.         ||
        ||                                                                              ||
        ||______________________________________________________________________________||

*/

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.

int leftArmValue = 0;
int rightArmValue = 0;
int motorPower = 60;
int master;
int slave;
int delta;



void initializeRobot() //Set the motors to zero incase of leftover values
{
	servo[lockServo] = 135;
  motor[leftMotor] = 0;
  motor[rightMotor] = 0;
  motor[leftArmMotor] = 0;
	motor[rightArmMotor] = 0;
	clearDebugStream();
	nMotorEncoder[leftArmMotor] = 0;
	nMotorEncoder[rightArmMotor] = 0;
}

void setMotors()   //Converting joystick values into polar coordinates, rotating 45 degrees(PI/4 radians) counterclockwise
{                  //Then converting the coordinates into raw motor values
  /*
    float x = joystick.joy1_x2;             //Take in joystick values
    float y = joystick.joy1_y2;

    float r = sqrt( pow(x, 2) + pow(y, 2)); //Get the radius of the polar coordinates.
    float theta = atan2(y,x);                //Get the theta of the polar coordinates.
    float theta2 = theta + PI/4;            //We add 45 degrees to theta to make the x and y values distinct when moving forward
                                            //Just think about the four graph quadrants and their signs to understand.

    float x2 = -r*cos(theta2);              //This converts the polar coordinates back into cartesian points
    float y1 = r*sin(theta2);

    motor[rightMotor] = ((100*x2)/127);     //Set the 2 motors to scaled motor values using the new cartesian points
    motor[leftMotor] = ((100*y1)/127);
  */

  //This is simplified form resulting in an efficient computation time:
  motor[rightMotor] = -(((float)(100)) / 127) * sqrt(pow(joystick.joy1_x2, 2) + pow(joystick.joy1_y2, 2))
                                              * cos(atan2(joystick.joy1_y2, joystick.joy1_x2) + (PI / 4));

  motor[leftMotor] = (((float)(100)) / 127) * sqrt(pow(joystick.joy1_x2, 2) + pow(joystick.joy1_y2, 2))
                                            * sin(atan2(joystick.joy1_y2, joystick.joy1_x2) + (PI / 4));
}


void setArmMotors()
{
    master = nMotorEncoder[leftArmMotor];
		slave = nMotorEncoder[rightArmMotor];
		delta = master - slave;

		clearDebugStream();
		writeDebugStreamLine("LEFT ENCODER: %d RIGHT ENCODER: %d" , nMotorEncoder[leftArmMotor], nMotorEncoder[rightArmMotor]);
		writeDebugStreamLine("Left: %d Right: %d Added Power: %d", motor[leftArmMotor], motor[rightArmMotor], delta / 6);

		if(joystick.joy2_TopHat == 4)
		{
				rightArmValue = - motorPower + (delta / 6);
				leftArmValue = - motorPower;
		}
		else if(joystick.joy2_TopHat == 0)
		{
				rightArmValue = motorPower + (delta / 6);
				leftArmValue = motorPower;

			//displayTextLine(3, "Raising Arm");
		}
		else
		{
			rightArmValue = 0;
			leftArmValue = 0;
			//displayTextLine(3, "LEFT: %d" , nMotorEncoder[leftArmMotor]);
			//displayTextLine(4, "RIGHT: %d" , nMotorEncoder[rightArmMotor]);
		}
}

task driverTwo()
{
    while (true)
    {
      motor[leftArmMotor] = leftArmValue;
      motor[rightArmMotor] = rightArmValue;
      setArmMotors();

      wait1Msec(20);
    }
}

task main()
{
  initializeRobot();

  waitForStart();

  startTask(driverTwo);

  while (true)
  {
    if(abs(joystick.joy1_x2) > 5 || abs(joystick.joy1_y2) > 5) //Account for dead zones on the controller
    {
      setMotors();
    }
    else   //if there is a dead zone then just stop the motors
    {
      motor[leftMotor] = 0;
      motor[rightMotor] = 0;
    }
    if(joy1Btn(4))
    {
    	servo[lockServo] = 100;
    }
    else if(joy1Btn(2))
    {
    	servo[lockServo] = 150;
    }
    wait1Msec(20);
  }
}