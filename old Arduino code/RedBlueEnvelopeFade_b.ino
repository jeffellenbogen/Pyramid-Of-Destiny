

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
//#include <SPI.h>

// Pin defines for the 32x32 RGB matrix.
#define CLK 11  
#define LAT 10
#define OE  9
#define A   A0
#define B   A1
#define C   A2
#define D   A3

#define ENVELOPE_PIN A4
#define POT_PIN A5

#define MIN_VOLUME 7


/* Other Pin Mappings...hidden in the RGB library:
 *  Sig   Uno  Mega
 *  R0    2    24
 *  G0    3    25
 *  B0    4    26
 *  R1    5    27
 *  G1    6    28
 *  B1    7    29
 */

// Note "false" for double-buffering to consume less memory, or "true" for double-buffered.
// Double-buffered makes animation smoother, but on the UNO, we don't quite have enough mem for that.
RGBmatrixPanel matrix(A, B, C,  D,  CLK, LAT, OE, true);

#define MATRIX_WIDTH 32
#define MATRIX_HEIGHT 32

int rectRed = 0;
int rectGreen = 0;
int rectBlue = 0;
int greenVar;
   
void setup() {
  // put your setup code here, to run once:
  matrix.begin();
  Serial.begin(9600);
  

}

void loop() {
  matrix.swapBuffers(true);
  int rectX, rectY, rectWidth, rectHeight;
  int envelopeValue, max_volume;

  envelopeValue = analogRead(ENVELOPE_PIN);
  max_volume = analogRead(POT_PIN);
  max_volume = map(max_volume,0,1023,0,100);
  max_volume = constrain(max_volume,20,100);
  rectBlue = (mapEnvelope(envelopeValue,max_volume)/2)+1;
  rectRed = mapEnvelope(envelopeValue,max_volume);

  if (envelopeValue <= 80)
      rectGreen = 0;
  else
      rectGreen = (envelopeValue % 8);
  Serial.print("envelopeValue = ");
  Serial.print(envelopeValue);
  Serial.print(" max_volume = ");
  Serial.print(max_volume);
    
  Serial.print(" rectRed = ");
  Serial.print(rectRed);
  Serial.print(" rectGreen = ");
  Serial.print(rectGreen); 
  Serial.print(" rectBlue = ");
  Serial.println(rectBlue); 

  for (int i = 0; i <= 16; i++)
  {
    rectX = i;
    rectY = i;
    rectWidth = 32-2*i;
    rectHeight = 32-2*i;
    if (rectRed > 5)
        rectRed--;
 
    matrix.drawRect(rectX, rectY, rectWidth, rectHeight, matrix.Color444(rectRed,rectGreen,rectBlue)); 
  }
/*
  for (int i = 16; i >= 0; i--)
  {
    rectX = i;
    rectY = i;
    rectWidth = 32-2*i;
    rectHeight = 32-2*i;
    matrix.drawRect(rectX, rectY, rectWidth, rectHeight, matrix.Color444(rectRed,rectGreen,rectBlue)); 
  //  delay(10);
  }  

*/
}

int mapEnvelope(int passedEnvelopeValue, int passedPotValue)
{
  int ret_val;
  ret_val = constrain(passedEnvelopeValue,MIN_VOLUME,passedPotValue);
  ret_val = map (ret_val, MIN_VOLUME, passedPotValue, 0, 15); // changed from NUM_LEDS to 12
  return ret_val;
}
