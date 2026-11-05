
#include <WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>

// WIFI
const char* WIFI_SSID = "KINAR HOME";
const char* WIFI_PASS = "kinar7878";

// MQTT
const char* MQTT_SERVER = "broker.hivemq.com";
const char* TOPIC = "tugasiot-kinar7391/tes";

// SENSOR BH1750
#define BH_ADDR 0x23

WiFiClient espClient;
PubSubClient mqtt(espClient);

// Membaca sensor BH1750
float bacaCahaya() {

  byte jumlah = Wire.requestFrom(BH_ADDR, (uint8_t)2);

  if (jumlah == 2) {

    uint16_t data = Wire.read() << 8;
    data |= Wire.read();

    float lux = data / 1.2;

    return lux;
  }

  return -1;
}

void setup() {

  Serial.begin(115200);
  delay(1000);

  // I2C SENSOR
  Wire.begin(21, 22);
  Wire.setClock(100000);

  // POWER ON BH1750
  Wire.beginTransmission(BH_ADDR);
  Wire.write(0x01);
  Wire.endTransmission();

  delay(10);

  // CONTINUOUS HIGH RESOLUTION MODE
  Wire.beginTransmission(BH_ADDR);
  Wire.write(0x10);
  Wire.endTransmission();

  delay(200);

  // WIFI
  Serial.print("Menyambung ke WiFi");

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi terhubung!");

  // MQTT
  mqtt.setServer(MQTT_SERVER, 1883);
}

void loop() {

  if (!mqtt.connected()) {

    Serial.print("Menyambung ke broker MQTT...");

    String id = "esp32-" + String(random(0xffff), HEX);

    if (mqtt.connect(id.c_str())) {

      Serial.println("berhasil!");

    } else {

      Serial.println("gagal, coba lagi 2 detik");

      delay(2000);
      return;
    }
  }

  mqtt.loop();

  // BACA SENSOR BH1750
  float lux = bacaCahaya();

  if (lux < 0) {
    Serial.println("Gagal membaca sensor BH1750!");
    delay(2000);
    return;
  }

  // KIRIM DATA KE MQTT
  char payload[20];
  snprintf(payload, sizeof(payload), "%.2f", lux);

  mqtt.publish(TOPIC, payload);

  Serial.print("Terkirim: ");
  Serial.print(lux);
  Serial.println(" lux");

  delay(2000);
}
