#include <SD.h>
#include <SPI.h>


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
