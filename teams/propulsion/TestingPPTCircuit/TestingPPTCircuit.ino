boolean testingFiltering = false;
boolean testingChargeFire = false;
boolean testingSDCard = false;

void loop() {
  //// Methods need to run at same time.
  //// debouncing Input is an
  //// extra redundant software
  //// signal filter that will ensure
  //// the true state of the signal is 
  //// not changed by noise.
  ///////////////////////////////  
  
  if(testingFiltering){
    debouncingInput();
  }
  
  if(testingChargeFire){
    chargeAndFirePPT();
  }

  //As soon as the PPT starts firing (signalState == HIGH) and a
  //plasma stream is created, the probe arm should extend and 
  //start taking temperatures and writing to the SD card.
  if(testingSDCard){
   if(PPTJustStartedFiring && testingPPT){
      extend();
      PPTJustStartedFiring = false;
    }
    if(PPTFiringNow && testingPPT){
      writingTempDataToSD();
    }
    else{
      writingTempDataTOSD();
    }
  }
  if(signalState == LOW){
    PPTJustStartedFiring = true;
    This temporary variable is needed so that the PPT fires right away the 
    first time it starts firing in its charge-fire cycle (instead of putting
    a space interval at the beggining)
    tempFireSpacingInterval = 0;
    retract();
  }
}
  
