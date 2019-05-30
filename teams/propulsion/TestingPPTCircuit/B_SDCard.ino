void writingTempDataSD() {
  String dataString = "(";
  dataString += micros(); dataString += ","; dataString += "temp";
  dataString += ")";

  // Open the file,note that only one file can be written to before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }
}
