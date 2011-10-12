#define X_DIR_PIN 14   //A0
#define X_STEP_PIN 8

#define Y_DIR_PIN 15    //A1
#define Y_STEP_PIN 17   //A3

#define Z_DIR_PIN 12
#define Z_STEP_PIN 13

#define IDLE 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define IN 5
#define OUT 6

//LCD pins
#define rxPin 16                 // not used but must be defined
#define txPin 18                 // pin 14 is analog pin A4 to Serial LCD

int stepSpeed = 1000;



/****************************************************************************************
 * Here's where you define the overall electronics setup for your machine.
 ****************************************************************************************/

// define the parameters of our machine.
#define X_STEPS_PER_INCH 3600 //416.772354 <- reprap #
#define X_STEPS_PER_MM   141.7323 // 11.7892
#define X_MOTOR_STEPS    400

#define Y_STEPS_PER_INCH 3600 //416.772354 <- reprap #
#define Y_STEPS_PER_MM   141.7323 // 11.0716
#define Y_MOTOR_STEPS    400

#define Z_STEPS_PER_INCH 3600 //16256.0 <- reprap #  // this needs to be changed for half-step mode
#define Z_STEPS_PER_MM   141.7323 //320.0
#define Z_MOTOR_STEPS    400

//our maximum feedrates  << this has not been changed for the itp mill
#define FAST_XY_FEEDRATE 2500.0
#define FAST_Z_FEEDRATE  1000.0

// Units in curve section << this has not been changed for the itp mill
#define CURVE_SECTION_INCHES 0.019685
#define CURVE_SECTION_MM 0.5


#define PACKET_TIMEOUT 500
#define HOST_SERIAL_SPEED 19200
#define SLAVE_SERIAL_SPEED 38400

//uncomment to enable debugging functions
//#define ENABLE_DEBUG 1
//#define ENABLE_COMMS_DEBUG 1

//our RS485 pins
//#define RX_ENABLE_PIN	13
//#define TX_ENABLE_PIN	12

/****************************************************************************************
 * Stepper Driver Behaviour Definition
 ****************************************************************************************/

//do we want a step delay (ie: length of pulse in microseconds) comment out to disable.
//#define STEP_DELAY 5


/****************************************************************************************
 * Various Buffer Size Declarations
 ****************************************************************************************/
//we store all queueable commands in one big giant buffer.
// Explicitly allocate memory at compile time for buffer.
#define COMMAND_BUFFER_SIZE 2048
#define POINT_QUEUE_SIZE 32
#define POINT_SIZE 9
#define MAX_PACKET_LENGTH 32

