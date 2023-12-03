#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"
#define SD_FAT_TYPE 1 
// 1 for FAT16/FAT32,

const int8_t DISABLE_CHIP_SELECT = -1;

// Try to select the best SD card configuration.
#if HAS_SDIO_CLASS
#define SD_CONFIG SdioConfig(FIFO_SDIO)
#elif  ENABLE_DEDICATED_SPI
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SPI_HALF_SPEED)
#else  // HAS_SDIO_CLASS
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SPI_HALF_SPEED)
#endif  // HAS_SDIO_CLASS
//------------------------------------------------------------------------------

int AudioPin = A0;
#ifndef SDCARD_SS_PIN
const uint8_t SD_CS_PIN = 53;
#endif
#define SPI_CLOCK SD_SCK_MHZ(50)

//------------------------------------------------------------------------------

#if SD_FAT_TYPE == 0
SdFat sd;
File myFile;
File root;
#elif SD_FAT_TYPE == 1
SdFat32 sd;
File32 myFile;
File32 root;
#elif SD_FAT_TYPE == 2
SdExFat sd;
ExFile myFile;
ExFile root;
#elif SD_FAT_TYPE == 3
SdFs sd;
FsFile myFile;
FsFile root;
#endif  // SD_FAT_TYPE

#define error(s) sd.errorHalt(&Serial, F(s))

void setup(){
  Serial.begin(115200);
//  analogRead(AudioPin);
  if(!sd.begin(SD_CONFIG)){
    Serial.println("initialization failed!");
    sd.initErrorHalt();}

   myFile = sd.open("test.txt", O_RDWR | O_CREAT | O_AT_END);
   if(myFile){
    Serial.print("Writing to test.txt");
    myFile.println("Testing 1,2,3");
    // closing
    myFile.close();
    Serial.println("done");
    }
    else{
      Serial.println("error opening and writing .txt");
      }
  }

void loop(){
  }
