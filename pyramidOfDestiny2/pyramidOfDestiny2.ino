#include <RGBmatrixPanel.h>

#define CLK  11   // USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

//This is a comment  

#define ENVELOPE_PIN A4
#define POT_PIN A5
#define BUTTON_PIN 8

#define MIN_ENVELOPE 6
#define MAX_ENVELOPE 100

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



uint16_t color_palette_HSV1[] = // HSV
{
  matrix.ColorHSV(450, 255, 20, true), 
  matrix.ColorHSV(490, 255, 255, true), 
  matrix.ColorHSV(1410, 255, 255, true), 
  matrix.ColorHSV(420, 255, 255, true), 
  matrix.ColorHSV(540, 255, 255, true), 
  matrix.ColorHSV(500, 255, 100, true),         
  
};

uint16_t color_palette_HSV2[] = // HSV
{
  matrix.ColorHSV(1250, 255, 255, true), 
  matrix.ColorHSV(270, 255, 255, true), 
  matrix.ColorHSV(1210, 255, 40, true), 
  matrix.ColorHSV(1100, 255, 255, true), 
  matrix.ColorHSV(1300, 255, 255, true), 
  matrix.ColorHSV(1200, 255, 20, true),         
  
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
  else if (pattern == 5)
    swipes(potValue, envelopeValue); 
  else if (pattern == 6)
    droplets(potValue, envelopeValue); 

}

void buttonPressed()
{
    if (digitalRead(BUTTON_PIN) == LOW)
    {
      pattern++;
      if (pattern > 6)
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
      matrix.drawRect(c, c, MATRIX_WIDTH-2*c, MATRIX_HEIGHT-2*c, matrix.ColorHSV(rectColor + audioModifier, 255, 255, true));
      rectColor = rectColor + colorGradient;
      buttonPressed();
    }
  matrix.swapBuffers(true);
}

void circularDivision(int passedPotValue, int passedEnvelopeValue)
{
  //envelopeValue = analogRead(ENVELOPE_PIN);
  //potValue = analogRead(POT_PIN);
  int maxEnv = map(passedPotValue,0,1024,20, 90);
  int constrainedEnvelope = constrain(passedEnvelopeValue,7,maxEnv);
  int divisions = map(constrainedEnvelope,6,maxEnv,0,2);

  Serial.print("  maxEnv = ");
  Serial.print(maxEnv);
  Serial.print("  constrainedEnvelope = ");
  Serial.print(constrainedEnvelope);  
  Serial.print(" divisions = ");
  Serial.println(divisions);

  if (constrainedEnvelope > 9)
     drawCircles(divisions);
  else
    {
     matrix.fillRect(0, 0, MATRIX_WIDTH, MATRIX_HEIGHT, matrix.ColorHSV(0, 0, 0, true));
     matrix.swapBuffers(true);
    }
}


void redBlueFade(int passedPotValue, int passedEnvelopeValue)
{
  int rectX, rectY, rectWidth, rectHeight, rectRed, rectBlue, rectGreen;
  int colorMod = map(passedPotValue, 0, 1023, 2, 10);
  int maxEnvMod = (MAX_ENVELOPE * colorMod*10) / 100;
  rectBlue = map(passedEnvelopeValue, MIN_ENVELOPE,maxEnvMod, 0, 15) / 2 + 1;
  rectRed = map(passedEnvelopeValue, MIN_ENVELOPE,maxEnvMod, 3, 15);
  
  if (passedEnvelopeValue <= 80)
    rectGreen = 0;
  else
    rectGreen = passedEnvelopeValue % 8;
  Serial.print("colorMod = ");
  Serial.print(colorMod);
  Serial.print("  maxEnvMod = ");
  Serial.print(maxEnvMod);
  Serial.print("  rectRed = ");
  Serial.print(rectRed);
  Serial.print(" rectGreen = ");
  Serial.print(rectGreen);
  Serial.print("  rectBlue = ");
  Serial.println(rectBlue);  
  
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
    colorRandomizer = random(1,7);

  for (int a = 0; a < rows_cols; a++)
  {
    for (int b = 0; b < rows_cols; b++)
    {
      static int x, y;
      x = (b * radius*2)-radius;
      y = (a * radius*2)-radius;
      
      if (colorRandomizer%2==0)
        matrix.fillCircle(x,y,radius,color_palette_HSV1[(a+b)%2+colorRandomizer]);
      else
        matrix.fillCircle(x,y,radius,color_palette_HSV2[(a+b)%2+colorRandomizer]);

    }
    buttonPressed();
  }
  matrix.swapBuffers(true);
}


