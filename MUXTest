void initializeSensors()
{
  SensorType[S4] = sensorEV3_GenericI2C;
  wait1Msec(100);
  if (!initSensorMux(msensor_S4_1, gyroAngle))
    return;
  if (!initSensorMux(msensor_S4_3, colorMeasureColor))
    return;
  wait1Msec(50);
  SensorType[S1] = sensorEV3_Touch;
  wait1Msec(50);
  SensorType[S3] = sensorEV3_IRSensor;
  wait1Msec(50);
}

task main()
{
  if (SensorValue(msensor_S4_1) > 100)
  {
    motor[motorA] = 100;
  }
}
