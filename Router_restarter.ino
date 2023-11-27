#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
 
const char* ssid = "pkgalaxy"; // fill in here your router or wifi SSID
const char* password = "mbez4354"; // fill in here your router or wifi password
 #define RELAY 0 // relay connected to  GPIO0
#define MAX_RECONNECT_COUNT 5
#define RELAY_OFF_TIME_MS 10000
#define LOOP_SLEEP_MS 5000

#define MAX_DISCONNECT_TIME_MINUTES 2
#define MAX_DISCONNECT_TIME_MS 60000*MAX_DISCONNECT_TIME_MINUTES 

bool reconnectWifi(){
  bool result = false;
    if (WiFi.status() != WL_CONNECTED) {
    // Connect to Wi-Fi
    Serial.println("Reconnecting to WiFi");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
  }

  int reconnectCount = 0;
  while ((WiFi.status() != WL_CONNECTED)&&(reconnectCount<20)) {
  delay(1000);
  Serial.println(".");
  reconnectCount++;
  }
  if(reconnectCount<19){
      result = true;

      if((reconnectCount>0)){
        Serial.println("WIFI CONNECTED");
        Serial.println(ssid);
        Serial.println(WiFi.localIP());
      
      }
      
  }

  return result;

  // Print ESP8266 Local IP Address
  
}
unsigned long lastmilis;
void setup() 
{
  Serial.begin(115200); // must be same baudrate with the Serial Monitor
 
  pinMode(RELAY,OUTPUT);
  digitalWrite(RELAY, HIGH);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  lastmilis = millis();
  //Start WIFI
  reconnectWifi();
 
}

void switchRelay(uint8_t value){
  Serial.print("Relay new setting: ");
  Serial.println(value);
  digitalWrite(RELAY, value);
}

bool checkInternetConnection(){
  bool pingResult = Ping.ping("www.google.com");
  if(!pingResult){
    pingResult = Ping.ping("www.wp.pl"); 
  }
  return pingResult;
}
 

void loop() 
{
  Serial.println("Loop start");
  unsigned long currentMillis = millis();
  if(reconnectWifi()){
    if(checkInternetConnection()){
      lastmilis=currentMillis;
      delay(LOOP_SLEEP_MS);
      return;
    }
  }


  unsigned long reconnectTime = currentMillis-lastmilis;
  Serial.print("Reconnect time=");
  Serial.println(reconnectTime/1000.0);
  if(reconnectTime>MAX_DISCONNECT_TIME_MS){
    //reset router
    switchRelay(LOW);
    delay(RELAY_OFF_TIME_MS);
    switchRelay(HIGH);
    lastmilis=millis();
    return;
    
  }
  delay(LOOP_SLEEP_MS);


}
