//========================
//  ESP32-S3 (Receiver) Code
//========================
// Include necessary libraries
#include <Arduino.h>
#include <Talkie.h>
#include <SPI.h>
#include <Vocab_US_Large.h>
#include <Vocab_Special.h>
#include <Vocab_US_TI99.h>
#include <Vocab_US_Clock.h>
#include <Vocab_US_TinyIE.h>

// Define pins for the ESP32-S3
#define SPK_PIN 45       // Speaker output pin on ESP32-S3
#define TRIGGER_PIN 10   // Trigger pin for the ultrasonic sensor
#define ECHO_PIN 11      // Echo pin for the ultrasonic sensor

// Define serial ports.  For ESP32, Serial2 is commonly available.
#define SENDER_SERIAL Serial2 // Serial port for communication with the sending ESP32

// Constants
#define MAX_TEXT_LENGTH 100   // Maximum text buffer size
#define DISTANCE_TIMEOUT 30000 // Timeout for distance measurement in microseconds (30ms)
#define MEASUREMENT_DELAY 500  // Delay between measurements in milliseconds

// Global variables for the ESP32-S3
Talkie voice;           // Talkie object for speech synthesis
char textBuffer[MAX_TEXT_LENGTH]; // Buffer to store received text

// Function Declarations
void setupReceiver();
void playTextWithDistance(const char *text);
float getDistance();
void speakNumber(int number);
void speakDistance(float distance);

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000);  // Wait for serial connection or timeout after 3 seconds
  
  // Initialize the speaker on ESP32-S3
  pinMode(SPK_PIN, OUTPUT);
  voice.beginPWM(SPK_PIN); // Use beginPWM instead of begin
  
  // Initialize the ultrasonic sensor pins on ESP32-S3
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Initialize serial communication for receiving text
  SENDER_SERIAL.begin(115200); // Initialize the serial port for the sender
  
  setupReceiver(); // Call setupReceiver() to initialize any additional hardware
  
  Serial.println("ESP32-S3 Receiver is ready. Waiting for text...");
  
  // Announce ready status
  voice.say(sp2_READY);
}

void loop() {
  // Check if data is available on the serial port
  if (SENDER_SERIAL.available() > 0) {
    // Read the incoming text from the serial port
    String receivedText = SENDER_SERIAL.readStringUntil('\n');
    receivedText.trim(); // Remove any trailing characters
    
    Serial.print("Received Text: ");
    Serial.println(receivedText);
    
    if (receivedText.length() > 0) {
      // Copy the received text into the buffer with overflow protection
      size_t copiedLength = min((size_t)(MAX_TEXT_LENGTH - 1), receivedText.length());
      receivedText.toCharArray(textBuffer, copiedLength + 1);
      textBuffer[copiedLength] = '\0'; // Ensure null termination
      
      // Just log the text and measure distance (can't speak arbitrary text)
      Serial.println("Received message (can't speak arbitrary text with Talkie)");
      Serial.println(textBuffer);
      
      // Say "data" and "received" from vocabulary
      voice.say(sp2_DATA);
      delay(100);
      voice.say(sp4_RECEIVED);
      
      delay(MEASUREMENT_DELAY);
      
      // Measure and speak the distance
      float distance = getDistance();
      speakDistance(distance);
    }
  }
}

/**
 * Initializes any additional hardware or settings for the receiver.
 * This function can be expanded as needed for additional setup.
 */
void setupReceiver() {
  // Set initial state for trigger pin
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  
  // Perform an initial distance measurement to warm up the sensor
  getDistance();
}

/**
 * Speaks the distance measured by the ultrasonic sensor.
 * Note: This function doesn't actually speak the received text since
 * Talkie can only speak pre-compiled words.
 *
 * @param text The text that was received (will only be printed to Serial)
 */
void playTextWithDistance(const char *text) {
  Serial.println("Received message (can't speak with Talkie):");
  Serial.println(text);
  
  // Say "data received"
  voice.say(sp2_DATA);
  delay(100);
  voice.say(sp4_RECEIVED);
  
  delay(MEASUREMENT_DELAY); // Add a delay between phrases
  
  // Measure and speak the distance
  float distance = getDistance();
  speakDistance(distance);
}

/**
 * Measures the distance using the ultrasonic sensor.
 *
 * @return The distance in centimeters or 0 if measurement fails.
 */
