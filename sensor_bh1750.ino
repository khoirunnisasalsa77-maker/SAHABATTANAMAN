
#include <Wire.h>

#define BH_ADDR 0x23

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);
  Wire.setClock(100000);

  // Power ON
  Wire.beginTransmission(BH_ADDR);
  Wire.write(0x01);
  byte error = Wire.endTransmission();

  Serial.println(error == 0 ? "Power ON ok" : "Power ON gagal");

  delay(10);

  // Continuous High Resolution Mode
  Wire.beginTransmission(BH_ADDR);
  Wire.write(0x10);
  error = Wire.endTransmission();

  Serial.println(error == 0 ? "Mode ok" : "Mode gagal");

  delay(200);
}

void loop() {

  // Minta 2 byte dari BH1750
  byte received = Wire.requestFrom(BH_ADDR, (uint8_t)2);

  if (received == 2) {

    uint16_t data = Wire.read() << 8;
    data |= Wire.read();

    float lux = data / 1.2;

    Serial.print("Cahaya: ");
    Serial.print(lux);
    Serial.println(" lux");

  } else {
    Serial.print("Gagal baca, byte diterima: ");
    Serial.println(received);
  }

  delay(2000);
}
