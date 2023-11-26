#include <ESP8266WiFi.h>
 
const char* ssid = "testwifi"; // fill in here your router or wifi SSID
const char* password = "qwe12345"; // fill in here your router or wifi password
 #define RELAY 0 // relay connected to  GPIO0
#define MAX_RECONNECT_COUNT 5
#define RELAY_OFF_TIME_MS 10000
#define LOOP_SLEEP_MS 5000
WiFiServer server(80);

 

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
  reconnectWifi();
  // WiFi.begin(ssid, password);
 
  // while (WiFi.status() != WL_CONNECTED) 
  // {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("");
  // Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("https://192.168.0.178/");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}

void switchRelay(uint8_t value){
  Serial.print("Relay new setting: ");
  Serial.println(value);
  digitalWrite(RELAY, value);
}
 
int reconnectCount = 0;
void loop() 
{
  Serial.println("Loop start");
  if(reconnectWifi()){
    delay(LOOP_SLEEP_MS);
    reconnectCount = 0;
    return;
  }

  reconnectCount++;
  Serial.print("Reconnect count=");
  Serial.println(reconnectCount);
  if(reconnectCount>MAX_RECONNECT_COUNT){
    //reset router
    switchRelay(LOW);
    delay(RELAY_OFF_TIME_MS);
    switchRelay(HIGH);
    return;
    
  }
  delay(LOOP_SLEEP_MS);


}
