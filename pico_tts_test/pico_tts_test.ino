#include <Audio.h>
#include "pico_tts.h"

AudioOutputI2S audioOutput;
PicoTTS tts(audioOutput);

void setup() {
  Serial.begin(115200);       // For debugging
  Serial1.begin(9600, SERIAL_8N1, 16, 17);  // RX=16, TX=17 (adjust pins if needed)
  audioOutput.begin();
  Serial.println("Speaker ESP32 ready.");
}

void loop() {
  static String input = "";

  while (Serial1.available()) {
    char c = Serial1.read();
    if (c == '\n') {
      processMessage(input);
      input = "";
    } else {
      input += c;
    }
  }
}

void processMessage(String msg) {
  int sep = msg.indexOf(':');
  if (sep == -1) return;  // Invalid format

  String object = msg.substring(0, sep);
  String distance = msg.substring(sep + 1);

  String sentence = "A " + object + " is " + distance + " centimeters away.";
  Serial.println("Speaking: " + sentence);
  tts.say(sentence);
}
