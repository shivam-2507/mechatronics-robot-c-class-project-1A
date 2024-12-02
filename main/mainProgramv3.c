#include "EV3Servo-lib-UW.c"
#include "UW_sensorMux.c"

//initialize all sensors
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

int getColorSelection() {
    //4 menu options so colorSelection variable goes from range 1-4 which is why it is initialized at 1
    int colorSelection = 1;

    //loop continues while enter is not pressed
    while (!getButtonPress(buttonEnter)) {
        //right button positively increments the variable while left negatively increments it 
        if (getButtonPress(buttonRight)) {
            colorSelection++;
            wait1Msec(200); //wait so that the variable is not quickly incremented twice 
        }
        if (getButtonPress(buttonLeft)) {
            colorSelection--;
            wait1Msec(200);
        }

        //ensuring colorSelection stays within the range of 1-4
        if (colorSelection > 4) {
            colorSelection = 1;
        }
        if (colorSelection < 1) {
            colorSelection = 4;
        }

        //erasing display every time a new number is drawn onto it
        eraseDisplay();

        //switch case for all 4 colors
        switch (colorSelection) {
        case 1: displayBigTextLine(4, "Red"); break;
        case 2: displayBigTextLine(4, "Blue"); break;
        case 3: displayBigTextLine(4, "Yellow"); break;
        case 4: displayBigTextLine(4, "Green"); break;
        }
    }

    //erasing the display once a color is selected
   eraseDisplay();

    //if-else statement that returns values tested by the color sensor that represent each color
   if (colorSelection == 1) {
     return 5;
   }
   else if (colorSelection == 2) {
     return 2;
   }
   else if (colorSelection == 3) {
     return 6;
   }
   else {
     return 3;
   }
}

//using the menu once again to get user input for # of blocks
int getBlockCount() {

    //blockcount goes from range 1-9
    int blockCount = 1;

    //loop continues while enter is not pressed
    while (!getButtonPress(buttonEnter)) {

        //same logic as the getColorSelection function
        if (getButtonPress(buttonRight)) {
           if (blockCount < 9) {
               blockCount = blockCount + 1;
           } else {
               blockCount = 9;
           }
           wait1Msec(200);
       }
        if (getButtonPress(buttonLeft)) {
            if (blockCount > 1) {
                blockCount = blockCount - 1;
            } else {
                blockCount = 1;
            }
            wait1Msec(200);
        }

        eraseDisplay();

        //adding text on the screen so user can identify the current blockCount
        displayString(0, "Enter Block Count:");
        displayString(1, "Press buttons (1-9)");
        displayBigTextLine(4, "Count: %d", blockCount);
    }

    //erasing display and returning the blockCount variable to main
    wait1Msec(500);
    eraseDisplay();
    return blockCount;
}

// Detects if a block is nearby and returns bool variable
bool isBlock() {
      if (SensorValue[S3] < 100) {
    return true;
  }
  return false;
}

//drives robot towards block and stops when 10 units away
void driveToBlock() {

    //during every function the display is erased and the robot displays the purpose of the current function (for testing purposes)
    eraseDisplay();
    displayBigTextLine(4, "Driving to Block");

    //the robot moves towards the block until the IR sensor says it is 10 units away
    while (SensorValue[S3] > 10) {
        motor[motorA] = motor[motorD] = -25;
        motor[motorB] = motor[motorC] = 25;
    }

    //stopping the motor and waiting 2 seconds before continuing
    motor[motorA] = motor[motorD] = 0;
    motor[motorB] = motor[motorC] = 0;
    wait1Msec(2000);
}

//checks color and returns true if the color matches target color
bool checkColor(int targetColor) {
      if (readMuxSensor(msensor_S1_3) == targetColor) {
            return true;
  }
  else {
      return false;
  }
}

//function to rotate robot a specific amount of degrees
void rotateRobot(int degrees) {
    eraseDisplay();
    displayBigTextLine(4, "Rotating Robot");

    nMotorEncoder[motorA] = 0;
    int rotationSpeed;
    if (degrees > 0) {
        rotationSpeed = 20;
    } else {
        rotationSpeed = -20;
    }

    //ensuring that the motors on the left side go at the opposite direction as the ones on the right
    motor[motorA] = motor[motorB] = rotationSpeed;
    motor[motorC] = motor[motorD] = -rotationSpeed;

    //pause the program for a duration equal to the absolute amount of degrees * 10 millisections before continuing
    //chose this because the robot needed to stabilize after long rotations, but not shorter ones
    wait1Msec(abs(degrees) * 10);
    motor[motorA] = motor[motorD] = 0;
    motor[motorB] = motor[motorC] = 0;
}

