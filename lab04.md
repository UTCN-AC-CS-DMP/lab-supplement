# LAB 4

**TIMERS & INTERNALLY TRIGGERED INTERRUPTS**

<details>

<summary>Ex 1</summary>

## Ex 1 - TIMER1 simple overflow interrupt

```cpp
#include <avr/io.h>
#include <avr/interrupt.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
volatile int myVar;

void setup() {
  myVar = 0;
  //init Timer1
  cli();  // disable the global interrupts system in order to
  //setup the timers
  TCCR1A = 0;             // SET TCCR1A and B to 0
  TCCR1B = 0;             // timer set to Normal mode (WGMx3:0 = 0)
  TIMSK1 = (1 << TOIE1);  //timer overflow interrupt enable
  //for timer 1
  //Set the prescaler to 1024
  // CPU freq. is 16 MHZ and Timer1 is on 16 bits
  // Counter increment at every dt = 1024 / (16 * 10^6) sec
  // Overflow event occurs at every: dt * 2^16 = 4.194 sec.
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  lcd.begin(16, 2);
  lcd.print("Timers");
  sei();  // enable the global interrupts system
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print(myVar);
  lcd.setCursor(5, 1);
  lcd.print(TCNT1);
}

ISR(TIMER1_OVF_vect) {
  myVar = myVar + 1;
}
```

</details>

<details>

<summary>Ex 2</summary>

## Ex 2 - Timer with CTC

```cpp
#include <avr/io.h>
#include <avr/interrupt.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
volatile int myVar;

void setup() {
  // Initialize Timer1
  cli();       // disable the global interrupts system in order to setup the timers
  TCCR1A = 0;  // SET TCCR1A and B to 0
  TCCR1B = 0;
  // Set the OCR1A with the desired value:
  OCR1A = 15624;
  // Active CTC mode:
  TCCR1B |= (1 << WGM12);
  // Set the prescaler to 1024:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  // Enable the Output Compare interrupt (by setting the mask bit
  TIMSK1 |= (1 << OCIE1A);
  lcd.begin(16, 2);
  lcd.print("Timer with CTC");
  sei();  // enable global interrupts
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print(myVar);
  lcd.setCursor(5, 1);
  lcd.print(TCNT1);
}

ISR(TIMER1_COMPA_vect) {
  myVar = myVar + 1;
}
```

</details>

<details>

<summary>Ex 3</summary>

## Ex 3 - TimerOne lib

```cpp
#include <TimerOne.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
volatile int myVar;

void setup(void) {
  Timer1.initialize(1000000);  // init the timing interval
  //for event triggering (1s = 10-6s)
  Timer1.attachInterrupt(ShowMessage);  // The function is
  //called at the preset time interval
}

void ShowMessage(void) {
  lcd.setCursor(0, 0);
  lcd.print(myVar);
  myVar++;
}

void loop(void) {
  // Do something else …
}
```

</details>

<details>

<summary>Ex 4</summary>

## Ex 4 - Tone

```cpp
#include "pitches.h"  // contains the frequency values for all the notes

// notes in the melody – constant values defining frequency
// for each used note
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

void setup() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    // iterate over the notes of the melody:
    // to calculate the note duration, take one second
    // divided by the note type.
    // e.g. quarter note = 1000 / 4, eighth note = 1000/8,
    // etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);
    // to distinguish the notes, set a minimum time between them : note's duration + 30%
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);  // stop the tone playing for current note
  }
}

void loop() {}  // no need to repeat the melody
```

</details>

<details>

<summary>Ex 5</summary>

## Ex 5 - PWM

```cpp
int buzzerPin = 8;  // pin for attaching the piezoelectric speaker
int puls = 0;       // duty cycle, initially 0
int step = 10;      // duty cycle increment step
int ledPin = 13;    // on-board LED

void setup() {
  // Pin direction setup
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // set the duty cycle for both the buzzer and the LED
  analogWrite(buzzerPin, puls);
  analogWrite(ledPin, puls);
  // increment the duty cycle
  puls = puls + step;
  // change the increment direction at the end of the interval
  if (puls <= 0 || puls >= 255) {
    step = -step;
  }
  // small delay to sense the effects
  delay(30);
}
```

</details>
