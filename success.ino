#include <LiquidCrystal.h>
#include <IRremote.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Set the LCD pins
#define BUZZER_PIN 13
const int irPin = 0; // Connect the IR LED to digital pin 3
const int sensorPin = A0; // Analog pin for the MQ-4 sensor
const int threshold = 800; // Adjust this threshold based on your sensor readings

int buzzerState = 0; // Store the buzzer state (0: off, 1: on)

IRsend irSender;
IRrecv irrecv(1); // IR receiver on pin 1
decode_results results; // Results object for IR decoding

void setup() {
  lcd.begin(16, 2); // Initialize the LCD: 16 columns and 2 rows
  lcd.print("Gas Sensor");
  pinMode(BUZZER_PIN, OUTPUT); // Set the buzzer pin as an output
  Serial.begin(9600); // Initialize Serial communication
  delay(2000);
  lcd.clear();
  irrecv.enableIRIn(); // Enable the IR receiver
}

void loop() {
  int sensorValue = analogRead(sensorPin); // Read sensor value
  float voltage = (sensorValue); // Convert to voltage
  float ppm = (voltage); // Convert to ppm (adjust as needed)

  lcd.setCursor(0, 0); // Set the cursor to the first row, first column
  lcd.print("Gas Value: ");
  lcd.print(ppm);
  lcd.print(" ppm");

  if (sensorValue < 720
  ) {
    analogWrite(BUZZER_PIN, 1000); // Turn on the buzzer
    buzzerState = 1; // Update the buzzer state
  } else {
    if (buzzerState == 0) {
      analogWrite(BUZZER_PIN, 0); // Turn off the buzzer
      buzzerState = 0; // Update the buzzer state
    }
  }

  int irSensorValue = analogRead(sensorPin);

  if (irSensorValue < 720) {
    // Send IR signal to turn off AC
    irSender.sendNEC(0xD5FFD64F, 32); // Adjust the IR code based on your AC's remote
    Serial.println("Turning off AC");

    delay(5000); // Wait for 5 seconds to avoid repeated signals
  }

  if (irrecv.decode(&results)) {
    // Print received IR code in HEX
    Serial.println(results.value, HEX);
    irrecv.resume(); // Continue receiving IR signals
  }

  delay(1000); // Adjust delay based on your application
  lcd.clear();
}