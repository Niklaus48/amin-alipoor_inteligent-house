#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <uri/UriBraces.h>
#include <ESP32Servo.h>
#include <DHT.h>
#include <TM1637Display.h>
#include <Adafruit_SSD1306.h>

// DHT Sensor Setup
#define DHT_PIN 23
#define DHT_TYPE DHT22  
DHT dht(DHT_PIN, DHT_TYPE);

Servo door1;
Servo myserv2;
Servo myserv3;
Servo myserv4;

int pirpin = 5;
int buzzerPin = 13; 
int pirStat = 0;
int lastPirStat = 0;
int redLed1 = 16;  
int redLed2 = 26;
int doorled = 4;

// متغیرهای آژیر
bool alarmActive = false;
unsigned long alarmStartTime = 0;
unsigned long lastToggleTime = 0;
bool lightsOn = false;

// متغیر برای جلوگیری از تکرار سریع
unsigned long lastTriggerTime = 0;
const unsigned long cooldownTime = 3000; // 3 ثانیه انتظار

// متغیرهای DHT
unsigned long lastDHTRead = 0;
const unsigned long dhtInterval = 3000; // هر 3 ثانیه یکبار بخون
float temperature = 0;
float humidity = 0;

// متغیرهای کنترل خودکار پنکه
bool autoFanEnabled = true;           // فعال/غیرفعال کردن کنترل خودکار
bool fansRunning = false;             // وضعیت پنکه‌ها
float tempThresholdHigh = 25.0;       // دمای روشن شدن پنکه
float tempThresholdLow = 23.0;        // دمای خاموش شدن پنکه
unsigned long lastFanAction = 0;      // آخرین بار که پنکه حرکت کرد
const unsigned long fanInterval = 4000; // فاصله بین حرکات پنکه (3 ثانیه)

void opendoor() {
  door1.write(90);              
  myserv2.write(90);              
  delay(1000);
  digitalWrite(doorled, HIGH);        
  delay(100);
}

void closedoor() {
  door1.write(0);              
  myserv2.write(180);              
  delay(1000);
  digitalWrite(doorled, LOW);        
  delay(100);
}

void pankeh1() {
  myserv3.write(90);   
  myserv4.write(90);   
  delay(500);
  
  myserv3.write(180);  
  myserv4.write(180);  
  delay(500);
  
  Serial.println(" Both fans moved together!");
}

void pankeh2() {
  myserv3.write(180);  
  myserv4.write(180);  
  delay(380);
  
  myserv3.write(0);    
  myserv4.write(0);    
  delay(380);
  
  myserv3.write(180);  
  myserv4.write(180);  
  delay(380);
  
  myserv3.write(0);    
  myserv4.write(0);    
  delay(100);
  
  Serial.println("Both fans complex movement!");
}

// پنکه خودکار - حرکت مداوم برای خنک کردن
void autoFanCooling() {
  
  // حرکت سریع برای خنک کردن
  myserv3.write(0);    // پنکه 1 چپ
  myserv4.write(180);  // پنکه 2 راست
  delay(300);
  
  myserv3.write(180);  // پنکه 1 راست
  myserv4.write(0);    // پنکه 2 چپ
  delay(300);
  
  myserv3.write(90);   // پنکه 1 وسط
  myserv4.write(90);   // پنکه 2 وسط
  delay(200);
  
  Serial.println("Auto cooling fans activated!");
}

