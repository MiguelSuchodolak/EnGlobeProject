#include "MessageBuffer.h"

#define MOSI_PIN 23
#define MISO_PIN 19
#define CLK 18
#define CS 5

MessageBuffer::MessageBuffer(){
  Serial.begin(115200);
  if(!SD.begin(5)){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }
  this->fs = &SD;
  this->size = 0;
  this->number = 0;
}

int MessageBuffer::getSize(){
  return this->size;
}

uint64_t MessageBuffer::availableSpace(){
  //Returns available space in SD card in bytes
  return (SD.totalBytes() - SD.usedBytes())/ (1024 * 1024);
}

char* MessageBuffer::readFile(File file){
  //Returns dynamically allocated array

  int rlen = file.available();
  uint8_t buf[rlen];

  file.read(buf, rlen); // Read to buffer

  file.close();

  return (char*)buf;
}

int MessageBuffer::writeFile(const char * path, const char * message){
  /*
  Writes the message to the specified path, which includes the filename
  */
  Serial.println("Writing message");
  File file = fs->open(path, FILE_WRITE);

  if(availableSpace() < 10000){ //If less than 10KB of space is available
    Serial.println("Less than 10KB of available space in SD card.");
    char* errorMessage = "{\n  \"error id:\" 0\n \"error:\" \"Less than 10KB of space in SD card.\"}";
    File errorfile = fs->open("/error.json",FILE_WRITE);
    errorfile.print(errorMessage);
    errorfile.close();
    this->size += 1;
    return 1;
  }else{
    if(!file){ //If failed to open file
      return 2;
    }
    if(file.print(message)){
      this->size += 1;
    } else {
      return 3;
    }
    file.close();
  }
  Serial.println("Finishing writing message");
  return 0;

}

int MessageBuffer::deleteFile(const char * path){
  if(fs->remove(path)){
    this->size -= 1;
    return 0;
  } else {
    return 1;
  }
}

int MessageBuffer::createMessage(float _batteryVolt,float _batteryTemp, float _pressure, float _temperature, float _flow){
  Serial.println("Creating message");
  time_t t = now();

  //int number=33;
  number += 1;

  char name[20];
  sprintf (name, "/%d.json",number);
  char bv[10];
  char bt[10];
  char pr[10];
  char tp[10];
  char fl[10];
  char ts[20] = "00/00/00 00:00:00";

  sprintf (bv, "%d.%05d", (int) _batteryVolt, (_batteryVolt-(int)_batteryVolt)*1000);
  sprintf (bt, "%d.%05d", (int) _batteryTemp, (_batteryTemp-(int)_batteryTemp)*1000);  
  sprintf (pr, "%d.%05d", (int) _pressure, (_pressure-(int)_pressure)*1000);
  sprintf (tp, "%d.%05d", (int) _temperature, (_temperature-(int)_temperature)*1000);
  sprintf (fl, "%d.%05d", (int) _flow, (_flow-(int)_flow)*1000);
  //sprintf (ts, "%d/%d/%d %d:%d:%d",day(t),month(t),year(t),hour(t),minute(t),second(t));
  
  char message[500] = "{\n  \"time\": \"";
  strcat(message, ts);
  strcat(message, "\",\n  \"battery voltage\": ");
  strcat(message, bv);
  strcat(message, ",\n  \"battery temperature\": ");
  strcat(message, bt);
  strcat(message, ",\n  \"pressure\": ");
  strcat(message, pr);
  strcat(message, ",\n  \"temperature\": ");
  strcat(message, tp);
  strcat(message, ",\n  \"flow\": ");
  strcat(message, fl);
  strcat(message,"\n}");

  Serial.println("Finished creating message");
  Serial.println(message);
  return writeFile(name,message);
}

char* MessageBuffer::lastMessage(){
  File root = fs->open("/");

  File file = root.openNextFile();
  if(file){
    char* msg = readFile(file);
    deleteFile(file.path());
    return msg;
  }else{
    return NULL;
  }
}