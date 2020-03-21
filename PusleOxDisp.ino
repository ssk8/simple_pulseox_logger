#include <SparkFun_Bio_Sensor_Hub_Library.h>
#include <Wire.h>
#include "RTClib.h"
#include <SD.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int resPin = 4;
const int mfioPin = 5;
const int chipSelect = 6;

SparkFun_Bio_Sensor_Hub bioHub(resPin, mfioPin); 
bioData sensor;  
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
RTC_DS3231 rtc;

void setup(){
  Wire.begin();
  rtc.begin();
  SD.begin(chipSelect);
  int result = bioHub.begin();
  int error = bioHub.configBpm(MODE_TWO); 

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
 
  display.println("PulseOxRecorder"); 
  display.print("Version: 0.03 ");
  display.display();

  delay(4000);
}

void loop(){
    
    DateTime now = rtc.now();
    sensor = bioHub.readBpm();

    Serial.print(now.unixtime());
    Serial.print(" ");
    Serial.print(sensor.extStatus); 
    Serial.print(" ");
    Serial.print(sensor.heartRate);
    Serial.print(" ");
    Serial.print(sensor.oxygen); 
    Serial.print(" ");
    Serial.println(sensor.confidence); 

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Pulse: ");
    display.print(sensor.heartRate);
    display.println(" bpm");
    display.print("SO2: ");
    display.print(sensor.oxygen);
    display.println(" %");

    display.println();    
    display.print("Status: ");
    display.print(sensor.extStatus);
    display.print(" conf: ");
    display.print(sensor.confidence);
    display.println(" %");
   
    display.display();


  File dataFile = SD.open("pulseox.csv", FILE_WRITE);

  if (dataFile and sensor.extStatus == 0) {
    dataFile.print(now.unixtime()); 
    dataFile.print(", ");
    dataFile.print(sensor.heartRate);
    dataFile.print(", ");
    dataFile.print(sensor.oxygen); 
    dataFile.print(", ");
    dataFile.print(sensor.confidence);
    dataFile.println(); 
    dataFile.close();
  }
    delay(1000); 
}
