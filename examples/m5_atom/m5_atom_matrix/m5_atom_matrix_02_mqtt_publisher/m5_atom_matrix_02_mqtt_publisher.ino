#include <M5Atom.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid        = "Wifi_IoT";
const char* password    = "I4.8IoT@Nantes";

const char* broker_url  = "app.icam.fr";
const int   broker_port = 1883;
const char* client_id   = "e4c541a5195c";
const char* pub_topic   = "test/e4c541a5195c/status";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
int state;
unsigned long timestamp;

void setup() {
  M5.begin(true, false, true);
  state = 0;
  set_leds();
  mqttClient.setServer(broker_url, broker_port);
  WiFi.begin(ssid, password);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (mqttClient.connected() == 0) { 
      if (has_spent(500, timestamp)) {
        timestamp = millis();
        if (mqttClient.connect(client_id)) {
        }
      }
    } else {
      mqttClient.loop();
      if (M5.Btn.wasPressed()) {
        state = (state == 3) ? 0 : state + 1;
        set_leds();
        const char* message = (state == 0 ? "none" : (state == 1 ? "green" : (state == 2 ? "red" : "blue")));
        mqttClient.publish(pub_topic, message);
      }
    }  
  }
  delay(50);
  M5.update();
}

int has_spent(unsigned long duration, unsigned long timestamp) {
  return millis() > timestamp + duration;
}

void set_leds() {
  switch (state) {
    case 0: M5.dis.clear(); break;
    case 1: set_pix(CRGB::Green); break;
    case 2: set_pix(CRGB::Red); break;
    case 3: set_pix(CRGB::Blue); break;
    default: break;
  }
}

void set_pix(CRGB color) {
  for (int row = 0; row < 5; row++) {
    for (int col = 0; col < 5; col++) {
      M5.dis.drawpix(row, col, color);
    }
  }
}
