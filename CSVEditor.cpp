#include "CSVEditor.h"

CSVEditor::CSVEditor(String fileName, int chipSelectPin, bool debug)
    : fileName(fileName), chipSelectPin(chipSelectPin) {
  CSVEditor::debug = debug;
  initializeSDCard();
}

CSVEditor::~CSVEditor() {
  closeFile();
}

void CSVEditor::writeHeader(String header) {
  if (!SD.exists(fileName)) {
    writeData(header);
  }
}

void CSVEditor::writeData(String data) {
  if (openFile(WRITE)) {
    dataFile.println(data);
    debugMessage("Data written");
    checkAndHandleFileSize();
    closeFile();
  } else {
    debugMessage("Error opening file to write data");
  }
}

String CSVEditor::readLastRow() {
  String lastLine = "";
  if (openFile(READ)) {
    lastLine = getLastLine();
    closeFile();
  } else {
    debugMessage("Error opening file to read");
  }
  return lastLine;
}

void CSVEditor::checkAndHandleFileSize() {
  if (openFile(READ)) {
    size_t fileSize = dataFile.size();
    closeFile();
    if (fileSize > MAX_FILE_SIZE) {
      handleFileSizeExceeded();
    }
  } else {
    debugMessage("Error opening file to check size");
  }
}

String CSVEditor::readHeader() {
  String header = "";
  if (openFile(READ)) {
    header = dataFile.readStringUntil('\n');
    closeFile();
  } else {
    debugMessage("Error opening file to read header");
  }
  return header;
}

void CSVEditor::setDebug(bool debugValue) {
  debug = debugValue;
}

void CSVEditor::initializeSDCard() {
  if (!SD.begin(chipSelectPin)) {
    debugMessage("SD card initialization failed!");
    return;
  }
  debugMessage("SD card initialization done.");
}

bool CSVEditor::openFile(FileMode mode) {
  if (dataFile) {
    dataFile.close();
  }
  dataFile = SD.open(fileName, static_cast<uint8_t>(mode));
  return dataFile;
}

void CSVEditor::closeFile() {
  if (dataFile) {
    dataFile.close();
  }
}

void CSVEditor::debugMessage(String message) {
  if (debug) {
    Serial.println(message);
  }
}

String CSVEditor::getLastLine() {
  String lastLine = "";
  char ch;
  for (int seekPos = dataFile.size() - 1; seekPos >= 0; seekPos--) {
    dataFile.seek(seekPos);
    ch = dataFile.read();
    if (ch == '\n' && lastLine.length() > 0) {
      break;
    }
    lastLine = ch + lastLine;
  }
  return lastLine;
}

void CSVEditor::handleFileSizeExceeded() {
  String header = readHeader();
  String lastRow = readLastRow();
  String newFileName = "new_" + fileName;

  fileName = newFileName;
  writeHeader(header);
  writeData(lastRow);

  String oldFileName = fileName;
  int commaIndex = lastRow.indexOf(',');
  if (commaIndex != -1) {
    oldFileName = lastRow.substring(0, commaIndex);
  }
  SD.rename(fileName, oldFileName);

  SD.rename(newFileName, fileName);
}

bool CSVEditor::debug = false;
