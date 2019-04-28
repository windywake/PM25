#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

Adafruit_AM2320 am2320 = Adafruit_AM2320();

int measurePin = 0; //Connect dust sensor to Arduino A0 pin
int ledPower = 2;   //Connect 3 led driver pins of dust sensor to Arduino D2

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

int BezzPin =  7;

// ESP13 wifi
#define WIFI_SSID       "DoitWiFi_Config"
#define WIFI_PASSWORD   "12345678"

// firebase
#define FIREBASE_HOST "fir-592ea.firebaseio.com"
#define FIREBASE_KEY "nshDkpXLCPmWkWjBULfHQGQ8TAholYZ14ABdMYD3"

// Color definitions
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0  
#define WHITE   0xFFFF

#define __CS 10
#define __DC 8
#define __RES 9

TFT_ILI9163C tft = TFT_ILI9163C(10, 8, 9);

void setup() {
  tft.begin();
  Serial.begin(9600);
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
  
  while (!Serial) {
    delay(10); // hang out until serial port opens
  }
  Serial.println("Adafruit AM2320 Basic Test");
  am2320.begin();
  pinMode(ledPower,OUTPUT);
  pinMode(BezzPin, OUTPUT);
  digitalWrite(BezzPin, LOW);
}

void loop(){
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin); // read the dust value
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
  calcVoltage = voMeasured * (5.0 / 1024.0);
  dustDensity = (0.17 * calcVoltage - 0.1)*100;
   
 unsigned long testText();{
  tft.fillScreen();
  tft.setCursor(0, 0);
  tft.setTextColor(BLUE);
  tft.setTextSize(2);
  tft.print("Temp:");
  tft.println(am2320.readTemperature());

  tft.setCursor(0, 20);
  tft.setTextColor(GREEN);
  tft.setTextSize(2);
  tft.print("Rav:");
  tft.println(voMeasured);

  tft.setCursor(0, 40);
  tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.print("Volt:");
  tft.println(calcVoltage);

  tft.setCursor(0, 60);
  tft.setTextColor(RED);
  tft.setTextSize(2);
  tft.print("Dust:");
  tft.println(dustDensity);

  Serial.print("Temp:"); 
  Serial.println(am2320.readTemperature()); // ° C 
  
  Serial.print("Raw Signal Value (0-1023): ");
  Serial.print(voMeasured);
  Serial.print(" - Voltage: ");
  Serial.print(calcVoltage);
  Serial.print(" - Dust Density: ");
  Serial.println(dustDensity); // unit: mg/m3
  
 }
  if (dustDensity > 100){
  digitalWrite(BezzPin, HIGH);
  tft.setCursor(0, 80);
  tft.setTextColor(RED);
  tft.setTextSize(2);
  tft.print("WARNING!");
  Serial.println ("WARNING!!!");
  }
 else {
 digitalWrite(BezzPin, LOW);
 tft.setCursor(0, 80);
 tft.setTextColor(GREEN);
 tft.setTextSize(2);
 tft.print("SECURE");
 Serial.println ("SECURE");
 }
 delay(1000);
}
