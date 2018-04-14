/* Encoder Library - NoInterrupts Example
   http://www.pjrc.com/teensy/td_libs_Encoder.html

   This example code is in the public domain.
*/


#include <Encoder.h>


Encoder race(4, 5);
Encoder income(6, 7);
Encoder education(2, 3);

long racePosition;
long oldRacePosition  = -999;

long incomePosition;
int incomeCounter = 80;
long oldIncomePosition  = -999;

long educationPosition;
long oldEducationPosition  = -999;
int educationCounter = 159;

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
  Serial.begin(9600);
  pinMode(searchButtonPin, INPUT_PULLUP);
  pinMode(resetButtonPin, INPUT_PULLUP);
  pinMode(randomButtonPin, INPUT_PULLUP);
  pinMode(randomLedPin, OUTPUT);
  pinMode(resetLedPin, OUTPUT);
  pinMode(searchLedPin, OUTPUT);
}

void loop() {

  racePosition = race.read();
  if (racePosition != oldRacePosition) {
    if (racePosition > oldRacePosition) {

      //      Serial.println("clockwise");
    } else {
      //      Serial.println("counter clockwise");
    }

    oldRacePosition = racePosition;
    if (racePosition > 79) {
      racePosition = racePosition % 80;
    }

    while (racePosition < 0) {
      racePosition += 80;
    }
    Serial.println(racePosition);
  }
  delay(1);

  //INCOME ROTARY ENCODER
  incomePosition = income.read();
  if (incomePosition != oldIncomePosition) {
    if (incomePosition > oldIncomePosition) {
      incomeCounter++;
      //            Serial.println("clockwise");
    } else {
      incomeCounter--;
      //            Serial.println("counter clockwise");
    }

    oldIncomePosition = incomePosition;
    if (incomeCounter > 159) {
      incomeCounter = 80;
      //     incomeCounter = incomeCounter % 80;
    }

    while ( incomeCounter < 80) {
      incomeCounter += 80;
    }
    //    Serial.println(racePosition);
    Serial.println(incomeCounter);
  }
  delay(1);

  //EDUCATION ENCODER
  educationPosition = education.read();
  if (educationPosition != oldEducationPosition) {
    if (educationPosition > oldEducationPosition) {
      educationCounter++;
      //            Serial.println("clockwise");
    } else {
      educationCounter--;
      //            Serial.println("counter clockwise");
    }

    oldEducationPosition = educationPosition;
    if ( educationCounter > 238) {
      educationCounter = 80;
      //     incomeCounter = incomeCounter % 80;
    }

    while (educationCounter < 160) {
      educationCounter += 80;
    }
    //        Serial.println(racePosition);
    Serial.println( educationCounter);
  }
  search(); //callback functions
  reset();
  randomplay();
  delay(50);
}

void search() {
  int searchButtonState = digitalRead(searchButtonPin);

  if ( searchButtonState != lastSearchButtonState) {
    if (searchButtonState == LOW) {
      digitalWrite(searchLedPin, HIGH);
      //      counter++;
      Serial.println("search pressed");
      //      Serial.println("163");

    }
    else {
      digitalWrite(searchLedPin, LOW);
      Serial.println("search released");
    }

    delay(10);
  }

  lastSearchButtonState = searchButtonState;
}

void reset() {
  int resetButtonState = digitalRead(resetButtonPin);

  if ( resetButtonState != lastResetButtonState) {
    if (resetButtonState == LOW) {
      digitalWrite(resetLedPin, HIGH);
      //      counter++;
      Serial.println("reset pressed");
      //      Serial.println(counter);
    }
    else {
      digitalWrite(resetLedPin, LOW);
      Serial.println("reset released");
    }
    delay(10);
  }
  lastResetButtonState = resetButtonState;
}

void randomplay() {
  int randomButtonState = digitalRead(randomButtonPin);

  if ( randomButtonState != lastRandomButtonState) {
    if (randomButtonState == LOW) {
      digitalWrite(randomLedPin, HIGH);
      //      counter++;
      Serial.println("random pressed");
      //      Serial.println(counter);
    }
    else {
      Serial.println("random released");
      digitalWrite(randomLedPin, LOW);
    }
    delay(10);
  }
  lastRandomButtonState = randomButtonState;
}



