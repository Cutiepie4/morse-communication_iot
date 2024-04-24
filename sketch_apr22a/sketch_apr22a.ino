#include <LiquidCrystal_I2C.h>

#define pResistor A0
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  int light = analogRead(pResistor);
  Serial.println(light);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Value:");
  lcd.setCursor(0, 1);
  lcd.print(light);
  delay(1000);
}