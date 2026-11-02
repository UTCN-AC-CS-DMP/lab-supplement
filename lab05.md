# LAB 5

**COMMS - Serial, UART and I2C**

<details>

<summary>Ex 1 - Serial</summary>

## Ex 1 - Serial

```cpp
#include <LiquidCrystal.h>  //include LCD library

String inputString = "";    // create an empty string to hold the incoming serial data

// Boolean flag to test whether a complete string has been
// received (enter pressed in serial monitor)
boolean stringComplete = false;

// LCD WITH BUTTONS
// LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// LCD WITHOUT BUTTONS
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() {
  // initialize serial interface
  Serial.begin(9600);  // implicit serial frame format

  //initialize lcd
  lcd.begin(16, 2);

  // reserve 200 bytes for the string
  inputString.reserve(200);
}

void loop() {
  // display the string when new line is received
  if (stringComplete) {
    lcd.setCursor(0, 0);
    lcd.print(inputString);
    Serial.println(inputString);
    // empty the string
    inputString = "";
    // reset the flag
    stringComplete = false;
  }
}

/**
  SerialEvent is called when new data is received on the RX port
  This function is automatically called when the loop is called.
  If we add delays in loop we will also delay the showing of the
  result. 
  */
void serialEvent() {
  while (Serial.available()) {
    // read the new received byte
    char inChar = (char)Serial.read();

    // check if new line character has been received; if
    // not, add it to the string
    // we do not add new line in input string since it
    // will show an extra character in the LCD
    if (inChar != '\n')
      inputString += inChar;

    // diff the receive character is new line, set the flag
    // so that the loop will know to display
    // the received data
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
```

</details>

<details>

<summary>Ex 2 - UART</summary>

## Ex 2 - UART Bi-directional

```cpp
String fromMonitor = "";   // USB -> other board
String fromBoard   = "";   // other board -> USB
bool monitorComplete = false;
bool boardComplete   = false;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  fromMonitor.reserve(200);
  fromBoard.reserve(200);
}

void loop() {
  if (monitorComplete) {
    Serial.println("Sending: " + fromMonitor);
    Serial1.println(fromMonitor);
    fromMonitor = "";
    monitorComplete = false;
  }
  if (boardComplete) {
    Serial.println("Received: " + fromBoard);
    fromBoard = "";
    boardComplete = false;
  }
}

// called by the core when Serial (USB) has data
void serialEvent() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\n')       monitorComplete = true;
    else if (c != '\r')  fromMonitor += c;
  }
}

// called by the core when Serial1 has data
void serialEvent1() {
  while (Serial1.available()) {
    char c = (char)Serial1.read();
    if (c == '\n')       boardComplete = true;
    else if (c != '\r')  fromBoard += c;
  }
}
```

</details>

<details>

<summary>Ex 3 - I2C</summary>

## Ex 3 - I2C Unidirectional

### Transmitter

```cpp
// CODE FOR TRANSMITTER

#include <LiquidCrystal.h>  // include LCD library
#include <Wire.h>           // include I2C library

// LCD WITH BUTTONS
// LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// LCD WITHOUT BUTTONS
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int x = 0;

void setup() {
  // Open I2C bus as transmitter
  Wire.begin();
  lcd.begin(16, 2);
  lcd.print("TRANSMITTER");
}

void loop() {
  Wire.beginTransmission(9);  // transmit to device #9
  Wire.write(x);              // transmit x
  Wire.endTransmission();     // stop transmission
  lcd.setCursor(0, 1);        // set the cursor to the second row
  lcd.print(x);               // print the value
  x++;                        // increment x
  if (x > 5) x = 0;           // reset x when it reaches 6
  delay(500);
}
```

### Receiver

```cpp
// CODE FOR RECEIVER

#include <LiquidCrystal.h>  // include LCD library
#include <Wire.h>           // include I2C library

// LCD WITH BUTTONS
// LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// LCD WITHOUT BUTTONS
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int x = 0;

void setup() {
  // Start I2C receiver at address 9
  Wire.begin(9);

  // attach a function to be called when we receive
  // something on the I2C bus
  Wire.onReceive(receiveEvent);

  lcd.begin(16, 2);
  lcd.print("RECEIVER");
}

void receiveEvent(int bytes) {
  x = Wire.read();  // read I2C received character
}

void loop() {
  lcd.setCursor(0, 1);  // display received character
  lcd.print(x);
}
```

</details>
