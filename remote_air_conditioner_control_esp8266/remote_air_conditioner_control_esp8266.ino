// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLZSxVsFHu"
#define BLYNK_TEMPLATE_NAME "SmartHome Project"
#define BLYNK_AUTH_TOKEN "_b5q6K7k2z2m5_OO327qyVK7dfcnYR3o"
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"

#define DHTPIN 5 // what digital pin we're connected to
#define POWERPIN 4
#define POWERSWITCH 0
#define INCPIN 14
#define DRCPIN 12
#define FANPIN 13
#define MODEPIN 15

#define DHTTYPE DHT22 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
// const char* ssid = "Nha_cua_Hien_My_Ne";   
// const char* password = "MatKhaukhodoan813"; 

const char* ssid = "VNUK4-1";   
const char* password = "Z@q12wsx"; 

BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.

  Serial.println("Processing data");
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
  }

  // Serial.print("Temp: ");
  // Serial.println(t);
  // Serial.print("Humid: ");
  // Serial.println(h);
  // Serial.print("Temp Faren: ");
  // Serial.println(f);

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  
  Serial.println("Sending data");
  digitalWrite(LED_BUILTIN, HIGH);
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V2, f);
  Blynk.virtualWrite(V3, hic);
  Blynk.virtualWrite(V4, hif);
  digitalWrite(LED_BUILTIN, LOW);
}

//Power switch
BLYNK_WRITE(V5)
{
  int switchStatus = param.asInt(); // assigning incoming value from pin V5 to a variable
  Serial.println("V5: POWER SWITCH CHANGE");
  digitalWrite(POWERPIN, switchStatus);
}

//Control Switch ON/OFF
BLYNK_WRITE(V6)
{
  int switchStatus = param.asInt(); // assigning incoming value from pin V6 to a variable
  Serial.println("V6: CONTROL SWITCH CHANGE");
  digitalWrite(POWERSWITCH, switchStatus);
}

//Temp up
BLYNK_WRITE(V7)
{
  int switchStatus = param.asInt(); // assigning incoming value from pin V7 to a variable
  Serial.println("V7: TEMP UP");
  digitalWrite(INCPIN, switchStatus);
}

//Temp down
BLYNK_WRITE(V8)
{
  int switchStatus = param.asInt(); // assigning incoming value from pin V8 to a variable
  Serial.println("V8: TEMP DOWN");
  digitalWrite(DRCPIN, switchStatus);
}

//remote fan
BLYNK_WRITE(V9)
{
  int switchStatus = param.asInt(); // assigning incoming value from pin V9 to a variable
  Serial.println("V9: FAN MODE CHANGE");
  digitalWrite(FANPIN, switchStatus);
}

//remote mode
BLYNK_WRITE(V10)
{
  int switchStatus = param.asInt(); // assigning incoming value from pin V10 to a variable
  Serial.println("V10: REMOTE MODE CHANGE");
  digitalWrite(MODEPIN, switchStatus);
  
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  dht.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(POWERPIN, OUTPUT);
  pinMode(POWERSWITCH, OUTPUT);
  pinMode(INCPIN, OUTPUT);
  pinMode(DRCPIN, OUTPUT);
  pinMode(FANPIN, OUTPUT);
  pinMode(MODEPIN, OUTPUT);
  
  Blynk.begin(auth, ssid, password);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(2000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