//function that uses servo controls
void graspBlock() {
    long timeStart = nSysTime; // Record the start time
    eraseDisplay();
    displayBigTextLine(4, "Grasping Block");

    //set servo to a wide open position before rotating the robot
    wait1Msec(50);
    setServoPosition(S2, 6, 0);
    setServoPosition(S2, 5, 0);
    wait1Msec(500);

    //305 degrees in our rotateRobot function seemed to be the closest we could get to a 180 degree rotation visibly
    rotateRobot(305);
    wait1Msec(50);

    motor[motorA] = motor[motorD] = 15;
    motor[motorB] = motor[motorC] = -15;

    // Wait until the touch sensor (S4) is triggered or timeout occurs
    while (SensorValue[S4] != 1) {
        if (nSysTime - timeStart > 15000) { // 10-second timeout
            displayBigTextLine(6, "Timeout! Moving on.");
            motor[motorA] = motor[motorD] = 0;
            motor[motorB] = motor[motorC] = 0;
            return; // Exit the function
        }
    }

    wait1Msec(100);

    motor[motorA] = motor[motorD] = 0;
    motor[motorB] = motor[motorC] = 0;

    //after the touch sensor is triggered, the servo motors rotate to grasp the block. 90 and -90 are values gained through testing
    wait1Msec(1000);
    setServoPosition(S2, 6, 90);
    setServoPosition(S2, 5, -90);
    wait1Msec(1000);
}

// Release the block
void releaseBlock() {
    //resetting servo position to release block
    setServoPosition(S2, 6, 0);
    setServoPosition(S2, 5, 0);
    wait1Msec(5000);
}

void followIRBeacon() {
    wait1Msec(50);
    int startTime = nSysTime; // Record the start time in milliseconds
    int elapsedTime = 0;      // Track elapsed time

    while (true) {
        elapsedTime = nSysTime - startTime; // Calculate elapsed time

        // Exit if 20 seconds (20000 ms) have passed
        if (elapsedTime >= 20000) {
            motor[motorA] = motor[motorB] = motor[motorC] = motor[motorD] = 0; // Stop all motors
            break;
        }

        int heading = getIRBeaconDirection(S3); // Get the beacon's direction (-25 to 25)
        int signalStrength = getIRBeaconStrength(S3); // Adjust index if needed for signal strength

        // Stop if the beacon's signal strength is within the desired range
        if (signalStrength < 5 && signalStrength > 1) {
            motor[motorA] = motor[motorB] = motor[motorC] = motor[motorD] = 0;
            break;
        }

        // Adjust movement based on heading
        if (heading > 0) {
            // Beacon is to the right
            motor[motorA] = -20;
            motor[motorD] = 20;
            motor[motorB] = -20;
            motor[motorC] = 20;
        } else if (heading < 0) {
            // Beacon is to the left
            motor[motorA] = 20;
            motor[motorD] = -20;
            motor[motorB] = 20;
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

//this function calls all the other functions related to robot movement (acts as an overall function)
void searchAndCollectBlocks(int targetColor, int blockCount) {
    int collectedBlocks = 0;
    long timeStart = nSysTime; // Record the start time

    //the program runs until the robot has collected blocks a blockCount number of times
    while (collectedBlocks < blockCount) {
        // Exit if 40 seconds have passed since the last block was found
        if (nSysTime - timeStart > 40000) {
            displayBigTextLine(6, "Time's up! Exiting.");
            break;
        }

        //if the isBlock() method identifies a block
        if (isBlock()) {
            timeStart = nSysTime; // Reset the timer when a block is detected

            //drive to the block and grasp it
            driveToBlock();
            graspBlock();

            //if the color matches the target color
            if (checkColor(targetColor)) {

                //hold the block and take it back to the IR beacon (base)
                followIRBeacon();
                releaseBlock();

                //increment the number of collected blocks
                collectedBlocks++;
            } else {

                //if not, release the block, return to base, and attempt to search again
                releaseBlock();
                followIRBeacon();
                wait1Msec(2000);
                rotateRobot(180);
            }
        } else {

            //if the robot cannot even identify a block infront of it, rotate 10 degrees and try again
            rotateRobot(10); // Rotate to search for a block
        }
    }
}

//main function
task main() {

    //initialize the sensors and the mux sensor separately
    initializeSensors();
    SensorType[S1] = sensorEV3_GenericI2C;
    wait1Msec(100);

    //if the mux sensor fails
    if (!initSensorMux(msensor_S1_3, colorMeasureColor)) {
        displayString(3, "Error initializing msensor_S1_3");
        return;
    }

    // Get user inputs
    int targetColor = getColorSelection();
    wait1Msec(1000);
    int blockCount = getBlockCount();
    wait1Msec(2000);

    //print the target color and block count on the screen
    eraseDisplay();
    displayString(1, "Target Color: %d", targetColor);
    displayString(2, "Block Count: %d", blockCount);

    // Start the block search and collection
    searchAndCollectBlocks(targetColor, blockCount);
}
