const int buttonPin = 5;  // Button connected to pin 2
const int buzzerPin = 6;  // Buzzer connected to pin 9

bool buzzerState = false; // To store buzzer state
bool lastButtonState = HIGH; // Previous state of the button
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Enable internal pull-up resistor
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  bool buttonState = digitalRead(buttonPin);
  bool currentButtonState = digitalRead(buttonPin) == LOW; // Button pressed (LOW due to pull-up)

  // Toggle mode: Only change state if button was released and pressed again
  if (currentButtonState && !buttonState) {
    buzzerState = !buzzerState;
  }

  // If button is held down, keep buzzer ON
  digitalWrite(buzzerPin, buzzerState || currentButtonState);

  buttonState = currentButtonState; // Update button state
  // Debounce check
  if (buttonState == LOW && lastButtonState == HIGH && millis() - lastDebounceTime > debounceDelay) {
    lastDebounceTime = millis(); // Reset debounce timer
    buzzerState = !buzzerState; // Toggle buzzer state

    if (buzzerState) {
      tone(buzzerPin, 1000); // Start buzzer at 1kHz
    } else {
      noTone(buzzerPin); // Stop buzzer
    }
  }

  lastButtonState = buttonState; // Update button state
}
/**

const int buttonPin = 5;   // Button connected to pin 2
const int buzzerPin = 6;   // Buzzer connected to pin 9

bool buzzerState = false;  // Variable to store buzzer state
bool buttonState = false;  // Variable to store last button state

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  bool currentButtonState = digitalRead(buttonPin) == LOW; // Button pressed (LOW due to pull-up)

  // Toggle mode: Only change state if button was released and pressed again
  if (currentButtonState && !buttonState) {
    buzzerState = !buzzerState;
  }

  // If button is held down, keep buzzer ON
  digitalWrite(buzzerPin, buzzerState || currentButtonState);

  buttonState = currentButtonState; // Update button state
}
**/