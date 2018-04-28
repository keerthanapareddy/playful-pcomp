#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>
#include <Bounce2.h>
#include <Keyboard.h>
#include <Adafruit_NeoPixel.h>

Encoder race(8, 9);
Encoder income(6, 7);
Encoder education(10, 11);

String incomeLabels[] = { "lower", "middle", "upper-middle", "upper" };
char incomeKeys[] = { 'Q', 'W', 'E', 'R' };
String raceLabels[] = { "black", "hispanic", "other", "asian", "white" };
char raceKeys[] = { 'A', 'S', 'D', 'F', 'G' };
String educationLabels[] = { "less-than-high-school", "high-school", "some-college", "bachelors", "advanced" };
char educationKeys[] = { 'Z', 'X', 'C', 'V', 'B' };

long oldIncomePosition = -1;
int incomeSelection = 0;

long oldRacePosition = -1;
int raceSelection = 0;

long oldEducationPosition = -1;
int educationSelection = 0;

Bounce resetButton = Bounce();
Bounce searchButton = Bounce();
Bounce randomButton = Bounce();

int searchButtonPin = 12;
int searchLedPin = 13;

int resetButtonPin = 2;
int resetLedPin = 3;

int randomButtonPin = 5;
int randomLedPin = 4;

//const int dinPin = 2;    // Din pin to Arduino pin 4
//const int numOfLeds = 48;

//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numOfLeds, dinPin, NEO_GRB + NEO_KHZ800);

int red = 255;    //Value from 0(led-off) to 255().
int green = 0;
int blue = 0;

int incomingByte;
int sceneNumber;

void setup() {
  Keyboard.begin();
  Serial.begin(115200);
  pinMode(resetButtonPin, INPUT_PULLUP);
  pinMode(searchButtonPin, INPUT_PULLUP);
  pinMode(randomButtonPin, INPUT_PULLUP);
  pinMode(resetLedPin, OUTPUT);
  pinMode(searchLedPin, OUTPUT);
  pinMode(randomLedPin, OUTPUT);

  searchButton.attach(searchButtonPin);
  searchButton.interval(5);
  resetButton.attach(resetButtonPin);
  resetButton.interval(5);
  randomButton.attach(randomButtonPin);
  randomButton.interval(5);
}

void loop() {
  //== Check all our encodersASSSSSSSSDDDDDDDDFFFFFFFFGGGGGGGGAAAAAAAASSSS
  checkIncomeEncoder();
  checkRaceEncoder();
  checkEducationEncoder();

  //== Check all our buttons
  checkSearch();
  checkReset();
  checkRandom();

//  lights();

  if (Serial.available() > 0) {   // see if there's incoming serial data
    incomingByte = Serial.read(); // read it
    sceneNumber = incomingByte;
  }

  //== global delay
  delay(1);
}

// Random utility to "constrain" but wrapover
int rollover(int x, int lower, int upper) {
  int range = upper - lower;
  while (x < lower) x += range;
  while (x >= upper) x -= range;

  return x;
}

int getEncoderSelection(int x, int num_selections) {
  x = rollover(x, 0, 80);

  // NB. The OBOB is not a bug. It's important!
  return map(x, 0, 80, 0, num_selections);
}

void checkIncomeEncoder() {
  long incomePosition = income.read();

  if (incomePosition != oldIncomePosition) {
    incomeSelection = getEncoderSelection(incomePosition, 4);
    oldIncomePosition = incomePosition;


    Keyboard.write(incomeKeys[incomeSelection]);
    Serial.print("income,");
    Serial.println(incomeLabels[incomeSelection]);
  }
}

void checkRaceEncoder() {
  long racePosition = race.read();

  if (racePosition != oldRacePosition) {
    raceSelection = getEncoderSelection(racePosition, 5);
    oldRacePosition = racePosition;

    Keyboard.write(raceKeys[raceSelection]);
    Serial.print("race,");
    Serial.println(raceLabels[raceSelection]);
  }
}

void checkEducationEncoder() {
  long educationPosition = education.read();

  if (educationPosition != oldEducationPosition) {
    educationSelection = getEncoderSelection(educationPosition, 5);
    oldEducationPosition = educationPosition;

    Keyboard.write(educationKeys[educationSelection]);
    Serial.print("education,");
    Serial.println(educationLabels[educationSelection]);
  }
}

void checkReset() {
  resetButton.update();

  if (resetButton.fell()) {
    digitalWrite(resetLedPin, HIGH);
    Keyboard.write('I');
    Serial.print("button,");
    Serial.println("reset");
  }
}

void checkSearch() {
  searchButton.update();

  if (searchButton.fell()) {
    digitalWrite(searchLedPin, HIGH);
    Keyboard.write('O');
    Serial.print("button,");
    Serial.println("search");
  }
}

void checkRandom() {
  randomButton.update();

  if (randomButton.fell()) {
    digitalWrite(randomLedPin, HIGH);
    Keyboard.write('P');
    Serial.print("button,");
    Serial.println("random");
  }
}
