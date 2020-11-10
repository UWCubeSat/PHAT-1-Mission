void chargeAndFirePPT(){
  chargeAndFirePPTFeedback();
  //chargeAndFirePPTNoFeedback();
}

void chargeAndFirePPTNoFeedback(){
  //CHARGE
  digitalWrite(chargePin, HIGH);
  delay(maxChargeTime);
  digitalWrite(chargePin, LOW);

  //FIRE
  digitalWrite(firePin, HIGH);
  delay(noFeedBackFireTime);
  digitalWrite(firePin, LOW);

  //WAIT
  digitalWrite(firePin, LOW);
  digitalWrite(chargePin, LOW);
  delay(delayBetweenFires);
}

void chargeAndFirePPTFeedback(){
  if(timesMaxedAtLowVoltage< maxLowVolStalledTimes){
    Serial.println(measureVoltage() <= minimumVoltageThreshold);
    // If it's been in the firing state and above minVol, continue. If freshly fully charged, start firing
    if((firing && (measureVoltage() > minimumVoltageThreshold)) || (measureVoltage() >= maxVoltageThreshold)){
      Serial.print(measureVoltage()); Serial.println(" FIRING");
      firing = true;
      fireHasOccured = true;
      digitalWrite(firePin, HIGH); digitalWrite(chargePin, LOW);
    }
    ////////////////////////////////////////////////////////////////////////////////////
    if(measureVoltage() <= minimumVoltageThreshold || (!fireHasOccured && measureVoltage() <= maxVoltageThreshold)){
      Serial.println("minimumVoltThreshold");
      digitalWrite(firePin, LOW);
      firing = false;
      // If the charge, fire, wait cycle has started, wait a minute after firing
      if(fireHasOccured){
        Serial.println("DELAYING");
        digitalWrite(chargePin, LOW);
        delay(delayBetweenFires);
      }
      // Charge until maximumVoltageThreshold has been reached or maxChargeTime has elapsed
      startChargeTime = millis();
      while((measureVoltage() < maxVoltageThreshold) && (elapsedChargeTime <= maxChargeTime)){
        Serial.print(measureVoltage()); Serial.print(" "); Serial.print(elapsedChargeTime); Serial.println(" CHARGING");
        elapsedChargeTime= millis() - startChargeTime;
        digitalWrite(chargePin, HIGH);
      }
      elapsedChargeTime = 0;
      // If the voltage is less than the minimumVoltageThreshold, a problem must have occured
      // causing a max at a lower voltage 
      if(measureVoltage() < minimumVoltageThreshold){
        Serial.print(measureVoltage());Serial.println(" Maxed at lowVoltage, red flag");
        timesMaxedAtLowVoltage++;
      }
      // Done attempting to charge, will try to fire now;
      firing = true;
      //////////////////////////////////////////////////////////////////////////////////////
    }
  }
  else{
    Serial.println("Shutting down ppt");
    digitalWrite(chargePin, LOW);
    digitalWrite(firePin, LOW);
  }
}
