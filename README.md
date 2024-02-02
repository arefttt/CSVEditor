# CSVEditor Library

The CSVEditor library is a powerful and flexible tool for handling CSV files on an SD card. It provides a simple and efficient way to read, write, and manage CSV files in your Arduino projects.

## Features

- **Easy Initialization**: Initialize the library with the filename, chip select pin, and debug mode.
- **Read and Write Operations**: Easily write data or headers to the CSV file. Read the last row or the header of the CSV file.
- **File Size Management**: Automatically checks and handles the file size. If the file size exceeds the maximum limit, it creates a new file, writes the header and last row to the new file, and renames the old and new files.
- **Debugging Support**: If debug mode is enabled, the library will print helpful debug messages to the Serial monitor.

## API Reference

### `CSVEditor(String fileName, int chipSelectPin, bool debug)`

This is the constructor for creating a new CSVEditor object. It takes three parameters:

- `fileName`: The name of the CSV file.
- `chipSelectPin`: The chip select pin for the SD card module.
- `debug`: A boolean indicating whether debug mode is enabled.

### `~CSVEditor()`

This is the destructor for the CSVEditor object. It ensures that the file is closed when the object is destroyed.

### `void writeHeader(String header)`

This function writes a header to the CSV file. It only writes the header if the file does not already exist.

### `void writeData(String data)`

This function writes a string of data to the CSV file. Each call to this function will append the data to a new line in the file.

### `String readLastRow()`

This function reads and returns the last row of data from the CSV file.

### `void checkAndHandleFileSize()`

This function checks the size of the CSV file. If the file size exceeds `MAX_FILE_SIZE`, it creates a new file, writes the header and last row to the new file, and renames the old and new files.

### `String readHeader()`

This function reads and returns the header of the CSV file.

### `bool openFile(uint8_t mode)`

This function opens the CSV file in the specified mode (`FILE_READ` or `FILE_WRITE`). It returns `true` if the file was opened successfully, and `false` otherwise.

### `void closeFile()`

This function closes the CSV file.

### `bool renameFile(File &file, const String &oldName, const String &newName)`

This function renames a file. It takes three parameters: a reference to a `File` object, the old filename, and the new filename. It returns `true` if the file was renamed successfully, and `false` otherwise.

### `void debugMessage(const String &message)`

This function prints a debug message to the Serial monitor if debug mode is enabled.

## Example Usage

```cpp
#include "CSVEditor.h"

CSVEditor csvEditor("data.csv", 10, true);

void setup() {
  Serial.begin(9600);
  csvEditor.writeHeader("Temperature,Humidity");
}

void loop() {
  String data = String(analogRead(A0)) + "," + String(analogRead(A1));
  csvEditor.writeData(data);
  delay(1000);
}
```
