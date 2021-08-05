#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN 6

#define NUMPIXELS 180
#define BRIGHT 75
#define OFF 0

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 5 // Time (in milliseconds) to pause between pixels

int pixelsEditable = NUMPIXELS;

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
  WipeStack(12, 0, BRIGHT, 0, 0, 20);
  
}

void WipeStack(int len, float wait, int R, int G, int B, short compact)
{
  Stack(len, wait, BRIGHT, 0, 0); //Red stack
  Trans(75, 2); //Trans
  TheaterChase(50, BRIGHT, 0, 0); //Red chase
  Wipe(BRIGHT, BRIGHT, 0, compact); //Yellow wipe
  Stack(len, wait, BRIGHT, BRIGHT, 0); //Yellow stack
  Trans(75, 2); //Trans
  TheaterChase(50, BRIGHT, BRIGHT, 0); //Yellow chase
  Wipe(0, BRIGHT, 0, compact); //Green wipe
  Stack(len, wait, 0, BRIGHT, 0); //Green stack
  Trans(75, 2); //Trans
  TheaterChase(50, 0, BRIGHT, 0); //Green chase
  Wipe(0, BRIGHT, BRIGHT, compact); //Cyan Wipe
  Stack(len, wait, 0, BRIGHT, BRIGHT); //Cyan stack
  Trans(75, 2); //Trans
  TheaterChase(50, 0, BRIGHT, BRIGHT); //Cyan chase
  Wipe(0, 0, BRIGHT, compact); //Blue wipe
  Stack(len, wait, 0, 0, BRIGHT); //Blue stack
  Trans(75, 2); //Trans
  TheaterChase(50, 0, 0, BRIGHT); //Blue chase
  Wipe(BRIGHT, 0, BRIGHT, compact); //Magenta wipe
  Stack(len, wait, BRIGHT, 0, BRIGHT); //Magenta stack
  Trans(75, 2); //Trans
  TheaterChase(50, BRIGHT, 0, BRIGHT); //Magenta chase

  
  Wipe(BRIGHT, BRIGHT, BRIGHT, compact); //White wipe
  RainbowStack(len / 4, 0); //Rainbow stack
  Trans(75, 2); //Trans
  TheaterChaseRainbow(75, 1); //Rainbow chase
}

void Trans(short delayTime, short diff)
{
  for(int i = 0; i < pixels.numPixels(); i++)
  {
    pixels.setPixelColor(i, pixels.Color(OFF, OFF, OFF));
    pixels.setPixelColor(i + 1, pixels.Color(OFF, OFF, OFF));
    pixels.show();
    delay(delayTime);

    i+= diff;
  }
}


void TheaterChase(int wait, int R, int G, int B) {
  int firstPixelHue = 0;
  
  for(int a=0; a<30; a++)
  {
    for(int b=0; b<3; b++)
    {
      pixels.clear();
      
      for(int c=b; c<pixels.numPixels(); c += 3)
      {
        pixels.setPixelColor(c, pixels.Color(R, G, B));
      }
      
      pixels.show();
      delay(wait);
    }
  }
}

void TheaterChaseRainbow(int wait, int compact) {
  int firstPixelHue = 0;
  
  for(int a=0; a<30; a++)
  {
    for(int b=0; b<3; b++)
    {
      pixels.clear();
      
      for(int c=b; c<pixels.numPixels(); c += 3)
      {
        int      hue   = firstPixelHue + c * 65536L / pixels.numPixels() * compact;
        uint32_t color = pixels.gamma32(pixels.ColorHSV(hue)); // hue -> RGB
        pixels.setPixelColor(c, color);
      }
      
      pixels.show();
      delay(wait);
      firstPixelHue += 65536 / 90;
    }
  }
}

void Wipe(int R, int G, int B, short compact)
{
  int cut = pixelsEditable / compact;
    
  for(int i=0; i<pixelsEditable; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    
    pixels.setPixelColor(i, pixels.Color(R, G, B));
    pixels.setPixelColor(i - cut, pixels.Color(OFF, OFF, OFF));
    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }

  for(int i = pixelsEditable - cut; i < pixelsEditable; i++)
  {
    pixels.setPixelColor(i, pixels.Color(OFF, OFF, OFF));
    pixels.show();

    delay(DELAYVAL);
  }
}

void Stack(int barLength, float wait, int R, int G, int B)
{
  while(true)
  {
    for(int i=0; i<pixelsEditable; i++) { // For each pixel...
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      
      pixels.setPixelColor(i, pixels.Color(R, G, B));
      pixels.setPixelColor(i-barLength, pixels.Color(OFF, OFF, OFF));
  
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

void RainbowStack(int barLength, float wait)
{
  int count = 0;
  
  int reds[6] = {BRIGHT, BRIGHT, 0, 0, 0, BRIGHT};
  int greens[6] = {0, BRIGHT, BRIGHT, BRIGHT, 0, 0};
  int blues[6] = {0, 0, 0, BRIGHT, BRIGHT, BRIGHT};
  
  while(true)
  {
    
    for(int i=0; i<pixelsEditable; i++) { // For each pixel...
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255

      int R = reds[count];
      int G = greens[count];
      int B = blues[count];
          
      pixels.setPixelColor(i, pixels.Color(R, G, B));
      pixels.setPixelColor(i-barLength, pixels.Color(OFF, OFF, OFF));
  
      pixels.show();   // Send the updated pixel colors to the hardware.
  
      delay(wait); // Pause before next pass through loop
    }

    if(count > 4) //IDK why the hell I do this but it works so ya
    {
      count = 0;
    }
    else
    {
      count++;
    }
  
    pixelsEditable -= barLength;
  
    if(pixelsEditable < 0)
    {
      pixelsEditable = NUMPIXELS;
      break;
    }
  }
}
