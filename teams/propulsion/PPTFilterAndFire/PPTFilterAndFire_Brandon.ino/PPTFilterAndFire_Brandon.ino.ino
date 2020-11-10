#include <SD.h>
#include <SPI.h>
#include <Stepper.h>



// Writing to SD Card Variables & Constants
int chipSelect = 10;

// Filtering Signal Variables & Constants
int readPin = 3;
int lastSignalState = LOW;
int signalState = LOW;
//int signalHigh = false;
float voltage;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 200;
boolean signalTest = LOW;

// Test Signal Generator Variables & Constants
int signalPin = 2;
unsigned long pattern;
unsigned long timeShiftPrev;
byte shiftCount = 0, signalSelect;
int intervalCount = 0;
int numberOfBitsOfSignal = 32;
byte experimentalRightConditionSignal = 4;

// Charge and Fire PPT Variables and Constants
int chargePin = 7; 
int firePin = 8;
int command = 9;

float maxVoltageThreshold = 2.14;
double tempFireSpacingInterval;
double fireSpacingInterval = 1000;
double startFireTime = 0;
double lastFireTime = 0;
double fireInterval = 5000;
boolean startingFire = true;
boolean PPTFiringNow = false;

// Reading Analog Pins - Voltages Constants & Variables
int feedbackPin = A0;
int groundReferencePin = A1;

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


void setup() {
  pinMode(signalPin, OUTPUT);
  pinMode(readPin, INPUT);
  pinMode(feedbackPin, INPUT);
  pinMode(groundReferencePin, INPUT);
  pinMode(firePin, OUTPUT);
  pinMode(chargePin, OUTPUT);  
  Serial.begin(9600);
  
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
  }
  s.setSpeed(motor_speed);
}

void loop() {
  //// Methods need to run at same time.
  //// debouncing Input is an
  //// extra redundant software
  //// signal filter that will ensure
  //// the true state of the signal is 
  //// not changed by noise.
  ///////////////////////////////  
  
  if(millis() > 0){
    //debouncingInput();
  }
  
  while(true){
    chargeAndFirePPT();
  }

  // As soon as the PPT starts firing (signalState == HIGH) and a
  // plasma stream is created, the probe arm should extend and 
  // start taking temperatures and writing to the SD card.
  if(millis() > 0 && signalState){
    if(PPTJustStartedFiring){
      extend();
      PPTJustStartedFiring = false;
    }
    if(PPTFiringNow){
      writingTempDataToSD();
    }
  }
  if(signalState == LOW){
    PPTJustStartedFiring = true;
    // This temporary variable is needed so that the PPT fires right away the 
    // first time it starts firing in its charge-fire cycle (instead of putting
    // a space interval at the beggining)
    tempFireSpacingInterval = 0;
    retract();
  }
}

 void chargeAndFirePPT(){
      while(measureVoltage()<= maxVoltageThreshold){
        Serial.println("Charging...");
        digitalWrite(chargePin, HIGH);
        delay(2000);
      } 
      digitalWrite(firePin, HIGH);
      delay(50);
      digitalWrite(firePin, LOW);
      delay(50);     
      Serial.println("waiting...");
      digitalWrite(chargePin, LOW);
      delay(8000);
}


// feedback telling PPT when it needs to start charging
double measureVoltage(){
  int sensorValue = abs(analogRead(feedbackPin) - analogRead(groundReferencePin));
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  double voltage = sensorValue * (5.0 / 1023.0);
  //Serial.print("VOLTAGE ");
  //Serial.println(voltage);
  return voltage; 
}

void writingTempDataToSD(){
  String dataString = "("; 
  dataString += micros(); dataString += ","; dataString += "temp";
  dataString += ")";

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }
}

void extend() {
  s.step(100*reduction_ratio);
}

void retract() {
  s.step(-100*reduction_ratio);
}

// Method for Filtering Signal
void debouncingInput(){
  int reading = digitalRead(readPin);
 
  // if consective readings don't match up, then start a debouncing period
  // at that point so we can wait it out and see if its noise or a signal change
  if(reading != lastSignalState){
    lastDebounceTime = micros();
  }
  // If its been 200 microseconds, the length of noise 
  if((micros() - lastDebounceTime) > debounceDelay){
    if(reading != signalState){
      signalState = reading;
    }
  }
  lastSignalState = reading;
  //Serial.println(signalState);
}


// Describes the test noise signal used to test debouncingInput(), testing still needs to be done
// Will not be present in code deployed to Arduino upon flight (replaced by actual signal from main comp)
void testSignalGenerator() {
  // Noise lasting about 196 milliseconds and dipping low briefly at ends, should still be registered 
  // as "HIGH" by debouncing input                       
  const unsigned long noisySignal =        0b00111111111111111111111111111100;  
  // 224 milliseconds high signal, should be registered as "HIGH" for debouncing input                           
  const unsigned long normallyHighSignal = 0b11111111111111111111111111111111;
  const unsigned long shiftInterval = 7; //3.125

  // Should all scale down to micros for final product, millis() easier to see for testing
  unsigned long timeShiftNow = micros();

  if (timeShiftNow - timeShiftPrev > shiftInterval)
  {
    timeShiftPrev += shiftInterval;
    pattern = pattern >> 1;
    shiftCount++;
    
    if(shiftCount >= numberOfBitsOfSignal){
      shiftCount = 0;
      signalSelect++;
      (signalSelect & experimentalRightConditionSignal) ? pattern = noisySignal : pattern = normallyHighSignal;
    }
    digitalWrite(signalPin, pattern & 1);
    signalState = digitalRead(readPin);
    //Serial.println(signalState);
 }
}
