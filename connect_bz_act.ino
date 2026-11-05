#define BUZZER_PIN 25

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  digitalWrite(BUZZER_PIN, HIGH);  // bunyi
  delay(500);
  digitalWrite(BUZZER_PIN, LOW);   // diam
  delay(1000);
}