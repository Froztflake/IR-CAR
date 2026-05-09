#include <Arduino.h>
#include <IRremote.hpp> 

// Pin Definitions
const int RECV_PIN = 7; // Connect IR receiver to Pin 7
int RforwardPin = 11;
int RbackwardPin = 12;
int LforwardPin = 9;
int LbackwardPin = 10;
int RMotorSpeedPin = 6;
int LMotorSpeedPin = 5;

// Movement States
enum State { STOPPED, FORWARD, BACKWARD };
State currentState = STOPPED;

// Function Prototypes
void DriveForward();
void DriveBackward();
void DriveStop();

void setup() {
  pinMode(LforwardPin, OUTPUT);
  pinMode(LbackwardPin, OUTPUT);
  pinMode(RforwardPin, OUTPUT);
  pinMode(RbackwardPin, OUTPUT);
  pinMode(RMotorSpeedPin, OUTPUT); 
  pinMode(LMotorSpeedPin, OUTPUT);

  // Start IR Receiver
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  Serial.begin(9600);
  Serial.println("System Ready - Waiting for IR...");
  
  // Start at base in DriveStop
  DriveStop();
}

void loop() {
  if (IrReceiver.decode()) {
    // Get the command (modern IRremote v4 syntax)
    uint16_t command = IrReceiver.decodedIRData.command;
    Serial.print("Command: 0x");
      Serial.println(command, HEX);

    // Logic based on your buttons
      switch (command) {
        case 0xC:  // Button 1 (Forward)
            currentState = FORWARD;
          break;
        case 0x18: // Button 2 (Backward)
            currentState = BACKWARD;
          break;
        case 0x5E: // Button 3 (Stop)
            currentState = STOPPED;
          break;
      }
    
    IrReceiver.resume(); // Ready for next signal
}

  // Execute the state continuously without delay()
  if (currentState == FORWARD) {
    DriveForward();
  } else if (currentState == BACKWARD) {
    DriveBackward();
  } else {
    DriveStop();
  }
}

// Removed delay() from these functions so the loop stays responsive
void DriveForward() {
  analogWrite(RMotorSpeedPin, 255); 
  analogWrite(LMotorSpeedPin, 200); 
  digitalWrite(RforwardPin, HIGH);
  digitalWrite(RbackwardPin, LOW);
  digitalWrite(LforwardPin, HIGH);
  digitalWrite(LbackwardPin, LOW);
}

void DriveBackward() {
  analogWrite(RMotorSpeedPin, 255); 
  analogWrite(LMotorSpeedPin, 200); 
  digitalWrite(RforwardPin, LOW);
  digitalWrite(RbackwardPin, HIGH);
  digitalWrite(LforwardPin, LOW);
  digitalWrite(LbackwardPin, HIGH);
}

void DriveStop() {
  analogWrite(RMotorSpeedPin, 0);
  analogWrite(LMotorSpeedPin, 0);
  digitalWrite(RforwardPin, LOW);
  digitalWrite(RbackwardPin, LOW);
  digitalWrite(LforwardPin, LOW);
  digitalWrite(LbackwardPin, LOW);
