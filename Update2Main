#include "EV3Servo-lib-UW.c"

// Function Prototypes
int getColorSelection();
int getBlockCount();
bool driveToBlock();
bool checkColor(int targetColor);
void graspBlock();
void releaseBlock();
void returnToOriginalPosition();
void searchAndCollectBlocks(int targetColor, int blockCount);

// Global Variables
int leftMotorStartPosition = 0;
int rightMotorStartPosition = 0;

// Gyro Initialization
void initializeSensors() {
    //SensorType[C1] = sensorEV3_Gyro;
    wait1Msec(50);
    //SensorMode[S4] = modeEV3Gyro_Calibration;
    wait1Msec(50);
    //SensorMode[S4] = modeEV3Gyro_Angle;
    wait1Msec(50);
    //SensorType[S1] = sensorEV3_Color;
    wait1Msec(50);
    SensorType[S1] = sensorEV3_Touch;
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
            case 1: displayString(1, "Red"); break;
            case 2: displayString(1, "Blue"); break;
            case 3: displayString(1, "Yellow"); break;
            case 4: displayString(1, "Green"); break;
        }
    }
    eraseDisplay();
    return colorSelection;
}

// DONE
int getBlockCount() {
    int blockCount = 1;
    displayString(0, "Enter Block Count:");
    displayString(1, "Press buttons (1-9)");
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
        displayString(1, "Count: %d", blockCount);
    }
    wait1Msec(500); // Debounce delay
    eraseDisplay();
    return blockCount;
}

// Drives towards a detected block using IR sensor
bool driveToBlock() {
    motor[motorA] = motor[motorB] = 50;
    while (SensorValue[S3] < 5) {} // Adjust threshold based on environment
    motor[motorA] = 50;
    motor[motorB] = -50;
    while (abs(getGyroDegrees(C1)) < 180) {}
    motor[motorA] = motor[motorB] = 0;

    // Verify if block is detected (e.g., check touch sensor)
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
            }
        }
    }
}

task main() {
    initializeSensors();

    leftMotorStartPosition = nMotorEncoder[motorA];
    rightMotorStartPosition = nMotorEncoder[motorB];

    int targetColor = getColorSelection();
    wait1Msec(2000);
    int blockCount = getBlockCount();

    eraseDisplay();
    displayString(1, "Target Color: %d", targetColor);
    displayString(2, "Block Count: %d", blockCount);

    searchAndCollectBlocks(targetColor, blockCount);
}
