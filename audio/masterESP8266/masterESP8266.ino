#include <ESP8266WiFi.h>
const char* ssid = "WifiSE"; //“your_wifi_ssid”
const char* password = "bbtheme7"; //"your_wifi_password"
uint8_t retries=0;

void setup() {
Serial.begin(115200);
  delay(100);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println();
  digitalWrite(LED_BUILTIN, HIGH);
  //Try and Connect to the Network
  WiFi.begin(ssid,password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println("...");
 
  //Wait for WiFi to connect for a maximum timeout of 20 seconds
  while(WiFi.status()!=WL_CONNECTED && retries<20)
  {
    Serial.print(".");
    retries++;
    digitalWrite(LED_BUILTIN, LOW);
    delay(2000);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
  }
   Serial.println();
  //Inform the user whether the timeout has occured, or the ESP8266 is connected to the internet
  if(retries==20)//Timeout has occured
  {
    Serial.print("Unable to Connect to ");
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println(ssid);
  }
   
  if(WiFi.status()==WL_CONNECTED)//WiFi has succesfully Connected
  {
    Serial.print("Successfully connected to ");
    Serial.println(ssid);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }

///// SPI MASTER MODE
}


void loop() {
}
