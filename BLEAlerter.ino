
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEAddress.h>

#define outputPin 13

int scanTime = 5; //In seconds
BLEScan* pBLEScan;

const BLEAddress myAppleWatch = BLEAddress(std::string("1c:1a:c0:ca:fe:00")); // lower case hex

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      if (advertisedDevice.getAddress().equals(myAppleWatch)) {
          digitalWrite(outputPin, HIGH); // trigger LED
          Serial.printf("Matched Device: %s \n", advertisedDevice.getAddress().toString().c_str());        
      } else {
          Serial.printf("Other Device: %s \n", advertisedDevice.getAddress().toString().c_str()); 
      }
    }
};

void setup() {
  Serial.begin(115200);

  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, LOW); // start with LED off
  
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(outputPin, LOW); // reset LED to off 
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(5000); // hold for five seconds before next scan
}