// کنترل خودکار پنکه بر اساس دما
void checkAutoFan() {
  if (!autoFanEnabled) return;
  
  unsigned long currentTime = millis();
  
  // اگر دما بالا رفت و پنکه‌ها خاموش هستن
  if (temperature >= tempThresholdHigh && !fansRunning) {
    fansRunning = true;
    lastFanAction = currentTime;
    Serial.println("HIGH TEMPERATURE DETECTED!");
    Serial.print(" Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
    Serial.println(" AUTO FANS ACTIVATED!");
    autoFanCooling();
  }
  
  // اگر دما پایین اومد و پنکه‌ها روشن هستن
  else if (temperature <= tempThresholdLow && fansRunning) {
    fansRunning = false;
    Serial.println("TEMPERATURE NORMALIZED");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
    Serial.println("AUTO FANS STOPPED");
  }
  
  // اگر پنکه‌ها روشن هستن، هر 3 ثانیه حرکت کنن
  else if (fansRunning && (currentTime - lastFanAction >= fanInterval)) {
    lastFanAction = currentTime;
    autoFanCooling();
  }
}

// خواندن سنسور رطوبت و دما
void readDHT() {
  unsigned long currentTime = millis();
  
  // فقط هر 5 ثانیه یکبار بخون
  if (currentTime - lastDHTRead >= dhtInterval) {
    float newTemp = dht.readTemperature();
    float newHum = dht.readHumidity();
    
    // چک کن که مقادیر معتبر باشن
    if (!isnan(newTemp) && !isnan(newHum)) {
      temperature = newTemp;
      humidity = newHum;
      
      Serial.println("🌡️ ===== DHT Reading =====");
      Serial.print("🌡️  Temperature: ");
      Serial.print(temperature);
      Serial.println("°C");
      Serial.print("💧  Humidity: ");
      Serial.print(humidity);
      Serial.println("%");
      
      // نمایش وضعیت پنکه خودکار
      if (autoFanEnabled) {
        Serial.print("❄️  Auto Fan: ");
        if (fansRunning) {
          Serial.println("ON (Cooling)");
        } else {
          Serial.print("OFF (Threshold: ");
          Serial.print(tempThresholdHigh);
          Serial.println("°C)");
        }
      } else {
        Serial.println("  Auto Fan: DISABLED");
      }
      
      Serial.println("========================");
      
      // هشدار دما
      if (temperature > 30) {
        Serial.println(" EXTREME HIGH TEMPERATURE!");
      } else if (temperature < 10) {
        Serial.println(" LOW TEMPERATURE WARNING!");
      }
      
      // هشدار رطوبت
      if (humidity > 80) {
        Serial.println(" HIGH HUMIDITY WARNING!");
      } else if (humidity < 30) {
        Serial.println(" LOW HUMIDITY WARNING!");
      }
      
    } else {
      Serial.println("DHT Reading Error!");
    }
    
    lastDHTRead = currentTime;
  }
}

// نمایش فوری DHT (برای دستور دستی)
void showDHTNow() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  
  Serial.println("===== INSTANT DHT =====");
  if (!isnan(temp) && !isnan(hum)) {
    Serial.print("  Temperature: ");
    Serial.print(temp);
    Serial.println("°C");
    Serial.print("  Humidity: ");
    Serial.print(hum);
    Serial.println("%");
    Serial.print("  Auto Fan Status: ");
    Serial.println(fansRunning ? "RUNNING" : "STOPPED");
  } else {
    Serial.println(" DHT Reading Error!");
  }
  Serial.println("=========================");
}

// شروع آژیر
void startPoliceAlarm() {
  alarmActive = true;
  alarmStartTime = millis();
  lastToggleTime = millis();
  lightsOn = false;
  Serial.println(" Police Alarm Started!");
}

// آژیر بدون delay
void updatePoliceAlarm() {
  if (!alarmActive) return;
  
  unsigned long currentTime = millis();
  
  // اگر 6 ثانیه گذشت، آژیر رو متوقف کن
  if (currentTime - alarmStartTime > 6000) {
    noTone(buzzerPin);
    digitalWrite(redLed1, LOW);
    digitalWrite(redLed2, LOW);
    alarmActive = false;
    Serial.println(" Police Alarm Finished! Ready for next trigger...");
    return;
  }
  
  // تغییر حالت هر 150 میلی‌ثانیه
  if (currentTime - lastToggleTime >= 150) {
    if (lightsOn) {
      digitalWrite(redLed1, LOW);
      digitalWrite(redLed2, LOW);
      tone(buzzerPin, 600);
      lightsOn = false;
    } else {
      digitalWrite(redLed1, HIGH);
      digitalWrite(redLed2, HIGH);
      tone(buzzerPin, 800);
      lightsOn = true;
    }
    
    lastToggleTime = currentTime;
  }
}

void pir() {             
  pirStat = digitalRead(pirpin);
  unsigned long currentTime = millis();
  
  // Debug - نمایش وضعیت PIR
  static unsigned long lastDebug = 0;
  if (currentTime - lastDebug > 1000) {
    Serial.print("🔍 PIR: ");
    Serial.print(pirStat);
    Serial.print(" | Cooldown: ");
    Serial.print((currentTime - lastTriggerTime) / 1000);
    Serial.print("s | Temp: ");
    Serial.print(temperature);
    Serial.print("°C | Fan: ");
    Serial.println(fansRunning ? "ON" : "OFF");
    lastDebug = currentTime;
  }
  
  // فعال‌سازی آژیر با شرایط
  if (pirStat == HIGH && 
      lastPirStat == LOW && 
      (currentTime - lastTriggerTime > cooldownTime) && 
      !alarmActive) {
    digitalWrite(doorled, LOW);                
    Serial.println(" PIR Triggered! Starting alarm...");
    startPoliceAlarm();
    lastTriggerTime = currentTime;
    
  } else if (pirStat == LOW && lastPirStat == HIGH) {
    Serial.println(" PIR Clear");
    digitalWrite(doorled, LOW);         
  }
  
  lastPirStat = pirStat;
}

