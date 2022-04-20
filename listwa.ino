//#define terminal.println print  
#define BLYNK_PRINT Serial 
#define DHTPIN 2
#define DHTTYPE DHT11

#include <NTPClient.h>               
#include <TimeLib.h> 

#include <Blynk.h>
#include <ESP8266WiFi.h>  // for ESP8266
#include <BlynkSimpleEsp8266.h>  // for ESP8266
#include <ESP8266mDNS.h>  // For OTA w/ ESP8266
#include <WiFiUdp.h>  // For OTA
#include <ArduinoOTA.h>  // For OTA
#include <Simpletimer.h>
#include <time.h>
#include <DHT.h>
#include <math.h>

char auth[] = "c07ZKr3rcQUSeULUpMJGvc0AetLk-FhS";
char ssid[] = "WLAN2-67Q7EE";
char pass[] = "happydiamond338";
char server[] = "blynk-cloud.com";
int port = 8080;
int Digital_Ports[5];
float t=0,h=0;

WidgetTerminal terminal(V3);

DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;

void(* resetFunc) (void) = 0;

void toggle(int num){
    
    if(digitalRead(Digital_Ports[num])==0){
      digitalWrite(Digital_Ports[num],HIGH);
    }
    else {
       digitalWrite(Digital_Ports[num],LOW);
    }
}
void BLYAT(){
   Blynk.virtualWrite(V3, "\n\n");
  Blynk.virtualWrite(V3, "    ___  __          __ \n");
  Blynk.virtualWrite(V3, "   / _ )/ /_ _____  / /__ \n");
  Blynk.virtualWrite(V3, "  / _  / / // / _ \\/  '_/ \n");  // to display a backslash, print it twice
  Blynk.virtualWrite(V3, " /____/_/\\_, /_//_/_/\\_\\ \n");  // to display a backslash, print it twice
  Blynk.virtualWrite(V3, "        /___/ v");
  Blynk.virtualWrite(V3, BLYNK_VERSION" on "BLYNK_INFO_DEVICE"\n\n");
  Blynk.virtualWrite(V3, "                Wiktor L \n");
  
  terminal.flush();
}

void sendSensor()
{
h = dht.readHumidity();
t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit


if (isnan(h) || isnan(t)) {
Serial.println("Failed to read from DHT sensor!");
return;
}
// You can send any value at any time.
// Please don't send more that 10 values per second.
Blynk.virtualWrite(V1, t); // Humidity for gauge
Blynk.virtualWrite(V2, h); // Temperature for gauge
Blynk.virtualWrite(V4, t); // Humidity for graph
Blynk.virtualWrite(V5, h); // Temperature for graph
}


BLYNK_WRITE(V3)

{
  if (String("RESTART") == param.asStr()) {
  terminal.println(F("Device is restarting....."));
  delay(1000);
  resetFunc();
  }
  if (String("1") == param.asStr()) {
    toggle(1);
  }
  if (String("2") == param.asStr()) {
    toggle(2);
  }
  if (String("3") == param.asStr()) {
    toggle(3);
  }
  if (String("4") == param.asStr()) {
    toggle(4);
  }
  if (String("TEMP") == param.asStr()) {
    terminal.println(" ");
    Blynk.virtualWrite(V3,t);
    Blynk.virtualWrite(V3,"*C\n");
    Blynk.virtualWrite(V3,h);
    Blynk.virtualWrite(V3,"%\n");
    
  }

  if (String("STATE") == param.asStr()) {
    terminal.println(" ");
    for(int i=1;i<=4;i++){
      if(digitalRead(Digital_Ports[i])==0){
        terminal.println("| ON  |");
      }
      else {
        terminal.println("| OFF |");
      }
    }
    
  }
  if (String("PRINT") == param.asStr()){
     terminal.println("          ") ;
     terminal.println(" +        ") ;
     terminal.println(" +        ") ;
     terminal.println(" +        ") ;
     terminal.println(" +        ") ;
     terminal.println("   +++    ") ;
     terminal.println("          ") ;
     terminal.println("          ") ;
     terminal.println("  +++++   ") ;
     terminal.println("          ") ;
  }
   else {
    //terminal.print("UNKNOWN COMMAND:");
   // terminal.write(param.getBuffer(), param.getLength());
    terminal.println();
  }
  terminal.flush();
}


void setup() {
  Serial.begin(9600);  // BLYNK_PRINT data

  WiFi.begin(ssid, pass); 
  Blynk.config(auth, server, port);
  Blynk.connect();
  timer.setInterval(1000L, sendSensor);

  ArduinoOTA.setHostname("listwa");  // For OTA - Use your own device identifying name
  ArduinoOTA.begin();  // For OTA
    BLYAT();
    terminal.println(F(" "));
  terminal.println(F("-------------"));
  terminal.println(F("DEVICE ONLINE"));
  terminal.println(F("-------------\n"));
  terminal.flush();

  Digital_Ports[1]=4;
  Digital_Ports[2]=0;
  Digital_Ports[3]=5;
  Digital_Ports[4]=16;
}


  
void loop() {
  Blynk.run();
  ArduinoOTA.handle();  // For OTA

  timer.run();
  
}
