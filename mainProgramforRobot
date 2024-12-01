#include "EV3Servo-lib-UW.c"
#include "UW_sensorMux.c"

void initializeSensors() {
    SensorType[S4] = sensorEV3_Touch;
    wait1Msec(50);
    SensorType[S3] = sensorEV3_IRSensor;
    wait1Msec(50);
    SensorMode[S3] = modeEV3IR_Seeker; // Set IR sensor to Seeker mode
    wait1Msec(50);
    SensorType[S2] = sensorI2CCustom9V;
    wait1Msec(50);
}

// DONE
int getColorSelection() {
    int colorSelection = 1;`
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

// Detects if a block is nearby
bool isBlock() {
    return SensorValue[S3] < 100;
}

void driveToBlock() {
    eraseDisplay();
    displayBigTextLine(4, "Driving to Block");

    while (SensorValue[S3] > 10) {
        motor[motorA] = motor[motorD] = -25;
        motor[motorB] = motor[motorC] = 25;
    }

    motor[motorA] = motor[motorD] = 0;
    motor[motorB] = motor[motorC] = 0;
    wait1Msec(2000);
}

// DONE
bool checkColor(int targetColor) {
    return readMuxSensor(msensor_S1_3) == targetColor;
}

void rotateRobot(int degrees) {
    eraseDisplay();
    displayBigTextLine(4, "Rotating %d°", degrees);

    nMotorEncoder[motorA] = 0;
    int rotationSpeed = (degrees > 0) ? 20 : -20;

    motor[motorA] = motor[motorB] = rotationSpeed;
    motor[motorC] = motor[motorD] = -rotationSpeed;

    wait1Msec(abs(degrees) * 10); // Adjust rotation timing as needed
    motor[motorA] = motor[motorD] = 0;
    motor[motorB] = motor[motorC] = 0;
}

void graspBlock() {
    rotateRobot(310);
    eraseDisplay();
    displayBigTextLine(4, "Grasping Block");

    motor[motorA] = motor[motorD] = 15;
    motor[motorB] = motor[motorC] = -15;

    while (SensorValue[S4] != 1) {}
    wait1Msec(100);

    motor[motorA] = motor[motorD] = 0;
    motor[motorB] = motor[motorC] = 0;

    wait1Msec(1000);
    setServoPosition(S2, 6, 70);
    setServoPosition(S2, 5, -70);
    wait1Msec(1000);
}

// Release the block
void releaseBlock() {
    setServoPosition(S2, 6, 0);
    setServoPosition(S2, 5, 0);
    wait1Msec(5000);
}

// DONE
void followIRBeacon() {
    wait1Msec(50);

    while (true) {
        int heading = getIRBeaconDirection(S3); // Get the beacon's direction (-25 to 25)
        int signalStrength = getIRBeaconStrength(S3); // Adjust index if needed for signal strength

        
        /*if (getIRRemoteButtons(S3) == 0) {
        	motor[motorA] = motor[motorD] = motor[motorB] = motor[motorC] = 0;
        	break;
        }*/
        // Adjust movement based on heading
        if (signalStrength == 1) {
        	motor[motorA] = motor[motorB] = motor[motorC] = motor[motorD] = 0;
        	break;
        }
        if (heading > 0) {
            // Beacon is to the right
            motor[motorA] = -20;  // Right wheels slower
            motor[motorD] = 20;
            motor[motorB] = -20;  // Left wheels faster
            motor[motorC] = 20;
        } else if (heading < 0) {
            // Beacon is to the left
            motor[motorA] = 20;  // Right wheels faster
            motor[motorD] = -20;
            motor[motorB] = 20;  // Left wheels slower
            motor[motorC] = -20;
        } else {
            // Beacon is straight ahead
            motor[motorA] = motor[motorD] = -40; // Move forward
            motor[motorB] = motor[motorC] = 40;
        }

        // Display current status
        eraseDisplay();
        displayBigTextLine(6, "Heading: %d", heading);
        displayBigTextLine(10, "Strength: %d", signalStrength);
        displayString(3, "%d", getIRRemoteButtons(S3));

        wait1Msec(100); // Short delay for smooth updates
    }
}


// Main search and collection routine
void searchAndCollectBlocks(int targetColor, int blockCount) {
    int collectedBlocks = 0;
    while (collectedBlocks < blockCount) {
        if (isBlock()) {
            driveToBlock();
            graspBlock();
            if (checkColor(targetColor)) {
                followIRBeacon();
                releaseBlock();
                collectedBlocks++;
            } else {
                releaseBlock();
                followIRBeacon();
            }
        } else {
            rotateRobot(10); // Rotate to search for a block
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

    // Start the block search and collection
    searchAndCollectBlocks(targetColor, blockCount);
}
