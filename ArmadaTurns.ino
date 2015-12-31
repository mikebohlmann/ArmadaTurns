#include <Bridge.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// If you want to use this without a speaker, remove the Bridge library and all references
// to playing sounds

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// pos 0,16 is beginning of blue

// leftButton reduces the current turn
int leftButton = 3;

// rightButton increases the current turn
int rightButton = 2;

// pewpewButton plays a random sound
int pewpewButton = 4;

// currentTurn keeps track of the current turn
int currentTurn = 0;

// bitmap for the blue side of the activation token
static const unsigned char PROGMEM blueSide[] =
{ B11111111, B00000000, B00000000, B00000000, B00001111, B11111111,
  B11111111, B00000000, B00000000, B00000000, B01111111, B11111100,
  B11111111, B00000000, B00000000, B00000000, B11111111, B11111000,
  B11111111, B00000000, B00000000, B00000011, B11111111, B11100000,
  B11111111, B00000000, B00000000, B00000111, B11111111, B11000000,
  B11111111, B00000000, B00000000, B00011111, B11111111, B00000000,
  B11111111, B00000000, B00000000, B00111111, B11111110, B00000000,
  B11111111, B11111111, B11111111, B11111111, B11111000, B00000000,
  B11111111, B11111111, B11111111, B11111111, B11110000, B00000000,
  B11111111, B11111111, B11111111, B11111111, B11000000, B00000000,
  B11111111, B11111111, B11111111, B11111111, B10000000, B00000000,
  B11111111, B11111111, B11111111, B11111110, B00000000, B00000000,
  B11111111, B11111111, B11111111, B11111000, B00000000, B00000000,
  B11111111, B11111111, B11111111, B11100000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B11111111, B11111111, B11111111, B11100000, B00000000, B00000000,
  B11111111, B11111111, B11111111, B11111000, B00000000, B00000000,
  B11111111, B11111111, B11111111, B11111110, B00000000, B00000000,
  B11111111, B11111111, B11111111, B11111111, B10000000, B00000000,
  B11111111, B11111111, B11111111, B11111111, B11000000, B00000000,
  B11111111, B11111111, B11111111, B11111111, B11110000, B00000000,
  B11111111, B11111111, B11111111, B11111111, B11111000, B00000000,
  B11111111, B00000000, B00000000, B00111111, B11111110, B00000000,
  B11111111, B00000000, B00000000, B00011111, B11111111, B00000000,
  B11111111, B00000000, B00000000, B00000111, B11111111, B11000000,
  B11111111, B00000000, B00000000, B00000011, B11111111, B11100000,
  B11111111, B00000000, B00000000, B00000000, B11111111, B11111000,
  B11111111, B00000000, B00000000, B00000000, B01111111, B11111100,
  B11111111, B00000000, B00000000, B00000000, B00001111, B11111111
  };

// bitmap for the red side of the activation token
static const unsigned char PROGMEM redSide[] =
{ B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00001111, B11111111, B11111111, B11110000, B00000000,
  B00000000, B00011111, B11111111, B11111111, B11111000, B00000000,
  B00000000, B01111111, B11111111, B11111111, B11111100, B00000000,
  B00000000, B01111111, B11111111, B11111111, B11111110, B00000000,
  B00000001, B11111111, B11111111, B11111111, B11111111, B10000000,
  B00000011, B11111111, B11111111, B11111111, B11111111, B11000000,
  B00000111, B11111111, B11111111, B11111111, B11111111, B11100000,
  B00001111, B11111110, B00000000, B00000000, B01111111, B11110000,
  B00011111, B11111000, B00000000, B00000000, B00011111, B11111000,
  B00111111, B11100000, B00000000, B00000000, B00000111, B11111100,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00111111, B11111100, B00000000, B00000000,
  B00000000, B00000000, B01111111, B11111110, B00000000, B00000000,
  B00000000, B00000000, B01111111, B11111110, B00000000, B00000000,
  B00000000, B00000000, B01111111, B11111110, B00000000, B00000000,
  B00000000, B00000000, B00111111, B11111100, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00111111, B11100000, B00000000, B00000000, B00000111, B11111100,
  B00011111, B11111000, B00000000, B00000000, B00011111, B11111000,
  B00001111, B11111110, B00000000, B00000000, B01111111, B11110000,
  B00000111, B11111111, B11111111, B11111111, B11111111, B11100000,
  B00000011, B11111111, B11111111, B11111111, B11111111, B11000000,
  B00000001, B11111111, B11111111, B11111111, B11111111, B10000000,
  B00000000, B01111111, B11111111, B11111111, B11111110, B00000000,
  B00000000, B01111111, B11111111, B11111111, B11111100, B00000000,
  B00000000, B00011111, B11111111, B11111111, B11111000, B00000000,
  B00000000, B00001111, B11111111, B11111111, B11110000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
  };

void setup() {

  pinMode(leftButton, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(pewpewButton, INPUT);

  // initialize the display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();

  // display a "loading" message while the Yun's linux boots
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("Preparing hyperspace jump...");

  display.drawBitmap(15, 24,  blueSide, 48, 32, 1);
  display.drawBitmap(73, 24,  redSide, 48, 32, 1);

  display.display();

  Bridge.begin();
  delay(5000);

  // once linux and the Arduino chip are talking, play the main theme
  
  Process p;
  p.runShellCommand("madplay -Q --adjust-volume=-15 --time=00:00:17.110 /mnt/sd/sounds/StarWarsTheme.mp3");
  // this shouldn't matter, but it can be useful just to be sure the sound finishes
  while(p.running()); 

  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("Press right button to\rstart game.");

  display.drawBitmap(15, 24,  blueSide, 48, 32, 1);
  display.drawBitmap(73, 24,  redSide, 48, 32, 1);
  
  display.display();
}

void loop() {

  if (digitalRead(pewpewButton) == LOW) {
    Process paudio;
    paudio.runShellCommand("~/playRandom");
    while(paudio.running());
  }

  if (digitalRead(leftButton) == LOW && currentTurn > 1) {
    currentTurn -= 1;
    updateTurn();
  }

  if (digitalRead(rightButton) == LOW && currentTurn < 6) {
    currentTurn += 1;

    updateTurn();
    playAudio();
  }

}

void updateTurn() {
    String displayText = "";
    
    display.clearDisplay();

    display.setCursor(0,0);
    display.setTextColor(WHITE);
    display.setTextSize(2);

    displayText = "Turn: " + String(currentTurn); 
    display.println(displayText);
    
    display.setCursor(0,16);
    display.setTextColor(WHITE);
    display.setTextSize(3);

    if (currentTurn % 2 == 1) {
      display.drawBitmap(40, 24,  blueSide, 48, 32, 1);
    } else {
      display.drawBitmap(40, 24,  redSide, 48, 32, 1);
    }

    display.display();
    delay(200); // found that the screen didn't like redrawing this quickly
}

void playAudio() {

  String playSound = "";

  // I had to create shell scripts to play the sounds because the filenames here used too
  // much program memory to properly run.

  switch(currentTurn) {
    case 1:
      playSound = "~/playTurn1";
      break;
    case 2:
      playSound = "~/playTurn2";
      break;
    case 3:
      playSound = "~/playTurn3";
      break;
    case 4:
      playSound = "~/playTurn4";
      break;
    case 5:
      playSound = "~/playTurn5";
      break;
    case 6:
      playSound = "~/playTurn6";
      break;
    default:
    break;     
  } 

  Process paudio;
  paudio.runShellCommand(playSound);
  while(paudio.running());
  
}