void sparkles(int passedPotValue, int passedEnvelopeValue)
{
  int sparkleSize = map(passedPotValue, 0, 1023, 2, 7);
  int numSparkles = map(passedEnvelopeValue, MIN_ENVELOPE, MAX_ENVELOPE, 0, 8);
  Serial.print("sparkleSize = ");
  Serial.print(sparkleSize);
  Serial.print(" | numSparkles = ");
  Serial.println(numSparkles);

  if (passedEnvelopeValue > 40)
  {
    for (int i=0; i < numSparkles; i++)
      {
      buttonPressed();  
      int randX = random(0,MATRIX_WIDTH);
      int randY = random(0,MATRIX_HEIGHT);
      int randomColor = random(1500);
      for (int j=1; j < sparkleSize; j++)
        {
          matrix.fillCircle(randX, randY, j-i, matrix.ColorHSV(randomColor, 170, 255, true));
          delay(2);
          matrix.swapBuffers(true);
        }
      }
  }
  else
  {
      matrix.fillRect(0, 0, MATRIX_WIDTH, MATRIX_HEIGHT, matrix.ColorHSV(0, 0, 0, true));
      matrix.swapBuffers(true);
  }
}

void swipes(int passedPotValue, int passedEnvelopeValue)
{
  int colorMod = 10;
  int swipeColor = map(passedPotValue, 0, 1023, 0, 1500);
  int swipeSpeed = map(passedEnvelopeValue, MIN_ENVELOPE, MAX_ENVELOPE, 5,0);
  int randDirection = random (1,5);
  Serial.print("randDirection = ");
  Serial.print(randDirection);
  Serial.print("  swipeColor = ");
  Serial.print(swipeColor);
  Serial.print("   passedEnvelopeValue = ");
  Serial.print(passedEnvelopeValue);
  Serial.print("   swipeSpeed = ");
  Serial.println(swipeSpeed);
  
  if (passedEnvelopeValue > 20)
  {
      if (randDirection == 1)
      {
        for (int i = 0; i <= MATRIX_WIDTH; i++)
        {
          matrix.drawLine(i, 0, i, MATRIX_HEIGHT,  matrix.ColorHSV(swipeColor+i*colorMod, 255, 255, true));
          delay(swipeSpeed);
          matrix.swapBuffers(true);
          matrix.drawLine(i, 0, i, MATRIX_HEIGHT,  matrix.ColorHSV(0, 0, 0, true));   
          buttonPressed(); 
        }
      }
      else if (randDirection == 2)
      {
        for (int i = MATRIX_WIDTH; i >=0; i--)
        {
          matrix.drawLine(i, 0, i, MATRIX_HEIGHT,  matrix.ColorHSV(swipeColor+i*colorMod, 255, 255, true));
          delay(swipeSpeed);
          matrix.swapBuffers(true);
          matrix.drawLine(i, 0, i, MATRIX_HEIGHT,  matrix.ColorHSV(0, 0, 0, true)); 
          buttonPressed();
        }
      } 
      if (randDirection == 3)
      {
        for (int i = 0; i <= MATRIX_HEIGHT; i++)
        {
          matrix.drawLine(0, i,MATRIX_WIDTH, i, matrix.ColorHSV(swipeColor+i*colorMod, 255, 255, true));
          delay(swipeSpeed);
          matrix.swapBuffers(true);
          matrix.drawLine(0, i, MATRIX_WIDTH, i,  matrix.ColorHSV(0, 0, 0, true));  
          buttonPressed();
        }
      }
       else if (randDirection == 4)
      {
        for (int i = MATRIX_HEIGHT; i >=0; i--)
        {
          matrix.drawLine(0, i,MATRIX_WIDTH,i,  matrix.ColorHSV(swipeColor+i*colorMod, 255, 255, true));
          delay(swipeSpeed);
          matrix.swapBuffers(true);
          matrix.drawLine(0, i, MATRIX_WIDTH,i,  matrix.ColorHSV(0, 0, 0, true));    
          buttonPressed();
        }
      } 
      matrix.swapBuffers(true); 
  }
   
}

void droplets(int passedPotValue, int passedEnvelopeValue)
{
  int dropletSize = map(passedPotValue, 0, 1023, 2, 9);
  int numDroplets = map(passedEnvelopeValue, MIN_ENVELOPE, MAX_ENVELOPE, 1, 5);
  int randomColor = random(800,1200);
  Serial.print("dropletSize = ");
  Serial.print(dropletSize);
  Serial.print(" | numDroplets= ");
  Serial.println(numDroplets);

  if (passedEnvelopeValue > 40)
  {
    for (int i=0; i < numDroplets; i++)
      {
      buttonPressed();  
      int randX = random(0,MATRIX_WIDTH);
      int randY = random(0,MATRIX_HEIGHT);

      for (int j=1; j < dropletSize; j++)
        {
          matrix.fillCircle(randX, randY, j, matrix.ColorHSV(randomColor, 255, 255, true));
          matrix.fillCircle(randX, randY, j-1, matrix.ColorHSV(randomColor, 0, 0, true));
          matrix.swapBuffers(true); 
          delay(5);
        }
      }
      matrix.fillRect(0, 0, MATRIX_WIDTH, MATRIX_HEIGHT, matrix.ColorHSV(0, 0, 0, true));
      matrix.swapBuffers(true);
  }
  else
  {
      matrix.fillRect(0, 0, MATRIX_WIDTH, MATRIX_HEIGHT, matrix.ColorHSV(0, 0, 0, true));
      matrix.swapBuffers(true);
  }
}
