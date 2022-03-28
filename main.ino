#include <Adafruit_BMP085.h>
#include "DHT.h"

//DHT11 Declarations
#define DHTPIN 2
#define DHTTYPE DHT11

//LDR Declarations
#define ldrPin A0
#define ledPin 12

int colDelay = 10;
int rowDelay = 100;

//DHT function
DHT dht(DHTPIN, DHTTYPE);

//BMP180 variable
Adafruit_BMP085 bmp;
  
void setup() {
  Serial.begin(9600);
  pinMode(ldrPin, INPUT);
  dht.begin();
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

 // Serial.print("Humidity\tTemp_C\tTemp_F\tHIF\tHIC\n");
}
  
void loop() {

    ldr(); 
    dht11();
    bmp180();
    delay(500);
}

/*void printStr(char *message, char endLn='\t') {
  Serial.print(message);
  Serial.print(endLn);
}

void print(float value, char endLn='\t') {
  Serial.print(value);
  Serial.print(endLn);
}*/

void dht11(){
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
    float hif = dht.computeHeatIndex(f, h);
    float hic = dht.computeHeatIndex(t, h, false);

    if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
    }

    /*print(h);
    print(t);
    print(f);
    print(hif);
    print(hic);
    print(hif, '\n');*/
    

    Serial.println("#####DHT11 VALUES$#####");
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(f);
    Serial.print(F("°F  Heat index: "));
    Serial.print(hic);
    Serial.print(F("°C "));
    Serial.print(hif);
    Serial.println(F("°F"));
}

void bmp180(){ 
    Serial.println("#####BMP180 VALUES$#####");
    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters");

    Serial.print("Pressure at sealevel (calculated) = ");
    Serial.print(bmp.readSealevelPressure());
    Serial.println(" Pa");

    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.println(" meters");
    
    Serial.println();
}


void ldr(){
    if (analogRead(ldrPin) < 980) {
        Serial.println("DARK");
        Serial.println(String(analogRead(ldrPin)));
        digitalWrite(ledPin, HIGH);
    }
    else {
        Serial.println("BRIGHT");
        Serial.println(String(analogRead(ldrPin)));
        digitalWrite(ledPin, LOW);
    }
}
