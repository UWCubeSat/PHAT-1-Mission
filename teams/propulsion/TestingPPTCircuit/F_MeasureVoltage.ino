
// feedback telling PPT when it needs to start charging
double measureVoltage(){
  int sensorValue = abs(analogRead(feedbackPin) - analogRead(groundReferencePin));
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  double voltage = sensorValue * (5.0 / 1023.0);
  //Serial.print("VOLTAGE ");
  //Serial.println(voltage);
  return voltage; 
}
