#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>  // Including library for DHT sensor
#include <Fonts/FreeSerif9pt7b.h>  // Add a custom font
#include <Fonts/FreeMono9pt7b.h>  // Add a custom font
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
 
#define DHTPIN D4          //pin where the dht11 is connected
DHT dht(DHTPIN, DHT11);
 
String apiKey = "9LHQ4F4PAP9DI47R"; // Enter your Write API key from ThingSpeak
const char *ssid = "POCO F1";     // replace with your wifi ssid and wpa2 key
const char *pass = "Sahil123@#";
const char* server = "api.thingspeak.com";
 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
const int AirValue = 790;   //you need to replace this value with Value_1
const int WaterValue = 390;  //you need to replace this value with Value_2
const int SensorPin = A0;
int soilMoistureValue = 0;
int soilmoisturepercent=0;
int relaypin = D5;

WiFiClient client;
 
 
void setup() {
  Serial.begin(115200); // open serial port, set the baud rate to 9600 bps
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  display.clearDisplay();
  pinMode(relaypin, OUTPUT);
 
  dht.begin();
  
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");
    delay(4000);
}  
 
 
void loop() 
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
 
  Serial.print("Humidity: ");
  Serial.println(h);
  Serial.print("Temperature: ");
  Serial.println(t);
  
  soilMoistureValue = analogRead(SensorPin);  //put Sensor insert into soil
  Serial.println(soilMoistureValue);
  
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
 
 
if(soilmoisturepercent > 100)
{
  Serial.println("100 %");
  
  display.setCursor(0,14);  //oled display
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);  
  display.setTextColor(WHITE);
  display.print("Soil Rh: "); 
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);
  display.print("100");
  display.println(" %");
  display.setCursor(0,46);  //oled display
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);
  display.print("Air Rh: ");
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);  
  display.print(h);
  display.println(" %");
  display.setCursor(0,29);  //oled display
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);
  display.print("Temp: ");
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);
  display.print(t);
  display.print((char)247);  
  display.println("*C");
  display.display();
  
  delay(250);
  display.clearDisplay();
}
 
 
else if(soilmoisturepercent <0)
{
  Serial.println("0 %");
  
  display.setCursor(0,14);  //oled display
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);
  display.setTextColor(WHITE);
  display.print("Soil Rh: "); 
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);
  display.print("0");
  display.println(" %");
  display.setCursor(0,46);  //oled display
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);
  display.print("Air R: ");
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);
  display.print( h);
  display.println(" %");
  display.setCursor(0,29);  //oled display
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);
  display.print("Temp: ");
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);
  display.print( t);
  display.print((char)247);  
  display.println("*C");
  display.display();
 
  delay(250);
  display.clearDisplay();
}
 
 
else if(soilmoisturepercent >=0 && soilmoisturepercent <= 100)
{
  Serial.print(soilmoisturepercent);
  Serial.println("%");
  
  display.setCursor(0,14);  //oled display
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);
  display.setTextColor(WHITE);
  display.print("Soil Rh: ");
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);  
  display.print(soilmoisturepercent);
  display.println(" %");
  display.setCursor(0,46);  //oled display
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);  
  display.print("Air Rh: ");
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);
  display.print( h);
  display.println(" %");
  display.setCursor(0,29);  //oled display
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);
  display.print("Temp: ");
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);
  display.print( t);
  display.print((char)247);  
  display.println("*C");
  display.display();
 
  delay(250);
  display.clearDisplay();
}
 
  if(soilmoisturepercent >=0 && soilmoisturepercent <= 50)
  {
    digitalWrite(relaypin, HIGH);
    Serial.println("Motor is ON");
    display.setCursor(0,61);  //oled display
    display.setTextSize(1);    
    display.print("Motor is ON");    
  }
  else if (soilmoisturepercent >50 && soilmoisturepercent <= 100)
  {
    digitalWrite(relaypin, LOW);
    Serial.println("Motor is OFF");
    display.setCursor(0,61);  //oled display
    display.setTextSize(1);    
    display.print("Motor is OFF");    
  }
  
  if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
      postStr += "&field1=";
      postStr += String(soilmoisturepercent);
      postStr += "&field2=";
      postStr += String(h);
      postStr += "&field3=";
      postStr += String(t);
      postStr += "&field4=";
      postStr += String(relaypin);
      postStr += "\r\n\r\n\r\n\r\n";
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
   
  }
    client.stop();
}