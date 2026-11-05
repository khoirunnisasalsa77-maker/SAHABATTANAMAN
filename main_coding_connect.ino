#include <WiFi.h>
#include <PubSubClient.h>

const char* WIFI_SSID = "KINAR HOME";
const char* WIFI_PASS = "kinar7878";
const char* MQTT_SERVER = "broker.hivemq.com";
const char* TOPIC = "tugasiot-kinar7391/tes";

WiFiClient espClient;
PubSubClient mqtt(espClient);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.print("Menyambung ke WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi terhubung!");

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

  int angka = random(0, 100);
  mqtt.publish(TOPIC, String(angka).c_str());
  Serial.print("Terkirim: ");
  Serial.println(angka);
  delay(2000);
}