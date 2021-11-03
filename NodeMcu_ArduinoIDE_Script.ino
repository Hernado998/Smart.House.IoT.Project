#include <iostream>
#include <string>
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>// including the library of DHT11 temperature and humidity sensor

#define DHTTYPE DHT11   // DHT 11
#define dht_dpin D4

DHT dht(dht_dpin, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

const char* ssid     = "TOPNET_D4388";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "nadir123";     // The password of the Wi-Fi network

const char *mqtt_broker = "192.168.43.184";
const char *topic = "esp8266/test";
const int mqtt_port = 1883;
const int firePin = D8;
const int alertPin = D1;
const int lumPin = A0;

int red_light_pin= D7;
int green_light_pin = D6;
int blue_light_pin = D5;

int digitalVal;
int lumiereVal;
String s="";
String f="";

void setup(void)
{ 
  dht.begin();
  Serial.begin(9600);
  pinMode(firePin, INPUT);
  pinMode(alertPin, OUTPUT);
  pinMode(lumPin, INPUT);
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
  Serial.println("Humidity and temperature\n\n");
  Serial.println(F("Flame Sensor Initialized"));
  WifiSetup();
  MqttSetup();
  
  delay(700);
}
void WifiSetup(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
}
void MqttSetup(){
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");  
    }else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe("esp8266/test");
}
void loop() {
  readFlameSensor();
  readLightSensor();
  digitalVal = LOW;
  client.loop(); 
  PublishTempHumData();
  
  delay(1000);
}

void readFlameSensor(){
   f="";
  digitalVal = digitalRead(firePin);
  Serial.println(digitalVal);
  if (digitalVal == HIGH){
    f="Flame detected !!!! ";
    client.publish("esp8266/test", (char*) f.c_str());
    AlertFlame();
    f="";
  }
}
int y=0;
void readLightSensor(){
  lumiereVal = analogRead(lumPin);
  Serial.println(lumiereVal);
  if(lumiereVal -y > 5){
    RGB_color(255,255,255);
    fadelka();
    delay(5000);
  }
  y = lumiereVal;
  RGB_color(0,0,0);
}
void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
{
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}
void AlertFlame(){
  for(int i=0;i<20;i++){
      digitalWrite(alertPin, HIGH);
      RGB_color(255,0,0);
      delay(200);
      digitalWrite(alertPin, LOW);
      RGB_color(0,0,0);
      delay(200);
  }
}
void fadelka(){
  for(int i=0;i<3;i++){
    digitalWrite(alertPin, HIGH);
    delay(50);
    digitalWrite(alertPin, LOW);
    delay(100);
  }
  
}
float milliseconds;
int m = 0;
void PublishTempHumData(){
  milliseconds = millis();
  int seconds = (int)(milliseconds / 1000)%60 ;
  int minutes = (int)(std::fmod(milliseconds / 60000,60));
  int hours   = (int)(std::fmod(milliseconds / 3600000,24));
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  s+="Current humidity = ";
  s.concat(h);
  s+="%  ";
  s+="temperature = ";
  s.concat(t);
  s+="C  ";
  s+="hh ";
  s.concat(hours);
  s+=" mm ";
  s.concat(minutes);
  s+=" ss ";
  s.concat(seconds);
  if(minutes == m+1){
    client.publish("esp8266/test", (char*) s.c_str());
    m=m+1;
  }
  s="";
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  static char message[1000];
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  strncpy(message, (char *)payload, length);
  message[length] = '\0';
  if((char)payload[0]=='l'){
    int a = message[1]*100+ message[2]*10+ message[3];
    int b = message[4]*100+ message[5]*10+ message[6];
    int c = message[7]*100+ message[8]*10+ message[9];
    Serial.println();Serial.println(message);
    RGB_color(a-5328,b-5328,c-5328);
    delay(10000);
    RGB_color(0,0,0);
  }
  if(((char)payload[0] == 'h') && ((char)payload[1] == 'e') && ((char)payload[2] == 'l') && ((char)payload[3] == 'p')){
    help();
  }
  Serial.println();
  Serial.println("-----------------------");
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp8266/test");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void help(){
  for(int i=0;i<20;i++){
      digitalWrite(alertPin, HIGH);
      RGB_color(255,0,0);
      delay(200);
      digitalWrite(alertPin, LOW);
      RGB_color(0,0,255);
      delay(200);
  }
}
