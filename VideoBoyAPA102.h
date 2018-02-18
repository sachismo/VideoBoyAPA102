#ifndef VideoBoyAPA102_h
#define VideoBoyAPA102_h

#include "Arduino.h"
#include "SPI.h"



struct Pixels {
  uint16_t r;
  uint16_t g;
  uint16_t b;  
};

//void SPISend();

class VideoBoy
{
  public:
   
   VideoBoy(int, float, float);
    void Begin();
    void TestLEDs(); 
    void RGBBoys(byte, byte, byte);
    void Show(Pixels[]);
    void ShowLive(Pixels[]);
    void Show8bit(Pixels[]);
    void PixelTest(int);
    void printGamma();
    void WhiteBalance(int, int, int);
   


  private:

  	void SPISend();

  	int bytesPerLED;
  	int LEDLength;

  	uint16_t endFrames;
    uint16_t frameLength;

    uint8_t* LED_Data;
    uint16_t* LEDs13;

    uint8_t* gammaTable8R;
    uint8_t* gammaTable8G;
    uint8_t* gammaTable8B;

    uint16_t* gammaTable13R;
    uint16_t* gammaTable13G;
    uint16_t* gammaTable13B;

    uint8_t* brightLevel;
    uint16_t maxColor;

    uint8_t Cal_Red;
    uint8_t Cal_Green;
    uint8_t Cal_Blue;


};



#endif