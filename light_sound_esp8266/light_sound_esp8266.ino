#define BLYNK_TEMPLATE_ID "TMPLZSxVsFHu"
#define BLYNK_TEMPLATE_NAME "SmartHome Project"
#define BLYNK_AUTH_TOKEN "_b5q6K7k2z2m5_OO327qyVK7dfcnYR3o"
#define BLYNK_PRINT Serial

#include <Wire.h>
#include <BH1750.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// const char* ssid = "Nha_cua_Hien_My_Ne";   
// const char* password = "MatKhaukhodoan813"; 

const char* ssid = "VNUK4-1";   
const char* password = "Z@q12wsx"; 
const int threshold = 580;
char auth[] = BLYNK_AUTH_TOKEN;

int led = D7;
int sound_digital = D8;
int sound_analog = A0;

long soundHighTime = 0;
long soundLowTime = 0;
long lastReadingTime = 0;
bool isLedOn = false;

BH1750 lightMeter;
BlynkTimer timer;

void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Wire.begin(D4, D3);
  lightMeter.begin();

  if (lightMeter.begin()) {
    Serial.println(F("BH1750 initialised"));
  }
  else {
    Serial.println(F("Error initialising BH1750"));
  }

  Serial.println("Processing data");

  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  if (isnan(lux)) {
    Serial.println("Failed to read from BH1750 sensor!");
  }

  Serial.println("Sending data");

  digitalWrite(LED_BUILTIN, HIGH);
  Blynk.virtualWrite(V11, lux);
  digitalWrite(LED_BUILTIN, LOW);
}

void setup(){
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(sound_digital, INPUT);  
  pinMode(LED_BUILTIN, OUTPUT);


  Blynk.begin(auth, ssid, password);
  // Setup a function to be called every second
  timer.setInterval(5000L, myTimerEvent);
}

void loop(){

  Blynk.run();
  timer.run(); // Initiates BlynkTimer

  int val_digital = digitalRead(sound_digital);
  int val_analog = analogRead(sound_analog);
  
  // Serial.println("-------------------------------------------");
  // // Serial.print(val_analog);
  // // Serial.print("\t");
  // Serial.print("Sound: ");
  // Serial.println(val_analog);
  // Serial.println("-------------------------------------------");
  // if (val_digital == HIGH)
  // {
  //   digitalWrite(led, HIGH);
  //   delay(3000);
  // }
  // else
  // {
  //   digitalWrite(led, LOW);
  // }
  // if (val_analog >= threshold) {
  //   digitalWrite(led, HIGH);
  //   delay(5000);
  // } else {
  //   digitalWrite(led, LOW);
  // }
  
  if (val_analog >= 585) {
    soundHighTime = millis();
    Serial.println(val_analog);
  }
  if (val_analog < 585) {
    soundLowTime = millis();
  }
  // Serial.println(soundHighTime);
  
  if ( 
    (soundHighTime != 0 ) &&
    (soundLowTime != 0 ) &&
    ((soundHighTime - soundLowTime) > 40 || (soundLowTime - soundHighTime) > 40) && (millis() - lastReadingTime > 1000)
    ) {
      Serial.println("ok");
      soundHighTime = 0 ;
      soundLowTime = 0 ;
      lastReadingTime = millis();
      if (isLedOn == false) {
        digitalWrite(led, HIGH);
        Serial.println("LED ON");
        isLedOn = true;
      } else {
        digitalWrite(led, LOW);
        Serial.println("LED OFF");
        isLedOn = false;
      }
    }

  // Light detection

}