#include <Wire.h>
#include <map>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define pResistor A0
#define THRESHOLD 2500
#define TIME_THRESHOLD 400
#define CHAR_GAP 1000
#define WORD_GAP 2000

LiquidCrystal_I2C lcd(0x27, 16, 2);
String morseCode = "";
String result = "";
bool lightOn = false;
unsigned long startTime = 0;
unsigned long lastChangeTime = 0;

std::map<String, char> morseMap = {
  { ".-", 'A' }, { "-...", 'B' }, { "-.-.", 'C' }, { "-..", 'D' }, { ".", 'E' }, { "..-.", 'F' }, { "--.", 'G' }, { "....", 'H' }, { "..", 'I' }, { ".---", 'J' }, { "-.-", 'K' }, { ".-..", 'L' }, { "--", 'M' }, { "-.", 'N' }, { "---", 'O' }, { ".--.", 'P' }, { "--.-", 'Q' }, { ".-.", 'R' }, { "...", 'S' }, { "-", 'T' }, { "..-", 'U' }, { "...-", 'V' }, { ".--", 'W' }, { "-..-", 'X' }, { "-.--", 'Y' }, { "--..", 'Z' }, { ".----", '1' }, { "..---", '2' }, { "...--", '3' }, { "....-", '4' }, { ".....", '5' }, { "-....", '6' }, { "--...", '7' }, { "---..", '8' }, { "----.", '9' }, { "-----", '0' }, { "/", ' ' }
};

// Hàm chuyển chuỗi mã Morse thành ký tự
char morseToChar(String morse) {
  if (morseMap.find(morse) != morseMap.end()) {
    return morseMap[morse];
  } else {
    return '\0';  // Trả về ký tự null nếu không tìm thấy
  }
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  int light = analogRead(pResistor);
  unsigned long currentTime = millis();

  if (light > THRESHOLD) {
    if (!lightOn) {
      startTime = currentTime;
      lightOn = true;
    }
  } else {
    if (lightOn) {
      unsigned long duration = currentTime - startTime;
      if (duration >= TIME_THRESHOLD) {
        morseCode += "-";
      } else if (duration >= 200) {
        morseCode += ".";
      }
      lightOn = false;
      lastChangeTime = currentTime;
    }
  }

  if (!lightOn && !morseCode.isEmpty() && (currentTime - lastChangeTime >= CHAR_GAP)) {
    char ch = morseToChar(morseCode);
    if (ch != '\0') {
      result += ch;
      Serial.println(result);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Text Received:");
      lcd.setCursor(0, 1);
      String temp = result;
      temp.trim();
      lcd.print(temp);
    }
    morseCode = "";
  }

  if (!lightOn && !result.isEmpty() && result[result.length() - 1] != ' ' && (currentTime - lastChangeTime >= WORD_GAP)) {
    result += " ";
    Serial.println(result);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Text Received:");
    lcd.setCursor(0, 1);
    String temp = result;
    temp.trim();
    lcd.print(temp);
  }

  if (currentTime - lastChangeTime >= 8000) {
    result = "";
    lcd.clear();
  }
}
