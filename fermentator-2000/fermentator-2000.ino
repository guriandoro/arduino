#include <LiquidCrystal.h>
#include "DHT.h"

// https://github.com/Seeed-Studio/Grove_Temperature_And_Humidity_Sensor
// Grover Temperature & Humidity sensor v1.2
#define DHTTYPE DHT11   // DHT 11 
// Temp & Humid sensor connected to pin D2
#define DHTPIN 2

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
DHT dht(DHTPIN, DHTTYPE);

// LCD connected to pins D7-D12
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Light relay signal connected to pin D6
#define LIGHT_PIN 6
// Fan relay signal connected to pin D5
#define FAN_PIN 5

// Define low and high temperatures
#define LOW_TEMP 35
#define HIGH_TEMP 40

#define LIGHT_ON_SECS 600
#define FAN_ON_SECS 5

int light, fan;

void print_base_lcd_screen() {

  lcd.clear();
  
  // Temperature section
  lcd.setCursor(0,0);
  lcd.print("Tmp: ");
  lcd.setCursor(7,0);
  lcd.print("C");
  // Humidity section  
  lcd.setCursor(0,1);
  lcd.print("Hum: ");
  lcd.setCursor(7,1);
  lcd.print("%");
  // Light section
  lcd.setCursor(9,0);
  lcd.print("Luz: ");
  // Fan section
  lcd.setCursor(9,1);
  lcd.print("Fan: ");
}

void display_light (int secs){  
  if (secs > 0) {
    if (secs >= 100) {
      lcd.setCursor(13,0);
    } else if (secs >= 10) {
      lcd.setCursor(13,0);
      lcd.print(" ");
      lcd.setCursor(14,0);
    } else {
      lcd.setCursor(13,0);
      lcd.print("  ");
      lcd.setCursor(15,0);
    }

    lcd.print(secs, DEC);
  } else {
    print_base_lcd_screen();
    lcd.setCursor(14,0);
    lcd.print("NO");
  }
}

void display_fan (int is_on){
  lcd.setCursor(14,1);
  if (is_on)
    lcd.print("SI");
  else
    lcd.print("NO");
}

void setup() {
  // Initialize LCD screen
  lcd.begin(16, 2);
  
  // Initialize screen
  print_base_lcd_screen();
    
  // Initialize DHT
  //Serial.begin(9600); 
  //Serial.println("DHTxx test!");
  dht.begin();

  // Light is off to begin with
  digitalWrite(LIGHT_PIN, LOW);
  display_light(0);
  light = 0;
  // Fan is off to begin with
  digitalWrite(FAN_PIN, LOW);
  display_fan(0);
  fan = 0;

  //Set pin modes
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
}

void loop() {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    int humid = dht.readHumidity();
    int temp = dht.readTemperature();

    // Check if readings are valid
    if (isnan(temp) || isnan(humid)) {
      lcd.setCursor(6,0);
      lcd.print("N/A");
      lcd.setCursor(6,1);
      lcd.print("N/A");
    } 
    else {
      /*
      Serial.print("Humidity: ");Serial.print(humid);Serial.print(" %\t");
      Serial.print("Temperature: ");Serial.print(temp);Serial.print(" *C\t");
      Serial.print("Light: ");Serial.print(light);Serial.print("\t");
      Serial.print("Fan: ");Serial.print(fan);Serial.println();
      */
      lcd.setCursor(5,0);
      lcd.print(temp, DEC);
      lcd.setCursor(5,1);
      lcd.print(humid, DEC);
    }

    // If temperature is higher than allowed:
    // - turn off light
    // - turn on fan for FAN_ON_SECS seconds
    if (temp > HIGH_TEMP) {
        digitalWrite(LIGHT_PIN, LOW);
        light = 0;
        //display_light(0);
        if (fan == 0) {
          digitalWrite(FAN_PIN, HIGH);
          display_fan(1);
          fan = FAN_ON_SECS;
        }
    }
    
    // If temperature is lower than allowed:
    // - turn on light for LIGHT_ON_SECS seconds
    // - turn off fan
    if (temp < LOW_TEMP) {
      digitalWrite(FAN_PIN, LOW);
      fan = 0;
      //display_fan(0);
      if (light == 0) {
        digitalWrite(LIGHT_PIN, HIGH);
        light = LIGHT_ON_SECS;
        //display_light(light);
      }
    }

    // If there is one cycle remaining, turn off
    if (light == 1) {
      digitalWrite(LIGHT_PIN, LOW);
      light = 0;
    } else if (light > 1) light--;
    
    // If there is one cycle remaining, turn off
    if (fan == 1) {
      digitalWrite(FAN_PIN, LOW);
      fan = 0;
    } else if (fan > 1) fan--;

    display_light(light);
    display_fan(fan);
    // It's important for delay to be 1 second, so that math above works
    // Correction for sensor readings: 250ms
    delay(750);
}

