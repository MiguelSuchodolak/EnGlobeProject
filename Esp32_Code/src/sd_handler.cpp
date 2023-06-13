#include <sd_handler.h>

const char* ERROR_OPENING_FILE = "NO_FILE_FOUND";

void setupSdCard(){
    if(!SD.begin()){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s ", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf(" Deleting file: %s ", path);
    if(fs.remove(path)){
        Serial.println(" File Deleted");
    } else {
        Serial.println(" Delete failed");
    }
}

const char* GetAFileName(fs::FS &fs, const char * path){
    if(path){
        File root = fs.open(path);
        if(!root){
            Serial.println("Failed to open directory");
            return ERROR_OPENING_FILE;
        }
        if(!root.isDirectory()){
            Serial.println("Not a directory");
            return ERROR_OPENING_FILE;
        }
        File file = root.openNextFile();
    
        while(file){
            if( !file.isDirectory()){
                return file.name();
            }
            file = root.openNextFile();
        }
    }
    return ERROR_OPENING_FILE;
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

String GetReadFromFile(fs::FS &fs, const char * path){
    if( path != ERROR_OPENING_FILE ){
        File file = fs.open(path);
        if( file ){
            return file.readString();
        }
    }
    else{
        Serial.print( "No files in SD" );
    }
    return ERROR_OPENING_FILE;
}
