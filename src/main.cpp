/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----      
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// RearLeft             motor         11              
// RearRight            motor         20              
// CenterLeft           motor         12              
// CenterRight          motor         19              
// FrontLeft            motor         1               
// FrontRight           motor         10              
// Controller1          controller                    
// leftWing             motor         15     
// rightWing            motor         4         
// ---- END VEXCODE CONFIGURED DEVICES ----
#include <iostream>

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
// Movement Motor Variable Declarations
int deadzone;
double controllerYAxis;
double controllerXAxis;
double leftVelocity;
double rightVelocity;
bool openWing;
bool closeWing;


// driver control function Declarations
void setMotorVelocities(double velocityAmountLeft, double velocityAmountRight);
void spinMotors();
void stopMotors();
void spinIntake();
void stopIntake();
void spinClawOut();
void stopClawSpin();
void setWingVelocity(double velocityAmount);
int openWings();
int closeWings();

// Autonomous Variable Declarations
bool resetOrientation;

double KP = 0.0;
double KI = 0.0;
double KD = 0.0;  

double TURNKP = 0.0;
double TURNKI = 0.0;
double TURNKD = 0.0;  

int error; // SensorValue - desiredValue : positional value (deltaX)
int prevError = 0; // Position 20 miliseconds ago
int derivative; // error - prevError : speed
int totalError = 0; // total error = totalError += error

int turnError; // SensorValue - desiredValue : positional value (deltaX)
int turnPrevError = 0; // Position 20 miliseconds ago
int turnDerivative; // error - prevError : speed
int turnTotalError = 0; // total error = totalError += error

// autonomous function declarations
void spinMotorsFor(double distanceAmount);
void spinMotorsTurn(double distanceAmount);
void readyBotToGoal();
void drivePID();

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/



void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  // Autonomous Code, write your code sequentially to perform actions.
  setMotorVelocities(100,100);
  setWingVelocity(100);

  task myTask = task(openWings);
  spinMotors();
  wait(1000,msec);
  stopMotors();
  myTask.stop();
  

}
                                      // PID CODE
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// bool enablePID = true;
// double desiredPosition = 200;
// double desiredTurning = 0;
// Auton Functions
// void drivePID()
// {
//   while(enablePID)
//   {
//     // Lateral PID
//     double leftMotorPosition = CenterLeft.position(degrees);
//     double rightMotorPosition = CenterRight.position(degrees);
//     // average of two variables
//     double averagePosition = (leftMotorPosition + rightMotorPosition) / 2;

//     // Potential
//     error = averagePosition - desiredPosition; 
//     // Derivative
//     derivative = error - prevError;

//     // Velocity -> position -> Absement
//     // totalError += error;

//     double lateralMotorPower = (error * KP + derivative * KD);


//     // Turning PID
    

//     double turnDifference = (leftMotorPosition - rightMotorPosition) / 2;
//     turnError = turnDifference - desiredPosition; 
//     turnDerivative = turnError - turnPrevError;

//     // turnTotalError += turnError;

//     double turnMotorPower = (turnError * TURNKP + turnDerivative * TURNKD);

//     ////////////////////////////////////////////////////////////////////////////////
//     CenterLeft.spinFor(forward, lateralMotorPower + turnMotorPower,degrees);

//     prevError = error;
//     turnPrevError = turnError;
//     task::sleep(20);

//   }
// }
////////////////////////////////////////////////////////////////////////////////////////////////////////


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // Controller Variables
    deadzone = 5;
    controllerYAxis = Controller1.Axis3.position(percent);
    controllerXAxis = Controller1.Axis1.position(percent);

    // Wing Expansion Bools
    openWing = Controller1.ButtonL1.pressing();
    closeWing = Controller1.ButtonL2.pressing();
    leftWing.setVelocity(100,percent);
    rightWing.setVelocity(100,percent);
    
    // Velocities of said variables
    leftVelocity = controllerYAxis + controllerXAxis;
    rightVelocity = controllerYAxis - controllerXAxis;
    setMotorVelocities(leftVelocity, rightVelocity);
    

    // Movement Code
    if(fabs(controllerYAxis) > deadzone || fabs(controllerXAxis) > deadzone)
    {
      spinMotors();
    }
    else
    {
      stopMotors();
    }
    if(openWing)
    {
      openWings();
    }
    else if(closeWing)
    {
      closeWings();
    }
    else
    {
      leftWing.stop();
      rightWing.stop();
    }
    

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}

void setMotorVelocities(double leftValue, double rightValue)
{
  RearLeft.setVelocity(leftValue, percent);
  RearRight.setVelocity(rightValue, percent);
  CenterLeft.setVelocity(leftValue, percent);
  CenterRight.setVelocity(rightValue, percent);
  FrontLeft.setVelocity(leftValue, percent);
  FrontRight.setVelocity(rightValue, percent);
}

void spinMotors()
{
  RearLeft.spin(forward);
  RearRight.spin(forward);
  CenterLeft.spin(forward);
  CenterRight.spin(forward);
  FrontLeft.spin(forward);
  FrontRight.spin(forward);
}
void spinMotorsFor(double distanceAmount)
{
  RearRight.spinFor(forward, distanceAmount, degrees);
  CenterLeft.spinFor(forward, distanceAmount, degrees);
  CenterRight.spinFor(forward, distanceAmount, degrees);
  FrontLeft.spinFor(forward, distanceAmount, degrees);
  FrontRight.spinFor(forward, distanceAmount, degrees);
}
void spinMotorsTurn(double distanceAmount)
{
  RearLeft.spinFor(forward, distanceAmount,degrees);
  RearRight.spinFor(reverse, distanceAmount, degrees);
  CenterLeft.spinFor(forward, distanceAmount, degrees);
  CenterRight.spinFor(reverse, distanceAmount, degrees);
  FrontLeft.spinFor(forward, distanceAmount, degrees);
  FrontRight.spinFor(reverse, distanceAmount, degrees);
}
void setWingVelocity(double velocityAmount)
{
  leftWing.setVelocity(velocityAmount,percent);
  rightWing.setVelocity(velocityAmount,percent);
}
int openWings()
{
  leftWing.spin(forward);
  rightWing.spin(forward);
  return 0;
}
int closeWings()
{
  leftWing.spin(reverse);
  rightWing.spin(reverse);

  return 0;
}

void stopMotors()
{
  RearLeft.stop();
  RearRight.stop();
  CenterLeft.stop();
  CenterRight.stop();
  FrontLeft.stop();
  FrontRight.stop();
}


// Autonomous Functions
void readyBotToGoal()
{
  setMotorVelocities(100,100);
  int distanceAmount = 1000;
  RearLeft.spinFor(forward, distanceAmount,degrees);
  RearRight.spinFor(forward, distanceAmount, degrees);
  CenterLeft.spinFor(forward, distanceAmount, degrees);
  CenterRight.spinFor(forward, distanceAmount, degrees);
  FrontLeft.spinFor(forward, distanceAmount, degrees);
  FrontRight.spinFor(forward, distanceAmount, degrees);  // spinMotorsTurn(300);
  // spinMotorsFor(1000);
  // spinMotorsTurn(-300);

  // Repeat Spin
}



// Tasks for Motors
