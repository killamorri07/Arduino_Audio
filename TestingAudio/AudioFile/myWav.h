/*********************************************************/

/* WAV HEADER STRUCTURE */
struct wavStruct {
  const char chunkID[4] = {'R', 'I', 'F', 'F'};
  uint32_t chunkSize = 36;                     //Size of (entire file in bytes - 8 bytes) or (data size + 36)
  const char format[4] = {'W', 'A', 'V', 'E'};
  const char subchunkID[4] = {'f', 'm', 't', ' '};
  const uint32_t subchunkSize = 16;
  const uint16_t audioFormat = 1;              //PCM == 1
  uint16_t numChannels = 1;                    //1=Mono, 2=Stereo
  uint32_t sampleRate = 16000;
  uint32_t byteRate = 16000;                   //== SampleRate * NumChannels * BitsPerSample/8
  uint16_t blockAlign = 1;                     //== NumChannels * BitsPerSample/8
  uint16_t bitsPerSample = 8;                  //8,16,32...
  const char subChunk2ID[4] = {'d', 'a', 't', 'a'};
  uint32_t subChunk2Size = 0;                  //== NumSamples * NumChannels * BitsPerSample/8
  //Data                                       //The audio data
};

/*********************************************************/
uint32_t counter = 0;
/*********************************************************/
uint32_t sampleRate = 16000;

void ADC_Handler(void) {                                   //ADC Interrupt triggered by ADC sampling completion
  aaAudio.getADC();
  if (myFile) {
    myFile.write(aaAudio.adcBuffer, MAX_BUFFER_SIZE);     //Write the data to SD as it is available
    counter++;
  }
}

void startRecording(const char *fileName) {
#if defined (RECORD_DEBUG)
  Serial.print("Start recording: ");
  Serial.println(fileName);
#endif

if(myFile){
  aaAudio.adcInterrupts(false);
  myFile.close();
  }
myFile = sd.open(fileName, O_RDWR | O_CREAT | O_AT_END);
if(!myFile){
#if defined(RECORD_DEBUG)
  Serial.println("Failed to open the file to write");
#endif
  return;
  }
  myFile.seek(0);
  uint8_t bb =0;
  for(int i=0; i<44; i++){
    myFile.write(bb);}

  aaAudio.adcBitsPerSample =8;
  aaAudio.setSampleRate(sampleRate);
  aaAudio.getADC();
  aaAudio.getADC();
  aaAudio.adcInterrupts(true);
}
/*********************************************************/
void createWavHeader(const char *fileName) {
if(!sd.exists(fileName)){
#if defined(RECORD_DEBUG)
Serial.println("File does not exist, no file created.");
#endif
return;}

myFile=sd.open(fileName,O_RDWR | O_CREAT | O_AT_END);

if(myFile.size() <= 44){
 #if defined (RECORD_DEBUG)
 Serial.println("File contais no data, exiting");
 #endif
 myFile.close();
 return;
 }
wavStruct wavHeader;
wavHeader.chunkSize = myFile.size() -8;
wavHeader.sampleRate = sampleRate; 
wavHeader.byteRate = sampleRate*wavHeader.numChannels*wavHeader.bitsPerSample/8;
wavHeader.blockAlign = wavHeader.numChannels * wavHeader.bitsPerSample / 8;
wavHeader.subChunk2Size = myFile.size() - 44;
#if defined (RECORD_DEBUG)
  Serial.print("WAV Header Write ");
#endif
  myFile.seek(0);
  if ( myFile.write((byte*)&wavHeader, 44) > 0) {
#if defined (RECORD_DEBUG)
    Serial.println("OK");
  } else {
    Serial.println("Failed");
#endif
  }
  myFile.close();
}

void stopRecording(const char *fileName) {
aaAudio.adcInterrupts(false);
myFile.close();
createWavHeader(fileName);
#if defined (RECORD_DEBUG)
  Serial.println("Recording Stopped");
#endif
}
