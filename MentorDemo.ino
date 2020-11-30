/*
 * Riyadth's demo program for Spartronics Electronics show-and-tell
 * 
 * Copyright 2020 - All rights reserved
 */

#include <Servo.h>

// Analog input pins
#define KNOB_1      A3
#define KNOB_2      A4
#define KNOB_3      A5
#define LIGHTSENSOR A2

// RGB LED output pins (PWM)
#define LED_RED     5
#define LED_GREEN   11
#define LED_BLUE    6

// Servo output pin (PWM)
#define SERVO       9

// Digital input pins (buttons)
#define BUTTON_1    7
#define BUTTON_2    4
#define BUTTON_3    2

// Piezo output pin
#define PIEZO       8

// Constants for value mapping
#define MAX_ANALOG_WRITE  255
#define MAX_ANALOG_READ   1023
#define MIN_PIEZO_HZ      50
#define MAX_PIEZO_HZ      4000

// Create a Servo 'object' to contro the servo with
Servo servo;

void setup() {
  /* Initialize all the pins */
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIEZO, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
  
  /* Initialize the servo, and move it home to 0 degrees */
  servo.attach(SERVO);
  servo.write(0);

  /* Initialize the serial console */
  Serial.begin(115200);
}

// Some 'global' variables to save the previous values
int previous_r = -1;
int previous_g = -1;
int previous_b = -1;

/* Function to print out the RGB values on the console */
void print_rgb(int r, int g, int b)
{
  /* Only print out the values if they changed since last time */
  if ((r != previous_r) || (g != previous_g) || (b != previous_b))
  {
    /* Print out the values neatly formatted */
    Serial.print('[');
    Serial.print(r);
    Serial.print(',');
    Serial.print(g);
    Serial.print(',');
    Serial.print(b);
    Serial.println(']');
    
    /* Save the new values for next time around */
    previous_r = r;
    previous_g = g;
    previous_b = b;
  }
}

// Saved information from the last time blink_led() was executed
unsigned long previousMillis;
int ledState = LOW;

/* Function to blink the on-board LED if the interval has passed */
void blink_led(long interval)
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval)
  {
    // Save the last time we blinked the LED
    previousMillis = currentMillis;

    // Switch (toggle) the state of the variable
    if (ledState == LOW)
    {
      ledState = HIGH;
    }
    else
    {
      ledState = LOW;
    }

    // Set the built-in LED to the toggled state
    digitalWrite(LED_BUILTIN, ledState);
  }

}

void loop() {
  /* Declare some varialbes to use later */
  int red_intensity;
  int green_intensity;
  int blue_intensity;

  /* Toggle (blink) the on-board LED every 1000ms */
  blink_led(1000);
  
  /* Declare other variables and read the ADC to initialize them */
  int knob_1 = analogRead(KNOB_1);
  int knob_2 = analogRead(KNOB_2);
  int knob_3 = analogRead(KNOB_3);

  /* If button 1 is pressed, play a 300Hz tone */
  if (digitalRead(BUTTON_1) == 0)
  {
    Serial.print("Button 1 pressed...");
    servo.write(50);  // Move to 50 degrees
    tone(PIEZO, 300);    
    while (digitalRead(BUTTON_1) == 0)
    {
      /* BUSY_WAIT */
      blink_led(150);
    }
    noTone(PIEZO);    // Stop the tone
    Serial.println(" and released.");
    servo.write(0);   // Move back to zero
  }

  /* If button 2 is pressed, play a 600Hz tone */
  if (digitalRead(BUTTON_2) == 0)
  {
    Serial.print("Button 2 pressed...");
    servo.write(100);  // Move to 100 degrees
    tone(PIEZO, 600);    
    while (digitalRead(BUTTON_2) == 0)
    {
      /* BUSY_WAIT */
      blink_led(300);
    }
    noTone(PIEZO);    // Stop the tone
    Serial.println(" and released.");
    servo.write(0);   // Move back to zero
  }

  /* If button 3 is pressed, make tones based on the light sensor */
  if (digitalRead(BUTTON_3) == 0)
  {
    Serial.print("Button 3 pressed...");
    servo.write(150);  // Move to 150 degrees
    while (digitalRead(BUTTON_3) == 0)
    {
      /* Read the light intensity and comute a tone value */
      int light_intensity = analogRead(LIGHTSENSOR);
      int pitch = map(light_intensity, 0, MAX_ANALOG_READ, MIN_PIEZO_HZ, MAX_PIEZO_HZ);
      tone(PIEZO, pitch);
      delay(100);      
      blink_led(500);
    }
    noTone(PIEZO);    // Stop the tone
    Serial.println(" and released.");
    servo.write(0);   // Move back to zero
  }

  /* Compute the RGB LED intensities based on the knob positions */
  red_intensity = map(knob_1, 0, MAX_ANALOG_READ, 0, MAX_ANALOG_WRITE);
  green_intensity = map(knob_2, 0, MAX_ANALOG_READ, 0, MAX_ANALOG_WRITE);
  blue_intensity = map(knob_3, 0, MAX_ANALOG_READ, 0, MAX_ANALOG_WRITE);

  /* Print out the intensity numbers for debugging */
  print_rgb(red_intensity, green_intensity, blue_intensity);

  /* Set the outputs to turn on the RGB LED */
  analogWrite(LED_RED, red_intensity);
  analogWrite(LED_GREEN, green_intensity);
  analogWrite(LED_BLUE, blue_intensity);
}
