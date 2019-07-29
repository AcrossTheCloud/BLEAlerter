
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEAddress.h>
#include <BLEUUID.h>

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

#define outputPin 13

int scanTime = 5; //In seconds
BLEScan* pBLEScan;

const std::string myBoseHeadphones = "038dcafed00d";

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      if (advertisedDevice.haveManufacturerData()) {
        std::string hexManufacturerString = std::string(BLEUtils::buildHexData(nullptr, (uint8_t*)advertisedDevice.getManufacturerData().data(), advertisedDevice.getManufacturerData().length()));
        if (hasEnding(hexManufacturerString, myBoseHeadphones)) {
          digitalWrite(outputPin, HIGH); // trigger LED
          Serial.printf("Matched Device: %s \n", advertisedDevice.toString().c_str());   
        } else {
          Serial.printf("Other Device: %s \n", advertisedDevice.toString().c_str()); 
        }     
      } else {
        Serial.printf("Other Device: %s \n", advertisedDevice.toString().c_str()); 
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
