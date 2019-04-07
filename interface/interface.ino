#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>
#include <Bounce2.h>
#include <Keyboard.h>
#include <Wire.h>

Encoder income(6, 7);
Encoder race(8, 9);
Encoder education(10, 13);

String incomeLabels[] = { "lower", "middle", "upper-middle", "upper" };
char incomeKeys[] = { 'Q', 'W', 'E', 'R' };
String raceLabels[] = { "black", "asian", "white", "hispanic", "other" };
char raceKeys[] = { 'A', 'G', 'F', 'S', 'D' };
String educationLabels[] = { "less-than-high-school", "high-school", "some-college", "bachelors", "advanced" };
char educationKeys[] = { 'Z', 'X', 'C', 'V', 'B' };

long oldIncomePosition = 0;
int incomeSelection = 0;
int lastIncomeSpin = NULL;

long oldRacePosition = 0;
int raceSelection = 0;
int lastRaceSpin = NULL;

long oldEducationPosition = 0;
int educationSelection = 0;
int lastEducationSpin = NULL;

Bounce resetButton = Bounce();
Bounce randomButton = Bounce();

Bounce searchButton1 = Bounce();
int searchButtonPin1 = 4;
int searchLedPin1 = 5;

Bounce searchButton2 = Bounce();
int searchButtonPin2 = A2;
int searchLedPin2 = A1;

Bounce searchButton3 = Bounce();
int searchButtonPin3 = A4;
int searchLedPin3 = A3;

long lastSearchDebounceTime = 0;
long debounceDelay = 50;

int resetButtonPin = 2;
int resetLedPin = 3;

int randomButtonPin = 14;
bool firstSpin = true;

String sceneName = "Intro";
String prevSceneName;

int lastStartedAt;
int searchingStartedAt;

void setup() {
  Wire.begin();
  Keyboard.begin();
  Serial.begin(115200);
  pinMode(resetButtonPin, INPUT_PULLUP);
  pinMode(randomButtonPin, INPUT_PULLUP);
  pinMode(searchButtonPin1, INPUT_PULLUP);
  pinMode(searchButtonPin2, INPUT_PULLUP);
  pinMode(searchButtonPin3, INPUT_PULLUP);
  pinMode(resetLedPin, OUTPUT);
  pinMode(searchLedPin1, OUTPUT);
  pinMode(searchLedPin2, OUTPUT);
  pinMode(searchLedPin3, OUTPUT);

  resetButton.attach(resetButtonPin);
  resetButton.interval(5);
  randomButton.attach(randomButtonPin);
  randomButton.interval(5);
  searchButton1.attach(searchButtonPin1);
  searchButton1.interval(10);
  searchButton2.attach(searchButtonPin2);
  searchButton2.interval(10);
  searchButton3.attach(searchButtonPin3);
  searchButton3.interval(10);

  setSceneLighting();

  lastStartedAt = millis();
}

void loop() {
  //== Check scene change and set initial lighting
  if (Serial.available() > 0) {
    String incomingString = Serial.readString();
    Serial.println(incomingString);
    sceneName = incomingString;

    setSceneLighting();
  }

  //== Complex lighting
  if (sceneName == "Intro") {
    int elapsedTime = millis() - lastStartedAt;

    if (elapsedTime / 700 % 2 == 0) {
      searchLedsHigh();
    } else {
      searchLedsLow();
    }
  }

  // if (sceneName == "Searching") {
  //   int elapsedTime = millis() - searchingStartedAt;
  //   int progress = elapsedTime / 500 % 3;
  //   searchLeds(progress);
  // }

  //== Check all our encoders
  if (sceneName == "Profile") {
    checkIncomeEncoder();
    checkRaceEncoder();
    checkEducationEncoder();
  }

  // Always active
  checkReset();
  checkSearch();

  // Only on slot pull scenes
  if (sceneName == "Prompt" || sceneName == "RandomGame") {
    checkRandom();
  }

  //== Global delay
  delay(1);
}

void sendToUno(char key) {
  Wire.requestFrom(9, 1);
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(key);
  Wire.endTransmission();    // stop transmitting

  return;
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
    sendToUno(incomeKeys[incomeSelection]);
  }
}

void checkRaceEncoder() {
  long racePosition = race.read();

  if (racePosition != oldRacePosition) {
    raceSelection = getEncoderSelection(racePosition, 5);
    oldRacePosition = racePosition;

    Keyboard.write(raceKeys[raceSelection]);
    sendToUno(raceKeys[raceSelection]);
  }
}

void checkEducationEncoder() {
  long educationPosition = education.read();

  if (educationPosition != oldEducationPosition) {
    educationSelection = getEncoderSelection(educationPosition, 5);
    oldEducationPosition = educationPosition;

    Keyboard.write(educationKeys[educationSelection]);
    sendToUno(educationKeys[educationSelection]);
  }
}

