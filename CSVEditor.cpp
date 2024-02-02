#include "CSVEditor.h"

CSVEditor::CSVEditor(String fileName, int chipSelectPin, bool debug)
    : fileName(fileName), chipSelectPin(chipSelectPin), debug(debug) {
  if (!SD.begin(chipSelectPin)) {
    debugMessage("SD card initialization failed!");
    return;
  }
  debugMessage("SD card initialization done.");
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
  if (openFile(FILE_WRITE)) {
    dataFile.println(data);
    closeFile();
    debugMessage("Data written");
  } else {
    debugMessage("Error opening file to write data");
  }
}

String CSVEditor::readLastRow() {
  String lastLine = "";
  if (openFile(FILE_READ)) {
    while (dataFile.available()) {
      String line = dataFile.readStringUntil('\n');
      if (line.length() > 0) {
        lastLine = line;
      }
    }
    closeFile();
  } else {
    debugMessage("Error opening file to read");
  }
  return lastLine;
}

void CSVEditor::checkAndHandleFileSize() {
  if (openFile(FILE_READ)) {
    size_t fileSize = dataFile.size();
    if (fileSize > MAX_FILE_SIZE) {  // Define MAX_FILE_SIZE as per your needs
      String header = readHeader();
      String lastRow = readLastRow();
      String newFileName = "new_" + fileName;
      dataFile.close();

      // Create new file and write header and last row
      fileName = newFileName;
      writeHeader(header);
      writeData(lastRow);

      // Rename old file
      String oldFileName = fileName;
      int commaIndex = lastRow.indexOf(',');
      if (commaIndex != -1) {
        oldFileName = lastRow.substring(0, commaIndex);
      }
      renameFile(dataFile, fileName, oldFileName);

      // Rename new file to original file name
      renameFile(dataFile, newFileName, fileName);
    }
    closeFile();
  } else {
    debugMessage("Error opening file to check size");
  }
}

String CSVEditor::readHeader() {
  String header = "";
  if (openFile(FILE_READ)) {
    header = dataFile.readStringUntil('\n');
    closeFile();
  } else {
    debugMessage("Error opening file to read header");
  }
  return header;
}

bool CSVEditor::openFile(uint8_t mode) {
  if (dataFile) {
    dataFile.close();
  }
  dataFile = SD.open(fileName, mode);
  return dataFile;
}

void CSVEditor::closeFile() {
  if (dataFile) {
    dataFile.close();
  }
}

bool CSVEditor::renameFile(File &file, const String &oldName, const String &newName) {
  if (SD.exists(newName)) {
    SD.remove(newName);
  }
  file.close();
  file = SD.open(oldName);
  if (!file) {
    return false;
  }
  File newFile = SD.open(newName, FILE_WRITE);
  if (!newFile) {
    return false;
  }
  while (file.available()) {
    newFile.write(file.read());
  }
  newFile.close();
  file.close();
  return SD.remove(oldName);
}

void CSVEditor::debugMessage(const String &message) {
  if (debug) {
    Serial.println(message);
  }
}
