#include <M5Atom.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid        = "Wifi_IoT";
const char* password    = "I4.8IoT@Nantes";

const char* broker_url  = "app.icam.fr";
const int   broker_port = 1883;
const char* client_id   = "a28162ec24ca";
const char* pub_topic   = "test/pub/data";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
int state;
unsigned long timestamp;

void setup() {
  M5.begin(true, false, true);
  state = 0;
  set_led();
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
      if (M5.Btn.wasPressed()) {
        state = (state == 3) ? 0 : state + 1;
        set_led();
        const char* message = (state == 0 ? "none" : 
                               (state == 1 ? "red" : 
                                (state == 2 ? "green" : "blue")));
        mqttClient.publish(pub_topic, message);
      }
    }  
  }
  delay(5);
  M5.update();
}

int has_spent(unsigned long duration, unsigned long timestamp) {
  return millis() > timestamp + duration;
}

void set_led() {
  switch (state) {
    case 0: M5.dis.clear(); break;
    case 1: set_pix(CRGB::Green); break;
    case 2: set_pix(CRGB::Red); break;
    case 3: set_pix(CRGB::Blue); break;
    default: break;
  }
}

void set_pix(CRGB color) {
  M5.dis.drawpix(0, color);
}
