#include <M5Atom.h>

uint16_t value;

void setup() {
  M5.begin(true, false, true);
  Serial.begin(115200);
  pinMode(36, INPUT);
  pinMode(26, INPUT);
  value = 0;
}

void loop() {
  value = value = analogRead(36);
  Serial.println(value);
  delay(50);
  value = digitalRead(26);
  Serial.println(value);
  delay(50);
  Serial.println(value == 0 ? "light" : "dark");
  delay(200);
}
