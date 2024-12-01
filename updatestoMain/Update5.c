#include "EV3Servo-lib-UW.c"
#include "UW_sensorMux.c"

// Global Variables
int leftMotorStartPosition = 0;
int rightMotorStartPosition = 0;

// Gyro Initialization
void initializeSensors() {
	SensorType[S4] = sensorEV3_GenericI2C;
	wait1Msec(100);
	if (!initSensorMux(msensor_S4_2, touchStateBump))
		return;
	if (!initSensorMux(msensor_S4_3, colorMeasureColor))
		return;
	wait1Msec(50);
	SensorType[S1] = sensorEV3_Gyro;
  wait1Msec(50);
  SensorMode[S1] = modeEV3Gyro_Calibration;
  wait1Msec(50);
  SensorMode[S1] = modeEV3Gyro_Angle;
  wait1Msec(50);
	SensorType[S3] = sensorEV3_IRSensor;
	wait1Msec(50);
}

// DONE
int getColorSelection() {
	int colorSelection = 1;
	while (!getButtonPress(buttonEnter)) {
		if (getButtonPress(buttonRight)) {
			colorSelection++;
			wait1Msec(200); // Debounce
		}
		if (getButtonPress(buttonLeft)) {
			colorSelection--;
			wait1Msec(200); // Debounce
		}
		if (colorSelection > 4) {
			colorSelection = 1;
		}
		if (colorSelection < 1) {
			colorSelection = 4;
		}
		eraseDisplay();
		switch (colorSelection) {
		case 1: displayBigTextLine(4, "Red"); break;
		case 2: displayBigTextLine(4, "Blue"); break;
		case 3: displayBigTextLine(4, "Yellow"); break;
		case 4: displayBigTextLine(4, "Green"); break;
		}
	}
	eraseDisplay();
	return colorSelection;
}

// DONE
int getBlockCount() {
	int blockCount = 1;
	while (!getButtonPress(buttonEnter)) {
		if (getButtonPress(buttonEnter)) break;
		if (getButtonPress(buttonRight)) {
			blockCount = (blockCount < 9) ? blockCount + 1 : 9;
			wait1Msec(200); // Debounce
		}
		if (getButtonPress(buttonLeft)) {
			blockCount = (blockCount > 1) ? blockCount - 1 : 1;
			wait1Msec(200); // Debounce
		}
		eraseDisplay();
		displayString(0, "Enter Block Count:");
		displayString(1, "Press buttons (1-9)");
		displayBigTextLine(4, "Count: %d", blockCount);
	}
	wait1Msec(500); // Debounce delay
	eraseDisplay();
	return blockCount;
}

// Drives towards a detected block using IR sensor
bool driveToBlock() {
	// Start moving backward
	motor[motorA] = motor[motorD] = -25;
	motor[motorB] = motor[motorC] = 25;

	// Stop when an obstacle is detected
	while (SensorValue[S3] > 5) {} // Adjust threshold based on environment
	motor[motorA] = motor[motorD] = 0;
	motor[motorB] = motor[motorC] = 0;
	wait1Msec(2000);

	// Start turning
	motor[motorA] = 25;
	motor[motorD] = -25;

	// Turn until a 180-degree change is detected
	while (abs(getGyroDegrees(S1)) < 180) {}

	// Stop motors
	motor[motorA] = motor[motorD] = 0;
	motor[motorB] = motor[motorC] = 0;

	// Verify if block is detected (e.g., touch sensor)
	return SensorValue[S2] == 1;
}



// DONE
bool checkColor(int targetColor) {
	return SensorValue[S1] == targetColor;
}

// DONE
void graspBlock() {
	motor[motorC] = -100;
	wait1Msec(1000);
	motor[motorC] = 0;
}

// DONE
void releaseBlock() {
	motor[motorC] = 100;
	wait1Msec(1000);
	motor[motorC] = 0;
}

// DONE
void returnToOriginalPosition() {
	motor[motorA] = motor[motorB] = -50;
	wait1Msec(2000); // Adjust time based on travel distance
	motor[motorA] = motor[motorB] = 0;
}

// Main search and collection routine
void searchAndCollectBlocks(int targetColor, int blockCount) {
	int collectedBlocks = 0;
	while (collectedBlocks < blockCount) {
		if (driveToBlock()) {
			graspBlock();
			if (checkColor(targetColor)) {
				returnToOriginalPosition();
				releaseBlock();
				collectedBlocks++;
				} else {
				releaseBlock();  // Release block if incorrect color
				returnToOriginalPosition();
			}
		}
	}
}

task main() {
	initializeSensors();

	leftMotorStartPosition = nMotorEncoder[motorA];
	rightMotorStartPosition = nMotorEncoder[motorB];

	int targetColor = getColorSelection();
	wait1Msec(1000);
	int blockCount = getBlockCount();
	wait1Msec(2000);

	eraseDisplay();
	displayString(1, "Target Color: %d", targetColor);
	displayString(2, "Block Count: %d", blockCount);

	searchAndCollectBlocks(targetColor, blockCount);
}
