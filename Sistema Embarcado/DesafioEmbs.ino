#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#define motorPin  3
#define buzzerPin 2

LiquidCrystal_I2C lcd(0x3f, 16, 2);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {11, 10, 9, 8};
byte colPins[COLS] = { 7,  6, 5, 4};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String str = "";

float octave = 0;
float octaves[ROWS][COLS] = {
  {220, 261, 329, 392},
  {493, 587, 698, 880},
  {1046, 1318, 1567, 1975},
  {2349, 2793, 3520, 4186}
};

String vib[ROWS][COLS] = {
  {"**- ", "*-**", "*-*-", "-*- "},
  {"*-**", "**  ", "***-", "--**"},
  {"*--*", "****", "*** ", "--*-"},
  {"----", "*---", "-***", "--**"},
};

void setup() {
  Serial.begin(9600);

  pinMode(motorPin,  OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  lcd.begin();
  lcd.backlight();
  lcd.print("Hello, world!");
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    if (key == '*') str = "";
    else str += key;

    Serial.println(key);
    keyToAssist(key);

    lcd.clear();
    lcd.print(str);
  }
}

void keyToAssist(char key) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (key == keys[i][j]) {
        octave = octaves[i][j];
        tone(buzzerPin, octave, 150);

        motorCode(vib[i][j], 75);
      }
    }
  }
}

void motorCode(String code, int wait) {
  for (int i = 0; i < code.length(); i++) {
    digitalWrite(motorPin, LOW);
    delay(wait * 2);

    if (code[i] == '*') {
      digitalWrite(motorPin, HIGH);
      delay(wait);
    }

    else if (code[i] == '-') {
      digitalWrite(motorPin, HIGH);
      delay(wait * 2.5);
    }
  }

  digitalWrite(motorPin, LOW);
}
