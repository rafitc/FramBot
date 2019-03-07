#include <DHT.h>  //DHT library 
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <TelegramBot.h>

#define DHTTYPE DHT11
#define dht_dpin 0 



DHT dht(dht_dpin, DHTTYPE);
const int sensor_pin = A0;
int motor = 2; // in D4

const char* ssd = "Naveed";
const char* pass = "atrnvd007";   // Your Pass
const char Token[] = "704660615:AAH8PFyJQ5A9d8M0O-CwIvkRKCQnBsZyWZ8";   //Bot token

WiFiClientSecure net_ssl;
TelegramBot bot (Token, net_ssl);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssd, pass);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  bot.begin(); 
  
  dht.begin();
  pinMode(motor,OUTPUT);
  
}

void loop() { 
  message m = bot.getUpdates();
  
  if (m.text.equals("hi")){     
    bot.sendMessage(m.chat_id, "Your bot is ready !!! ");  
 }
   else if (m.text.equals("FarmTemp")){     
   String t;
    //message m = bot.getUpdates();
    t = dht.readTemperature(); 
    Serial.print("% ");
    Serial.print("temperature = ");
    Serial.print(t); 
    delay(500);
    bot.sendMessage(m.chat_id, "temperature = ");
    bot.sendMessage(m.chat_id, t);  
 }
   else if (m.text.equals("FarmMoist")){     
   String moisture_percentage;
    moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );
    Serial.print("Soil Moisture(in Percentage) = ");
    Serial.print(moisture_percentage);
    Serial.println("%");
    delay(500);
    bot.sendMessage(m.chat_id, "moisture percentage = "); 
    bot.sendMessage(m.chat_id, moisture_percentage); 
  } 
    else if (m.text.equals("FarmHumidity")){     
    float h;
    String myh;
   
    h = dht.readHumidity();        
    Serial.print("Current humidity = ");
    Serial.print(h);
    myh = String(h);
    delay(500);
    bot.sendMessage(m.chat_id, "Humidity = ");
    bot.sendMessage(m.chat_id, myh);
    }
    else if (m.text.equals("motoron")){ 
      digitalWrite(motor, HIGH);
      bot.sendMessage(m.chat_id, "Motor Turned ON");
    }
   else if (m.text.equals("motoroff")){ 
      digitalWrite(motor, LOW);
      bot.sendMessage(m.chat_id, "Motor Turned OFF");
    }
    else if (m.text.equals("Help")) { 
   bot.sendMessage(m.chat_id, " ==>>FarmTemp :- To know the temperature of your farm ."); 
   bot.sendMessage(m.chat_id, " ==>>FarmMoisture  :-  To know the Moisture level of your farm.");
   bot.sendMessage(m.chat_id, "==>>FarmHumidity  :- To Know the Humidity level of your farm.");
   bot.sendMessage(m.chat_id, "motoron :- To turn on Your Motor");
   bot.sendMessage(m.chat_id, "motoroff :- To turn off Your Motor");;
    }
    
}
 
