//this returns with motor encoders
#include "EV3Servo-lib-UW.c"
#include "UW_sensorMux.c"

// Gyro Initialization
void initializeSensors() {
	SensorType[S4] = sensorEV3_Touch;
	wait1Msec(50);
	SensorType[S3] = sensorEV3_IRSensor;
	wait1Msec(50);
	SensorMode[S3] = modeEV3IR_Seeker;
	wait1Msec(50);
	SensorType[S2] = sensorI2CCustom9V;
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
bool isBlock() {
	if (SensorValue[S3] <100 ) {
		return true;
	}
	else {
		return false;
	}
}

void driveToBlock() {
	eraseDisplay();
	displayBigTextLine(4, "big driver");
	motor[motorA] = motor[motorD] = -25;
	motor[motorB] = motor[motorC] = 25;

	// Stop when an obstacle is detected
	while (SensorValue[S3] > 5) {} // Adjust threshold based on environment
	motor[motorA] = motor[motorD] = 0;
	motor[motorB] = motor[motorC] = 0;
	wait1Msec(2000);
}

// DONE
bool checkColor(int targetColor) {
	if (readMuxSensor(msensor_S1_3) == targetColor) {
		return true;
	}
	else {
		return false;
	}
}

void rotateRobot(int degrees) {
		eraseDisplay();
		displayBigTextLine(4, "rotator");
    const float WHEEL_DIAMETER = 5.6; // Replace with your wheel diameter (in cm)
    const float TRACK_WIDTH = 12.0;   // Replace with your robot's track width (in cm)

    // Calculate the number of encoder degrees needed
    float targetDegrees = (360.0 * TRACK_WIDTH * degrees) / (WHEEL_DIAMETER * 360.0);

    // Reset motor encoders
    nMotorEncoder[motorA] = 0;
    nMotorEncoder[motorC] = 0;

    // Set motors for rotation
    if (degrees > 0) {
        motor[motorA] = motor[motorB] = 20;
				motor[motorD] = motor[motorC] = -20;
    } else {
       	motor[motorA] = motor[motorB] = -20;
				motor[motorD] = motor[motorC] = 20;
    }

    // Wait until the robot completes the rotation
    while (abs(nMotorEncoder[motorA]) < abs(targetDegrees)) {}

    // Stop motors
    motor[motorA] = motor[motorB] = 0;
    motor[motorC] = motor[motorD] = 0;
}

// DONE
void graspBlock() {
	eraseDisplay();
	displayBigTextLine(4, "grasp");
	// Start turning
	//setServoPosition(S2, 6, 0);
	//setServoPosition(S2, 5, 0);

	rotateRobot(180);
	wait1Msec(100);

	motor[motorA] = motor[motorD] = 15;
	motor[motorB] = motor[motorC] = -15;

	while (SensorValue[S4] != 1) {}
	wait1Msec(100);

	motor[motorA] = motor[motorD] = 0;
	motor[motorB] = motor[motorC] = 0;
	setServoPosition(S2, 6, 70);
	setServoPosition(S2, 5, -70);
	wait1Msec(100);
}

// DONE
/*void releaseBlock() {
setServoPosition(S2, 6, 0);
setServoPosition(S2, 5, 0);
wait1Msec(100);
}*/

// DONE
void returnToOriginalPosition(int initialEncoderA, int initialEncoderC) {
		eraseDisplay();
		displayBigTextLine(4, "return to og");
    int currentEncoderA = nMotorEncoder[motorA];
    int currentEncoderC = nMotorEncoder[motorC];

    // Calculate the distance to drive back
    int distanceA = initialEncoderA - currentEncoderA;
    int distanceC = initialEncoderC - currentEncoderC;

    // Set motors to reverse direction
    motor[motorA] = (distanceA > 0) ? -20 : 20;
    motor[motorC] = (distanceC > 0) ? -20 : 20;

    // Drive until back to initial position
    while (abs(nMotorEncoder[motorA] - initialEncoderA) > 5 || abs(nMotorEncoder[motorC] - initialEncoderC) > 5) {
        // Update motor speeds dynamically for fine adjustments
        motor[motorA] = (nMotorEncoder[motorA] < initialEncoderA) ? -20 : 20;
        motor[motorC] = (nMotorEncoder[motorC] < initialEncoderC) ? -20 : 20;
    }

    // Stop motors
    motor[motorA] = 0;
    motor[motorC] = 0;

    displayBigTextLine(8, "Back to Start!");
}

// Main search and collection routine
void searchAndCollectBlocks(int targetColor, int blockCount, int initialEncoderA, int initialEncoderC) {
	int collectedBlocks = 0;
	while (collectedBlocks < blockCount) {
		if (isBlock()) {
			driveToBlock();
			graspBlock();
			if (checkColor(targetColor)) {
				returnToOriginalPosition(initialEncoderA, initialEncoderC);
				//releaseBlock();
				collectedBlocks++;
			}
			else {
				//releaseBlock();
				returnToOriginalPosition(initialEncoderA, initialEncoderC);
			}
		}
		else {
			rotateRobot(10);
		}
	}
}

task main() {
    initializeSensors();
    SensorType[S1] = sensorEV3_GenericI2C;
    wait1Msec(100);

    if (!initSensorMux(msensor_S1_3, colorMeasureColor)) {
        displayString(3, "Error initializing msensor_S1_3");
        return;
    }

    // Get user inputs
    int targetColor = getColorSelection();
    wait1Msec(1000);
    int blockCount = getBlockCount();
    wait1Msec(2000);

    eraseDisplay();
    displayString(1, "Target Color: %d", targetColor);
    displayString(2, "Block Count: %d", blockCount);

    // Record initial position
    nMotorEncoder[motorA] = 0;
    nMotorEncoder[motorC] = 0;
    int initialEncoderA = nMotorEncoder[motorA];
    int initialEncoderC = nMotorEncoder[motorC];

    // Start the block search and collection
    searchAndCollectBlocks(targetColor, blockCount, initialEncoderA, initialEncoderC);
}
