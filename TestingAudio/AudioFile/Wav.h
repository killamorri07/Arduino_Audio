struct wavStruct {
  const char chunkID[4] = {'R', 'I', 'F', 'F'}; 
  /// 36 + SubChunk2Size
  uint32_t chunkSize = 36;                    
 // You Don't know this until you write your data but at a minimum it is 36 for an empty file
//Size of (entire file in bytes - 8 bytes) or (data size + 36)
  const char format[4] = {'W', 'A', 'V', 'E'};
  /// "should be characters "WAVE"
  /// " This should be the letters "fmt ", note the space character
  const char subchunkID[4] = {'f', 'm', 't', ' '};
  const uint32_t subchunkSize = 16; ///: For PCM == 16, since audioFormat == uint16_t
  const uint16_t audioFormat = 1;              //PCM == 1
  uint16_t numChannels = 1;                    //1=Mono, 2=Stereo
  uint32_t sampleRate = 16000;  ///: Sample Rate of file
  uint32_t byteRate = 16000 ;                   //== SampleRate * NumChannels * BitsPerSample/8
  uint16_t blockAlign = 1;                     //== NumChannels * BitsPerSample/8
  uint16_t bitsPerSample = 8;                  //selection 8,16,32...
  const char subChunk2ID[4] = {'d', 'a', 't', 'a'};
  uint32_t subChunk2Size = 0;        // You Don't know this until you write your data       
};



///https://stackoverflow.com/questions/66484763/how-to-convert-analog-input-readings-from-arduino-to-wav-from-sketch

void ADC_Handler(void) {
  //Buffer Size 0 to 255
  aaAudio.getADC(SERIAL_BUFFER_SIZE);
  if (wavFile) {
    #if defined(ARDUINO_SAM_DUE)
    for (int i = 0; i < SERIAL_BUFFER_SIZE; i++) {
      aaAudio.adcBuffer[i] >> 4;
    }
   #endif
  wavFile.write(aaAudio.adcBuffer, SERIAL_BUFFER_SIZE);//Write the data to SD as it is available

  }
}

void Recording(const char *fileName, uint32_t sampleRate) {
  if(wavFile){
    aaAudio.adcInterrupts(false); //0ff for Analog interrupt
    wavFile.close(); //Closing the file has the effect of flushing the buffered data to the SD card
    }
//  int rootFileCount = 0;
//  if (!root.open("/")) {
//    error("open root"); }
//  if (!sd.mkdir("Folder1")) {
//    error("Create Folder1 failed");
//    cout << F("Created Folder1\n");
//  }
  dataFile = sd.open(FileName, O_CREAT | O_WRITE); //Open the file for writing
  if (!dataFile) {
    Serial.println("opening SD data failed"); //double check on file access
    sd.initErrorHalt();
  }
  wavFile.seek(0) //Write a blank Wav header
  uint8_t blank = 0;
  for (int i = 0; i < 44; i++) {
    wavFile.write(blank);
  }
  aaAudio.adcBitsPerSample = 8; //Configure AAAudio
  aaAudio.setSampleRate(sampleRate);
  aaAudio.getADC();
  aaAudio.adcInterrupts(true);
}


void writeWavHeader(const char *fileName, unt32_t sampleRate){
  if(!sd.exists(fileName)){
    #if defined (RECORD_DEBUG)
        Serial.println("File does not exist, please write WAV/PCM data starting at byte 44");
        sd.initErrorHalt();
    #endif
    return; }
    if (wavFile.size() <= 44) { //44 is the file offset in bytes
  #if defined (RECORD_DEBUG)
    Serial.println("File contains no data, exiting");
  #endif
    wavFile.close();
    return;
  }
  wavStruct wavHeader;
  wavHeader.chunkSize = recFile.size() - 8;
  //wavHeader.numChannels = numChannels;
  wavHeader.sampleRate = sampleRate;
  wavHeader.byteRate = sampleRate * wavHeader.numChannels * wavHeader.bitsPerSample / 8;
  wavHeader.blockAlign = wavHeader.numChannels * wavHeader.bitsPerSample / 8;
  //wavHeader.bitsPerSample = bitsPerSample;
  wavHeader.subChunk2Size = recFile.size() - 44;

///Writing Header of wav file should be success/// 
  #if defined (RECORD_DEBUG)
  Serial.print("WAV Header Write ");
  #endif

  wavFile.seek(0);
  //reference to the structure
  if(wavFile.write((byte*)&wavHeader, 44) > 0) {
 #if defined (RECORD_DEBUG)
    digitalWrite(RecLED, HIGH);
    Serial.println("OK");} 
    else {
    Serial.println("Failed");
#endif }
  wavFile.close();
  }} 

///Stoping the Recording for the time being/// 
void Stop(const char *fileName, uint32_t sampleRate){
  aaAudio.adcInterrupts(false);
  createWavHeader(fileName, sampleRate);                   //Add appropriate header info, to make it a valid *.wav file
  #if defined (RECORD_DEBUG)
  Serial.println("Recording Stopped");
  #endif
  
  }
