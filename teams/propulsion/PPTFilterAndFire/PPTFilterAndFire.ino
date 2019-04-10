// Note to self: 1st try debouncing, try to make test noise more specific to needs

// Filtering signal variables
int signalPin = 2;
int lastButtonState = LOW;
int buttonState;
int buttonPressed = false;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 200;

//test diff for merge on git
int x =1;


// PatternGenerator variables
unsigned long pattern;
unsigned long timeShiftPrev;
byte shiftCount = 0, signalSelect;

// charge and fire ppt variables and constants
int firePin = 5;
int feedbackPin = 7;
int chargePin = 8;
// Not sure about min voltage, chose 1
int minVoltage = 1;
// Put in dummy values for r1 and r2 because I wasn't sure about them
int r1 = 300;
int r2 = 500;
boolean readyToFire;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(signalPin, INPUT);
  pinMode(firePin, OUTPUT);
  pinMode(chargePin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //patternGenerator(); 
  //charge and fire code in here:
  debouncingInput();
}

// Checks if input has been stable for a certain number of seconds, 
// which would then reveal that it isn't noise
void debouncingInput(){
  if(buttonPressed){
    chargeAndFirePPT();
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

// Checks if PPT is ready to fire by checking feedback circuit, and fires
// if ready, charges otherwise
// Question : Is the PPT done firing after it writes a high to the firePin or does it 
// need to be a constant high? Is there ever a point where the ppt needs to be done firing
// and doing nothing? 
void chargeAndFirePPT(){
  // Did I use the voltage divider formula correctly?
  int voltageDividerVal = digitalRead(feedbackPin) * (r2 / (r1 + r2));
  if(voltageDividerVal <= minVoltage){
    readyToFire = false;
  }
  else{
    readyToFire = true;
  }

  if(readyToFire){
    digitalWrite(firePin, HIGH);
  }
  else{
    digitalWrite(chargePin, HIGH);
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
