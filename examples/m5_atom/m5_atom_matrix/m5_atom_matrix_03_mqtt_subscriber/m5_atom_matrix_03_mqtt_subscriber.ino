#include <M5Atom.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid        = "Wifi_IoT";
const char* password    = "I4.8IoT@Nantes";

const char* broker_url  = "app.icam.fr";
const int   broker_port = 1883;
const char* client_id   = "5aa5b3b1b0b3";
const char* sub_topic   = "test/pub/data";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
int state;
unsigned long timestamp;

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  char* content = (char*) payload;
  if (strcmp(content, "none") == 0) {
    state = 0;
  } else if (strcmp(content, "red") == 0) {
    state = 1;
  } else if (strcmp(content, "green") == 0) {
    state = 2;
  } else if (strcmp(content, "blue") == 0) {
    state = 3;
  }
  set_leds();
}

void setup() {
  M5.begin(true, false, true);
  state = 0;
  set_leds();
  mqttClient.setServer(broker_url, broker_port);
  mqttClient.setCallback(callback);
  WiFi.begin(ssid, password);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (mqttClient.connected() == 0) { 
      if (has_spent(500, timestamp)) {
        timestamp = millis();
        if (mqttClient.connect(client_id)) {
          mqttClient.subscribe(sub_topic);
        }
      }
    } else {
      mqttClient.loop();
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
