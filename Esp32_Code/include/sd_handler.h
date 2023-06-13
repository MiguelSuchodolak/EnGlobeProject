#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <string> // Includes the string library for string operations
#include <string>
#include <algorithm>
#include <vector>

extern const char* ERROR_OPENING_FILE;

void setupSdCard();

void writeFile(fs::FS &fs, const char * path, const char * message);

void deleteFile(fs::FS &fs, const char * path);

const char* GetAFileName(fs::FS &fs, const char *path);

String GetReadFromFile(fs::FS &fs, const char * path);
