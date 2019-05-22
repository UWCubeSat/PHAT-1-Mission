

void chargeAndFirePPT(){
    //for now the ppt will be disabled after the first failed charge attempt
    initialTime = millis(); //grabs time stamp
    RED_FLAG = true;
    while(measureVoltage() <= maxVoltageThreshold && RED_FLAG){
      timeElapsed = millis() - initialTime;  
      digitalWrite(chargePin, HIGH);
      if(timeElapsed >= maxTimeToCharge){
        RED_FLAG=false;
      }
    }
    digitalWrite(chargePin, LOW);
    if(measureVoltage() >= maxVoltageThreshold){
      digitalWrite(firePin, HIGH);
      delay(10);
    }
    digitalWrite(firePin, LOW);
}
