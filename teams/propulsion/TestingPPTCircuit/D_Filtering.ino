//void setup(){
//  pinMode(signalPin, OUTPUT);
//  pinMode(readPin, INPUT);
//}

// Filtering Signal Variables & Constants
//int lastSignalState = LOW;
//int signalState = LOW;
//
////int signalHigh = false;
//float voltage;
//unsigned long lastDebounceTime = 0;
//unsigned long debounceDelay = 200;
//boolean signalTest = LOW;
//
//// Test Signal Generator Variables & Constants
//int signalPin = 2;
//unsigned long pattern;
//unsigned long timeShiftPrev;
//byte shiftCount = 0, signalSelect;
//int intervalCount = 0;
//int numberOfBitsOfSignal = 32;
//byte experimentalRightConditionSignal = 4;
//
//
//
//// Method for Filtering Signal
//void debouncingInput(){
//  int reading = digitalRead(readPin);
// 
//  // if consective readings don't match up, then start a debouncing period
//  // at that point so we can wait it out and see if its noise or a signal change
//  if(reading != lastSignalState){
//    lastDebounceTime = micros();
//  }
//  // If its been 200 microseconds, the length of noise 
//  if((micros() - lastDebounceTime) > debounceDelay){
//    if(reading != signalState){
//      signalState = reading;
//    }
//  }
//  lastSignalState = reading;
//  Serial.println(signalState);
//}
//
//
//// Describes the test noise signal used to test debouncingInput(), testing still needs to be done
//// Will not be present in code deployed to Arduino upon flight (replaced by actual signal from main comp)
//void testSignalGenerator() {
//  // Noise lasting about 196 milliseconds and dipping low briefly at ends, should still be registered 
//  // as "HIGH" by debouncing input                       
//  const unsigned long noisySignal =        0b00111111111111111111111111111100;  
//  // 224 milliseconds high signal, should be registered as "HIGH" for debouncing input                           
//  const unsigned long normallyHighSignal = 0b11111111111111111111111111111111;
//  const unsigned long shiftInterval = 7; //3.125
//
//  // Should all scale down to micros for final product, millis() easier to see for testing
//  unsigned long timeShiftNow = micros();
//
//  if (timeShiftNow - timeShiftPrev > shiftInterval)
//  {
//    timeShiftPrev += shiftInterval;
//    pattern = pattern >> 1;
//    shiftCount++;
//    
//    if(shiftCount >= numberOfBitsOfSignal){
//      shiftCount = 0;
//      signalSelect++;
//      (signalSelect & experimentalRightConditionSignal) ? pattern = noisySignal : pattern = normallyHighSignal;
//    }
//    digitalWrite(signalPin, pattern & 1);
//    signalState = digitalRead(readPin);
//    //Serial.println(signalState);
// }
//}
