#include "main.h"
#include "lemlib/api.hpp"

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER); // controller goes crazy
// flywheel
pros::Motor_Group flyWheel({31, 32}); // flywheel ports 10 (left) & 20 (right), keep both positive
// intake motor
pros::Motor intake(30, pros::E_MOTOR_GEAR_GREEN, false); // togglable motor for intake

// drivetrain constuct
pros::Motor left_front_motor(2, pros::E_MOTOR_GEARSET_06, false); // port 1, blue gearbox, not reversed
pros::Motor left_back_motor(1, pros::E_MOTOR_GEARSET_06, false);  // port 2, blue gearbox, not reversed
pros::Motor right_front_motor(21, pros::E_MOTOR_GEARSET_06, true); // port 3, blue gearbox, reversed
pros::Motor right_back_motor(10, pros::E_MOTOR_GEARSET_06, true);  // port 4, blue gearbox, reversed

pros::MotorGroup left_side_motors({left_front_motor, left_back_motor});
pros::MotorGroup right_side_motors({right_front_motor, right_back_motor});

lemlib::Drivetrain_t drivetrain{
    &left_side_motors,  // left drivetrain motors
    &right_side_motors, // right drivetrain motors
    17.5,               // track width
    4.125,              // wheel diameter
    200,                // wheel rpm
    8                   // chasePower
};

// odometry construct
// left tracking wheel encoder
pros::Rotation left_rot(13, false); // port 2, not reversed ---still need to change----
// right tracking wheel encoder
//pros::Rotation right_rot(1, false); // port 1, not reversed
// back tracking wheel encoder
pros::Rotation back_rot(17, false); // port 3, not reversed

// future note on finding tracking center
// left and right are the x" from center of bot, bottom is the y" from bot

// uses "enc" as the encoder. 2.75" wheel diameter, 4.3" offset from tracking center, 1:1 gear ratio
lemlib::TrackingWheel left_tracking_wheel(&left_rot, 2.75, 3.1, 1); // ---still need to change----
// right tracking wheel
// back tracking wheel 
lemlib::TrackingWheel back_tracking_wheel(&back_rot, 2.75, 0.2); // 2.75" wheel diameter, 4.5" offset from tracking center ---still need to change----

// inertial sensor
pros::Imu inertial_sensor(3); // port 2 ---still need to change----

// odometry struct
lemlib::OdomSensors_t sensors{
    &left_tracking_wheel,               //&left_tracking_wheel, // vertical tracking wheel 1
    nullptr, // vertical tracking wheel 2
    &back_tracking_wheel,  // horizontal tracking wheel 1
    nullptr,               // no second horizontal tracking wheel, so we set it to nullptr
    &inertial_sensor       // inertial sensor
};

// PIDs
//  forward/backward PID
lemlib::ChassisController_t lateralController{
    // ---still need to change----
    8,   // kP
    30,  // kD
    1,   // smallErrorRange
    100, // smallErrorTimeout
    3,   // largeErrorRange
    500, // largeErrorTimeout
    5    // slew rate
};

// turning PID
lemlib::ChassisController_t angularController{
    // ---still need to change----
    4,   // kP
    40,  // kD
    1,   // smallErrorRange
    100, // smallErrorTimeout
    3,   // largeErrorRange
    500, // largeErrorTimeout
    0    // slew rate
};

// create the chassis
lemlib::Chassis chassis(drivetrain, lateralController, angularController, sensors);