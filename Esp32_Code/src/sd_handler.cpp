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
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){

    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf(" Deleting file: %s\n", path);
    if(fs.remove(path)){

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

const char* GetReadFromFile(fs::FS &fs, const char * path){
    if( path != ERROR_OPENING_FILE ){

        File file = fs.open(path);
        std::vector<char> char_vet;
        
        while(file.available()){
            char_vet.push_back(file.read());
        }
        file.close();

        std::string msgg(char_vet.begin(), char_vet.end());
        return msgg.c_str();

    }
    else{
        Serial.print( "No files in SD" );
    }
    return ERROR_OPENING_FILE;
}
