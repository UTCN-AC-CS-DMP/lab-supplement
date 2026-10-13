# LAB 3

**THE HITACHI HD44780 LCD SHIELD & EXTERNALLY TRIGGERED INTERRUPTS**

<details>

<summary>Ex 1</summary>

## Ex 1 - HD44780 Read the number of elapsed seconds from the program

```cpp
//includes the LCD library
#include <LiquidCrystal.h>

/* pin 7 - LCD RS 
 * pin 6 - LCD Enable
 * pin 5 - LCD D4 
 * pin 4 - LCD D5 
 * pin 3 - LCD D6 
 * pin 2 - LCD D7 
 The 7-th pin of the LCD is connected to the display brightness control
 potentiometer! */

// Init the LCD with the stated pin numbers
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
unsigned long time;

void setup() {
  // Sets the no. of rows and columns of the LCD
  lcd.begin(16, 2);
}

void loop() {
  // READ THE NUMBER OF ELAPSED SECONDS FROM THE PROGRAM

  // start
  time = millis() / 1000;

  // Set the cursor on col 0, row 0 (first row)
  lcd.setCursor(0, 0);

  // Write a string of characters
  lcd.print("Hello Children");

  // Move the cursor in the middle of the second row (row 1)
  lcd.setCursor(7, 1);

  // Display the elapsed time
  lcd.print(time);
}
```

</details>

<details>

<summary>Ex 2</summary>

## Ex 2 - HD44780 Custom characters

```cpp
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Character matrix for the first character: every line is a row of pixels of
// the character
byte happy[8] = {
  B00000,
  B11011,
  B11011,
  B00000,
  B00000,
  B10001,
  B01110,
};

// Matrix for the second character
byte sad[8] = {
  B00000,
  B11011,
  B11011,
  B00000,
  B00000,
  B01110,
  B10001,
};

void setup() {
  lcd.begin(16, 2);

  // The 2 characters are stored in the CGROM, user defined area, pos. 0 and 1
  lcd.createChar(0, happy);
  lcd.createChar(1, sad);

  // Display the first line: a string followed by the 1st user defined char
  lcd.setCursor(0, 0);
  lcd.print("Happy ");
  lcd.write(byte(0));  // See the difference between print and write

  /*
   When you are referring the "0" user defined char you must write a cast to
   the "byte" type, otherwise the compiler throws an error. Exception is the
   case when you are referring a varaiable:
     byte zero = 0;
     lcd.write(zero);
  */

  // Display the second line
  lcd.setCursor(0, 1);
  lcd.print("Sad ");
  lcd.write(1);  // when you are referring other characters then "0" it is not
                 // necessary to cast
}

void loop() {}
```

</details>

<details>

<summary>Ex 3</summary>

## Ex 3 - External interrupts using the AVR regs

```cpp
// Include the header for the avr interrupt system
#include "avr/interrupt.h"

// Include the LCD library
#include <LiquidCrystal.h>

// Init the LCD
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
volatile int buttonVariable;  //public variable that can be modified by the ISR

void setup(void) {
  // Init the variable shared between the ISR and the main program
  buttonVariable = 0;

  // Set the LCD row and col number
  lcd.begin(16, 2);
  lcd.print("Incepe experimentul");
  delay(1000);  // perform a 1 sec delay to display this message on the LCD

  // Set pin 21 as input (the pin corresponding to INT0)
  pinMode(21, INPUT_PULLUP);

  // Set pin 20 as input (the pin corresponding to INT1)
  pinMode(20, INPUT_PULLUP);

  pinMode(13, OUTPUT);     // Set pin 13 as output
  digitalWrite(13, HIGH);  // Light up the onboard LED
  delay(1000);
  
  EIMSK |= (1 << INT0);   // Activate INT0
  EIMSK |= (1 << INT1);   // Activate INT1
  EICRA |= (1 << ISC01);  // Specify INT0 triggering behavior: falling edge
  EICRA |= (1 << ISC11);  // Same behavior for INT1
  sei();                  // Global interrupt system activation
  digitalWrite(13, LOW);  // Turn off the onboard LED
  lcd.clear();            // Erase the LCD screen
}

void loop() {
  // If an interrupt was triggered/executed the LCD has to be erased and the
  // main message displayed
  if (buttonVariable == 1) {
    lcd.clear();         // Erase the LCD
    buttonVariable = 0;  // Global variable re-initialized
  }
  delay(1000);
  lcd.setCursor(0, 0);    // Set the LCD cursor
  lcd.print("Liniste…");  // Display a message
}

// ISR for INT0
// "INT0_vect" is a predefined name (address) for INT0 ISR
ISR(INT0_vect) {
  digitalWrite(13, !digitalRead(13));  // Toggle pin 13
  lcd.setCursor(0, 0);                 // Move the LCD cursor to the top-left
  lcd.print("Intrerupem");             // Display message
  lcd.setCursor(0, 1);                 // Cursor to bottom-left
  lcd.print("ptr stirea zilei");       // Display message
  buttonVariable = 1;
}

// ISR for INT1
ISR(INT1_vect) {
  digitalWrite(13, !digitalRead(13));
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Stirea Doi");
  buttonVariable = 1;
}
```

</details>

<details>

<summary>Ex 4</summary>

## Ex 4 - External interrupts using Arduino functions

```cpp
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

volatile int buttonVariable;

void setup() {
  buttonVariable = 0;

  lcd.begin(16, 2);
  lcd.print("A inceput");
  lcd.setCursor(0, 1);
  lcd.print("din nou");
  delay(1000);

  // (OLD WAY) The two interrupt pins 21 and 20 declared as inputs with pull-up
  // resistors activated by writing a HIGH value to the inputs
  // pinMode(20, INPUT);
  // pinMode(21, INPUT);
  // digitalWrite(20, HIGH);
  // digitalWrite(21, HIGH);

  // (NEW WAY) The two interrupt pins 21 and 20 declared as inputs with pull-up
  pinMode(20, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);

  // Attach ISRs to the interrupts corresponding to pins 21 and 20 (INT0 and INT1)
  attachInterrupt(digitalPinToInterrupt(20), functieUnu, RISING);
  attachInterrupt(digitalPinToInterrupt(21), functieDoi, CHANGE);
}

void loop() {
  // Insert task here for the normal program flow ...
  lcd.print("Programul principal");
  delay(1000);
}

// First ISR function
void functieUnu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Functia Unu");
}

// Second ISR function
void functieDoi() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Functia Doi");
}
```

</details>