float getDistance() {
  // Trigger the sensor
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  // Measure the duration
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, DISTANCE_TIMEOUT);
  
  // If we timed out or got an unreasonable reading, return 0
  if (duration == 0 || duration > 23529) { // Max distance ~4m (23529 microseconds)
    return 0;
  }
  
  // Calculate distance in centimeters
  return duration * 0.034 / 2;
}

/**
 * Speaks a number using the Talkie library.
 * 
 * @param number The number to speak (0-999)
 */
void speakNumber(int number) {
  if (number == 0) {
    voice.say(sp2_ZERO);
    return;
  }
  
  if (number < 0) {
    voice.say(sp2_MINUS);
    number = -number;
  }
  
  if (number >= 1000) {
    // For larger numbers, just say the individual digits
    int thousands = number / 1000;
    speakNumber(thousands);
    voice.say(sp2_THOUSAND);
    number %= 1000;
    if (number == 0) return;
  }
  
  if (number >= 100) {
    int hundreds = number / 100;
    switch (hundreds) {
      case 1: voice.say(sp2_ONE); break;
      case 2: voice.say(sp2_TWO); break;
      case 3: voice.say(sp2_THREE); break;
      case 4: voice.say(sp2_FOUR); break;
      case 5: voice.say(sp2_FIVE); break;
      case 6: voice.say(sp2_SIX); break;
      case 7: voice.say(sp2_SEVEN); break;
      case 8: voice.say(sp2_EIGHT); break;
      case 9: voice.say(sp2_NINE); break;
    }
    voice.say(sp2_HUNDRED);
    number %= 100;
    if (number == 0) return;
  }
  
  // Handle tens and ones
  if (number >= 20) {
    int tens = number / 10;
    switch (tens) {
      case 2: voice.say(sp2_TWENTY); break;  
      case 3: voice.say(sp3_THIRTY); break;
      case 4: voice.say(sp3_FOURTY); break;
      case 5: voice.say(sp3_FIFTY); break;
      case 6: voice.say(sp3_SIXTY); break;
      case 7: voice.say(sp3_SEVENTY); break;
      case 8: voice.say(sp3_EIGHTY); break;
      case 9: voice.say(sp3_NINETY); break;
    }
    number %= 10;
    if (number == 0) return;
  }
  
  // Handle teens
  if (number >= 10 && number <= 19) {
    switch (number) {
      case 10: voice.say(sp2_TEN); break;
      case 11: voice.say(sp2_ELEVEN); break;
      case 12: voice.say(sp2_TWELVE); break;
      case 13: voice.say(sp3_THIRTEEN); break;
      case 14: voice.say(sp3_FOURTEEN); break;
      case 15: voice.say(sp3_FIFTEEN); break;
      case 16: voice.say(sp3_SIXTEEN); break;
      case 17: voice.say(sp3_SEVENTEEN); break;
      case 18: voice.say(sp3_EIGHTEEN); break;
      case 19: voice.say(sp3_NINETEEN); break;
    }
    return;
  }
  
  // Handle ones
  switch (number) {
    case 1: voice.say(sp2_ONE); break;
    case 2: voice.say(sp2_TWO); break;
    case 3: voice.say(sp2_THREE); break;
    case 4: voice.say(sp2_FOUR); break;
    case 5: voice.say(sp2_FIVE); break;
    case 6: voice.say(sp2_SIX); break;
    case 7: voice.say(sp2_SEVEN); break;
    case 8: voice.say(sp2_EIGHT); break;
    case 9: voice.say(sp2_NINE); break;
  }
}

/**
 * Speaks the distance using the Talkie library with appropriate units.
 * 
 * @param distance The distance in centimeters
 */
void speakDistance(float distance) {
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  if (distance <= 0) {
    // If distance is invalid
    voice.say(sp3_ERROR);
    delay(100);
    voice.say(sp2_MEASURE);
    return;
  }
  
  // Say "range"
  voice.say(sp3_RANGE);
  delay(100);
  
  // Convert to integer centimeters
  int cm = (int)distance;
  
  // Speak the number
  speakNumber(cm);
  delay(100);
  
  // Say "centimeter"
  voice.say(sp2_CENTI);
  delay(50);
  voice.say(sp2_METER);
}
