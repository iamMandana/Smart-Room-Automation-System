#include <Wire.h>
#include "rgb_lcd.h"
#include <math.h>

// PIN definitions

// Analog sensors
#define TEMP_PIN A0          // Temperature sensor (thermistor)
#define LDR_PIN A1           // Light sensor (LDR)

// Digital inputs
#define PIR_PIN 7            // Motion sensor (PIR)
#define BUTTON_PIN 2         // User button

// Outputs
#define YELLOW_LED 3         // Room light indicator
#define RED_LED 4            // Security alert indicator
#define BUZZER_PIN 5         // Alarm buzzer

// Motor driver (H-bridge)
#define IN1 8
#define IN2 9
#define ENA 10

rgb_lcd lcd;

// System State

// mode: 0 = AUTO (environment controlled), 1 = AWAY (security mode)
int mode = 0;

// Alarm state remains active until manually cleared
bool alarmActive = false;

// Button handling
// Used to detect short vs long press
unsigned long pressStartTime = 0;
bool buttonPressed = false;

// Fan controll
// Fan state with delayed shutdown logic
bool fanOn = false;
unsigned long fanOffTime = 0;

// Motion processing
// Smooths PIR signal to avoid flickering
unsigned long lastMotionTime = 0;
bool motionState = false;

// Light control
// Keeps light ON for a period after motion stops
unsigned long lastLightTime = 0;
bool lightState = false;

// Buzzer control
// Non-blocking blinking for alarm
unsigned long lastBuzzerToggle = 0;
bool buzzerState = false;

// LCD update timing
unsigned long lastLCDUpdate = 0;

void setup() {
  // Configure input pins
  pinMode(PIR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);

  // Configure output pins
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.setRGB(0, 255, 0);

  // Serial for debugging
  Serial.begin(9600);
}

void loop() {

  unsigned long currentTime = millis();

  // Button logic
  // Detect press start
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == HIGH && !buttonPressed) {
    buttonPressed = true;
    pressStartTime = currentTime;
  }

  // Detect release and determine press type
  if (buttonState == LOW && buttonPressed) {
    buttonPressed = false;

    unsigned long pressDuration = currentTime - pressStartTime;

    if (pressDuration > 1500) {
      // Long press clears alarm
      alarmActive = false;
    } else {
      // Short press toggles AUTO/AWAY mode (only if no alarm active)
      if (!alarmActive) {
        mode = !mode;
      }
    }
  }

  // Temperature processing
  // Convert thermistor reading to Celsius using Steinhart-Hart equation
  int rawValue = analogRead(TEMP_PIN);
  float resistance = (1023.0 / rawValue - 1.0) * 10000.0;
  float temperature = 1.0 / (log(resistance / 10000.0) / 3975 + 1 / 298.15) - 273.15;

  // Light sensor
  // Determine if environment is dark based on threshold
  int lightLevel = analogRead(LDR_PIN);
  bool isDark = (lightLevel < 400);

  // Motion detection
  // Smooth PIR output to avoid rapid toggling
  bool pirReading = digitalRead(PIR_PIN);

  if (pirReading == HIGH) {
    lastMotionTime = currentTime;
    motionState = true;
  }

  // Motion remains true for 3 seconds after last detection
  if (currentTime - lastMotionTime > 3000) {
    motionState = false;
  }

  // Security logic
  // Trigger alarm in AWAY mode if motion detected
  if (mode == 1 && motionState) {
    alarmActive = true;
  }

  // Fan Control
  // Turn fan on above threshold temperature
  if (temperature > 22) {
    fanOn = true;
    fanOffTime = currentTime;
  }

  // Keep fan running for 5 seconds after cooling
  if (fanOn) {
    if (temperature < 20 && currentTime - fanOffTime > 5000) {
      fanOn = false;
    }
  }

  // Apply fan control through H-bridge
  if (fanOn) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 180); // Speed control
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  }

  // Light control
  // Activate light only in AUTO mode when dark and motion is present

  if (mode == 0 && isDark && motionState) {
      lightState = true;
      lastLightTime = currentTime;
  }

  // Turn OFF immediately if it becomes bright
  if (!isDark) {
      lightState = false;
  }

  // Keep light ON for 10 seconds after motion stops
  if (lightState && !motionState && (currentTime - lastLightTime > 10000)) {
      lightState = false;
  }

  digitalWrite(YELLOW_LED, lightState ? HIGH : LOW);

  // Alarm output
  // Red LED indicates alarm state
  digitalWrite(RED_LED, alarmActive ? HIGH : LOW);

  // Non-blocking buzzer toggle
  if (alarmActive) {
    if (currentTime - lastBuzzerToggle > 100) {
      lastBuzzerToggle = currentTime;
      buzzerState = !buzzerState;
      digitalWrite(BUZZER_PIN, buzzerState);
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    buzzerState = false;
  }

  // LCD display
  // Update LCD periodically to avoid flicker
  if (currentTime - lastLCDUpdate > 200) {
    lastLCDUpdate = currentTime;

    // LINE 1: MODE + TEMPERATURE 
    lcd.setCursor(0, 0);

    if (mode == 0) {
      lcd.print(isDark ? "NIGHT " : "DAY   ");
    } else {
      lcd.print("AWAY  ");
    }

    lcd.print(" ");
    lcd.print(temperature, 1);
    lcd.print("C   ");

    // LINE 2: SYSTEM STATUS 
    lcd.setCursor(0, 1);

    if (alarmActive) {
      lcd.print("ALERT ACTIVE   ");
      lcd.setRGB(255, 0, 0);
    }
    else {
      char status[17];

      // Combine system states for accurate feedback
      if (fanOn && motionState) {
        strcpy(status, "Fan+Motion     ");
        lcd.setRGB(0, 150, 255);
      }
      else if (fanOn) {
        strcpy(status, "Fan Running    ");
        lcd.setRGB(0, 0, 255);
      }
      else if (motionState) {
        strcpy(status, "Motion Detected");
        lcd.setRGB(0, 255, 0);
      }
      else {
        strcpy(status, "System Idle    ");
        lcd.setRGB(255, 255, 0);
      }

      lcd.print(status);
    }
  }
}
