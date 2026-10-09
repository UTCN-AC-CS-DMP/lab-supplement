const int latchPin = 4;
const int clockPin = 7;
const int dataPin = 8;

const unsigned char ssdlut[10] = {
  0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
  0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111
};

const unsigned char anodelut[4] = { 0b0001, 0b0010, 0b0100, 0b1000 };


int number = 1234;                       // starting value
const unsigned long INCREMENT_MS = 200;  // increment period
unsigned long lastInc = 0;


void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  if (millis() - lastInc >= INCREMENT_MS) {
    lastInc = millis();
    number = (number + 1) % 10000;  // 9999 -> 0
  }

  unsigned char digits[4] = {
    (unsigned char)((number / 1000) % 10),  // thousands
    (unsigned char)((number / 100) % 10),   // hundreds
    (unsigned char)((number / 10) % 10),    // tens
    (unsigned char)(number % 10)            // ones
  };

  for (uint8_t i = 0; i < 4; i++) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, ~ssdlut[digits[i]]);  // cathodes: 0 = lit
    shiftOut(dataPin, clockPin, MSBFIRST, anodelut[i]);         // anode: one digit
    digitalWrite(latchPin, HIGH);
    delay(2);
  }
}
