// Function Prototypes
#include "EV3Servo-lib-UW.c"

// Function to get color selection from user
int getColorSelection() {
    int colorSelection = 1;

    while (!getButtonPress(buttonEnter)) {
        if (getButtonPress(buttonRight)) {
            colorSelection += 1;
        }
        if (getButtonPress(buttonLeft)) {
            colorSelection -= 1;
            eraseDisplay();
        }

        if (colorSelection > 6) {
            colorSelection = 1;
        }
        if (colorSelection < 1) {
            colorSelection = 6;
        }

        eraseDisplay();
        switch (colorSelection) {
            case 1:
                displayString(1, "Red");
                break;
            case 2:
                displayString(1, "Blue");
                break;
            case 3:
                displayString(1, "Yellow");
                break;
            case 4:
                displayString(1, "Green");
                break;
            default:
                displayString(1, "Invalid");
                break;
        }
    }

    if (colorSelection < 1 || colorSelection > 6) {
        displayString(7, "Invalid selection.");
        wait1Msec(1000);
        eraseDisplay();
    }

    // Map selection to color sensor values
    switch (colorSelection) {
        case 1:
            return (int)colorRed;
        case 2:
            return (int)colorBlue;
        case 3:
            return (int)colorYellow;
        case 4:
            return (int)colorGreen;
        default:
            return -1;  // Fallback, should never occur
    }
}

// Drives towards a detected block using IR sensor
bool driveToBlock() {
    while (SensorValue[IRSensor] < 5) {  // Adjust threshold as needed
        motor[leftMotor] = 50;
        motor[rightMotor] = 50;
    }

    motor[leftMotor] = 0;
    motor[rightMotor] = 0;

    // Verify if block is really there (e.g., check touch sensor)
    return SensorValue[touchSensor] == 1;
}

task main() {
    int targetColor = getColorSelection();  // Get color from user
    eraseDisplay();
    displayString(1, "%d", targetColor);

    // Drive towards the block
    if (driveToBlock()) {
        displayString(2, "Block touched!");
    }
}