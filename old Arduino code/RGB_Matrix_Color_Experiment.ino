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

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, true);
int potValue, envelopeValue, audioModifier, pyramidColor;
int colorGradient = 40;
int audioGradientMax = 500;

 
void setup() {
  matrix.begin();
  Serial.begin(9600);
}

void loop() {
  envelopeValue = analogRead(ENVELOPE_PIN);
  envelopeValue = constrain(envelopeValue,0,100);
  audioModifier = map(envelopeValue,6,100,0,audioGradientMax);
  potValue = analogRead(POT_PIN);
  pyramidColor = map(potValue,0,1023,0,1536);

  Serial.print("envelope Value = ");
  Serial.print(envelopeValue);
  Serial.print(" audioModifier  = ");
  Serial.print(audioModifier);
  Serial.print(" pot Value = ");
  Serial.print(potValue);
  Serial.print(" pyramid Color = ");
  Serial.println(pyramidColor); 

  drawColorRect();
}

void drawColorRect()
{ 
  int rectColor = pyramidColor;
  for (int c = 0; c < 16; c++)
      {
      matrix.drawRect(c, c, 32-2*c, 32-2*c, matrix.ColorHSV(rectColor+audioModifier, 255, 255, true));
      rectColor = rectColor+colorGradient;
      }
      matrix.swapBuffers(true);
}
