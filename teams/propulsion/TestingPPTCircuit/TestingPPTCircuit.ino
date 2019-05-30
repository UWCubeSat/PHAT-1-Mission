#include <SD.h>
#include <SPI.h>
#include <Stepper.h>
/*
 * 
 * 
 *       Variable          Pin      Purpose
 *       firePin           8        This is a digital output that when high engages the relay for firing PPT.
 *       chargePin         7        This is a digital output that when high turns on a MOSFET that allows the stepup
 *                                  volage from HV generator to turn on, charging the PPT
 *       command           9        This is a digital input that when high, triggers the charging and firing
 *                                  cycle of the PPT
 *                                  
 * 
 * 
 */

// Controls testing states in order to test various systems
boolean testFireState = true; //this becomes false for flight
boolean sdCardState = false;
boolean motorState = false; 
boolean probeState = false;

// Charge and Fire PPT Variables and Constants
int chargePin = 7;
int firePin = 8;
int command = 9;
const double minimumVoltageThreshold = 0.9; //Actual: ??????????
const double maxVoltageThreshold= 1.6; // Actual: 2.5
const int maxChargeTime = 5000; // About 3 seconds recc
const int delayBetweenFires = 6000; // Actual: 60000
const int maxLowVolStalledTimes = 4;

const int noFeedBackFireTime = 5000;

int timesMaxedAtLowVoltage = 0;
boolean firing = false;
boolean fireHasOccured = false;
int startChargeTime;
int elapsedChargeTime = 0;

// Writing to SD Card Variables & Constants
int chipSelect = 10;

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

// Reading Analog Pins Constants & Variables
int feedbackPin = A0;
int groundReferencePin = A1;

void setup(){
  Serial.begin(9600);
  pinMode(feedbackPin, INPUT);
  pinMode(groundReferencePin, INPUT);
  pinMode(firePin, OUTPUT);
  pinMode(chargePin, OUTPUT); 
}

// three seconds
void loop() {
  if(digitalRead(command) || testFireState){
    extend();
    chargeAndFirePPT();
  }
  else {
    retract();
  }
}
