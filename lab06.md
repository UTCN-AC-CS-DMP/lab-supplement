# LAB 6

**ANALOG SIGNALS**

<details>

<summary>Ex 1 - Potentiometer</summary>

## Ex 1 - Potentiometer

```cpp
#include <LiquidCrystal.h>

// LCD WITH BUTTONS
// LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// LCD WITHOUT BUTTONS
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() {
  analogReference(DEFAULT);  // set the reference voltage to the
                             // default value (optional)
  lcd.begin(16, 2);          //init LCD
  lcd.setCursor(0, 0);
  lcd.print("Read sensor");
  pinMode(A1, INPUT);  // Set pin A1 as input (make sure that it
                       // was not set as output
}

void loop() {
  int val = analogRead(A1);  //reading the analog value
  lcd.setCursor(0, 1);
  lcd.print(val);
}
```

</details>

<details>

<summary>Ex 2 - Analog Temp</summary>

## Ex 2 - Analog temperature sensor

```cpp
float resolutionADC = .0049;   // default ADC resolution for the 5V
                               // reference = 0.049 [V] / unit
float resolutionSensor = .01;  // sensor resolution = 0.01V / °C

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("Temp [C]: ");
  float temp = readTempInCelsius(10, 0);  // read temp. 10 times and return the average
  Serial.println(temp);                   // display the result
  delay(200);
}

float readTempInCelsius(int count, int pin) {
  // read temp. count times from the analog pin
  float sumTemp = 0;
  for (int i = 0; i < count; i++) {
    int reading = analogRead(pin);
    float voltage = reading * resolutionADC;

    // subtract the DC offset and converts the value in degrees (C)
    float tempCelsius = (voltage - 0.5) / resolutionSensor;
    sumTemp = sumTemp + tempCelsius;  // accumulates the readings
  }
  return sumTemp / (float)count;  // return the average value
}
```

</details>

<details>

<summary>Ex 3 - ADC</summary>

## Ex 3 - ADC General

```cpp
#include <LiquidCrystal.h>

// LCD WITH BUTTONS
// LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// LCD WITHOUT BUTTONS
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() {
  ADCSRA |= ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));  // set the ADC clock to 16MHz/128 = 125kHz
  ADMUX  |= (1 << REFS0);                                  // Set the ref. voltage to Vcc (5V)
  ADCSRA |= (1 << ADEN);                                   // Activate the ADC
  ADCSRA |= (1 << ADSC);
}


void loop() {
  int val = read_adc(0);  //read the value
  lcd.setCursor(0, 1);
  lcd.print(val);
}

uint16_t read_adc(uint8_t channel) {
  ADMUX &= 0xE0;                // delete MUX0-4 bits
  ADMUX |= channel & 0x07;      // Sets in MUX0-2 the value of the
                                // new channel to be read
  ADCSRB = channel & (1 << 3);  // Set MUX5 value
  ADCSRA |= (1 << ADSC);        // start conversion
  while (ADCSRA & (1 << ADSC))
    ;  // Wait for the conversion to finish
  return ADCW;
}
```

</details>
