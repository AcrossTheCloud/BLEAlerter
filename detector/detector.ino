/*********************************************************************
 This is an example for our nRF51 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

// This sketch is intended to be used with the NeoPixel control
// surface in Adafruit's Bluefruit LE Connect mobile application.
//
// - Compile and flash this sketch to a board connected to/bundled with a nRF51
// - Open the Bluefruit LE Connect app
// - Switch to the NeoPixel utility
// - Click the 'connect' button to establish a connection and
//   send the meta-data about the pixel layout
// - Use the NeoPixel utility to update the pixels on your device

/* NOTE: This sketch required at least version 1.1.0 of Adafruit_Neopixel !!! */

#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

    #define FACTORYRESET_ENABLE         1
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"

#include "BluefruitConfig.h"

#define FACTORYRESET_ENABLE     1
#define NEOPIXEL_VERSION_STRING "Neopixel v2.0\n"
#define LED_PIN 8   /* Pin used to drive the NeoPixels */
#define LED_COUNT 1


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Create the bluefruit object, either software serial...uncomment these lines
/*
SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);
*/

/* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
Adafruit_BluefruitLE_UART ble(Serial1, BLUEFRUIT_UART_MODE_PIN);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
//                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
//                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


// A small helper
void error(const __FlashStringHelper*err) {
  if (Serial) Serial.println(err);
  while (1);
}

void serial_printf(const char * format, ...) {
  char buffer [48];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  if (Serial) Serial.print(buffer);
}


/**************************************************************************/
/*!
    @brief  Sets up the HW an the BLE module (this function is called
            automatically on startup)
*/
/**************************************************************************/
void setup(void)
{

  delay(500);

  if (Serial) Serial.begin(115200);
  
  ble.verbose(false); 

  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'

  /* Initialise the module */
  if (Serial) Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  if (Serial) Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    if (Serial) Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      if (Serial) Serial.println("Couldn't factory reset");
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  if (Serial) {
    Serial.println("Requesting Bluefruit info:");
    /* Print Bluefruit information */
    ble.info();
  }


  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }


  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  }

  // Set module to DATA mode

  ble.setMode(BLUEFRUIT_MODE_DATA);

}

void loop()
{
  // Echo received data
  while (ble.available()) 
  {

    int command = ble.read();

    switch (command) {
        case 'V': {   // Get Version
          commandVersion();
          break;
        }
      case 'A': {   // Alert
          alert();
          break;
       }
    }
  } 
  
}


void commandVersion() {
  sendResponse(NEOPIXEL_VERSION_STRING);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
 
  for(j=0; j<256*1; j++) { // 1 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
  // back to off:

  for(i=0; i< strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0,0,0));
  }
  strip.show();

}


void alert() {
  rainbowCycle(20); // 500 = delay in ms
  sendResponse("OK\n");
}

void sendResponse(char const *response) {
    ble.write(response, strlen(response)*sizeof(char));
}
