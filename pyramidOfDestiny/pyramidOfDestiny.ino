#include <RGBmatrixPanel.h>

#define CLK  11   // USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

#define ENVELOPE_PIN A4
#define POT_PIN A5
#define BUTTON_PIN 8

#define MAX_ENVELOPE 150
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, true);

int potValue, envelopeValue;
int pattern = 1;


void setup() {
  matrix.begin();
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {  
  //Read envelopeValue and adjust audioModifier value based on that reading
  envelopeValue = analogRead(ENVELOPE_PIN);
  envelopeValue = constrain(envelopeValue,0,MAX_ENVELOPE);

  // Read potValue to set the baseColor for the pattern
  potValue = analogRead(POT_PIN);

  //Check for button Press to increment pattern
  buttonPressed();
  
  //select which pattern to display based on the current pattern
  if (pattern == 1)
    drawColorRects(potValue, envelopeValue);
  else if (pattern == 2)
    circularDivision(potValue, envelopeValue);
  else if (pattern == 3)
    redBlueFade();
  else if (pattern == 4)
    sparkles();
  
   // add additional patterns beyond #4 here later
}

void buttonPressed()
{
    if (digitalRead(BUTTON_PIN) == LOW)
    {
      pattern++;
      delay(1000);
    }

    if (pattern > 4)
      pattern = 1;  
}

void drawColorRects(int passedPotValue, int passedEnvelopeValue)
{ 
  int colorGradient = 40;
  int audioGradientMax = 500;
  
  int audioModifier = map(passedEnvelopeValue,6,MAX_ENVELOPE,0,audioGradientMax);
  int rectColor = map(passedPotValue,0,1023,0,1536);
  
  for (int c = 0; c < 16; c++)
    {
      matrix.drawRect(c, c, 32-2*c, 32-2*c, matrix.ColorHSV(rectColor + audioModifier, 255, 255, true));
      rectColor = rectColor + colorGradient;
    }
  matrix.swapBuffers(true);
}

void circularDivision(int passedPotValue, int passedEnvelopeValue)
{
  int powerOf2[]={0,1,2,4,8,16};
  int divisions = map(passedEnvelopeValue, 6, MAX_ENVELOPE, 0, 2);
  
  int radius = round(powerOf2[5-divisions])/2;
  int rows_cols = round(powerOf2[5-divisions])*2;

  static int colorRandomizer;
  
  uint16_t color_palette1[] =
    {
       matrix.Color333(7,0,0), // red
       matrix.Color333(0,7,0), // green
       matrix.Color333(0,0,7), // blue
       matrix.Color333(7,7,0), // yellow
       matrix.Color333(0,7,7), // cyan
       matrix.Color333(7,0,7) // purple
      
    };

  uint16_t color_palette2[] =
    {
       matrix.Color333(7,0,2), // magenta
       matrix.Color333(5,0,3), // pink
       matrix.Color333(3,0,7), // violet
       matrix.Color333(0,4,7), // aqua
       matrix.Color333(0,7,4), // azure
       matrix.Color333(2,4,2) // moss   
      
    };

  uint16_t color_palette3[] =
    {
       matrix.Color333(1,1,1), // white
       matrix.Color333(1,0,0), // dim red
       matrix.Color333(0,0,0), // black
       matrix.Color333(7,7,7), // bright white
       matrix.Color333(3,3,3), // medium white
       matrix.Color333(0,0,0) // black   
      
    };

  uint16_t color_palette4[] =
    {
       matrix.Color333(7,0,7), // purple
       matrix.Color333(7,7,0), // yellow
       matrix.Color333(7,4,7), // orange
       matrix.Color333(0,0,0), // black
       matrix.Color333(0,7,4), // azure
       matrix.Color333(7,0,2)  // magenta
      
    };
  
  if (divisions >= 1)
    colorRandomizer = random(0,4);
    

  for (int a = 0; a < rows_cols; a++)
      {
        for (int b = 0; b < rows_cols; b++)
          {
            static int x, y;
            x = (b * radius*2)-radius;
            y = (a * radius*2)-radius;
            if (colorRandomizer%6==0)
              matrix.fillCircle(x,y,radius,color_palette1[(a+b)%2+colorRandomizer]);
            else if (colorRandomizer%6==1)
              matrix.fillCircle(x,y,radius,color_palette2[(a+b)%2+colorRandomizer]);
            else if (colorRandomizer%6==2)
              matrix.fillCircle(x,y,radius,color_palette3[(a+b)%2+colorRandomizer]);     
            else
              matrix.fillCircle(x,y,radius,color_palette4[(a+b)%2+colorRandomizer]);      
          }
      }
  matrix.swapBuffers(true);
}

void redBlueFade()
{
  
}

void sparkles()
{
  
}
