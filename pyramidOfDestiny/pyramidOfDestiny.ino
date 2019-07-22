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

#define MIN_ENVELOPE 6
#define MAX_ENVELOPE 150

#define MATRIX_WIDTH 32
#define MATRIX_HEIGHT 32

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, true);

int potValue, envelopeValue;
int pattern = 1;

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

uint16_t color_palette5[] = //America Pallette
{
   matrix.Color333(7,0,0), // red
   matrix.Color333(7,7,7), // white
   matrix.Color333(0,0,7), // blue
   matrix.Color333(0,0,7), // blue
   matrix.Color333(7,7,7), // white
   matrix.Color333(7,0,0) // red  
  
};

uint16_t color_palette6[] = // Univ. of Oregon Pallette
{
   matrix.Color333(0,7,0), //green
   matrix.Color333(7,7,0), //yellow
   matrix.Color333(5,7,0), // yellow-green
   matrix.Color333(7,7,7), // bright white
   matrix.Color333(0,7,0), //green
   matrix.Color333(3,7,0)  //green-yellow 
  
};




void setup() {
  matrix.begin();
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  Serial.print ("Pattern #");
  Serial.println(pattern);
}

void loop() {  
  //Read envelopeValue and adjust audioModifier value based on that reading
  envelopeValue = analogRead(ENVELOPE_PIN);
  envelopeValue = constrain(envelopeValue, MIN_ENVELOPE, MAX_ENVELOPE);

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
    redBlueFade(potValue, envelopeValue);
  else if (pattern == 4)
    sparkles(potValue, envelopeValue);
  
   // add additional patterns beyond #4 here later
}

void buttonPressed()
{
    if (digitalRead(BUTTON_PIN) == LOW)
    {
      pattern++;
      if (pattern > 4)
        pattern = 1;  
      delay(250);
      Serial.print ("Pattern #");
      Serial.println(pattern);
    }
}

void drawColorRects(int passedPotValue, int passedEnvelopeValue)
{ 
  int colorGradient = 40;
  int audioGradientMax = 500;
  
  int audioModifier = map(passedEnvelopeValue,MIN_ENVELOPE,MAX_ENVELOPE,0,audioGradientMax);
  int rectColor = map(passedPotValue,0,1023,0,1536);
  
  for (int c = 0; c < 16; c++)
    {
      matrix.drawRect(c, c, 32-2*c, 32-2*c, matrix.ColorHSV(rectColor + audioModifier, 255, 255, true));
      rectColor = rectColor + colorGradient;
      buttonPressed();
    }
  matrix.swapBuffers(true);
}

void circularDivision(int passedPotValue, int passedEnvelopeValue)
{
  //envelopeValue = analogRead(ENVELOPE_PIN);
  //potValue = analogRead(POT_PIN);
  int maxEnv = map(passedPotValue,0,1024,20, 150);
  int constrainedEnvelope = constrain(passedEnvelopeValue,7,maxEnv);
  int divisions = map(constrainedEnvelope,6,maxEnv,0,2);
  drawCircles(divisions);
}


void redBlueFade(int passedPotValue, int passedEnvelopeValue)
{
  int rectX, rectY, rectWidth, rectHeight, rectRed, rectBlue, rectGreen;
  rectBlue = map(passedEnvelopeValue, MIN_ENVELOPE,MAX_ENVELOPE, 0, 15) / 2 + 1;
  rectRed = map(passedPotValue, 0, 1023, 3, 15);

  if (passedEnvelopeValue <= 70)
    rectGreen = 0;
  else
    rectGreen = 15 - rectRed;
  
  for (int i = 0; i <= 16; i++)
  {
    buttonPressed();
    rectX = i;
    rectY = i;
    rectWidth = 32-2*i;
    rectHeight = 32-2*i;
    if (rectRed > 5)
        rectRed--;
 
    matrix.drawRect(rectX, rectY, rectWidth, rectHeight, matrix.Color444(rectRed,rectGreen,rectBlue)); 
  }  
  matrix.swapBuffers(true);
}

void drawCircles(int passedDivisions)
{
  int radius = round(pow(2,5-passedDivisions))/2;
  int rows_cols = round(pow(2,passedDivisions))*2;
  int modifier = 3;
  static int colorRandomizer;
  
  if (passedDivisions >= 1)
    colorRandomizer = random(0,6);

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
      else if (colorRandomizer%6==3)
        matrix.fillCircle(x,y,radius,color_palette4[(a+b)%2+colorRandomizer]);      
       else if (colorRandomizer%6==4)
        matrix.fillCircle(x,y,radius,color_palette5[(a+b)%2+colorRandomizer]);
      else
        matrix.fillCircle(x,y,radius,color_palette6[(a+b)%2+colorRandomizer]);
    }
    buttonPressed();
  }
  matrix.swapBuffers(true);
}


void sparkles(int passedPotValue, int passedEnvelopeValue)
{
  int sparkleSize = map(passedPotValue, 0, 1023, 1, 8);
  int numSparkles = map(passedEnvelopeValue, MIN_ENVELOPE, MAX_ENVELOPE, 0, 7);
  Serial.print("sparkleSize = ");
  Serial.print(sparkleSize);
  Serial.print(" | numSparkles = ");
  Serial.println(numSparkles);
  
  for (int i=0; i < numSparkles; i++)
    {
    buttonPressed();  
    int randX = random(0,MATRIX_WIDTH);
    int randY = random(0,MATRIX_HEIGHT);
    int randomColor = random(0,1500);
    for (int j=1; j <= sparkleSize; j++)
      {
        matrix.fillCircle(randX, randY, j-i, matrix.ColorHSV(randomColor, 255, 255, true));
        delay(25);
        matrix.swapBuffers(true);
      }
    }
    matrix.fillRect(0, 0, MATRIX_WIDTH, MATRIX_HEIGHT, matrix.ColorHSV(0, 0, 0, true));
    matrix.swapBuffers(true);
}
