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

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, true);

int potValue, envelopeValue, audioModifier, baseColor;
int pattern = 1;
int colorGradient = 40;
int audioGradientMax = 500;

void setup() {
  matrix.begin();
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // check for button press and increment pattern number
  if (digitalRead(BUTTON_PIN) == LOW)
    {
      pattern++;
      delay(1000);
    }

  //Reset to first pattern if pattern is bigger than the number of patterns  
  if (pattern > 3)
    pattern = 1;

  //Read envelopeValue and adjust audioModifier value based on that reading
  envelopeValue = analogRead(ENVELOPE_PIN);
  envelopeValue = constrain(envelopeValue,0,100);
  audioModifier = map(envelopeValue,6,100,0,audioGradientMax);

  // Read potValue to set the baseColor for the pattern
  potValue = analogRead(POT_PIN);
  baseColor = map(potValue,0,1023,0,1536);

  //select which pattern to display based on the current pattern
  if (pattern == 1)
    drawColorRects();
  else if (pattern == 2)
    circularDivision();
  else if (pattern == 3)
    redBlueFade();
  
    // add additional patterns here later
}

void drawColorRects()
{ 
  int rectColor = baseColor;
  for (int c = 0; c < 16; c++)
      {
      matrix.drawRect(c, c, 32-2*c, 32-2*c, matrix.ColorHSV(rectColor + audioModifier, 255, 255, true));
      rectColor = rectColor + colorGradient;
      }
      matrix.swapBuffers(true);
}

void circularDivision()
{
  
}

void redBlueFade()
{
  
}
