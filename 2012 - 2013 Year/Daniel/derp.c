#pragma config(Sensor, S1,     ultrasonic,     sensorSONAR)
#pragma config(Sensor, S2,     ultrasonic2,     sensorSONAR)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
task main()
{
	while(true)
	{
			nxtDisplayCenteredTextLine(3, "Senser 1 %4d", SensorValue(ultrasonic));
			nxtDisplayCenteredTextLine(4, "Serser 2 %4d", SensorValue(ultrasonic2));
	}
}
