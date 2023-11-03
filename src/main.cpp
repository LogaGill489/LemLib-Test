#include "main.h"
#include "lemlib/api.hpp"
#include "variables.hpp"

void screen() {
    // loop forever
    while (true) {
        lemlib::Pose pose = chassis.getPose(); // get the current position of the robot
        pros::lcd::print(0, "x: %f", pose.x); // print the x position
        pros::lcd::print(1, "y: %f", pose.y); // print the y position
        pros::lcd::print(2, "heading: %f", pose.theta); // print the heading
        pros::delay(10);
    }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate the chassis
    pros::Task screenTask(screen); // create a task to print the position to the screen
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
ASSET(path_txt);
ASSET(testPath_txt);

void autonomous() {
 //chassis.follow(testPath_txt, 2000, 15);
 controller.set_text(1, 1, "auton called");

 chassis.moveTo(5, 10, 90, 4000, true); // move asynchronously
//chassis.waitUntilDist(5); // wait until the chassis has travelled 5 inches on the given route
//intake.move(127); // spin the intake
//chassis.waitUntilDist(5);
//intake.move(0);

//chassis.moveTo();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	while (true) {
// variable for # of motors per side
		const int sideMotors = 2;

		// get stick values
		int power = controller.get_analog(ANALOG_LEFT_Y);
		int turn = controller.get_analog(ANALOG_RIGHT_X);

		// calculate left and right values
		int left = power - turn; // overload of 127, range of -127 - 127 in watts
		int right = power + turn;

		// move motors
		left_side_motors.move(power - turn);
		right_side_motors.move(power + turn);

		// intake
		if (controller.get_digital(DIGITAL_R2))
		{
			intake = 127;
		}
		else if (controller.get_digital(DIGITAL_L2))
		{
			intake = -127;
		}
		else
		{
			intake = 0;
		}

		// cata
		if (controller.get_digital_new_press(DIGITAL_A))
		{
			flyWheel.move(127);
		}
		else if (controller.get_digital_new_press(DIGITAL_B))
		{
			flyWheel.move(0);
		}

		// print temp and power to
		controller.set_text(1, 1, "mh = " + std::to_string(static_cast<int>(left_front_motor.get_temperature())) + "°C  p = " + std::to_string(power));

		// pros::screen::print(pros::E_TEXT_MEDIUM, 3, "Power = %li", power);

		/*
		// X-Drive Code
		pros::Motor_Group M1(1); // Top left motor
		pros::Motor_Group M2(1); // Bottom left motor
		pros::Motor_Group M3(1); // Top right motor
		pros::Motor_Group M4(1); // bottom right motor

		M1 = master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_LEFT_X) - master.get_analog(ANALOG_RIGHT_X);

		M2 = master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_LEFT_X) + master.get_analog(ANALOG_RIGHT_X);

		M3 = master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_LEFT_X) + master.get_analog(ANALOG_RIGHT_X);

		M4 = master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_LEFT_X) - master.get_analog(ANALOG_RIGHT_X);
		*/
	}
}
