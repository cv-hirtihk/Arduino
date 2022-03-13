  /*
    ESP8266 | Arduino without Serial1 | Arduino with Serial1  
    --------------------------------------------------------
       RX   |         pin 7           |        TX1
       TX   |         pin 6           |        RX1
       GND  |          GND            |        GND
       VCC  |          5V             |        5V
      CH_PD |          5V             |        5V
*/      
  

#include <SimpleDHT.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

#include "ThingSpeak.h"
#include "WiFiEsp.h"
//#include "secrets.h"

#define SECRET_SSID "SSID"    // replace MySSID with your WiFi network name
#define SECRET_PASS "PASSWORD" // replace MyPassword with your WiFi password

#define SECRET_CH_ID 1674060     // replace 0000000 with your channel number
#define SECRET_WRITE_APIKEY "Y8FG8YKV9FXG4K8A"   // replace XYZ with your channel write API Key

int pinDHT11 = 2;
SimpleDHT11 dht11(pinDHT11);

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiEspClient  client;

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#define ESP_BAUDRATE  19200
#else
#define ESP_BAUDRATE  115200
#endif

Adafruit_BMP085 bmp;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
int number1 = 0;
int number2 = 0; // random(0,100);
int number3 = 0; //random(0,100);
int number4 =0; // random(0,100);
String myStatus = "";

void setup() {
  //Initialize serial and wait for port to open
  //Serial.begin(115200);  // Initialize serial
   bmp.begin();  
  
  // initialize serial for ESP module  
  //setEspBaudRate(ESP_BAUDRATE);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }

  //Serial.print("Searching for ESP8266..."); 
  // initialize ESP module
  //WiFi.init(&Serial1);

  // check for the presence of the shield
  //if (WiFi.status() == WL_NO_SHIELD) {
 //   Serial.println("WiFi shield not present");
    // don't continue
  //  while (true);
 // }
  Serial.println("found it!");
   
 // ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  // Connect or reconnect to WiFi
  /*
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  */



 byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) 
  {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }

  number1= (bmp.readPressure()/1000);
  
number2=bmp.readTemperature();
number3=humidity;
number4=analogRead(A0); //darkness

Serial.println(number1);
Serial.println(number2);
Serial.println(number3);
Serial.println(number4);




  // set the fields with the values
  /*
  ThingSpeak.setField(1, number1);
  ThingSpeak.setField(2, number2);
  ThingSpeak.setField(3, number3);
  ThingSpeak.setField(4, number4);
*/
  
  // write to the ThingSpeak channel
  //int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  //if(x == 200){
  //  Serial.println("Channel update successful.");
 // }
  //else{
  //  Serial.println("Problem updating channel. HTTP error code " + String(x));
  //}
  
  // change the values

  /*
  number1++;
  if(number1 > 99){
    number1 = 0;
  }
  number2 = random(0,100);
  number3 = random(0,100);
  number4 = random(0,100);

  */




  
  delay(2000); // Wait 20 seconds to update the channel again
}

// This function attempts to set the ESP8266 baudrate. Boards with additional hardware serial ports
// can use 115200, otherwise software serial is limited to 19200.
void setEspBaudRate(unsigned long baudrate){
  long rates[6] = {115200,74880,57600,38400,19200,9600};

  Serial.print("Setting ESP8266 baudrate to ");
  Serial.print(baudrate);
  Serial.println("...");

  for(int i = 0; i < 6; i++){
    Serial1.begin(rates[i]);
    delay(100);
    Serial1.print("AT+UART_DEF=");
    Serial1.print(baudrate);
    Serial1.print(",8,1,0,0\r\n");
    delay(100);  
  }
    
  Serial1.begin(baudrate);
}
