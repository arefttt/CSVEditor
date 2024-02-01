#ifndef CSVEditor_h
#define CSVEditor_h

#include "Arduino.h"
#include <SD.h>

#define MAX_FILE_SIZE 1000000

enum FileMode { READ = FILE_READ, WRITE = FILE_WRITE };

class CSVEditor {
private:
  File dataFile;
  String fileName;
  int chipSelectPin;
  static bool debug;

public:
  CSVEditor(String fileName, int chipSelectPin, bool debug = false);
  ~CSVEditor();

  void writeHeader(String header);
  void writeData(String data);
  String readLastRow();
  void checkAndHandleFileSize();
  String readHeader();
  static void setDebug(bool debugValue);

private:
  void initializeSDCard();
  bool openFile(FileMode mode);
  void closeFile();
  void debugMessage(String message);
  String getLastLine();
  void handleFileSizeExceeded();
};

#endif
