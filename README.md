# BLEAlerter
Code for alerting (on a neopixel ring) when a small transmitter comes into range.

## Software dependencies
[Arduino IDE](https://arduino.cc) required.
In preferences > additional boards manager URLs, add https://dl.espressif.com/dl/package_esp32_index.json and https://adafruit.github.io/arduino-board-index/package_adafruit_index.json to the comma-delimited list.
In tools > boards manager, search for esp32 and make sure the latest version is installed, and search for flora and make sure the latest version of the Adafruit AVR boards is installed.

Download the zip files for [Adafruit Neopixel](https://github.com/adafruit/Adafruit_NeoPixel) and [Adafruit Bluefruit LE nRF51](https://github.com/adafruit/Adafruit_BluefruitLE_nRF51).
Install both into your Arduino libraries folder by following [these instructions](https://www.arduino.cc/en/guide/libraries#toc4)

## Components
* [esp32 board](https://www.adafruit.com/product/3405)
* [flora](https://www.adafruit.com/product/659)
* [bluefruit LE module](https://www.adafruit.com/product/2487)
* [neopixel ring](https://www.adafruit.com/product/1586)

## Wiring
Best and safest to do all wiring while everything is powered off. Otherwise make sure ground is connected first when attaching, then power then signal lines and reverse that ordering for disconnect.

### flora <-> bluefruit LE
| flora  | bluefruit LE |
| ------------- | ------------- |
| GND  | GND  |
| 3.3V | 3.3V |
| TX | RX |
| RX | TX |

### flora <-> neopixel ring
| flora  | neopixel ring |
| ------------- | ------------- |
| GND  | GND  |
| VBATT | PWR +5V |
| #6 | Data Input |

Note that the neopixel ring has 2x 5V and 2x GND connectors for daisy chaining, you need only one. The neopixel ring linked above has the resistor built in but other neopixel strips may need one between Data Input and Data Out and if you're using a large battery / DC supply to separately power the strip, it may be a good idea to add a capacitor between + and - (GND), per [these instructions](https://learn.adafruit.com/adafruit-neopixel-uberguide/basic-connections)

## software configuration 
### esp32
You will need to update the transmitter code to look for the MAC address of your specific bluefruit LE, which can be found at [this line of code](https://github.com/AcrossTheCloud/BLEAlerter/blob/e13b4029e2bdf608566512221fb8cc17100353cc/transmitter/transmitter.ino#L16). The MAC address can be found via a BLE Scanner (except on iOS).

Alternatively (if you only have one bluefruit LE in your general vicinity) you could remove [this line]( https://github.com/AcrossTheCloud/BLEAlerter/blob/e13b4029e2bdf608566512221fb8cc17100353cc/transmitter/transmitter.ino#L108)

### flora
If you're using a different neopixel setup, you will need to update the [number of pixels]( https://github.com/AcrossTheCloud/BLEAlerter/blob/e13b4029e2bdf608566512221fb8cc17100353cc/detector/detector.ino#L47) and the final parameter for frequency and colour format at this [function call](https://github.com/AcrossTheCloud/BLEAlerter/blob/e13b4029e2bdf608566512221fb8cc17100353cc/detector/detector.ino#L50)




