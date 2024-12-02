# **Colourific - Robotic Toy Sorting System**

**Developed by**  
- Shivam Walia
- Saswath Yeshwanth   
- Kyle Surendran   
- Om Upadhyay  

---

## **Overview**  
Colourific is a robotic toy-sorting system designed to help children, particularly those with visual impairments or early-stage cognitive development, organize toys by color. Developed as part of the MTE 100/MTE 121 course project, this system leverages LEGO EV3 Mindstorm components to create a safe, efficient, and user-friendly solution.

The robot utilizes sensors for color detection and motors for sorting toys into designated areas. Its child-friendly design ensures safety, modularity, and accessibility, making it suitable for a diverse range of users.

---

## **Features**  
- **Color Detection**: Identifies toy colors using LEGO EV3 sensors.  
- **Block Count Selection**: User-defined sorting for 1 to 9 blocks.  
- **Interactive Controls**: Simple EV3 button-based interface for selecting target colors and block counts.  
- **Child-Friendly Design**: Sturdy structure with rounded edges for safety.  
- **Inclusive Design**: Tailored to assist children with disabilities, promoting accessibility and inclusivity.  

---

## **How It Works**  
1. **Sensor Initialization**:  
   Sensors for touch, color, and IR are set up using the `initializeSensors()` function.  

2. **User Input**:  
   - Target color is selected via the `getColorSelection()` function.  
   - Block count is specified using the `getBlockCount()` function.  

3. **Block Sorting Process**:  
   - The robot searches for blocks using IR sensors and moves toward them.  
   - Blocks are grasped, checked for the correct color, and either sorted or released based on the target color.  
   - Correctly colored blocks are transported to the designated area using IR beacon guidance.  

---

## **Getting Started**  
### **Prerequisites**  
- LEGO EV3 Mindstorm kit.  
- RobotC software.  

### **Setup Instructions**  
1. Clone this repository:  
   ```bash
   git clone https://github.com/your-username/colourific.git
2. Navigate to the mainprogramv3.c file and open the code in RobotC. Remove the .c from the file name
4. Upload the program to your LEGO EV3 Mindstorm robot.  
5. Follow the instructions displayed on the EV3 brick to select the target color and block count.

## **Code Structure**  

- `initializeSensors()`: Sets up and calibrates the sensors.  
- `getColorSelection()`: Allows users to select the target color via EV3 buttons.  
- `getBlockCount()`: Enables users to set the number of blocks to sort.  
- `searchAndCollectBlocks()`: Main routine for searching, grasping, and sorting blocks.  
- `graspBlock()` and `releaseBlock()`: Handles block manipulation.  
- `followIRBeacon()`: Navigates the robot to the designated area using the IR beacon.
  
## **Hardware Requirements**  

- LEGO EV3 Mindstorm kit.  
- EV3 IR sensor.  
- EV3 touch sensor.  
- EV3-compatible multiplexer (e.g., NXT sensor multiplexer).  

## **Future Improvements**  

- Enhance the color detection algorithm for improved accuracy.  
- Add support for additional toy shapes and sizes.  
- Implement advanced navigation techniques for dynamic environments.  

## **Acknowledgments**  

This project was developed as part of the MTE 100/MTE 121 course at the University of Waterloo. Special thanks to our instructors and peers for their guidance and feedback.  

## **Project Images** 
![Robot Image](images/IMG_6213.HEIC)
