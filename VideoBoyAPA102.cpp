
#include "Arduino.h"
#include "VideoBoyAPA102.h"
#include "SPI.h"





VideoBoy::VideoBoy(int numLeds, float videoGamma, float brightMAX) 
 {


 	
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
 
 bytesPerLED = 4;
 LEDLength=numLeds;
 

 endFrames = round( (((numLeds+1)/2)/8)*bytesPerLED ) ;
 frameLength = ((numLeds+2)*bytesPerLED)+endFrames;

 LED_Data = (byte*)malloc(frameLength);
 LEDs13 = (uint16_t*)malloc(numLeds*6);
 
 gammaTable8R =   (uint8_t*)malloc(256);
 gammaTable8G =   (uint8_t*)malloc(256);
 gammaTable8B =   (uint8_t*)malloc(256);

 gammaTable13R  = (uint16_t*)malloc(256*2);
 gammaTable13G  = (uint16_t*)malloc(256*2);
 gammaTable13B  = (uint16_t*)malloc(256*2);


 brightLevel = (byte*)malloc(numLeds); 


 


for (int i=0; i < 256; i++)  
{ 
 gammaTable8R[i]  = (int)(pow((float)i / 255.0, videoGamma) * brightMAX  + 0.5); 
 gammaTable8G[i]  = (int)(pow((float)i / 255.0, videoGamma) * brightMAX  + 0.5); 
 gammaTable8B[i]  = (int)(pow((float)i / 255.0, videoGamma) * brightMAX  + 0.5); 

 gammaTable13R[i] = (int)(pow((float)i / 255.0, videoGamma) * 31*brightMAX + 0.5); 
 gammaTable13G[i] = (int)(pow((float)i / 255.0, videoGamma) * 31*brightMAX + 0.5);
 gammaTable13B[i] = (int)(pow((float)i / 255.0, videoGamma) * 31*brightMAX + 0.5);
}



 }



void VideoBoy::WhiteBalance(int R, int G, int B)
{
for (int i=0; i < 256; i++) 
{
  gammaTable8R[i]= (gammaTable8R[i]*R) / 255.0;
  gammaTable8G[i]= (gammaTable8R[i]*G) / 255.0;
  gammaTable8B[i]= (gammaTable8R[i]*B) / 255.0;

  gammaTable13R[i]= (gammaTable13R[i]*R) / 255.0;
  gammaTable13G[i]= (gammaTable13G[i]*G) / 255.0;
  gammaTable13B[i]= (gammaTable13B[i]*B) / 255.0;
}

}
void VideoBoy::SPISend()

{    SPI.transfer16(0x00); //frame Start bits
     SPI.transfer16(0x00); 


    for(int i=0; i < frameLength; i++)
    {
     SPI.transfer(LED_Data[i]); 
    } 
}

void VideoBoy::RGBBoys(byte R, byte G, byte B)
{
  for(int i=0; i < LEDLength; i++)
  {
    LED_Data[bytesPerLED*i] = 255;  //brightness
    LED_Data[bytesPerLED*i+1] = B;  //blue
    LED_Data[bytesPerLED*i+2] = G;  //green
    LED_Data[bytesPerLED*i+3] = R;  //red
  }

   SPISend();
}
void VideoBoy::TestLEDs()

{

RGBBoys(255,0,0);
delay(500);

RGBBoys(0,255,0);
delay(500);

RGBBoys(0,0,255);
delay(500);

}



void VideoBoy::Show(Pixels PixInput[])  //13bit pixels -- uses 13bit gamma curve with 8bit RGB values + 5bit brightness per pixel
{

for ( int i=0; i < LEDLength; i++ )   //set gammaTable values
 { 
 	LEDs13[3*i]   = gammaTable13B[PixInput[i].b]; 
 	LEDs13[3*i+1] = gammaTable13G[PixInput[i].g]; 
 	LEDs13[3*i+2] = gammaTable13R[PixInput[i].r]; 
 	  }

for ( int i=0; i < LEDLength; i++)  //set apa102 bright levels
{ 
 maxColor = max( LEDs13[3*i] , LEDs13[3*i+1]);
 	maxColor = max( maxColor, LEDs13[3*i+2]); 

 	 brightLevel[i] = 1;

 while(255*brightLevel[i] < maxColor) 
brightLevel[i]++;
 }

for (int i=0; i < LEDLength; i++) // Set pixels
{ LED_Data[i*bytesPerLED]    =  0xE0 | brightLevel[i] ; // set first 3 bits high for APA protocol + 5 bit brightness level per pixel
  LED_Data[i*bytesPerLED+1]  =  ( LEDs13[3*i]  +  (brightLevel[i] / 2) ) / brightLevel[i];   //blue
  LED_Data[i*bytesPerLED+2]  =  ( LEDs13[3*i+1]  +  (brightLevel[i] / 2) ) / brightLevel[i];   //greeen
  LED_Data[i*bytesPerLED+3]  =  ( LEDs13[3*i+2] +  (brightLevel[i] / 2) ) / brightLevel[i];     //red
 } 
 
SPISend();
}


void VideoBoy::Show8bit(Pixels PixInput[]) //8bit pixels  -- uses 8bit gamma curve with brightness always at full
{

  for( int i=0; i<LEDLength; i++ )
  {
    LED_Data[i*bytesPerLED] = 255;
    LED_Data[i*bytesPerLED+1] = gammaTable8B[PixInput[i].b];
    LED_Data[i*bytesPerLED+2] = gammaTable8G[PixInput[i].g];
    LED_Data[i*bytesPerLED+3] = gammaTable8R[PixInput[i].r];
  }
        
SPISend();
}

void VideoBoy::PixelTest(int Seconds)
{
	int delayTime = (Seconds*1000)/LEDLength;

  for( int i=0; i<LEDLength; i++ )
{ LED_Data[i*bytesPerLED+0] = 255;
  LED_Data[i*bytesPerLED+1] = 0;
  LED_Data[i*bytesPerLED+2] = 0;
  LED_Data[i*bytesPerLED+3] = 0; }

  SPISend();

for( int i=0; i<LEDLength; i++ )
{ LED_Data[i*bytesPerLED+0] = 255;
  LED_Data[i*bytesPerLED+1] = 255;
  LED_Data[i*bytesPerLED+2] = 255;
  LED_Data[i*bytesPerLED+3] = 255;

  LED_Data[(i-1)*bytesPerLED+0] = 255;
  LED_Data[(i-1)*bytesPerLED+1] = 0;
  LED_Data[(i-1)*bytesPerLED+2] = 0;
  LED_Data[(i-1)*bytesPerLED+3] = 0;

SPISend();
delay(delayTime);

}



}

void VideoBoy::printGamma()
{
	  for (int i=0; i < 256; i++) {
    
    Serial.print("Gamma[");
    Serial.print(i);
    Serial.print("] ");
    Serial.print("13bit=");
    Serial.print(gammaTable13R[i]);
    Serial.print("  8bit=");
    Serial.println(gammaTable8R[i]);
    delay(1);
  }
}