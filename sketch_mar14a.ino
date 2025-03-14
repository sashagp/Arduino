const int buzzerPin = 9;    // Buzzer connected to pin 9
const int buttonPin = 7;    // Button connected to pin 7

bool buzzerState = false;   // Buzzer state (ON/OFF)
bool lastButtonState = HIGH;
bool currentButtonState;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;  // Debounce delay

void setup() {
    pinMode(buzzerPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor
}

void loop() {
    currentButtonState = digitalRead(buttonPin);

    if (currentButtonState == LOW && lastButtonState == HIGH && (millis() - lastDebounceTime) > debounceDelay) {
        buzzerState = !buzzerState;  // Toggle buzzer state
        digitalWrite(buzzerPin, buzzerState);
        lastDebounceTime = millis();
    }

    lastButtonState = currentButtonState;
}