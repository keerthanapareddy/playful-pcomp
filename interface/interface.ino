/* Encoder Library - NoInterrupts Example
   http://www.pjrc.com/teensy/td_libs_Encoder.html

   This example code is in the public domain.
*/

#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>
#include <Bounce2.h>

Encoder race(4, 5);
Encoder income(6, 7);
Encoder education(2, 3);

String raceLabels[] = { "black", "hispanic", "other", "asian", "white" };
String incomeLabels[] = { "lower", "middle", "upper-middle", "upper" };
String educationLabels[] = { "less-than-high-school", "high-school", "some-college", "bachelors", "advanced" };

long oldIncomePosition = -1;
int incomeSelection = 0;

long oldRacePosition = -1;
int raceSelection = 0;

long oldEducationPosition = -1;
int educationSelection = 0;

Bounce resetButton = Bounce();
Bounce searchButton = Bounce();
Bounce randomButton = Bounce();

int searchButtonPin = 10;
int searchLedPin = 11;

int resetButtonPin = 9;
int resetLedPin = 8;

int randomButtonPin = 13;
int randomLedPin = 12;

void setup() {
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
  //== Check all our encoders
  checkIncomeEncoder();
  checkRaceEncoder();
  checkEducationEncoder();

  //== Check all our buttons
  checkSearch();
  checkReset();
  checkRandom();

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

    Serial.print("income,");
    Serial.println(incomeLabels[incomeSelection]);
  }
}

void checkRaceEncoder() {
  long racePosition = race.read();

  if (racePosition != oldRacePosition) {
    raceSelection = getEncoderSelection(racePosition, 5);
    oldRacePosition = racePosition;

    Serial.print("race,");
    Serial.println(raceLabels[raceSelection]);
  }
}

void checkEducationEncoder() {
  long educationPosition = education.read();

  if (educationPosition != oldEducationPosition) {
    educationSelection = getEncoderSelection(educationPosition, 5);
    oldEducationPosition = educationPosition;

    Serial.print("education,");
    Serial.println(educationLabels[educationSelection]);
  }
}

void checkSearch() {
  searchButton.update();

  if (searchButton.fell()) {
    digitalWrite(searchLedPin, HIGH);
    Serial.print("button,");
    Serial.println("search");
  }
}

void checkReset() {
  resetButton.update();

  if (resetButton.fell()) {
    digitalWrite(resetLedPin, HIGH);
    Serial.print("button,");
    Serial.println("reset");
  }
}

void checkRandom() {
  randomButton.update();

  if (randomButton.fell()) {
    digitalWrite(randomLedPin, HIGH);
    Serial.print("button,");
    Serial.println("random");
  }
}



