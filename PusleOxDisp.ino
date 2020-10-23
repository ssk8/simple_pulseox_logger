#include <SparkFun_Bio_Sensor_Hub_Library.h>
#include <Wire.h>
#include "RTClib.h"
#include <SD.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>


#define CHIPSELECT A1
#define RES_PIN A2
#define MFIO_PIN A3


SparkFun_Bio_Sensor_Hub bioHub(RES_PIN, MFIO_PIN); 
bioData sensor;  
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
RTC_DS3231 rtc;

void setup(){
  Wire.begin();
  rtc.begin();
  SD.begin(CHIPSELECT);
  bioHub.begin();
  bioHub.configBpm(MODE_TWO); 

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
 
  display.println("PulseOxRecorder"); 
  display.print("Version: 0.3 ");
  display.display();

  delay(900);
}

void loop(){
    
    DateTime now = rtc.now();
    sensor = bioHub.readBpm();

    display.clearDisplay();
    display.setCursor(0, 5);
    display.print("BPM:");
    display.setCursor(55, 5);
    display.print("S02:");
    display.setCursor(25, 0);
    display.setTextSize(2);
    display.print(sensor.heartRate);
    display.setCursor(80, 0);
    display.print(sensor.oxygen);
    display.setTextSize(1);
    display.println("%");
    display.setCursor(0, 18);
    switch (int(sensor.extStatus))
    {
      case 0:	 display.print("Confidence: ");
               display.print(sensor.confidence);
               display.print(" %");
        break;
      case -1:	display.print("Object Detected");
        break;
      case 1:	display.print("Not Ready");
        break;
      case -2:	display.print("Excessive sensor motion");
        break;
      case -3:	display.print("No Object Detected");
        break;
      case -4:	display.print("Pressing too hard");
        break;
      case -5:	display.print("That's not a \n finger, pervert!");
        break;
      case -6:	display.print("Excessive finger motion");
        break;
    }
  display.println();
  if (sensor.confidence > 80) {
    File dataFile = SD.open("pulseox.csv", FILE_WRITE);
      if (dataFile) {
        dataFile.print(now.unixtime()); 
        dataFile.print(", ");
        dataFile.print(sensor.heartRate);
        dataFile.print(", ");
        dataFile.print(sensor.oxygen); 
        dataFile.print(", ");
        dataFile.print(sensor.confidence);
        dataFile.println(); 
        display.print("writing to SD");
        dataFile.close();
      }
  }
    display.display();

  if (Serial) {
    Serial.print(now.unixtime());
    Serial.print(" ");
    Serial.print(sensor.extStatus); 
    Serial.print(" ");
    Serial.print(sensor.heartRate);
    Serial.print(" ");
    Serial.print(sensor.oxygen); 
    Serial.print(" ");
    Serial.print(sensor.confidence); 

    Serial.print(" status code: "); 
    Serial.print(sensor.extStatus); 
    Serial.println();
  }

    delay(1000); 
}
