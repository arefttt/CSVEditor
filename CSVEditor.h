#ifndef CSVEditor_h
#define CSVEditor_h

#include "Arduino.h"
#include "SD.h"

class CSVEditor {
private:
  File dataFile;
  String fileName;
  int chipSelectPin;
  bool debug;

  bool openFile(uint8_t mode);
  void closeFile();
  bool renameFile(File &file, const String &oldName, const String &newName);
  void debugMessage(const String &message);

public:
  CSVEditor(String fileName, int chipSelectPin, bool debug = false);
  ~CSVEditor();

  void writeHeader(String header);
  void writeData(String data);
  String readLastRow();
  void checkAndHandleFileSize();
  String readHeader();
};

#endif
