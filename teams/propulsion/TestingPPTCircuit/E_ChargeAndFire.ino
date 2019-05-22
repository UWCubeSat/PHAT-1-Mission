// Charge and Fire PPT Variables and Constants
int firePin = 7;
int chargePin = 8; 
float maxVoltageThreshold = 2.5;
double tempFireSpacingInterval;
double fireSpacingInterval = 1000;
double startFireTime = 0;
double lastFireTime = 0;
double fireInterval = 5000;
boolean startingFire = true;
boolean PPTFiringNow = false;

// Reading Analog Pins Constants & Variables
int feedbackPin = A0;
int groundReferencePin = A1;

void setup(){
  pinMode(feedbackPin, INPUT);
  pinMode(groundReferencePin, INPUT);
  pinMode(firePin, OUTPUT);
  pinMode(chargePin, OUTPUT); 
}

void chargeAndFirePPT(){
    if(measureVoltage() <= maxVoltageThreshold){
      digitalWrite(firePin, LOW);
      digitalWrite(chargePin, HIGH);
      PPTFiringNow = false;
    }
    
    //If the fire spacing interval has been reached ...
    else if(millis() - lastFireTime > tempFireSpacingInterval){
      if(startingFire){
        startingFire = false;
        startFireTime = millis(); 
      }
      digitalWrite(chargePin, LOW);
      digitalWrite(firePin, HIGH);
      PPTFiringNow = true;
      // ... fire PPT for specified fire interval and
      // reset fire spacing interval clock to 
      // zero after PPT is done firing
      if(millis() - startFireTime > fireInterval){
        lastFireTime = millis();
        tempFireSpacingInterval = fireSpacingInterval;
        startingFire = true;
      }
    }
    else{
      digitalWrite(firePin, LOW);
      digitalWrite(chargePin, LOW);
      PPTFiringNow = false;
    }
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
