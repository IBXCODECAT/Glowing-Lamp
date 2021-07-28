#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN 6

#define NUMPIXELS 120
#define BRIGHT 150

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 5 // Time (in milliseconds) to pause between pixels

int pixelsEditable = 120;

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin();
  pixels.clear();
}

void loop() {
  Rainbow(1);
  Rainbow(1);
  Rainbow(1);
  RainbowTrans(75, 2);
  TheaterChaseRainbow(75);
  TheaterChaseRainbow(75);
  TheaterChaseRainbow(75);
  Wipe(BRIGHT / 2, BRIGHT / 2, BRIGHT / 2);
  Stack(2, 1);
  
}

void Rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256)
  {
    for(int i=0; i<pixels.numPixels(); i++)
    {
      int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
    }
    
    pixels.show();
    delay(wait);
  }
}

void RainbowTrans(short delayTime, short diff)
{
  for(int i = 0; i < pixels.numPixels(); i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.setPixelColor(i + 1, pixels.Color(0, 0, 0));
    pixels.show();
    delay(delayTime);

    i+= diff;
  }
}


void TheaterChaseRainbow(int wait) {
  int firstPixelHue = 0;
  
  for(int a=0; a<30; a++)
  {
    for(int b=0; b<3; b++)
    {
      pixels.clear();
      
      for(int c=b; c<pixels.numPixels(); c += 3)
      {
        int      hue   = firstPixelHue + c * 65536L / pixels.numPixels();
        uint32_t color = pixels.gamma32(pixels.ColorHSV(hue)); // hue -> RGB
        pixels.setPixelColor(c, color);
      }
      
      pixels.show();
      delay(wait);
      firstPixelHue += 65536 / 90;
    }
  }
}

void Wipe(int R, int G, int B)
{
  for(int i=0; i<pixelsEditable; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    
    pixels.setPixelColor(i, pixels.Color(R, G, B));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }

  for(int i=0; i<pixelsEditable; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }
}

void Stack(int barLength, float wait)
{
  while(true)
  {

    int red = random(0, 3);
    int green = random(0, 3);
    int blue = random(0, 3);

    bool useCertain = false;
    
    int redVal; int greenVal; int blueVal;

      
    if(red == 0 and green == 0 and blue == 0)
    {
      useCertain = true;
    }

    if(red == 1 and green == 1 and blue == 1)
    {
      useCertain = true;
    }
    
    if(red == 2 and green == 2 and blue == 2)
    {
      useCertain = true;
    }
    

    if(not useCertain)
    {
      redVal = red * BRIGHT;
      greenVal = green * BRIGHT;
      blueVal = blue * BRIGHT;
    }
    else
    {
      int certainColor = random(0, 3);

      switch(certainColor)
      {
        case 0:
          redVal = BRIGHT * random(1, 3);
          break;

        case 1:
          greenVal = BRIGHT * random(1, 3);
          break;

        case 2:
          blueVal = BRIGHT * random(1, 3);
          break;
      }
    }
    
    for(int i=0; i<pixelsEditable; i++) { // For each pixel...
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      
      pixels.setPixelColor(i, pixels.Color(redVal, greenVal, blueVal));
      pixels.setPixelColor(i-barLength, pixels.Color(0, 0, 0));
  
      pixels.show();   // Send the updated pixel colors to the hardware.
  
      delay(wait); // Pause before next pass through loop
    }
  
    pixelsEditable -= barLength;
  
    if(pixelsEditable < 0)
    {
      pixelsEditable = NUMPIXELS;
      break;
    }
  }
}
