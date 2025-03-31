/**
 * DHT11 Weather Info Readout
 * This sketch reads humidity and temperature from a DHT11 sensor and outputs it
 * on a seven segment display, using two LEDs to indicate the current display mode
 * and a button to toggle between modes. In the spirit of the open-source community
 * and making electronics projects accessible  to everyone, this code is entirely
 * free to use.
 *
 * Author: Alex Griffin
 * License: MIT
 */

#include <DHT11.h>
#include <SevSeg.h>

// Declare what pins the components connect to
const int SENSOR_PIN = 13;
const int BUTTON_PIN = 12;

const int TEMPERATURE_LIGHT_PIN = 11;
const int HUMIDITY_LIGHT_PIN = 10;

const int DIGIT_ONE = 7;
const int DIGIT_TWO = 6;
const int DIGIT_THREE = 5;
const int DIGIT_FOUR = 4;
const int SEGMENT_A = 53;
const int SEGMENT_B = 52;
const int SEGMENT_C = 50;
const int SEGMENT_D = 48;
const int SEGMENT_E = 46;
const int SEGMENT_F = 44;
const int SEGMENT_G = 42;
const int SEGMENT_DOT = 40;

// Logic variables
int timer = 0; //How many repeats since last read the sensor
const int READ_SENSOR_WHEN = 10000;
int temperature;
int humidity;
int output; // What will be displayed on the screen
bool mode = 0;
bool pressingButton = 0; // Makes switching modes easier for the user
/*
*	temperature mode = 1
*	humidity mode = 0
*/

// Name and start libraries.
DHT11 sensor(SENSOR_PIN);
SevSeg screen;
void setupScreen() {
	byte numDigits = 4;
	byte digitPins[] = {
		DIGIT_ONE,
		DIGIT_TWO,
		DIGIT_THREE,
		DIGIT_FOUR
	};
	byte segmentPins[] = {
		SEGMENT_A,
		SEGMENT_B,
		SEGMENT_C,
		SEGMENT_D,
		SEGMENT_E,
		SEGMENT_F,
		SEGMENT_G,
		SEGMENT_DOT
	};
	screen.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
}

// Define important functions
void updateScreen(int number) {
	screen.setNumber(number, 0);
	screen.refreshDisplay();
}

void readSensor() {
    /* Reads information from the sensor and stores it in these variables:
    * results: stores the error code, if any (otherwise it stores "0" to say theres no errors)
    * temperature: stores temperature
    * humidity: stores humidity
    */
    int results = sensor.readTemperatureHumidity(temperature, humidity);
    if (results != 0) {
      // Prints the error code to the serial monitor
      Serial.println(DHT11::getErrorString(results)); 
    }
}

void pressButton() {
  if (digitalRead(BUTTON_PIN) & !pressingButton) {
    mode = !mode;
    pressingButton = 1;
  }
  if (!digitalRead(BUTTON_PIN)) {
    pressingButton = 0;
  }
}

void setup() {
  Serial.begin(9600); //Optional, adds shell to print debug messages in
  Serial.println("Hello world"); //Optional, example message
  setupScreen();
  
  // Setup lights
  pinMode(TEMPERATURE_LIGHT_PIN, OUTPUT);
  pinMode(HUMIDITY_LIGHT_PIN, OUTPUT);
  
  // Setup button
  pinMode(BUTTON_PIN, INPUT);
  
  // The sensor has already been set up on line 38

  // Initial temperature read
  readSensor();
}

void loop() {

  pressButton(); // Changes mode
  
  // Determine output based on mode
  if (mode) {
    output = temperature;
  } else {
    output = humidity;
  }
  
  /* Determine whether to show information on 
  the display or read the sensor */
  timer++;
  if (timer < READ_SENSOR_WHEN) {
    // Show information on the display
    updateScreen(output);
    digitalWrite(TEMPERATURE_LIGHT_PIN, mode);
    digitalWrite(HUMIDITY_LIGHT_PIN, !mode); // Opposite of temp light
  }
  else {
    timer = 0; // Resets the timer, to loop again
    readSensor();
  }
}


