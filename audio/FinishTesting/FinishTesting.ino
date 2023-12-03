#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"
#include <AutoAnalogAudio.h>

#define SD_FAT_TYPE 1 
// 1 for FAT16/FAT32,
#define AUDIO_DEBUG
#define RECORD_DEBUG

#ifndef SDCARD_SS_PIN
const uint8_t SD_CS_PIN = 53;
#endif
#define SPI_CLOCK SD_SCK_MHZ(50)

const char newWavFile[] = "Tests2.wav";
// Try to select the best SD card configuration.
#if HAS_SDIO_CLASS
#define SD_CONFIG SdioConfig(FIFO_SDIO)
#elif  ENABLE_DEDICATED_SPI
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SPI_HALF_SPEED)
#else  // HAS_SDIO_CLASS
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SPI_HALF_SPEED)
#endif  // HAS_SDIO_CLASS
//------------------------------------------------------------------------------



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
AutoAnalog aaAudio;

#include "myWav.h"
#define error(s) sd.errorHalt(&Serial, F(s))

void setup(){
  Serial.begin(115200);
  if(!sd.begin(SD_CONFIG)){
    Serial.println("initialization failed!");
    sd.initErrorHalt();}
  aaAudio.begin(1,0);
  aaAudio.autoAdjust = 0 ;
  }
uint32_t displayTimer = 0;

void loop(){
 if (millis() - displayTimer > 1000) {
    displayTimer = millis();
    if (counter) {
      Serial.print("Samples per Second: ");
      Serial.println(counter * MAX_BUFFER_SIZE);
    }
    counter = 0;}
  if (Serial.available()) {
    char input = Serial.read();
    switch (input) {
      case '1':  startRecording(newWavFile); break; //Start recording @11khz,8-bit,Mono
      case '2':  stopRecording(newWavFile);  break; //Stop the recording and finalize the file
    }
   }
  }
