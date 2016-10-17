#include <WiFi101.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiSSLClient.h>
#include <WiFiUdp.h>

#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiSSLClient.h>
#include <WiFiUdp.h>

#include <SPI.h>
#include <WiFi101.h>
#include "SocketIOClient.h"

char ssid[] = "JMIphone";// "NETGEAR05";//"JMIphone"; ////"Ellipsis Jetpack 63C4";//"Ellipsis Jetpack 63C4"; //  your network SSID (name)
char pass[] = "p@l@br@p@s@r11";//"p@l@br@p@s@r11";//"a0eed798";//"a0eed798";    // your network password (use for WPA, or use as key for WEP)

int keyIndex = 0;            // your network key Index number (needed only for WEP)
char hostname[] = "172.20.10.2"; // "108.74.168.239";//"184.254.134.190";// house: "108.74.168.239"; // justin: "192.168.1.11"
int port = 3000;
int status = WL_IDLE_STATUS;
String buildingroom = "1";
String serverroom = "arduino" + buildingroom;
String device = "device_id";
String power = "power";
int ledPin=12;
bool state=true;
int h = 0;

SocketIOClient client;

extern String RID;
extern String Rname;
extern String Rcontent;

// websocket message handler: do something with command from server
void ondata(SocketIOClient client, char *data)
{
  //Serial.print(data);
  Serial.println("message received");
  Serial.println(data);
  Serial.println();
  /*if(state){
    digitalWrite(ledPin, LOW);
    state = false;
  }else if(state == false){
    digitalWrite(ledPin,HIGH);
    state = true;
  }*/
}

String toJSON(String d, String p){
  return "{\"" + device + "\":" + d + ",\"" + power + "\":" + p +"}";
}
unsigned long previousMillis = 0; 
long interval = 10000; 
void setup() {
  pinMode(ledPin,OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
     // wait for serial port to connect. Needed for native USB port only
  }
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi!");
  if(client.connect(hostname, port))
  {
    Serial.println("Connection to wifi confirmed");
  }
  
  client.setDataArrivedDelegate(ondata);
  if (!client.connect(hostname, port))
  {
    Serial.println(F("Not connected."));
  }
  if (client.connected())
  {
    //client.send("connection", "message", "Connected !!!!");
    client.send("joinR", "message", serverroom);
  }
  else
  {
    Serial.println("Connection Error.");
    while(1);
  }
  delay(1000);
  
  //digitalWrite(ledPin, HIGH);
}

void loop()
{
  
  /*unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval)
  {
    previousMillis = currentMillis; 
    //client.heartbeat(0);
    client.send("atime", "message", "Time please?");
  }
  */
  if (client.monitor())
  {
    Serial.println(RID);
    if (RID == "execute")
    {
      Serial.println(Rcontent);
      if(state){
        digitalWrite(ledPin,LOW);
        state = false;
      }else{
        digitalWrite(ledPin,HIGH);
        state = true;
      }
    }
  }
  h = h+1;
  String num1 = String(h);
  String num2 = String(h);
  //if(state=true){
    //digitalWrite(ledPin, HIGH);
  //}else{
  //  digitalWrite(ledPin, LOW);
  //}
  String data = "[{\"room\":" + buildingroom + "}," + toJSON("1", num1) + "," + toJSON("2", num2) + "]";
  if (client.connected())
  {
    //Serial.println(data);
    client.sendJSON("data", data);
  }else{
    Serial.println("Not connected");
  }
  delay(2000);
  
}
