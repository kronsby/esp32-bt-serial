// ESP32 Bluetooth serial
// 
// This is the basis of a project I am working on to create a media controller
// that connects over bluetooth to my Android phone. The plan is to create a
// custom Android app that will allow play/pause/rewind functionality to aid
// in practicing my guitar.
//
// In the current state, it simply allows for 2 way communication between the
// ESP32 and my phone. The connection works and I can use serial to communicate
// between the ESP and the phone in both directions. I don't think my project
// will require both directions, but it is cool anyway.
//
// I found https://lastminuteengineers.com/esp32-bluetooth-classic-tutorial/
// very helpful!
//
#include "BluetoothSerial.h"

const int buttonPin = 13;

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

String sendData = "play_pause\n";

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    delay(50); // Debouncing
    if (digitalRead(buttonPin) == LOW) {
      Serial.println("Button Pressed - Sending Play/Pause");

      Serial.println(SerialBT.isClosed());
      Serial.println(SerialBT.connected());
      // Send the play/pause fwd rewind commands
      if (SerialBT.connected()) {
        if (SerialBT.write((const uint8_t *)sendData.c_str(), sendData.length()) != sendData.length()) {
          Serial.println("tx: error");
        } else {
          Serial.printf("tx: %s", sendData.c_str());
        }
      }

      while (digitalRead(buttonPin) == LOW); // Wait for button release
    }
  }
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(20);
}