void checkReset() {
  resetButton.update();
  if (resetButton.fell()) {
    digitalWrite(resetLedPin, HIGH);
    Keyboard.write('I');

    resetGame();
  }
}

void resetGame() {
  prevSceneName = "NULL";
  oldIncomePosition = income.read();
  oldRacePosition = race.read();
  oldEducationPosition = education.read();
  lastStartedAt = millis();
  firstSpin = true;
}

void checkSearch() {
  searchButton1.update();
  searchButton2.update();
  searchButton3.update();

  long currentTime = millis();

  if (currentTime - lastSearchDebounceTime < debounceDelay) {
    return;
  }

  if (searchButton1.fell() || searchButton2.fell() || searchButton3.fell()) {
    lastSearchDebounceTime = currentTime;

    searchLedsHigh();
    Keyboard.write('O');
  }
}

void searchLeds(int n) {
  if (n == 0) {
    digitalWrite(searchLedPin1, HIGH);
    digitalWrite(searchLedPin2, LOW);
    digitalWrite(searchLedPin3, LOW);
  } else if (n == 1) {
    digitalWrite(searchLedPin1, HIGH);
    digitalWrite(searchLedPin2, HIGH);
    digitalWrite(searchLedPin3, LOW);
  } else {
    digitalWrite(searchLedPin1, HIGH);
    digitalWrite(searchLedPin2, HIGH);
    digitalWrite(searchLedPin3, HIGH);
  }
}

void searchLedsHigh() {
  digitalWrite(searchLedPin1, HIGH);
  digitalWrite(searchLedPin2, HIGH);
  digitalWrite(searchLedPin3, HIGH);
}

void searchLedsLow() {
  digitalWrite(searchLedPin1, LOW);
  digitalWrite(searchLedPin2, LOW);
  digitalWrite(searchLedPin3, LOW);
}

void checkRandom() {
  randomButton.update();

  if (randomButton.fell()) {
    Keyboard.write('P');

    // NB slot sound is 2.821224 sec
    for (int x = 0; x < 14; x++) {
      if (firstSpin && x == 13) {
        firstSpin = false;
        spinnerRaceLoaded();
      } else {
        spinnerRace();
      }

      spinnerIncome();
      spinnerEducation();

      delay(202);
    }
  }
}

void setSceneLighting() {
  if (sceneName == prevSceneName) return;

  if (sceneName == "Intro") {
    resetGame();
    sendToUno('I'); // reset knob lights
    searchLedsHigh();
    digitalWrite(resetLedPin, LOW);
  }

  if (sceneName == "Profile") {
    searchLedsHigh();
  }

  if (sceneName == "Searching") {
    searchingStartedAt = millis();
    searchLedsHigh();
  }

  if (sceneName == "Game") {
    searchLedsHigh();
  }

  if (sceneName == "Explore") {
    searchLedsHigh();
  }

  if (sceneName == "Prompt") {
    sendToUno('I'); // reset knob lights
    searchLedsLow();
  }

  if (sceneName == "RandomProfile") {
    searchLedsLow();
  }

  if (sceneName == "RandomGame") {
    searchLedsHigh();
  }

  if (sceneName == "Outro") {
    sendToUno('0');
    searchLedsLow();
    digitalWrite(resetLedPin, HIGH);
  }

  prevSceneName = sceneName;
}

void spinnerIncome() {
  int randomSelection = random(4);
  while (randomSelection == lastIncomeSpin) {
    randomSelection = random(4);
  }

  Keyboard.write(incomeKeys[randomSelection]);
  sendToUno(incomeKeys[randomSelection]);
  lastIncomeSpin = randomSelection;
}

void spinnerRace() {
  int randomSelection = random(5);
  while (randomSelection == raceSelection || randomSelection == lastRaceSpin) {
    randomSelection = random(5);
  }

  Keyboard.write(raceKeys[randomSelection]);
  sendToUno(raceKeys[randomSelection]);
  lastRaceSpin = randomSelection;
}

void spinnerRaceLoaded() {
  // If asian or white, show black
  if (raceSelection >= 3) {
    Keyboard.write(raceKeys[0]);
    sendToUno(raceKeys[0]);
  } else { // show white
    Keyboard.write(raceKeys[4]);
    sendToUno(raceKeys[4]);
  }
}

void spinnerEducation() {
  int randomSelection = random(5);
  while (randomSelection == lastEducationSpin) {
    randomSelection = random(4);
  }

  Keyboard.write(educationKeys[randomSelection]);
  sendToUno(educationKeys[randomSelection]);
  lastEducationSpin = randomSelection;
}
