#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor


#define DHTTYPE DHT11   // DHT 11
#define dht_dpin 0
DHT dht(dht_dpin, DHTTYPE); 

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Naveed"
#define WLAN_PASS       "atrnvd007"

/************************* Information to Connect to Adafruit IO *********************************/
/*  You will need and account - Its FREE  */

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "rafitc"
#define AIO_KEY         "2b5b8f32cf134c50999315faa4d66346"

/************ Setting up your WiFi Client and MQTT Client ******************/
const int sensor_pin = A0;

// Create an ESP8266 WiFiClient class to connect to the MQTT server.

WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Set Up a Feed to Publish To ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>

Adafruit_MQTT_Publish myFirstValue = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/myFirstValue");
Adafruit_MQTT_Publish mySecondValue = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/mySecondValue");
Adafruit_MQTT_Publish moist = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/moist");
Adafruit_MQTT_Publish motor = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/motor");


/********************* Values ******************************/
// Need a changing value to send.  We will increment this value
// in the getVal function.  
//we start at zero and when it gets to 10 we start over.

uint32_t xVal=-1;

/*************************** Sketch Code ***********************************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  dht.begin();
}


void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

 
    float t = dht.readTemperature();         
    Serial.print("Current humidity = ");
    Serial.print(t); 
    Serial.println("C  ");
    delay(800);
  if (! myFirstValue.publish(t)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  
    float h ;
    h = dht.readHumidity();         
    Serial.print("temperature = ");
    Serial.print(t); 
    Serial.println("C  ");
    delay(800);
  if (! mySecondValue.publish(h)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  float moisture_percentage;
   moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );
   Serial.print("Soil Moisture(in Percentage) = ");
   Serial.print(moisture_percentage);
   Serial.println("%");
   if (! mySecondValue.publish(moisture_percentage)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
  delay(1000);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}


//  Use this increment myVal

uint8_t getVal(){

  if(xVal == 11){
      xVal = 0;
  }
  return xVal++;
  
}
