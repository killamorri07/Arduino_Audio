#define RECORD_DEBUG //for header usage
#include <Wire.h>
#include <SPI.h>
#include <AutoAnalogAudio.h>
#include <arduino.h>


#define AUDIO_DEBUG
#define RECORD_DEBUG
#define SERIAL_BUFFER_SIZE 256
#define RecLED 13

#include "SdFat.h"
#include "sdios.h"

#define SD_FAT_TYPE 1
#include <AutoAnalogAudio.h>
#include <AutoAnalog_config.h>
AutoAnalog aaAudio;
// SDCARD_SS_PIN is defined for the built-in SD on some boards.
#ifndef SDCARD_SS_PIN
const uint8_t SD_CS_PIN = 4;
#else  // SDCARD_SS_PIN
// Assume built-in SD is used.
const uint8_t SD_CS_PIN = 4;
#endif  // SDCARD_SS_PIN

// Try max SPI clock for an SD. Reduce SPI_CLOCK if errors occur.
#define SPI_CLOCK SD_SCK_MHZ(21)

// Try to select the best SD card configuration.
#if HAS_SDIO_CLASS
#define SD_CONFIG SdioConfig(FIFO_SDIO)
#elif  ENABLE_DEDICATED_SPI
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SPI_CLOCK)
#else  // HAS_SDIO_CLASS
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SPI_CLOCK)
#endif  // HAS_SDIO_CLASS


SdFat32 sd;
File32 wavFile;
AutoAnalog aaAudio;


#include "Wav.h" 
// Create a Serial output stream.
ArduinoOutStream cout(Serial);
void setup() {
  Serial.begin(115200);
  analogWriteResolution(12);
  pinMode(A0, OUTPUT);
  digitalWrite(A0, HIGH);
  pinMode(RecLED, OUTPUT);
  digitalWrite(RecLED, LOW);
  if (!sd.begin(SD_CONFIG)) {
    Serial.println("SD init Failed");
  }
  aaAudio.begin(1, 0);              //Setup aaAudio using ADC only
  aaAudio.autoAdjust = 0;           //Disable auto adjust of timers
  
  //Start loading ADC buffers
 

}

void loop() {
  
}
