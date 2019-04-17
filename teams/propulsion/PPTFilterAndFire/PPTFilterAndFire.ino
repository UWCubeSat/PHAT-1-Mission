// Note to self: 1st try debouncing, try to make test noise more specific to needs

// Filtering signal variables
int signalPin = 2;
int lastButtonState = LOW;
int buttonState;
int buttonPressed = false;
float voltage;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 200;

// PatternGenerator variables
unsigned long pattern;
unsigned long timeShiftPrev;
byte shiftCount = 0, signalSelect;

// charge and fire ppt variables and constants
int firePin = 5;
int commandPin = 6;
int chargePin = 8;

//reading analog, voltage (from feedback)
int feedbackPin = A0;
int groundReferencePin = A2;

// Not sure about min voltage, chose 1
float minVoltageThreshold = 1.973;
float maxVoltageThreshold = 2.467;
// Put in dummy values for r1 and r2 because I wasn't sure about them
boolean fireCommand;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(signalPin, INPUT);
  pinMode(feedbackPin, INPUT);
  pinMode(groundReferencePin, INPUT);
  pinMode(firePin, OUTPUT);
  pinMode(chargePin, OUTPUT);
  pinMode(commandPin, OUTPUT);
  //set fire command initially to false
  fireCommand=false;
  voltage = 0.0;
}
/* The main computer gives a fireCommand (high/low). This code does nothing
 *  until the main computer gives the command to fire. Once this happens, it 
 *  begins to charge the main capacitor for 4 seconds, reads the value, and 
 *  continues to charge until max threshold is met. It will fire than than
 *  measure the voltage. If it's 0 than it was a successfull fire.
 * 
 */
void loop() {
  // put your main code here, to run repeatedly:
  //patternGenerator(); 
  //charge and fire code in here:
  while(fireCommand){
     chargeToMax();
     digitalWrite(firePin, HIGH); //firePPT
     digitalWrite(firePin, LOW);
     measureVoltage();  //refresh voltage post fire
  }
  debouncingInput();
}

// Checks if input has been stable for a certain number of seconds, 
// which would then reveal that it isn't noise
void debouncingInput(){
  if(buttonPressed){
    //chargeAndFirePPT();
  }
  else{
    int reading = digitalRead(signalPin);
    if(reading != lastButtonState){
      lastDebounceTime = micros();
    }
    if((micros() - lastDebounceTime) > debounceDelay){
      if(reading != buttonState){
        buttonState = reading;
        if(buttonState){
          buttonPressed = true;
        }
      }
    }
    lastButtonState = reading;
  }
}

/*  Measure the voltage of the main capacitor over the voltage divider 
 *   through analog input 0.
 *   0.329 V = 100 V
 *         ...
 *   1.645 V = 500 V
 *         ...
 *   2.467 V = 750 V
 */
void measureVoltage(){
  // read the input on analog pin 0:
  int sensorValue = analogRead(feedbackPin) - analogRead(groundReferencePin);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  double voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
  Serial.println(voltage);
  
}


/* Charge the circuit for four seconds than measure until 
 *  max voltage threshold is reached. * 
 */
void chargeToMax(){
  while(voltage <= maxVoltageThreshold){
    digitalWrite(chargePin, HIGH); //begin charging
    delay(4000); //delay for four seconds
    digitalWrite(chargePin, LOW); //stop charging
    measureVoltage(); //refresh voltage
  }
}


// Creates a signal with some noise, you can ignore this code, it was 
// meant for testing
void patternGenerator(){
  const unsigned long normallyLowSignal = 0x000AF500;
  const unsigned long normallyHighSignal = 0xFFF50AFF;
  const unsigned long shiftInterval = 10000; //100

  unsigned long timeShiftNow = micros();
  if (timeShiftNow - timeShiftPrev > shiftInterval)
  {
    timeShiftPrev += shiftInterval;
    pattern = pattern >> 1;
    shiftCount++;
    if (shiftCount >= 32)
    {
      shiftCount = 0;
      signalSelect++;
      (signalSelect & 5000) ? pattern = normallyHighSignal : pattern = normallyLowSignal;
    }
    digitalWrite(signalPin, pattern & 1);
    int val = digitalRead(signalPin);
    Serial.print(val);
 }
}
