#include <M5Atom.h>

int state;

void setup() {
  M5.begin(true, false, true);
  state = 0;
  set_leds();
}

void loop() {
  if (M5.Btn.wasPressed()) {
    state = (state == 3) ? 0 : state + 1;
    set_leds();
  }
  delay(50);
  M5.update();
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
