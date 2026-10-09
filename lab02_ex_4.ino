int latchPin = 4;
int clockPin = 7;
int dataPin = 8;  // SSD pins

const unsigned char ssdlut[] = { 0b00111111, 0b00000110,
                                 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101,
                                 0b00000111, 0b01111111, 0b01101111 };

const unsigned char anodelut[] = { 0b00000001, 0b00000010,
                                   0b00000100, 0b00001000 };

const unsigned char digits[] = { 1, 2, 3, 4 };  // The number to be
//displayed is 1234. You can change it.

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  // The three pins connected to
  // the shift register must be output pins
}

void loop() {
  for (char i = 0; i <= 3; i++)  // For each of the 4 digits
  {
    unsigned char digit = digits[i];          // the current digit
    unsigned char cathodes = ~ssdlut[digit];  // The
    //cathodes of the current digit, we’ll
    //negate the value from the original LUT
    digitalWrite(latchPin, LOW);  // Activate the latch to
    //allow writing
    shiftOut(dataPin, clockPin, MSBFIRST, cathodes);  //
    //shift out the cathode byte
    shiftOut(dataPin, clockPin, MSBFIRST, anodelut[i]);
    // shift out the anode byte
    digitalWrite(latchPin, HIGH);  // De-activate the latch
    //signal
    delay(2);  // Short wait
  }
}
