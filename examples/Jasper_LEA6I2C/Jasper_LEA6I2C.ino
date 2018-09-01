/* LEA-6H I2C GPS Function
*/
#include <Jasper_LEA6I2C.h>
int GPSAddress = 0x42; // GPS I2C Address

void setup()
{
  Wire.begin(); //I2C Initialize
  delay (5000);  
}

void initReceive() 
{
  Wire.beginTransmission(GPSAddress);
  WireSend(0xff); //To send data address      
  Wire.endTransmission(); 
 
  Wire.beginTransmission(GPSAddress);
  Wire.requestFrom(GPSAddress,10); //Require 10 bytes read from the GPS device
}

void readNMEASentence(char *buff){
    int i = 0; // index of data received
    bool readingData = 0; // reading data off the wire

    while(1){
      initReceive(); //Receive data initialization    
      while(Wire.available())   
      { 
        buff[i] = WireRead(); // receive serial data
        if(readingData || buff[i]=='$') // sentences start with $
        { 
          // sentences end in new-line
          if(i>0 && buff[i] =='\r') {
            i = readingData = 0; // reset counters
            Wire.endTransmission(); // end receiving
            return;
          }
          readingData = 1;
          i++;
        }   
      }
      Wire.endTransmission(); //End receiving
    }
}

void loop()
{   
    String sentence = "";
    char buff[BUFFER_LENGTH];
    readNMEASentence(buff);

    // clean up the sentence and convert to a string
    for(int i=0; i<(BUFFER_LENGTH); i++){
      if(buff[i] == '\r'){
        i=BUFFER_LENGTH;
      }
      else{
        sentence += buff[i];
      }
    }
    Serial.println(sentence);
    delay (2000);
}