void setup() {
  Serial.begin(9600);        
  
  // شروع DHT
  dht.begin();
  Serial.println(" DHT22 Sensor initialized on pin 23");
  
  // servo تنظیم اولیه
  door1.attach(33);       
  myserv2.attach(18);       
  myserv3.attach(25);       
  myserv4.attach(15);
  
  door1.write(0);      
  myserv2.write(180);    
  myserv3.write(90);     
  myserv4.write(90);
  
  delay(1000);
       
  pinMode(pirpin, INPUT);        
  pinMode(buzzerPin, OUTPUT);
  pinMode(redLed1, OUTPUT);
  pinMode(redLed2, OUTPUT);
  pinMode(doorled, OUTPUT);
  
  digitalWrite(redLed1, LOW);
  digitalWrite(redLed2, LOW);
  digitalWrite(doorled, LOW);
  
  Serial.println(" Smart Home System Ready!");
  Serial.println(" Buzzer on pin 13");
  Serial.println(" Red LEDs on pins 16 & 26");
  Serial.println(" DHT22 on pin 23");
  Serial.println(" Auto Fan Control: ENABLED");
  Serial.print(" Fan ON Threshold: ");
  Serial.print(tempThresholdHigh);
  Serial.println("°C");
  Serial.print("Fan OFF Threshold: ");
  Serial.print(tempThresholdLow);
  Serial.println("°C");
  Serial.println("Commands:");
  Serial.println("   'o'=open door, 'c'=close door");
  Serial.println("   '1'=fan1, '2'=fan2 (manual)");
  Serial.println("   't'=test alarm, 'd'=show DHT");
  Serial.println("   'f'=toggle auto fan, 's'=status");
  Serial.println("Waiting for PIR trigger...");
  
  // تست بازر
  Serial.println("Testing buzzer...");
  tone(buzzerPin, 1000, 500);
  delay(600);
  Serial.println("Buzzer test complete!");
  
  // تست اولیه DHT
  Serial.println("Testing DHT sensor...");
  delay(2000); // DHT نیاز به کمی وقت داره
  showDHTNow();
}

void loop() {   
  pir();                    
  updatePoliceAlarm();     
  readDHT();              
  checkAutoFan();           
  
  // کنترل از طریق Serial
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    
    switch (cmd) {
      case 'o':
      case 'O':
        Serial.println(" Opening door...");
        opendoor();
        break;
        
      case 'c':
      case 'C':
        Serial.println(" Closing door...");
        closedoor();
        break;
        
      case '1':
        Serial.println(" Running fan pattern 1 (Manual)...");
        pankeh1();
        break;
        
      case '2':
        Serial.println(" Running fan pattern 2 (Manual)...");
        pankeh2();
        break;
        
      case 't':
      case 'T':
        Serial.println(" Testing alarm manually...");
        startPoliceAlarm();
        break;
        
      case 'd':
      case 'D':
        Serial.println(" DHT Reading requested...");
        showDHTNow();
        break;
        
      case 'f':
      case 'F':
        autoFanEnabled = !autoFanEnabled;
        Serial.print(" Auto Fan Control: ");
        Serial.println(autoFanEnabled ? "ENABLED" : "DISABLED");
        if (!autoFanEnabled) {
          fansRunning = false;
          Serial.println(" Auto fans stopped");
        }
        break;
        
      case 's':
      case 'S':
        Serial.println(" ===== SYSTEM STATUS =====");
        Serial.print(" Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");
        Serial.print(" Humidity: ");
        Serial.print(humidity);
        Serial.println("%");
        Serial.print(" Auto Fan: ");
        Serial.println(autoFanEnabled ? "ENABLED" : "DISABLED");
        Serial.print(" Fans Status: ");
        Serial.println(fansRunning ? "RUNNING" : "STOPPED");
        Serial.print(" Alarm: ");
        Serial.println(alarmActive ? "ACTIVE" : "INACTIVE");
        Serial.println("===========================");
        break;
        
      case 'h':
      case 'H':
        Serial.println("📱 Available Commands:");
        Serial.println("   'o' = Open door");
        Serial.println("   'c' = Close door");
        Serial.println("   '1' = Fan pattern 1 (Manual)");
        Serial.println("   '2' = Fan pattern 2 (Manual)");
        Serial.println("   't' = Test alarm");
        Serial.println("   'd' = Show DHT readings");
        Serial.println("   'f' = Toggle auto fan control");
        Serial.println("   's' = Show system status");
        Serial.println("   'h' = Show this help");
        break;
    }
  }
  
  delay(10);          
}