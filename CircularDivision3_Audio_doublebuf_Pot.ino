// Example primatives.

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

// Pin defines for the 32x32 RGB matrix.
#define CLK 11  
#define LAT 10
#define OE  9
#define A   A0
#define B   A1
#define C   A2
#define D   A3

#define show()  swapBuffers(true)

#define ENVELOPE_PIN A4
#define POT_PIN A5

int powerOf2[]={0,1,2,4,8,16};


// Note "false" for double-buffering to consume less memory, or "true" for double-buffered.
// Double-buffered makes animation smoother, but on the UNO, we don't quite have enough mem for that.
RGBmatrixPanel *matrix = new RGBmatrixPanel(A, B, C,D, CLK, LAT, OE, true);

int x, y, radius, rows_cols;
int divisions, envelopeValue, potValue, constrainedEnvelope, maxEnvelope;

uint16_t color_palette1[] =
{
   matrix->Color333(7,0,0), // red
   matrix->Color333(0,7,0), // green
   matrix->Color333(0,0,7), // blue
   matrix->Color333(7,7,0), // yellow
   matrix->Color333(0,7,7), // cyan
   matrix->Color333(7,0,7) // purple
  
};

uint16_t color_palette2[] =
{
   matrix->Color333(7,0,2), // magenta
   matrix->Color333(5,0,3), // pink
   matrix->Color333(3,0,7), // violet
   matrix->Color333(0,4,7), // aqua
   matrix->Color333(0,7,4), // azure
   matrix->Color333(2,4,2) // moss   
  
};

uint16_t color_palette3[] =
{
   matrix->Color333(1,1,1), // white
   matrix->Color333(1,0,0), // dim red
   matrix->Color333(0,0,0), // black
   matrix->Color333(7,7,7), // bright white
   matrix->Color333(3,3,3), // medium white
   matrix->Color333(0,0,0) // black   
  
};

uint16_t color_palette4[] =
{
   matrix->Color333(7,0,7), // purple
   matrix->Color333(7,7,0), // yellow
   matrix->Color333(7,4,7), // orange
   matrix->Color333(0,0,0), // black
   matrix->Color333(0,7,4), // azure
   matrix->Color333(7,0,2)  // magenta
  
};

uint16_t color_palette5[] = //America Pallette
{
   matrix->Color333(7,0,0), // red
   matrix->Color333(7,7,7), // white
   matrix->Color333(0,0,7), // blue
   matrix->Color333(0,0,7), // blue
   matrix->Color333(7,7,7), // white
   matrix->Color333(7,0,0) // red  
  
};

uint16_t color_palette6[] = // Univ. of Oregon Pallette
{
   matrix->Color333(0,7,0), //green
   matrix->Color333(7,7,0), //yellow
   matrix->Color333(5,7,0), // yellow-green
   matrix->Color333(7,7,7), // bright white
   matrix->Color333(0,7,0), //green
   matrix->Color333(3,7,0)  //green-yellow 
  
};


void setup() 
{
  matrix->begin();
  Serial.begin(9600);
}

void loop() 
{
  //This uses a loop to increment divisions from 0 to 2


  envelopeValue = analogRead(ENVELOPE_PIN);
  potValue = analogRead(POT_PIN);
  maxEnvelope = map(potValue,0,1024,20, 150);
  constrainedEnvelope = constrain(envelopeValue,7,maxEnvelope);
  divisions = map(constrainedEnvelope,6,maxEnvelope,0,2);
  drawCircles(divisions);
  Serial.print("envelopeValue = ");
  Serial.print(envelopeValue);
  Serial.print(" potValue = ");
  Serial.print(potValue); 
  Serial.print(" maxEnvelope = ");
  Serial.print(maxEnvelope);
  Serial.print(" constrainedEnvelope = ");
  Serial.print(constrainedEnvelope); 
  Serial.print(" divisions = ");
  Serial.print(divisions);  
 
}

void drawCircles(int passedDivisions)
{
   unsigned long startTime, endTime;

   radius = round(pow(2,5-passedDivisions))/2;
   rows_cols = round(pow(2,passedDivisions))*2;

 //  radius = round(powerOf2[5-passedDivisions])/2;
//   rows_cols = round(powerOf2[5-passedDivisions])*2;


  int modifier = 3;
  static int colorRandomizer;
  if (passedDivisions >= 1)
    colorRandomizer = random(0,6);
/* 
  Serial.print("envelopeValue = ");
  Serial.print(envelopeValue);
  Serial.print(" constrainedEnvelope = ");
  Serial.print(constrainedEnvelope);
  Serial.print(" divisions = ");
  Serial.print(divisions);    
  Serial.print(" passedDivisions = ");
  Serial.print(passedDivisions);
  Serial.print(" Radius = ");
  Serial.print(radius);
  Serial.print(" rows_cols = ");
  Serial.println(rows_cols);
  */
  
  startTime = millis();
  for (int a = 0; a < rows_cols; a++)
  {
    for (int b = 0; b < rows_cols; b++)
    {
      static int x, y;
      x = (b * radius*2)-radius;
      y = (a * radius*2)-radius;
      if (colorRandomizer%6==0)
        matrix->fillCircle(x,y,radius,color_palette1[(a+b)%2+colorRandomizer]);
      else if (colorRandomizer%6==1)
        matrix->fillCircle(x,y,radius,color_palette2[(a+b)%2+colorRandomizer]);
      else if (colorRandomizer%6==2)
        matrix->fillCircle(x,y,radius,color_palette3[(a+b)%2+colorRandomizer]);     
      else if (colorRandomizer%6==3)
        matrix->fillCircle(x,y,radius,color_palette4[(a+b)%2+colorRandomizer]);      
       else if (colorRandomizer%6==4)
        matrix->fillCircle(x,y,radius,color_palette5[(a+b)%2+colorRandomizer]);
      else
        matrix->fillCircle(x,y,radius,color_palette6[(a+b)%2+colorRandomizer]);
    }
    
  }
  matrix->show();
   
  endTime = millis();
  Serial.print(" Ellapsed time = ");
  Serial.println(endTime-startTime);
   
  
}
