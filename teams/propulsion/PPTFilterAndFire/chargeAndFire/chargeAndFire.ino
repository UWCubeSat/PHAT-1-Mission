

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
