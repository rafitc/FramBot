
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <TelegramBot.h>

 
const char* ssid = "Naveed";
const char* password = "atrnvd007";
const char BotToken[] = "704660615:AAH8PFyJQ5A9d8M0O-CwIvkRKCQnBsZyWZ8";

WiFiClientSecure net_ssl;
TelegramBot bot (BotToken, net_ssl);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  bot.begin(); 
}

void loop() {

  message m = bot.getUpdates(); 
  
  if (m.text.equals("hi")){     
    bot.sendMessage(m.chat_id, "Your bot is ready !!! ");  
 }
}
