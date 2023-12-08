#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32_NimBLE.h>

/*********
  Adam Ferrara
  2023-2-28
**/

//create instance, updating name

BLEMIDI_CREATE_INSTANCE("ESP32C3 Page Turner", MIDI)

unsigned long lastDebounceTime = 0;
unsigned long debounce = 15;
const int forwardPin = D9;//12 for WROOM, D9 for XIAO_ESP32C3
const int LED_BUILTIN = 13;

int midiChannel = 1;
int previousButtonStatus = 1;
int buttonStatus = 1;
int reading = 0;
int previousReading = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(9600);


  BLEMIDI.setHandleConnected(onConnected);
  BLEMIDI.setHandleDisconnected(onDisconnected);

  pinMode(forwardPin, INPUT_PULLUP);

  MIDI.begin();
}

void loop() {
  getValue();
}

void getValue() {

  reading = digitalRead(forwardPin);

  if (reading != previousReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounce) {
    buttonStatus = reading;
    if (buttonStatus == 0 && previousButtonStatus == 1) {
      MIDI.sendProgramChange(16, midiChannel);
      Serial.println("Program change sent");
    }
    previousButtonStatus = buttonStatus;
  }
  previousReading = reading;
}



//set connection handling
void onConnected() {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("connected");
}
void onDisconnected() {
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("disconnected");
}
