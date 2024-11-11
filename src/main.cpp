/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       jlyne                                                     */
/*    Created:      11/11/2024, 12:21:40 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/
bool initial_drivetrain_calibration_completed = false;

//define variables used for controlling motors based on controller inputs
bool left_drivetrain_engaged = false;
bool right_drivetrain_engaged = false;
bool remote_control_code_enabled = true;

void calibrateDrivetrain() {
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  Brain.Screen.print("Calibrating");
  Brain.Screen.newLine();
  Brain.Screen.print("Inertial");
  drivetrain_inertial.calibrate();
    
  while(drivetrain_inertial.isCalibrating())
  {
    wait(25, msec);
    initial_drivetrain_calibration_completed = true;
  }
        
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1,1);
}

void vexcodeInit() {
  // Calibrate the Drivetrain
  calibrateDrivetrain();
}

void pre_auton(void)
{
  return;
}

int drive_train_task()
{
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true)
  {
    if (remote_control_code_enabled)
    {
      // stop the motors if the brain is calibrating
      if (!initial_drivetrain_calibration_completed)
      {
        left_drive_smart.stop();
        right_drive_smart.stop();
        while(drivetrain_inertial.isCalibrating())
        {
          wait(25, msec);
        }
      }     
      
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = axis3
      // right = axis2
      int drivetrain_left_side_speed = drive_controller.Axis3.position();
      int drivetrain_right_side_speed = drive_controller.Axis2.position();
            
      // check if the value is inside of the deadband range
      if (drivetrain_left_side_speed < 5 && drivetrain_left_side_speed > -5)
      {
        // check if the left motor has already been stopped
        if (left_drivetrain_engaged)
        {
          // stop the left drive motor
          left_drive_smart.stop();
          // tell the code that the left motor has been stopped
          left_drivetrain_engaged = false;
        }
      }
      else
      {
        // reset the toggle so that the deadband code knows to stop the left motor next
        // time the input is in the deadband range
        left_drivetrain_engaged = true;
      }
      
       
      // check if the value is inside of the deadband range
      if (drivetrain_right_side_speed < 5 && drivetrain_right_side_speed > -5)
      {
        // check if the right motor has already been stopped
        if (right_drivetrain_engaged)
        {
          // stop the right drive motor
          right_drive_smart.stop();
          // tell the code that the right motor has been stopped
          right_drivetrain_engaged = false;
        }
      }
      else
      {
        //reset the toggle so that the deadband code knows to stop the right motor next
        // time the input is in the deadband range
        right_drivetrain_engaged = true;
      }
    
      // only tell the left drive motor to spin if the values are not in the deadband range
      if (left_drivetrain_engaged)
      {
        left_drive_smart.setVelocity(drivetrain_left_side_speed, percent);
        left_drive_smart.spin(forward);
      }
      
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (right_drivetrain_engaged)
      {
        right_drive_smart.setVelocity(drivetrain_right_side_speed, percent);
        right_drive_smart.spin(forward);
      }
    }
    
    // wait before repeating the process
    wait(20, msec);
  }

  return 0;
}

task rc_auto_loop_thread_controller_1(drive_train_task);

// "when started" hat block
int whenStarted() {
  return 0;
}

// "when autonomous" hat block
int onauton_autonomous() {
    bot_drivetrain.setDriveVelocity(10, percent);
    bot_drivetrain.turnToHeading(-45, degrees); // Uses the inertia sensor to accuratly turn - Based on starting position of rebot
    bot_drivetrain.driveFor(forward, 40, inches);
    bot_drivetrain.setTurnVelocity(5, percent); // Turn to heading doesn't support precent so set here  (can cacluate based on RPMs and set in each command as desired)
    bot_drivetrain.turnToHeading(-90, degrees);  // Uses the inertia sensor to accuratly turn - Based on starting position of rebot
    bot_drivetrain.driveFor(reverse, 10, inches);
    bot_drivetrain.turnToHeading(-180, degrees);
    bot_drivetrain.driveFor(reverse, 48, inches);
    bot_drivetrain.turnToHeading(-90, degrees);
    bot_drivetrain.driveFor(forward, 48, inches);
  return 0;
}

// "when driver control" hat block
int ondriver_drivercontrol() {
  while (true) {
    wait(5, msec);
  }
  return 0;
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
  remote_control_code_enabled = false;
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
    // Start the auton control tasks....
  vex::task auto0(onauton_autonomous);
  while(Competition.isAutonomous() && Competition.isEnabled())
  {
    this_thread::sleep_for(10);
  }
  
  auto0.stop();
  return;
}

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
  remote_control_code_enabled = true;

  // Start the driver control tasks....
  vex::task drive0(ondriver_drivercontrol);
  while(Competition.isDriverControl() && Competition.isEnabled()) 
  {
    this_thread::sleep_for(10);
  }
  
  drive0.stop();
  return;
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // post event registration

  // set default print color to black
  printf("\033[30m");

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
