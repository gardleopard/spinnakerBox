#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

#define LED_COUNT 5

#define FRONT 3
#define LEFT 0
#define BACK 1
#define RIGHT 2
#define TOP 4

String inputString = "";
boolean stringComplete = false;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(33, PIN, NEO_GRB + NEO_KHZ800);
int PINK[3] = {255,30,30};
int RED[3] = {255,0,0};
int GREEN[3] = {0,255,0};
int BLUE[3] = {0,0,255};
int WHITE[3] = {255,255,255};
int OFF[3] = {0,0,0};
int currentColor[3] = {0,0,0};
int SPACE=32;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  inputString.reserve(200);
  apiDoc();
}

void parseInput(){
  Serial.print("GOT: ");
  Serial.println(inputString);
  if(inputString[0] == 'O'){
    Serial.println("Shut light off");
    setAll(setAndGetColor(OFF));
  }
  if(inputString[0] == 'D') {
    Serial.println("DEMO!!!");
    demo(); 
  }

  if(inputString[0] == 'F') {
    Serial.println("FADE");
    String colors = inputString.substring(2);
    int color[3];
    populateColorCode(colors,color);
    fade(color);
  }

  if(inputString[0] == 'S') {
    char target = inputString.charAt(2);
    String colors = inputString.substring(4);
    int color[3];
    populateColorCode(colors,color);

    updateTarget(color, target);
  }
}

void updateTarget(int color[], char target) {
  if(target == 'A') {
    setAll(setAndGetColor(color));
  } 
  else {
    int ledIndex;
    if(target == 'F')
      ledIndex = FRONT;
    if(target == 'B')
      ledIndex = BACK;
    if(target == 'L')
      ledIndex = LEFT;
    if(target == 'R')
      ledIndex = RIGHT;
    if(target == 'T')
      ledIndex = TOP;
    setLed(setAndGetColor(color), ledIndex);
  }
}

void populateColorCode(String colorString, int color[]){
  int colorIndex = 0;
  String tmpColor = "";
  for(int i = 0 ; i < colorString.length(); i++) {
    char currentChar = colorString.charAt(i);
    if(currentChar == ' '){
      color[colorIndex++] = tmpColor.toInt();
      tmpColor = "";
    }
    tmpColor += currentChar; 
  }
  
  color[colorIndex++] = tmpColor.toInt();
  
}


void loop() {
if(stringComplete){
  parseInput();
  stringComplete = false;
  inputString = "";
}

 
}

void demo(){
fadeLoop();
}

void fadeLoop() {
     fade(PINK);
     delay(1000);
   fade(BLUE);
     delay(1000);
   fade(GREEN);
     delay(1000);
   fade(WHITE);
     delay(1000);
   fade(RED);
     delay(1000);
}

void fade(int color[]) {
  if(color[0] > currentColor[0]){
    currentColor[0] = currentColor[0] + 1;
  }
  if(color[1] > currentColor[1]){
    currentColor[1] = currentColor[1] + 1;
  }
  if(color[2] > currentColor[2]){
    currentColor[2] = currentColor[2] + 1;
  }
  if(color[0] < currentColor[0]){
    currentColor[0] = currentColor[0] - 1;
  }
  if(color[1] < currentColor[1]){
    currentColor[1] = currentColor[1] - 1;
  }
  if(color[2] < currentColor[2]){
    currentColor[2] = currentColor[2] - 1;
  }
  setAll(setAndGetColor(currentColor));
  if(color[0] != currentColor[0]) {
    fade(color);
  } else if(color[1] != currentColor[1]) {
    fade(color);
  } else if(color[2] != currentColor[2]) {
    fade(color);
  }
  
  
}

void apiDoc(){
  Serial.println("Set and fade");
Serial.println("F R G B");
Serial.println("S L R G B");
Serial.println("O/D");
Serial.println("FADE 0-255 0-255 0-255");
Serial.println("SET ALL/FRONT/BACK/LEFT/RIGHT 0-255 0-255 0-255");
Serial.println("OFF/DEMO");
Serial.println("[F/S/O/D] [A/F/B/L/R] R G B");
}

uint32_t setAndGetColor(int color[]) {
  currentColor[0] = color[0];
  currentColor[1] = color[1];
  currentColor[2] = color[2];
  return strip.Color(color[0], color[1], color[2]);
}

void setLed(uint32_t c, int index){
  strip.setPixelColor(index,c);
  strip.show();
}



void setAll(uint32_t c){
  for(int i =0 ; i < LED_COUNT ; i++) {
    strip.setPixelColor(i,c);
  }
  strip.show();
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}




