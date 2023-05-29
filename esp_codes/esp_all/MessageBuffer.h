#ifndef MessageBuffer_h
#define MessageBuffer_h

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <TimeLib.h>
#include <FirebaseJson.h>

class MessageBuffer{
  private:
    fs::FS *fs;
    int size;
    int number;
    
    int writeFile(const char * path, const char * message);
    char* readFile(File file);
    int deleteFile(const char * path);
  public:
    MessageBuffer();
    int getSize();
    uint64_t availableSpace();
    int createMessage(float _batteryVolt,float _batteryTemp,float _pressure, float _temperature, float _flow);
    char* lastMessage();
};

#endif