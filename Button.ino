#include <AccelStepper.h>

// Define the stepper motor pins
#define Pin1 8 // IN1 is connected to 10
#define Pin2 9 // IN2 is connected to 11
#define Pin3 10 // IN3 is connected to 12
#define Pin4 11 // IN4 is connected to 13
#define switchCW 3 // define input pin for CW push button
#define switchCCW 4 // define input pin for CCW push button

// Create an instance of the AccelStepper class
AccelStepper stepper(AccelStepper::FULL4WIRE, Pin1, Pin3, Pin2, Pin4);

// Variable to store direction status
int dirStatus = 3; // 3= stop

void setup() {
  // Set up the input pins
  pinMode(switchCW, INPUT_PULLUP);
  pinMode(switchCCW, INPUT_PULLUP);

  // Set the maximum speed and acceleration
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);
}

void loop() {
  // Read the push buttons and update direction status
  if(digitalRead(switchCCW) == LOW) {
    dirStatus = 1;
  } else if(digitalRead(switchCW) == LOW) {
    dirStatus = 2;
  } else {
    dirStatus = 3;
  }

  // Control the stepper motor based on direction status
  switch(dirStatus) {
    case 1:
      stepper.moveTo(stepper.currentPosition() + 1);
      break;
    case 2:
      stepper.moveTo(stepper.currentPosition() - 1);
      break;
    default:
      stepper.stop(); // Stop the motor
      break;
  }

  // Run the stepper motor to the new position
  stepper.run();
  delay(1);
}
