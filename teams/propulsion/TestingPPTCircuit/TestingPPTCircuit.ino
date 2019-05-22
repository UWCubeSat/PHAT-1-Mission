#include  <SD.h>
#include  <SPI.h>
#include <Stepper.h>
/*
 * 
 * 
 *       Variable          Pin      Purpose
 *        firePin          7        This is a digital output that when high engages the relay for firing PPT.
 *        chargePin        8        This is a digital output that when high turns on a MOSFET that allows the stepup
 *                                  volage from HV generator to turn on.
 *        command
 * 
 * 
 */
 
float maxVoltageThreshold = 2.5;
int initialTime;
int timeElapsed;
double maxTimeToCharge = 5000;
boolean RED_FLAG;

// Charge and Fire PPT Variables and Constants
int firePin = 7;
int chargePin = 8; 
int command = 9;

// Writing to SD Card Variables & Constants
int chipSelect = 10;

//these variables control the states in order to broaden the capabilies of the microcontroller.
boolean testFireState = true; //this becomes false for flight
boolean sdCardState = false;
boolean motorState = false; 
boolean probeState = false;

// Probe Arm Constants & Variables
const int in_1 = 9;
const int in_2 = 10;
const int in_3 = 11;
const int in_4 = 12;
const int steps_per_revolution = 64; // nominal steps per revlution
const int reduction_ratio = 32;
const int motor_speed = 8; // rpm

// Reading Analog Pins Constants & Variables
int feedbackPin = A0;
int groundReferencePin = A1;

int real_steps_per_revolution = steps_per_revolution*reduction_ratio;
Stepper s(real_steps_per_revolution, in_1, in_2, in_3, in_4);

void setup(){
    pinMode(feedbackPin, INPUT);
    pinMode(groundReferencePin, INPUT);
    pinMode(firePin, OUTPUT);
    pinMode(chargePin, OUTPUT); 
    if (!SD.begin(chipSelect) && sdCardState) {
      Serial.println("Card failed, or not present");
    }
    if(probeState){
      //this setups the probe arm for future use
      s.setSpeed(motor_speed);
    } 
}

void loop() {
  while(command || testFireState){
    chargeAndFirePPT();
    //one minute delay
    delay(1000*6);
  }


}

//  if(testingFiltering){
//    debouncingInput();
//  }
//  
//  if(testingChargeFire){
//    chargeAndFirePPT();
//  }
//
//  //As soon as the PPT starts firing (signalState == HIGH) and a
//  //plasma stream is created, the probe arm should extend and 
//  //start taking temperatures and writing to the SD card.
//  if(testingSDCard){
//   if(PPTJustStartedFiring && testingPPT){
//      extend();
//      PPTJustStartedFiring = false;
//    }
//    if(PPTFiringNow && testingPPT){
//      writingTempDataToSD();
//    }
//    else{
//      writingTempDataTOSD();
//    }
//  }
//  if(signalState == LOW){
//    PPTJustStartedFiring = true;
//    This temporary variable is needed so that the PPT fires right away the 
//    first time it starts firing in its charge-fire cycle (instead of putting
//    a space interval at the beggining)
//    tempFireSpacingInterval = 0;
//    retract();
//  }
//}
//  
