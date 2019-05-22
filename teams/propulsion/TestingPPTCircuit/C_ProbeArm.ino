#include <Stepper.h>

// Probe Arm Constants & Variables
const int in_1 = 9;
const int in_2 = 10;
const int in_3 = 11;
const int in_4 = 12;
const int steps_per_revolution = 64; // nominal steps per revlution
const int reduction_ratio = 32;
const int motor_speed = 8; // rpm
int real_steps_per_revolution = steps_per_revolution*reduction_ratio;
Stepper s(real_steps_per_revolution, in_1, in_2, in_3, in_4);
boolean PPTJustStartedFiring = true;

void setup(){
  s.setSpeed(motor_speed);
}

void extend() {
  s.step(100*reduction_ratio);
}

void retract() {
  s.step(-100*reduction_ratio);
}
