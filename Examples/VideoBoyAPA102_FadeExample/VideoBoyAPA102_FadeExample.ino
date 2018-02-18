
#include <VideoBoyAPA102.h>


const int NUM_LEDS=20;


Pixels pix[NUM_LEDS]; // VideoBoy Pixel Array

VideoBoy Boys (NUM_LEDS, 2.4, 255); 
//Call VideoBoy instance on default SPI port for current board.
//Elements: Name( Pixels Array , Gamma Curve (1.0-3.0), Brightness (0-255)  )

byte brightness; //fade sketch variables
bool ramp= true;


void setup() 
{

//Serial.begin(9600);

Boys.WhiteBalance (165,255,155); //Calibrate White Balance Levels ( Red (0-255), Green (0-255), Blue (0-255)  )

Boys.TestLEDs();  //All Leds Full Brightness RGB test sequence
Boys.PixelTest(2);  //Scans through each pixel in array in (x) seconds
 }

void loop() 

{


 for (int i=0; i<NUM_LEDS; i++)
{ pix[i]= {brightness, brightness, brightness}; } //set colors per pixel { Red (0-255), Green (0-255), Blue (0-255) }


 if (ramp) brightness++;
 else brightness --;
 
if (brightness>254) ramp=false;
if (brightness<1)   ramp=true;


Boys.Show(pix);  //Display Pixels in 13bit VideoBoy Mode
//Boys.Show8bit(pix);   // uncomment to show in mormal 8bit mode
delay(30);

}



