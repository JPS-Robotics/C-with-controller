using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor left_motor_front;
extern motor left_motor_middle;
extern motor left_motor_back;
extern motor_group left_drive_smart;
extern motor right_motor_front;
extern motor right_motor_middle;
extern motor right_motor_back;
extern motor_group right_drive_smart;
extern inertial drivetrain_inertial;
extern smartdrive bot_drivetrain;
extern controller drive_controller;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );