#include "pitches.h"
/*
  Displays the numbers 0-9 on the display, at one second intervals
  Implemented using 5611AH - 8-segment display
  Pin assignment for 5611AH
     G  F com A  B
   __|__|__|__|__|__
  | 10  9  8  7  6  |
  |  1  2  3  4  5  |
   --|--|--|--|--|--
     E  D com C  P

  A=7,B=6,C=4,D=2,E=1,F=9,G=10,P=5,com=GND
  LED segment assignment corresponding to pins
    A
   ---
  F |   | B
  | G |
   ---
  E |   | C
  |   |
   ---  . P
    D
*/

const uint8_t pinA = 15;
const uint8_t pinB = 16;
const uint8_t pinC = 11;
const uint8_t pinD = 10;
const uint8_t pinE = 9;
const uint8_t pinF = 14;
const uint8_t pinG = 13;
const uint8_t pinP = 12;
const uint8_t redLED = A3;
const uint8_t greenLED = A4;
const uint8_t buzzer = A5;

byte rows[] = {2, 3, 4};
const int rowCount = sizeof(rows) / sizeof(rows[0]);

byte cols[] = {5, 6, 7, 8};
const int colCount = sizeof(cols) / sizeof(cols[0]);

byte keys[colCount][rowCount];
uint8_t numbers[colCount][rowCount] = {
  {10, 0, 10},
  {7, 8, 9},
  {4, 5, 6},
  {1, 2, 3}
};

struct NUMBER {
  uint8_t number;
  bool isValidNumber;
};

NUMBER firstLevel[4];
NUMBER firstLevelSolution[2];
NUMBER secondLevel[8];
NUMBER secondLevelSolution[4];
NUMBER thirdLevel[12];
NUMBER thirdLevelSolution[6];
NUMBER fourthLevel[16];
NUMBER fourthLevelSolution[8];

uint8_t currentLevel = 1;
uint8_t currentLevelIndex = 0;
bool sequencePlayedOnce = false;

bool INTERACTION_SOLVED, INTERACTION_RUNNING;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinE, OUTPUT);
  pinMode(pinF, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinP, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
}

void loop() {
  if (!Serial) {
    Serial.begin(9600);
  }
  if (Serial.available()) {
    processSerialMessage();
  }
  if (INTERACTION_SOLVED == false && INTERACTION_RUNNING == true) {
    gameLoop();
  }
}

void gameLoop() {
  if (!sequencePlayedOnce) {
    Serial.println("Lighting Up Sequence");
    if (currentLevel == 1) {
      Serial.println("Lighting Up Sequence For Level 1");
      for (uint8_t i = 0; i < 4; i++) {
        printNumber(firstLevel[i]);
      }
    }
    else if (currentLevel == 2) {
      Serial.println("Lighting Up Sequence For Level 2");
      for (uint8_t i = 0; i < 8; i++) {
        printNumber(secondLevel[i]);
      }
    }
    else if (currentLevel == 3) {
      Serial.println("Lighting Up Sequence For Level 3");
      for (uint8_t i = 0; i < 12; i++) {
        printNumber(thirdLevel[i]);
      }
    }
    else if (currentLevel == 4) {
      Serial.println("Lighting Up Sequence For Level 4");
      for (uint8_t i = 0; i < 16; i++) {
        printNumber(fourthLevel[i]);
      }
    }
    sequencePlayedOnce = true;
  }
  checkPressedButton(readMatrixAndReturnPressedKey());
  delay(225);
}

void checkPressedButton(int8_t selectedNumber) {
  if (selectedNumber == -1) {
    return;
  }
  NUMBER number;
  number.number = selectedNumber;
  printNumber(number);
  if (currentLevel == 1) {
    if (selectedNumber == firstLevelSolution[currentLevelIndex].number) {
      if (currentLevelIndex == 1) {
        currentLevelIndex = 0;
        currentLevel++;
        sequencePlayedOnce = false;
        digitalWrite(greenLED, HIGH);
        tone(buzzer, NOTE_FS7, 1500);
        delay(1500);
        digitalWrite(greenLED, LOW);
      } else {
        currentLevelIndex++;
      }
    } else {
      sequencePlayedOnce = false;
      digitalWrite(redLED, HIGH);
      tone(buzzer, NOTE_GS2, 1500);
      delay(1500);
      digitalWrite(redLED, LOW);
      currentLevelIndex = 0;
      currentLevel = 1;
    }
  } else if (currentLevel == 2) {
    if (selectedNumber == secondLevelSolution[currentLevelIndex].number) {
      if (currentLevelIndex == 3) {
        currentLevelIndex = 0;
        currentLevel++;
        sequencePlayedOnce = false;
        digitalWrite(greenLED, HIGH);
        tone(buzzer, NOTE_FS7, 1500);
        delay(1500);
        digitalWrite(greenLED, LOW);
      } else {
        currentLevelIndex++;
      }
    } else {
      sequencePlayedOnce = false;
      digitalWrite(redLED, HIGH);
      tone(buzzer, NOTE_GS2, 1500);
      delay(1500);
      digitalWrite(redLED, LOW);
      currentLevelIndex = 0;
      currentLevel = 1;
    }
  } else if (currentLevel == 3) {
    if (selectedNumber == thirdLevelSolution[currentLevelIndex].number) {
      if (currentLevelIndex == 5) {
        currentLevelIndex = 0;
        currentLevel++;
        sequencePlayedOnce = false;
        digitalWrite(greenLED, HIGH);
        tone(buzzer, NOTE_FS7, 1500);
        delay(1500);
        digitalWrite(greenLED, LOW);
      } else {
        currentLevelIndex++;
      }
    } else {
      sequencePlayedOnce = false;
      digitalWrite(redLED, HIGH);
      tone(buzzer, NOTE_GS2, 1500);
      delay(1500);
      digitalWrite(redLED, LOW);
      currentLevelIndex = 0;
      currentLevel = 1;
    }
  } else if (currentLevel == 4) {
    if (selectedNumber == fourthLevelSolution[currentLevelIndex].number) {
      if (currentLevelIndex == 7) {
        currentLevelIndex = 0;
        currentLevel++;
        sequencePlayedOnce = false;
        digitalWrite(greenLED, HIGH);
        tone(buzzer, NOTE_FS7, 1500);
        delay(1500);
        digitalWrite(greenLED, LOW);
        checkWinning();
      } else {
        currentLevelIndex++;
      }
    } else {
      sequencePlayedOnce = false;
      digitalWrite(redLED, HIGH);
      tone(buzzer, NOTE_GS2, 1500);
      delay(1500);
      digitalWrite(redLED, LOW);
      currentLevelIndex = 0;
      currentLevel = 1;
    }
  }
}


void printNumber(NUMBER numberToPrint) {
  Serial.println(numberToPrint.number);
  if (numberToPrint.number == 1) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, LOW);
    digitalWrite(pinP, LOW);
  } else if (numberToPrint.number == 2) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, LOW);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, HIGH);
    digitalWrite(pinP, LOW);
  } else if (numberToPrint.number == 3) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, HIGH);
    digitalWrite(pinP, LOW);
  } else if (numberToPrint.number == 4) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
    digitalWrite(pinP, LOW);
  } else if (numberToPrint.number == 5) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
  } else if (numberToPrint.number == 6) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
    digitalWrite(pinP, LOW);
  } else if (numberToPrint.number == 7) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, LOW);
    digitalWrite(pinP, LOW);
  } else if (numberToPrint.number == 8) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
    digitalWrite(pinP, LOW);
  } else if (numberToPrint.number == 9) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
    digitalWrite(pinP, LOW);
  } else if (numberToPrint.number == 0) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, LOW);
    digitalWrite(pinP, LOW);
  }
  if (numberToPrint.isValidNumber) {
    digitalWrite(pinP, HIGH);
  }
  delay(1000);
  shutdown7SegmentDisplay();
  delay(100);
}

void shutdown7SegmentDisplay() {
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
  digitalWrite(pinC, LOW);
  digitalWrite(pinD, LOW);
  digitalWrite(pinE, LOW);
  digitalWrite(pinF, LOW);
  digitalWrite(pinG, LOW);
  digitalWrite(pinP, LOW);
}

void generateRandomNumberStructs(NUMBER numbers[], NUMBER validNumbers[], uint8_t arraySize) {
  uint8_t i = 0;
  for (i = 0; i < arraySize / 2; i++) {
    NUMBER randomNumber;
    randomNumber.number = random(0, 10);
    randomNumber.isValidNumber = true;
    numbers[i] = randomNumber;
  }
  for (i = arraySize / 2; i < arraySize; i++) {
    NUMBER randomNumber;
    randomNumber.number = random(0, 10);
    randomNumber.isValidNumber = false;
    numbers[i] = randomNumber;
  }
  for (i = 0; i < arraySize; i++) {
    uint8_t n = random(0, arraySize);
    NUMBER temp = numbers[n];
    numbers[n] = numbers[i];
    numbers[i] = temp;
  }
  uint8_t solutionArrayIndex = 0;
  for (i = 0; i < arraySize; i++) {
    if (numbers[i].isValidNumber) {
      validNumbers[solutionArrayIndex++] = numbers[i];
    }
  }
}

int8_t readMatrixAndReturnPressedKey() {
  int8_t pressedKey = 100;
  while (pressedKey == 100) {
    // iterate the columns
    for (int colIndex = 0; colIndex < colCount; colIndex++) {
      // col: set to output to low
      byte curCol = cols[colIndex];
      pinMode(curCol, OUTPUT);
      digitalWrite(curCol, LOW);

      // row: interate through the rows
      for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
        byte rowCol = rows[rowIndex];
        pinMode(rowCol, INPUT_PULLUP);
        keys[colIndex][rowIndex] = digitalRead(rowCol);
        pinMode(rowCol, INPUT);
      }
      // disable the column
      pinMode(curCol, INPUT);
    }

    for (int colIndex = 0; colIndex < colCount; colIndex++) {
      for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
        if (keys[colIndex][rowIndex] == LOW) {
          pressedKey = numbers[colIndex][rowIndex];
        }
      }
    }

    if (Serial.available()) {
      processSerialMessage();
      return -1;
    }

  }

  return pressedKey;

}

void processSerialMessage() {
  const uint8_t BUFF_SIZE = 64; // make it big enough to hold your longest command
  static char buffer[BUFF_SIZE + 1]; // +1 allows space for the null terminator
  static uint8_t length = 0; // number of characters currently in the buffer

  char c = Serial.read();
  if ((c == '\r') || (c == '\n')) {
    // end-of-line received
    if (length > 0) {
      tokenizeReceivedMessage(buffer);
    }
    length = 0;
  } else {
    if (length < BUFF_SIZE) {
      buffer[length++] = c; // append the received character to the array
      buffer[length] = 0; // append the null terminator
    }
  }
}

void tokenizeReceivedMessage(char *msg) {
  const uint8_t COMMAND_PAIRS = 10;
  char* tokenizedString[COMMAND_PAIRS + 1];
  uint8_t index = 0;

  char* command = strtok(msg, ";");
  while (command != 0) {
    char* separator = strchr(command, ':');
    if (separator != 0) {
      *separator = 0;
      tokenizedString[index++] = command;
      ++separator;
      tokenizedString[index++] = separator;
    }
    command = strtok(0, ";");
  }
  tokenizedString[index] = 0;

  processReceivedMessage(tokenizedString);
}

void processReceivedMessage(char** command) {
  if (strcmp(command[1], "START") == 0) {
    startSequence(command[3]);
  } else if (strcmp(command[1], "PAUSE") == 0) {
    pauseSequence(command[3]);
  } else if (strcmp(command[1], "STOP") == 0) {
    stopSequence(command[3]);
  } else if (strcmp(command[1], "INTERACTION_SOLVED_ACK") == 0) {
    setInteractionSolved();
  } else if (strcmp(command[1], "PING") == 0) {
    ping(command[3]);
  } else if (strcmp(command[1], "BAUD") == 0) {
    setBaudRate(atoi(command[3]), command[5]);
  } else if (strcmp(command[1], "SETUP") == 0) {
    Serial.println("COM:SETUP;INT_NAME:Simon Says Interaction;BAUD:9600");
    Serial.flush();
  }
}

//TODO: Review This Method once Interaction Is Completed
void startSequence(char* TIMESTAMP) {
  INTERACTION_SOLVED = false;
  INTERACTION_RUNNING = true;
  generateRandomNumberStructs(firstLevel, firstLevelSolution, 4);
  generateRandomNumberStructs(secondLevel, secondLevelSolution, 8);
  generateRandomNumberStructs(thirdLevel, thirdLevelSolution, 12);
  generateRandomNumberStructs(fourthLevel, fourthLevelSolution, 16);
  currentLevel = 1;
  currentLevelIndex = 0;
  sequencePlayedOnce = false;
  Serial.print("COM:START_ACK;MSG:");
  Serial.print("L1-");
  for (uint8_t i = 0; i < 2; i++) {
    Serial.print(firstLevelSolution[i].number);
  }
  Serial.print("L2-");
  for (uint8_t i = 0; i < 4; i++) {
    Serial.print(secondLevelSolution[i].number);
  }
  Serial.print("L3-");
  for (uint8_t i = 0; i < 6; i++) {
    Serial.print(thirdLevelSolution[i].number);
  }
  Serial.print("L4-");
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print(fourthLevelSolution[i].number);
  }
  Serial.print(";ID:");
  Serial.print(TIMESTAMP);
  Serial.print("\r\n");
  Serial.flush();
}

void pauseSequence(char* TIMESTAMP) {
  INTERACTION_RUNNING = !INTERACTION_RUNNING;
  if (INTERACTION_RUNNING) {
    Serial.print("COM:PAUSE_ACK;MSG:Device is now running;ID:");
  } else {
    Serial.print("COM:PAUSE_ACK;MSG:Device is now paused;ID:");
  }
  Serial.print(TIMESTAMP);
  Serial.print("\r\n");
  Serial.flush();
}

void stopSequence(char* TIMESTAMP) {
  INTERACTION_RUNNING = false;
  Serial.print("COM:STOP_ACK;MSG:Device is now stopped;ID:");
  Serial.print(TIMESTAMP);
  Serial.print("\r\n");
  Serial.flush();
}

void setInteractionSolved() {
  INTERACTION_SOLVED = true;
  INTERACTION_RUNNING = false;
}

void ping(char* TIMESTAMP) {
  Serial.print("COM:PING;MSG:PING;ID:");
  Serial.print(TIMESTAMP);
  Serial.print("\r\n");
  Serial.flush();
}

void setBaudRate(int baudRate, char* TIMESTAMP) {
  Serial.flush();
  Serial.begin(baudRate);
  Serial.print("COM:BAUD_ACK;MSG:The Baud Rate was set to ");
  Serial.print(baudRate);
  Serial.print(";ID:");
  Serial.print(TIMESTAMP);
  Serial.print("\r\n");
  Serial.flush();
}

bool checkWinning() {
  setInteractionSolved();
  Serial.println("COM:INTERACTION_SOLVED;MSG:User Introduced Correct Patterns;PNT:1500");
  Serial.flush();

  int NOTE_SUSTAIN = 85;
  tone(buzzer, NOTE_A5);
  delay(NOTE_SUSTAIN);
  tone(buzzer, NOTE_B5);
  delay(NOTE_SUSTAIN);
  tone(buzzer, NOTE_C5);
  delay(NOTE_SUSTAIN);
  tone(buzzer, NOTE_B5);
  delay(NOTE_SUSTAIN);
  tone(buzzer, NOTE_C5);
  delay(NOTE_SUSTAIN);
  tone(buzzer, NOTE_D5);
  delay(NOTE_SUSTAIN);
  tone(buzzer, NOTE_C5);
  delay(NOTE_SUSTAIN);
  tone(buzzer, NOTE_D5);
  delay(NOTE_SUSTAIN);
  tone(buzzer, NOTE_E5);
  delay(NOTE_SUSTAIN);
  tone(buzzer, NOTE_D5);
  delay(NOTE_SUSTAIN);
  tone(buzzer, NOTE_E5);
  delay(NOTE_SUSTAIN);
  tone(buzzer, NOTE_E5);
  delay(NOTE_SUSTAIN);
  noTone(buzzer);

  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, HIGH);
  digitalWrite(pinC, HIGH);
  digitalWrite(pinD, HIGH);
  digitalWrite(pinE, HIGH);
  digitalWrite(pinF, HIGH);
  digitalWrite(pinG, HIGH);
  digitalWrite(pinP, HIGH);

  delay(1000);

  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
  digitalWrite(pinC, LOW);
  digitalWrite(pinD, LOW);
  digitalWrite(pinE, LOW);
  digitalWrite(pinF, LOW);
  digitalWrite(pinG, LOW);
  digitalWrite(pinP, LOW);

  delay(1000);

  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, HIGH);
  digitalWrite(pinC, HIGH);
  digitalWrite(pinD, HIGH);
  digitalWrite(pinE, HIGH);
  digitalWrite(pinF, HIGH);
  digitalWrite(pinG, HIGH);
  digitalWrite(pinP, HIGH);

  delay(1000);

  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
  digitalWrite(pinC, LOW);
  digitalWrite(pinD, LOW);
  digitalWrite(pinE, LOW);
  digitalWrite(pinF, LOW);
  digitalWrite(pinG, LOW);
  digitalWrite(pinP, LOW);
}
