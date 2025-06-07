const int trigPins[] = {4, 16, 17, 18};
const int echoPins[] = {32, 33, 34, 35};
const int buzzPins[] = {19, 21, 22, 23};
const int numSensors = 4                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         ;

float distances[numSensors];
float initial[numSensors];
unsigned long previousMillis[numSensors] = {0};
unsigned long intervals[numSensors] = {1000, 1000, 1000};
bool buzzerStates[numSensors] = {LOW};

void setup() {
    Serial.begin(115200);
    for (int i = 0; i < numSensors; i++) {
        pinMode(trigPins[i], OUTPUT);
        pinMode(echoPins[i], INPUT);
        pinMode(buzzPins[i], OUTPUT);
        digitalWrite(buzzPins[i], LOW);
        
        initial[i] = getDistance(trigPins[i], echoPins[i]);
        Serial.print("Initial Distance"); Serial.print(i); Serial.print(": "); 
        Serial.println(initial[i]);
        delay(500);
    }
}

void loop() {
    unsigned long currentMillis = millis();
    
    for (int i = 0; i < numSensors; i++) {
        distances[i] = getDistance(trigPins[i], echoPins[i]);
        Serial.print("Distance"); Serial.print(i); Serial.print(": "); 
        Serial.println(distances[i]);

        if (distances[i] <= initial[i]- 5)  {
            intervals[i] = map(distances[i], 10, 60, 50, 1000);
            handleBuzzer(buzzPins[i], previousMillis[i], intervals[i], buzzerStates[i], currentMillis);
        } 
        else if(distances[i] > initial[i]+ 5)  {
            digitalWrite(buzzPins[i], HIGH);
        }
        else{
          digitalWrite(buzzPins[i], LOW);
        }
    }
    delay(5);
}

float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float duration = pulseIn(echoPin, HIGH);
  return (duration * 0.0343) / 2;
}

void handleBuzzer(int buzzerPin, unsigned long &previousMillis, unsigned long interval, bool &buzzerState, unsigned long currentMillis) {
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Save the last toggle time
    buzzerState = !buzzerState; // Toggle state
    digitalWrite(buzzerPin, buzzerState ? HIGH : LOW);
  }
}
