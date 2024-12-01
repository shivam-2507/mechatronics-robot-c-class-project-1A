#include "EV3Servo-lib-UW.c"
task main()
{
    // configure servo controller port
    SensorType[S1] = sensorI2CCustom9V;
    setServoPosition(S2, 6, 0);
    setServoPosition(S2, 5, 0);
    motor[motorA] = motor[motorD] = 15;
    motor[motorB] = motor[motorC] = -15;
    while (SensorValue[S4] != 1)
    {
    }
    wait1Msec(1000);
    setServoPosition(S2, 6, 70);
    setServoPosition(S2, 5, -70);
}