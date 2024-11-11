#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor left_motor_front = motor(PORT8, ratio6_1, false);
motor left_motor_middle = motor(PORT6, ratio6_1, false);
motor left_motor_back = motor(PORT20, ratio6_1, false);
motor_group left_drive_smart = motor_group(left_motor_front, left_motor_middle, left_motor_back);
motor right_motor_front = motor(PORT4, ratio6_1, true);
motor right_motor_middle = motor(PORT5, ratio6_1, true);
motor right_motor_back = motor(PORT11, ratio6_1, true);
motor_group right_drive_smart = motor_group(right_motor_front, right_motor_middle, right_motor_back);
inertial drivetrain_inertial = inertial(PORT1);
smartdrive bot_drivetrain = smartdrive(left_drive_smart, right_drive_smart, drivetrain_inertial, 299.24, 320, 40, mm, 1);
controller drive_controller = controller();

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}