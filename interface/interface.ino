/* Encoder Library - NoInterrupts Example
   http://www.pjrc.com/teensy/td_libs_Encoder.html

   This example code is in the public domain.
*/

#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>

Encoder race(4, 5);
Encoder income(6, 7);
Encoder education(2, 3);

String raceLabels[] = { "black", "white", "asian", "hispanic", "other" };
String incomeLabels[] = { "lower", "middle", "upper-middle", "upper" };
String educationLabels[] = { "less-than-high-school", "high-school", "some-college", "bachelors", "advanced" };

long oldIncomePosition  = -999;
int incomeSelection = 0;

long oldRacePosition  = -999;
int raceSelection = 0;

long oldEducationPosition  = -999;
int educationSelection = 0;

int lastSearchButtonState = HIGH;
int lastResetButtonState = HIGH;
int lastRandomButtonState = HIGH;

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
}

// Random utility to "constrain" but wrapover
int rollover(int x, int lower, int upper) {
  int range = upper - lower;
  while (x < lower) x += range;
  while (x >= upper) x -= range;

  return x;
}

int getEncoderSelection(int x, int num_selections) {
  x = rollover(x, 0, num_selections);

  // NB. The OBOB is not a bug. It's important!
  //return map(x, 0, 80, 0, num_selections);

  return x;
}

void checkIncomeEncoder() {
  long incomePosition = income.read();

  if (incomePosition != oldIncomePosition) {
    if (incomePosition > oldIncomePosition) {
      incomeSelection = getEncoderSelection(incomeSelection + 1, 4);
    } else {
      incomeSelection = getEncoderSelection(incomeSelection - 1, 4);
    }

    oldIncomePosition = incomePosition;

    Serial.print("income,");
    Serial.println(incomeLabels[incomeSelection]);
  }
}

void checkRaceEncoder() {
  long racePosition = race.read();

  if (racePosition != oldRacePosition) {
    if (racePosition > oldRacePosition) {
      raceSelection = getEncoderSelection(raceSelection + 1, 5);
    } else {
      raceSelection = getEncoderSelection(raceSelection - 1, 5);
    }

    oldRacePosition = racePosition;

    Serial.print("race,");
    Serial.println(raceSelection);
    //Serial.println(raceLabels[raceSelection]);
  }
}


void checkEducationEncoder() {
  long educationPosition = education.read();

  if (educationPosition != oldEducationPosition) {
    if (educationPosition > oldEducationPosition) {
      educationSelection = getEncoderSelection(educationSelection + 1, 5);
    } else {
      educationSelection = getEncoderSelection(educationSelection - 1, 5);
    }

    oldEducationPosition = educationPosition;

    Serial.print("education,");
    Serial.println(educationLabels[educationSelection]);
  }
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
  delay(50);
}

void checkSearch() {
  int searchButtonState = digitalRead(searchButtonPin);

  if (searchButtonState != lastSearchButtonState) {
    if (searchButtonState == LOW) {
      digitalWrite(searchLedPin, HIGH);
      Serial.print("button,");
      Serial.println("search");
    }
  }

  lastSearchButtonState = searchButtonState;
}

void checkReset() {
  int resetButtonState = digitalRead(resetButtonPin);

  if (resetButtonState != lastResetButtonState) {
    if (resetButtonState == LOW) {
      digitalWrite(resetLedPin, HIGH);
      Serial.print("button,");
      Serial.println("reset");
    }
  }

  lastResetButtonState = resetButtonState;
}

void checkRandom() {
  int randomButtonState = digitalRead(randomButtonPin);

  if (randomButtonState != lastRandomButtonState) {
    if (randomButtonState == LOW) {
      digitalWrite(randomLedPin, HIGH);
      Serial.print("button,");
      Serial.println("random");
    }
  }

  lastRandomButtonState = randomButtonState;
}



