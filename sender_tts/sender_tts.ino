//========================
//  ESP32 (Sender) Code
//========================
#ifndef ESP32S3 // This is the Sender ESP32

// Define serial port for sender
#define SENDER_SERIAL Serial2

// Function Declarations
void setupSender();
void sendText(const String& text);

void setup() {
  Serial.begin(115200);
  //while (!Serial); // Remove this line
  setupSender();
}

void loop() {
  // Send the text
  sendText("Hello from ESP32!");
  delay(5000); // Send every 5 seconds
}

/**
 * Sets up the ESP32 as a serial sender.
 */
void setupSender() {
  SENDER_SERIAL.begin(115200); // Initialize the serial port for the sender
  Serial.println("ESP32 Sender is ready.");
}

/**
 * Sends the given text to the ESP32-S3 receiver via Serial.
 * @param text The text to send.
 */
void sendText(const String& text) {
  Serial.print("Sending text: ");
  Serial.println(text);
  // Send the text followed by a newline character, which the receiver uses as a delimiter.
  SENDER_SERIAL.println(text);
}

#endif
