/*
 * Important things to note. 
 * Please select tools and select IwIP variant as v1.4Prebuilt for wifi reconnection to work
 * Spiff Size must be 4M(1M SPIFF)
 * Flash mode DIO
 */
#include <DNSServer.h>
#include <WiFiManager.h>  
#include <SimpleTimer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include "FS.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Adafruit_ADS1015.h>
#include <Wire.h>
#include <IRsend.h>
#include "Config.h"
#include "ServerHandlers.h"
#include <IRremoteESP8266.h>

//IR Commands
uint32_t on_off[135] = {9828, 9738,  9824, 9742,  4634, 2450,  370, 368,  400, 900,  366, 948,  408, 330,  400, 904,  408, 332,  402, 328,  402, 340,  360, 368,  394, 906,  406, 334,  360, 370,  404, 332,  400, 906,  408, 332,  400, 342,  402, 894,  408, 900,  402, 910,  368, 376,  398, 334,  400, 334,  402, 896,  410, 338,  404, 324,  400, 902,  408, 332,  396, 334,  360, 372,  404, 330,  360, 372,  396, 342,  402, 326,  402, 330,  404, 330,  398, 330,  404, 900,  410, 330,  404, 328,  402, 340,  402, 324,  360, 374,  402, 906,  410, 330,  402, 902,  408, 334,  396, 332,  400, 346,  400, 324,  398, 334,  404, 904,  410, 332,  402, 328,  402, 906,  408, 332,  404, 338,  404, 892,  410, 330,  400, 906,  366, 944,  408, 902,  412, 328,  400, 330,  360, 376,  404};
uint32_t sixteen_three[135] = {9778, 9786,  9776, 9788,  4586, 2500,  360, 376,  352, 954,  356, 958,  356, 382,  352, 954,  354, 384,  350, 380,  352, 390,  390, 336,  350, 952,  356, 382,  350, 380,  350, 382,  352, 956,  356, 384,  350, 390,  352, 948,  354, 958,  352, 958,  354, 384,  352, 952,  356, 384,  352, 378,  352, 392,  350, 944,  358, 956,  352, 384,  352, 380,  350, 382,  350, 384,  352, 378,  352, 390,  352, 376,  350, 382,  352, 382,  352, 378,  350, 958,  350, 384,  352, 378,  352, 390,  352, 374,  352, 384,  350, 958,  356, 384,  350, 954,  356, 382,  352, 380,  352, 390,  352, 374,  352, 954,  352, 958,  356, 382,  352, 954,  354, 384,  350, 380,  352, 390,  352, 374,  350, 382,  352, 956,  356, 384,  352, 954,  354, 958,  354, 956,  354, 388,  350}; 
uint32_t seventeen_three[135] = {9736, 9828,  9736, 9830,  4544, 2542,  324, 410,  316, 986,  322, 988,  326, 416,  318, 988,  322, 416,  316, 412,  318, 424,  318, 408,  318, 984,  324, 418,  316, 412,  318, 416,  316, 990,  324, 416,  318, 424,  316, 980,  322, 988,  322, 994,  318, 416,  318, 984,  324, 416,  318, 412,  318, 424,  318, 980,  322, 984,  324, 416,  318, 412,  318, 416,  318, 416,  318, 414,  318, 424,  316, 408,  318, 414,  318, 414,  320, 412,  318, 984,  324, 414,  320, 410,  318, 424,  318, 408,  318, 416,  318, 990,  324, 416,  318, 984,  324, 416,  318, 414,  318, 424,  318, 982,  318, 984,  326, 990,  324, 416,  318, 986,  324, 416,  318, 412,  318, 422,  318, 408,  318, 416,  318, 988,  324, 416,  318, 416,  318, 418,  316, 412,  318, 992,  326};
uint32_t eightteen_three[135] = {9778, 9786,  9778, 9786,  4584, 2504,  330, 408,  350, 956,  352, 964,  350, 386,  350, 954,  354, 384,  350, 382,  350, 390,  350, 376,  350, 958,  350, 384,  350, 380,  350, 384,  350, 960,  354, 384,  350, 390,  350, 948,  354, 384,  350, 384,  350, 956,  350, 958,  350, 384,  350, 380,  350, 390,  350, 948,  352, 956,  352, 384,  350, 380,  350, 384,  352, 382,  352, 380,  350, 392,  350, 376,  352, 382,  350, 382,  352, 380,  350, 954,  352, 386,  350, 378,  350, 392,  350, 376,  350, 384,  350, 960,  350, 386,  350, 958,  350, 386,  348, 382,  348, 392,  350, 376,  350, 384,  350, 384,  350, 950,  354, 958,  322, 416,  350, 382,  350, 390,  350, 376,  350, 384,  350, 960,  352, 386,  350, 954,  354, 964,  348, 386,  350, 964,  352}; 
uint32_t nineteen_three[135] = {9782, 9782,  9780, 9784,  4588, 2498,  334, 400,  326, 980,  328, 982,  332, 408,  326, 980,  330, 408,  326, 404,  326, 416,  326, 400,  324, 982,  328, 410,  324, 404,  326, 406,  326, 986,  328, 408,  326, 416,  326, 970,  330, 408,  326, 406,  326, 976,  330, 982,  328, 408,  326, 404,  326, 416,  326, 970,  332, 978,  330, 408,  326, 404,  326, 406,  326, 408,  326, 404,  326, 414,  326, 400,  326, 408,  326, 408,  326, 404,  326, 978,  332, 408,  326, 404,  326, 414,  326, 400,  326, 406,  326, 984,  330, 408,  326, 976,  332, 408,  326, 404,  326, 414,  326, 974,  328, 408,  326, 408,  326, 976,  330, 978,  330, 408,  326, 404,  326, 416,  326, 402,  326, 404,  326, 982,  330, 408,  326, 408,  326, 408,  326, 974,  332, 986,  330}; 
uint32_t twenty_three[135] = {9782, 9782,  9780, 9784,  4588, 2500,  334, 400,  326, 978,  332, 982,  330, 408,  356, 948,  330, 408,  356, 374,  356, 384,  358, 368,  356, 950,  330, 408,  356, 374,  356, 378,  356, 950,  332, 408,  356, 384,  356, 370,  356, 376,  356, 378,  356, 376,  356, 378,  356, 950,  332, 408,  356, 386,  356, 944,  328, 980,  328, 408,  356, 374,  356, 376,  356, 378,  356, 374,  358, 384,  356, 370,  356, 376,  356, 378,  356, 374,  356, 950,  330, 408,  356, 374,  356, 384,  358, 370,  356, 376,  356, 954,  330, 408,  356, 948,  330, 408,  356, 376,  356, 386,  356, 370,  356, 376,  356, 378,  356, 374,  356, 376,  358, 954,  330, 408,  356, 386,  356, 370,  356, 378,  356, 956,  328, 408,  356, 378,  356, 378,  356, 948,  328, 990,  328};
uint32_t twenty_one_three[135] = {9824, 9742,  9784, 9782,  4590, 2496,  368, 368,  358, 946,  362, 948,  366, 376,  358, 946,  402, 336,  358, 370,  360, 382,  358, 368,  360, 944,  364, 374,  360, 372,  360, 374,  358, 952,  362, 376,  358, 382,  360, 368,  358, 374,  360, 374,  360, 370,  360, 374,  358, 948,  364, 376,  358, 382,  358, 936,  364, 944,  364, 376,  358, 372,  358, 374,  358, 374,  360, 370,  360, 382,  360, 368,  358, 374,  360, 374,  358, 370,  360, 944,  364, 374,  360, 370,  360, 382,  360, 368,  358, 374,  358, 950,  362, 376,  358, 946,  362, 374,  360, 372,  360, 382,  360, 936,  366, 374,  360, 372,  360, 372,  358, 374,  358, 948,  364, 376,  358, 382,  360, 368,  358, 374,  360, 948,  364, 376,  360, 944,  364, 376,  360, 940,  364, 952,  366};
uint32_t twenty_two_three[135] = {9782, 9782,  9782, 9782,  4590, 2496,  334, 400,  356, 952,  328, 984,  328, 408,  356, 948,  330, 408,  356, 374,  356, 384,  358, 370,  356, 948,  330, 408,  356, 374,  356, 376,  358, 952,  360, 378,  356, 384,  358, 938,  362, 378,  356, 378,  358, 374,  356, 376,  356, 952,  360, 380,  356, 384,  358, 944,  328, 978,  330, 408,  356, 374,  358, 376,  356, 376,  356, 374,  356, 384,  356, 370,  356, 378,  356, 378,  358, 374,  356, 948,  330, 408,  358, 374,  356, 386,  356, 370,  356, 376,  358, 958,  324, 408,  356, 946,  362, 378,  356, 374,  356, 386,  356, 370,  356, 948,  330, 408,  356, 374,  358, 376,  356, 952,  330, 408,  356, 384,  356, 370,  356, 378,  356, 954,  358, 378,  356, 948,  360, 954,  330, 980,  330, 988,  330};
uint32_t twenty_three_three[135] =  {9782, 9782,  9824, 9740,  4590, 2498,  410, 324,  404, 898,  410, 904,  408, 334,  400, 902,  408, 330,  402, 330,  400, 340,  400, 326,  400, 902,  410, 330,  398, 332,  398, 336,  404, 904,  408, 334,  398, 340,  404, 892,  408, 332,  406, 330,  358, 370,  400, 334,  404, 904,  410, 330,  404, 336,  404, 892,  408, 900,  410, 330,  396, 334,  406, 328,  400, 334,  360, 372,  400, 342,  402, 326,  400, 332,  358, 376,  358, 370,  398, 904,  410, 332,  400, 330,  402, 338,  360, 366,  400, 334,  360, 948,  406, 334,  404, 900,  408, 330,  400, 330,  404, 340,  400, 894,  408, 900,  406, 334,  402, 332,  400, 332,  404, 902,  408, 332,  398, 342,  400, 328,  402, 330,  396, 910,  366, 376,  400, 334,  398, 334,  400, 330,  396, 340,  360};
uint32_t twenty_four_three[135] =  {9782, 9782,  9782, 9782,  4590, 2498,  364, 370,  358, 948,  360, 954,  360, 378,  358, 946,  362, 378,  358, 374,  358, 384,  358, 368,  358, 948,  360, 376,  358, 372,  358, 376,  358, 952,  360, 378,  358, 384,  358, 368,  358, 946,  362, 376,  358, 372,  358, 376,  358, 950,  362, 378,  358, 384,  358, 940,  360, 948,  362, 376,  358, 374,  358, 376,  358, 376,  358, 372,  358, 384,  358, 368,  358, 376,  358, 376,  358, 372,  358, 944,  364, 376,  358, 372,  358, 382,  358, 368,  358, 376,  356, 952,  362, 376,  358, 946,  364, 376,  358, 372,  358, 382,  358, 368,  358, 376,  358, 950,  362, 378,  356, 376,  358, 952,  360, 378,  358, 384,  358, 368,  358, 376,  358, 952,  360, 378,  358, 376,  358, 952,  362, 376,  358, 380,  358};
uint32_t twenty_five_three[135] = {9782, 9784,  9780, 9784,  4588, 2498,  336, 400,  326, 978,  330, 986,  328, 408,  326, 980,  330, 408,  326, 404,  356, 386,  356, 370,  356, 950,  328, 408,  326, 404,  356, 378,  356, 956,  328, 408,  354, 386,  356, 370,  356, 948,  330, 408,  326, 404,  326, 408,  326, 980,  332, 408,  326, 414,  356, 944,  328, 980,  330, 406,  326, 404,  326, 406,  356, 378,  356, 374,  356, 386,  356, 372,  354, 378,  356, 378,  356, 374,  356, 950,  330, 408,  356, 374,  356, 386,  356, 370,  356, 378,  356, 952,  332, 408,  356, 950,  330, 408,  354, 376,  354, 386,  356, 942,  328, 408,  326, 984,  330, 408,  356, 378,  356, 952,  330, 408,  326, 414,  356, 370,  356, 376,  356, 954,  330, 408,  356, 950,  330, 982,  332, 408,  356, 382,  356}; 
uint32_t twenty_six_three[135] = {9784, 9780,  9782, 9782,  4590, 2494,  366, 370,  356, 946,  362, 952,  360, 378,  358, 948,  360, 376,  358, 372,  360, 384,  358, 368,  358, 946,  362, 376,  358, 372,  358, 376,  358, 950,  362, 376,  358, 382,  358, 938,  362, 946,  362, 378,  358, 372,  358, 376,  358, 950,  362, 378,  358, 384,  358, 942,  360, 946,  362, 376,  358, 372,  358, 376,  358, 376,  358, 372,  358, 384,  358, 368,  358, 376,  358, 378,  356, 372,  358, 948,  360, 376,  358, 374,  356, 384,  360, 368,  358, 374,  358, 950,  362, 378,  356, 948,  360, 376,  358, 372,  358, 384,  358, 366,  358, 944,  364, 950,  362, 378,  358, 376,  358, 950,  364, 376,  358, 384,  358, 370,  358, 374,  360, 948,  362, 378,  358, 946,  362, 376,  358, 942,  364, 380,  356};

//Trigger soft AP by pressing the flashing button
#define TRIGGER_PIN 0

//Object for utility functions
Config conf;
ServerHandlers ServHand;
SimpleTimer timer;
Adafruit_ADS1115 ads (0x48);
Adafruit_ADS1115 ads1115_1(0x49);

bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}
//Wifi and broker parameters
char broker[50] = "-";//
char ssid[70] = "-";
char password[70] = "-";
char port[5] = "-";
char clientID[50] = "-";
char newHostName[50] = "-";
char static_ip[16] = "-";
char static_gw[16] = "-";
char static_sn[16] = "-";

char ip1[5] = "-";
char ip2[5] = "-";
char ip3[5] = "-";
char ip4[5] = "-";

char gw1[5] = "-";
char gw2[5] = "-";
char gw3[5] = "-";
char gw4[5] = "-";

char sn1[5] = "-";
char sn2[5] = "-";
char sn3[5] = "-";
char sn4[5] = "-";

//Array to store raw IR data received from server.
uint32_t J[150];

//Disconnect flag to set LED status
int disconnectFlag = 1;

//Device id
char device_id[50] = "Abcd";

//Timer id
int wd_timer_id;

//Default timer interval
int Interval = 5;

//Sawyer's defined variable for CT current measurements
int mVperAmp = 185;
int mVperAmp1 = 185;
int mVperAmp2 = 185;
int state;
double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;
double Voltage1 = 0;
double VRMS1 = 0;
double AmpsRMS1 = 0;
double Voltage2 = 0;
double VRMS2 = 0;
double AmpsRMS2 = 0;
uint32_t start_time;
uint32_t last = 0;
uint32_t m = 0;
int i, A, A2;
char a1 [300];
String CTdataString;

int check [10];
int k = 0;
//Sawyer's defined variable end

//Create instances of wificlient, IR and Server
WiFiClient espClient;
IRsend irsend(4);  
IRsend irsend1(5);
ESP8266WebServer server(80);

//HTML for OTA
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

//Function callback for mqtt
void callback(char* topic, byte* payload, unsigned int length);


PubSubClient client(broker, atoi(port), callback, espClient); //set  MQTT port number to 8883 as per //standard

//Timer function to send CT data
void repeatCT(){
    String CTdataString;
    char CTdataChar[50];
    CTdataString = String("");
    CTdataString = CTdataString + AmpsRMS + "," + AmpsRMS1;
    CTdataString.toCharArray(CTdataChar, CTdataString.length() + 1);
   
    String datam = "data/";
    String CTm = "/CT";
    String deviceidm(device_id);
    Serial.println(datam+deviceidm+CTm);
    client.publish((datam+deviceidm+CTm).c_str(),CTdataChar);
    Serial.println("Repeating");
}

void 

void setup(void) {
  Serial.begin(115200);
 
  pinMode(14, OUTPUT);
  pinMode (16, OUTPUT);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode (15, OUTPUT);
  digitalWrite (15, LOW);
  Wire.begin (13, 12);
  irsend.begin();
  irsend1.begin();
  SPIFFS.begin();
  //load all essential configurations.
  
  if (SPIFFS.exists("/wifi.json")){
      conf.loadWifi();
  }
  
  if (SPIFFS.exists("/clientID.json")){
      conf.loadClientID();
  }
  
  if (SPIFFS.exists("/interval.json")){
      conf.loadInterval();
  }
  
  if (SPIFFS.exists("/ip.json")){
      conf.loadIp();
  }
  
  
  if (SPIFFS.exists("/Mqtt.json")){
      conf.loadMqtt();
  }
  
  //Delete any previous timer id
  timer.deleteTimer(wd_timer_id);
  //Set new timer id
  wd_timer_id = timer.setInterval((Interval*1000), repeatCT);  
  //Set ads parameters
  ads1115_1.setGain(GAIN_ONE);
  ads.setGain(GAIN_ONE);
 
  delay(1);
  Serial.println();
  Serial.println("Booting Sketch...");
  //**Wifi.disconnect Needed for reonnect logic
  WiFi.disconnect();
  //Station mode to hide AP
  WiFi.mode(WIFI_STA);
  WiFi.hostname(newHostName);
  
  IPAddress ip(atoi(ip1),atoi(ip2),atoi(ip3),atoi(ip4));   
  IPAddress gateway(atoi(gw1),atoi(gw2),atoi(gw3),atoi(gw4));   
  IPAddress subnet(atoi(sn1),atoi(sn2),atoi(sn3),atoi(sn4));   
  WiFi.config(ip, gateway, subnet);

  WiFi.begin(ssid, password);

  delay(2000);
      
  //Server handlers
  server.on("/wificonfig", &ServerHandlers::handleWifi); 
  server.on("/clientID", &ServerHandlers::handleClientID); 
  server.on("/configip", &ServerHandlers::handleIP);
  server.on("/reset", &ServerHandlers::handleReset);
  server.on("/configInterval", &ServerHandlers::handleInterval);
//  server.on("/version", &ServerHandlers::handleVersion);
  server.on("/configMqtt", &ServerHandlers::handleMqtt);

    
    
}

//Use to trigger softAP
int serverFlag = 0;

//Wifi Reonnect logic
void wifiReconnect() {
 while (WiFi.status() != WL_CONNECTED) {
  Serial.println("Wifi Reconnecting");

       //Trigger LED to show disconnect status. Blinks every 2 seconds.
       if(disconnectFlag == 1){
        digitalWrite (16, LOW);
        delay (100);
        digitalWrite (16, HIGH);
        delay (100);
       }

    //Check pin state of flash button if zero spawn soft ap
    if(digitalRead(TRIGGER_PIN) ==0){
        digitalWrite(16, LOW);
        WiFiManagerParameter custom_mqtt_server("server", "mqtt server", broker, 50); //WiFi manager confi
        WiFiManagerParameter custom_clientID("clientid", "clientid", clientID, 50);
        WiFiManagerParameter custom_hostName("hostname", "hostname", newHostName, 50);
        WiFiManagerParameter custom_deviceID("deviceid", "deviceid", device_id, 50);
        WiFiManagerParameter custom_port("port", "port", port, 5); 
        WiFiManagerParameter custom_wifi("wifi", "wifi", ssid, 70); 
        WiFiManagerParameter custom_password("password", "password", password, 70); 
        
        WiFiManagerParameter ip1_port("ip1", "ip1", ip1, 5); 
        WiFiManagerParameter ip2_port("ip2", "ip2", ip2, 5); 
        WiFiManagerParameter ip3_port("ip3", "ip3", ip3, 5); 
        WiFiManagerParameter ip4_port("ip4", "ip4", ip4, 5); 
        
        WiFiManagerParameter gw1_port("gw1", "gw1", gw1, 5); 
        WiFiManagerParameter gw2_port("gw2", "gw2", gw2, 5); 
        WiFiManagerParameter gw3_port("gw3", "gw3", gw3, 5); 
        WiFiManagerParameter gw4_port("gw4", "gw4", gw4, 5);
         
        WiFiManagerParameter sn1_port("sn1", "sn1", sn1, 5); 
        WiFiManagerParameter sn2_port("sn2", "sn2", sn2, 5); 
        WiFiManagerParameter sn3_port("sn3", "sn3", sn3, 5); 
        WiFiManagerParameter sn4_port("sn4", "sn4", sn4, 5); 

           WiFiManager wifiManager;
           wifiManager.setSaveConfigCallback(saveConfigCallback);


           wifiManager.addParameter(&custom_wifi);
           wifiManager.addParameter(&custom_password);
             
           wifiManager.addParameter(&custom_mqtt_server);
           wifiManager.addParameter(&custom_port);
           wifiManager.addParameter(&custom_clientID);
           wifiManager.addParameter(&custom_hostName);
           wifiManager.addParameter(&custom_deviceID);
              
           wifiManager.addParameter(&ip1_port);
           wifiManager.addParameter(&ip2_port);
           wifiManager.addParameter(&ip3_port);
           wifiManager.addParameter(&ip4_port);

           wifiManager.addParameter(&gw1_port);
           wifiManager.addParameter(&gw2_port);
           wifiManager.addParameter(&gw3_port);
           wifiManager.addParameter(&gw4_port);

           wifiManager.addParameter(&sn1_port);
           wifiManager.addParameter(&sn2_port);
           wifiManager.addParameter(&sn3_port);
           wifiManager.addParameter(&sn4_port);

             
            if (!wifiManager.startConfigPortal("OnDemandAP", "lolololo")) {
                Serial.println("failed to connect and hit timeout");
      
            }

             strcpy(broker, custom_mqtt_server.getValue());
             strcpy(clientID, custom_clientID.getValue());
             strcpy(newHostName, custom_hostName.getValue());
             strcpy(device_id, custom_deviceID.getValue());
             strcpy(port, custom_port.getValue());
                     
             strcpy(ssid, custom_wifi.getValue());
             strcpy(password, custom_password.getValue());

             strcpy(ip1,  ip1_port.getValue());
             strcpy(ip2,  ip2_port.getValue());
             strcpy(ip3,  ip3_port.getValue());
             strcpy(ip4,  ip4_port.getValue());

             strcpy(gw1, gw1_port.getValue());
             strcpy(gw2, gw2_port.getValue());
             strcpy(gw3, gw3_port.getValue());
             strcpy(gw4, gw4_port.getValue());

             strcpy(sn1, sn1_port.getValue());
             strcpy(sn2, sn2_port.getValue());
             strcpy(sn3, sn3_port.getValue());
             strcpy(sn4, sn4_port.getValue());
         
             if (shouldSaveConfig) {
                Serial.println("saving config");
                DynamicJsonBuffer jsonBuffer;
                JsonObject& json = jsonBuffer.createObject();

                json["broker"] = broker;
                json["port"] = port;
                 File configFile = SPIFFS.open("/Mqtt.json", "w");
                 if (!configFile) {
                    Serial.println("failed to open config file for writing");
                 }
              
                 json.prettyPrintTo(Serial);
                 json.printTo(configFile);
                 configFile.close();
                }

                 if (shouldSaveConfig) {
                Serial.println("saving config");
                DynamicJsonBuffer jsonBuffer;
                JsonObject& json = jsonBuffer.createObject();

                  json["clientID"] = clientID;
                  json["hostID"] = newHostName;
                  json["deviceID"] = device_id;
                 
                 

                 File configFile = SPIFFS.open("/clientID.json", "w");
                 if (!configFile) {
                    Serial.println("failed to open config file for writing");
                 }
              
                 json.prettyPrintTo(Serial);
                 json.printTo(configFile);
                 configFile.close();
                }

                
                 if (shouldSaveConfig) {
                Serial.println("saving config");
                DynamicJsonBuffer jsonBuffer;
                JsonObject& json = jsonBuffer.createObject();

                  json["ssid"] = WiFi.SSID();
                  json["password"] = WiFi.psk();

                 File configFile = SPIFFS.open("/wifi.json", "w");
                 if (!configFile) {
                    Serial.println("failed to open config file for writing");
                 }
              
                 json.prettyPrintTo(Serial);
                 json.printTo(configFile);
                 configFile.close();
                }

                
                 if (shouldSaveConfig) {
                Serial.println("saving config");
                DynamicJsonBuffer jsonBuffer;
                JsonObject& json = jsonBuffer.createObject();

                    json["ip1"] = ip1;
                    json["ip2"] = ip2;
                    json["ip3"] = ip3;
                    json["ip4"] = ip4;
           
                    json["gw1"] = gw1;
                    json["gw2"] = gw2;
                    json["gw3"] = gw3;
                    json["gw4"] = gw4;
          
                    json["sn1"] = sn1;
                    json["sn2"] = sn2;
                    json["sn3"] = sn3;
                    json["sn4"] = sn4;

                 File configFile = SPIFFS.open("/ip.json", "w");
                 if (!configFile) {
                    Serial.println("failed to open config file for writing");
                 }
              
                 json.prettyPrintTo(Serial);
                 json.printTo(configFile);
                 configFile.close();
                }
             Serial.println("connected...yeey :)");
             ESP.restart();
       }
       
       delay(2000);
    }
 }
    
void reconnect() {
  // Loop until we're reconnected

  wifiReconnect();
  
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
     
      if(disconnectFlag == 1){
        digitalWrite (16, LOW);
        delay (100);
        digitalWrite (16, HIGH);
        digitalWrite (16, LOW);
        delay (100);
        digitalWrite (16, HIGH);
        digitalWrite (16, LOW);
        delay (100);
        digitalWrite (16, HIGH);
        digitalWrite (16, LOW);
        delay (100);
        digitalWrite (16, HIGH);
      }
    
      if(digitalRead(TRIGGER_PIN) ==0){
        digitalWrite(16, LOW);
        WiFiManagerParameter custom_mqtt_server("server", "mqtt server", broker, 50); //WiFi manager confi
        WiFiManagerParameter custom_clientID("clientid", "clientid", clientID, 50);
        WiFiManagerParameter custom_hostName("hostname", "hostname", newHostName, 50);
        WiFiManagerParameter custom_deviceID("deviceid", "deviceid", device_id, 50);
        WiFiManagerParameter custom_port("port", "port", port, 5); 
        WiFiManagerParameter custom_wifi("wifi", "wifi", ssid, 70); 
        WiFiManagerParameter custom_password("password", "password", password, 70); 
        
        WiFiManagerParameter ip1_port("ip1", "ip1", ip1, 5); 
        WiFiManagerParameter ip2_port("ip2", "ip2", ip2, 5); 
        WiFiManagerParameter ip3_port("ip3", "ip3", ip3, 5); 
        WiFiManagerParameter ip4_port("ip4", "ip4", ip4, 5); 
        
        WiFiManagerParameter gw1_port("gw1", "gw1", gw1, 5); 
        WiFiManagerParameter gw2_port("gw2", "gw2", gw2, 5); 
        WiFiManagerParameter gw3_port("gw3", "gw3", gw3, 5); 
        WiFiManagerParameter gw4_port("gw4", "gw4", gw4, 5);
         
        WiFiManagerParameter sn1_port("sn1", "sn1", sn1, 5); 
        WiFiManagerParameter sn2_port("sn2", "sn2", sn2, 5); 
        WiFiManagerParameter sn3_port("sn3", "sn3", sn3, 5); 
        WiFiManagerParameter sn4_port("sn4", "sn4", sn4, 5); 

        WiFiManager wifiManager;
        wifiManager.setSaveConfigCallback(saveConfigCallback);


        wifiManager.addParameter(&custom_wifi);
        wifiManager.addParameter(&custom_password);
             
        wifiManager.addParameter(&custom_mqtt_server);
        wifiManager.addParameter(&custom_port);
        wifiManager.addParameter(&custom_clientID);
        wifiManager.addParameter(&custom_hostName);
        wifiManager.addParameter(&custom_deviceID);
               
        
             
        wifiManager.addParameter(&ip1_port);
        wifiManager.addParameter(&ip2_port);
        wifiManager.addParameter(&ip3_port);
        wifiManager.addParameter(&ip4_port);

        wifiManager.addParameter(&gw1_port);
        wifiManager.addParameter(&gw2_port);
        wifiManager.addParameter(&gw3_port);
        wifiManager.addParameter(&gw4_port);

        wifiManager.addParameter(&sn1_port);
        wifiManager.addParameter(&sn2_port);
        wifiManager.addParameter(&sn3_port);
        wifiManager.addParameter(&sn4_port);

     
              
            if (!wifiManager.startConfigPortal("OnDemandAP", "lolololo")) {
                Serial.println("failed to connect and hit timeout");
      
            }

             strcpy(broker, custom_mqtt_server.getValue());
             strcpy(clientID, custom_clientID.getValue());
             strcpy(newHostName, custom_hostName.getValue());
             strcpy(device_id, custom_deviceID.getValue());
             strcpy(port, custom_port.getValue());
                     
             strcpy(ssid, custom_wifi.getValue());
             strcpy(password, custom_password.getValue());

             strcpy(ip1,  ip1_port.getValue());
             strcpy(ip2,  ip2_port.getValue());
             strcpy(ip3,  ip3_port.getValue());
             strcpy(ip4,  ip4_port.getValue());

             strcpy(gw1, gw1_port.getValue());
             strcpy(gw2, gw2_port.getValue());
             strcpy(gw3, gw3_port.getValue());
             strcpy(gw4, gw4_port.getValue());

             strcpy(sn1, sn1_port.getValue());
             strcpy(sn2, sn2_port.getValue());
             strcpy(sn3, sn3_port.getValue());
             strcpy(sn4, sn4_port.getValue());
                     
             if (shouldSaveConfig) {
                Serial.println("saving config");
                DynamicJsonBuffer jsonBuffer;
                JsonObject& json = jsonBuffer.createObject();

                json["broker"] = broker;
                json["port"] = port;
                 File configFile = SPIFFS.open("/Mqtt.json", "w");
                 if (!configFile) {
                    Serial.println("failed to open config file for writing");
                 }
              
                 json.prettyPrintTo(Serial);
                 json.printTo(configFile);
                 configFile.close();
                }

                 if (shouldSaveConfig) {
                Serial.println("saving config");
                DynamicJsonBuffer jsonBuffer;
                JsonObject& json = jsonBuffer.createObject();

                  json["clientID"] = clientID;
                  json["hostID"] = newHostName;
                  json["deviceID"] = device_id;
                 
                 

                 File configFile = SPIFFS.open("/clientID.json", "w");
                 if (!configFile) {
                    Serial.println("failed to open config file for writing");
                 }
              
                 json.prettyPrintTo(Serial);
                 json.printTo(configFile);
                 configFile.close();
                }

                
                 if (shouldSaveConfig) {
                Serial.println("saving config");
                DynamicJsonBuffer jsonBuffer;
                JsonObject& json = jsonBuffer.createObject();

                  json["ssid"] = WiFi.SSID();
                  json["password"] = WiFi.psk();

                 File configFile = SPIFFS.open("/wifi.json", "w");
                 if (!configFile) {
                    Serial.println("failed to open config file for writing");
                 }
              
                 json.prettyPrintTo(Serial);
                 json.printTo(configFile);
                 configFile.close();
                }

                
                 if (shouldSaveConfig) {
                Serial.println("saving config");
                DynamicJsonBuffer jsonBuffer;
                JsonObject& json = jsonBuffer.createObject();

                    json["ip1"] = ip1;
                    json["ip2"] = ip2;
                    json["ip3"] = ip3;
                    json["ip4"] = ip4;
           
                    json["gw1"] = gw1;
                    json["gw2"] = gw2;
                    json["gw3"] = gw3;
                    json["gw4"] = gw4;
          
                    json["sn1"] = sn1;
                    json["sn2"] = sn2;
                    json["sn3"] = sn3;
                    json["sn4"] = sn4;

                 File configFile = SPIFFS.open("/ip.json", "w");
                 if (!configFile) {
                    Serial.println("failed to open config file for writing");
                 }
              
                 json.prettyPrintTo(Serial);
                 json.printTo(configFile);
                 configFile.close();
                }
             Serial.println("connected...yeey :)");
             ESP.restart();
         
       }

      
    // Attempt to connect
       mqttReconnect();
  }
}

void mqttReconnect(){
  
       String dataSub = "data/";
       String versionSub = "/version";
       String commandSub = "cmnd/";
       String sendSub1 = "/send1";
       String sendSub2 = "/send2";
       String stats = "/status";
       String deviceidSub(device_id);
       String will_birth_Topic = dataSub + deviceidSub + stats;
       String versionTopic = dataSub + deviceidSub + versionSub;
  
  if (client.connect(clientID, will_birth_Topic.c_str(), 2, true, "offline")) {
      Serial.println("connected");
      disconnectFlag = 1;
      loadServer();
      // Once connected, publish an announcement...
      client.publish(will_birth_Topic.c_str(), "online",true);
    
      client.publish(versionTopic.c_str(),"1.0.7",true);
       //Subscribe to mqtt reset
      client.subscribe("reset");
      
      Serial.println(commandSub+deviceidSub+sendSub1);
      Serial.println(commandSub+deviceidSub+sendSub2);

      //Subscribe to IR commands for channel 1 and 2 IR
      client.subscribe((commandSub+deviceidSub+sendSub1).c_str());
      client.subscribe((commandSub+deviceidSub+sendSub2).c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }  
  
}

//OTA function
void loadServer() {
 
  if (WiFi.status() == WL_CONNECTED) {
 
    server.on("/firmware", HTTP_GET, [](){
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", serverIndex);
    });
    
    server.on("/update", HTTP_POST, []() {
    
      
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      pinMode (0, OUTPUT);
      digitalWrite (0, HIGH);
      digitalWrite (15, LOW);    
      ESP.restart();
    }, []() {
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.setDebugOutput(true);
        //        WiFiUDP::stopAll();
        Serial.printf("Update: %s\n", upload.filename.c_str());
        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        if (!Update.begin(maxSketchSpace)) { //start with max available size
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      }
      yield();
    });
    server.begin();
    Serial.println(WiFi.localIP());
    // MDNS.addService("http", "tcp", 80);

    Serial.printf("Ready! Open http://%s.local in your browser\n");
  } else {
    Serial.println("WiFi Failed");
  }

}
//CT1 measurement
float CT1 () {
  float result;
  int readValue;
  int maxValue;
  int minValue;
  uint32_t start_time = millis();
  while ((millis() - start_time) < 250) {
    readValue = ads.readADC_Differential_0_1();;
    if (readValue > maxValue) {
      maxValue = readValue;
    }
    if (readValue < minValue) {
      minValue = readValue;
    }
  }
  Voltage = ((maxValue - minValue) * 2.85) / 2600.0;
  VRMS = (Voltage / 2.0) * 0.707;
  AmpsRMS = (VRMS * 1000) / mVperAmp;
  return AmpsRMS;
};

//CT2 measurements
float CT2 () {
  float result1;
  int readValue1;
  int maxValue1;
  int minValue1;
  uint32_t start_time = millis();
  while ((millis() - start_time) < 250) {
    readValue1 = ads.readADC_Differential_2_3();;

    if (readValue1 > maxValue1) {
      maxValue1 = readValue1;
    }
    if (readValue1 < minValue1) {
      minValue1 = readValue1;
    }
  }
  Voltage1 = ((maxValue1 - minValue1) * 2.85) / 2600.0;
  VRMS1 = (Voltage1 / 2.0) * 0.707;
  AmpsRMS1 = (VRMS1 * 1000) / mVperAmp1;
  return AmpsRMS1;
};
void loop(void) {

//Check for mqtt connection
if (!client.connected()) {
    reconnect();
}

  client.loop();
  delay(10);
  //Run timer task
  timer.run();
  //Keep server Running
  server.handleClient();


  Serial.print("local IP:");
  Serial.println(WiFi.localIP());
  Serial.print("Memory Left:");
  Serial.println(ESP.getFreeHeap());
 // Serial.print("Pin 5 status:");
 // Serial.println(digitalRead(TRIGGER_PIN));
  Serial.print("CT1:");
  Serial.println(CT1());
  Serial.print("CT2:");
  Serial.println(CT2());


  delay(2000);
}

//Call back function for mqtt to trigger IR commands
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  Serial.print("length:");
  Serial.println(length);

   String dataSub = "data/";
   String commandSub = "cmnd/";
   String sendSub1 = "/send1";
   String sendSub2 = "/send2";
   String deviceidSub(device_id);

   String command1 = commandSub+deviceidSub+sendSub1;
   String command2 = commandSub+deviceidSub+sendSub2;
   String data1 = dataSub+deviceidSub+sendSub1;
   String data2 = dataSub+deviceidSub+sendSub2;
  
  String topics(topic);

  Serial.println();

  if(topics.equals("reset")){
      pinMode (0, OUTPUT);
      digitalWrite (0, HIGH);
      digitalWrite (15, LOW);
      ESP.restart();
  }
  
  if(topics.equals(command1)){

     if((char)payload[0] == '0'){
       irsend.sendRaw(on_off, 135, 38);
   
       client.publish(data1.c_str(),"on",true);
     }

     if((char)payload[0] == '3' && (char)payload[1] == '4'){
       irsend.sendRaw(sixteen_three, 135, 38);
 
       client.publish(data1.c_str(),"16_3_1",true);
     }

     if((char)payload[0] == '3' && (char)payload[1] == '5'){
       irsend.sendRaw(seventeen_three, 135, 38);

       client.publish(data1.c_str(),"17_3_1",true);
     }

     if((char)payload[0] == '3' && (char)payload[1] == '6'){
       irsend.sendRaw(eightteen_three, 135, 38);
  
       client.publish(data1.c_str(),"18_3_1",true);
     }

     if((char)payload[0] == '3' && (char)payload[1] == '7'){
       irsend.sendRaw(nineteen_three, 135, 38);

       client.publish(data1.c_str(),"19_3_1",true);
     }

     if((char)payload[0] == '3' && (char)payload[1] == '8'){
       irsend.sendRaw(twenty_three, 135, 38);
    
       client.publish(data1.c_str(),"20_3_1",true);
     }

     if((char)payload[0] == '3' && (char)payload[1] == '9'){
       irsend.sendRaw(twenty_one_three, 135, 38);
   
       client.publish(data1.c_str(),"21_3_1",true);
     }

      if((char)payload[0] == '4' && (char)payload[1] == '0'){
       irsend.sendRaw(twenty_two_three, 135, 38);
    
       client.publish(data1.c_str(),"22_3_1",true);
     }

     if((char)payload[0] == '4' && (char)payload[1] == '1'){
       irsend.sendRaw(twenty_three_three, 135, 38);
 
       client.publish(data1.c_str(),"23_3_1",true);
     }

     if((char)payload[0] == '4' && (char)payload[1] == '2'){
       irsend.sendRaw(twenty_four_three, 135, 38);

       client.publish(data1.c_str(),"24_3_1",true);
     }

     if((char)payload[0] == '4' && (char)payload[1] == '3'){
       irsend.sendRaw(twenty_five_three, 135, 38);
     
    
       client.publish(data1.c_str(),"25_3_1",true);
     }

     if((char)payload[0] == '4' && (char)payload[1] == '4'){
       irsend.sendRaw(twenty_six_three, 135, 38);
   
       client.publish(data1.c_str(),"26_3_1",true);
     }

  
  }

  if(topics.equals(command2)){

     if((char)payload[0] == '0'){
       irsend1.sendRaw(on_off, 135, 38);
  
       client.publish(data2.c_str(),"on",true);
     }


     
     if((char)payload[0] == '3' && (char)payload[1] == '4'){
       irsend1.sendRaw(sixteen_three, 135, 38);
  
       client.publish(data2.c_str(),"16_3_2",true);
     }

     if((char)payload[0] == '3' && (char)payload[1] == '5'){
       irsend1.sendRaw(seventeen_three, 135, 38);
   
       client.publish(data2.c_str(),"17_3_2",true);
     }

     if((char)payload[0] == '3' && (char)payload[1] == '6'){
       irsend1.sendRaw(eightteen_three, 135, 38);
 
       client.publish(data2.c_str(),"18_3_2",true);
     }

     if((char)payload[0] == '3' && (char)payload[1] == '7'){
       irsend1.sendRaw(nineteen_three, 135, 38);
    
       client.publish(data2.c_str(),"19_3_2",true);
     }

     if((char)payload[0] == '3' && (char)payload[1] == '8'){
       irsend1.sendRaw(twenty_three, 135, 38);
  
       client.publish(data2.c_str(),"20_3_2",true
       );
     }

     if((char)payload[0] == '3' && (char)payload[1] == '9'){
       irsend1.sendRaw(twenty_one_three, 135, 38);
  
       client.publish(data2.c_str(),"21_3_2",true);
     }

      if((char)payload[0] == '4' && (char)payload[1] == '0'){
       irsend1.sendRaw(twenty_two_three, 135, 38);
   
       client.publish(data2.c_str(),"22_3_2",true);
     }

     if((char)payload[0] == '4' && (char)payload[1] == '1'){
       irsend1.sendRaw(twenty_three_three, 135, 38);
   
       client.publish(data2.c_str(),"23_3_2",true
       );
     }

     if((char)payload[0] == '4' && (char)payload[1] == '2'){
       irsend1.sendRaw(twenty_four_three, 135, 38);
 
       client.publish(data2.c_str(),"24_3_2",true);
     }

     if((char)payload[0] == '4' && (char)payload[1] == '3'){
       irsend1.sendRaw(twenty_five_three, 135, 38);
  
       client.publish(data2.c_str(),"25_3_2",true);
     }

     if((char)payload[0] == '4' && (char)payload[1] == '4'){
       irsend1.sendRaw(twenty_six_three, 135, 38);
        digitalWrite (16, HIGH);
        delay (100);
        digitalWrite (16, LOW);
        delay (100);
        digitalWrite (16, HIGH);
        delay (100);
        digitalWrite (16, LOW);
        delay (100);
       client.publish(data2.c_str(),"26_3_2",true);
     }

    
  }

 if(topics.equals(command1)){

    if((char)payload[0] == 'Y'){
     
      
        Serial.println ("Will send the command received by MQTT.");
        int curr_i = 0;
        int curr_j = 0;
        
            for (int i = 1; i < 600 ; i++){
                if ((char)payload[i] == ','){
                      A = atoi (a1);
                      J [curr_j++] = A; 
                      memset(a1, 0, sizeof a1);
                      curr_i = 0;
                      continue;
                } 
              a1 [curr_i++] = (char)payload[i];
            }
            
             if (curr_j > 0){
                    A = atoi (a1);
                    J [curr_j++] = A;
              }
    Serial.println ("The received array is =");
    Serial.print ("{");
            for (i=0; i<150; i++){
                int a = J[i];
                Serial.print(a);
                Serial.print (","); 
            }
    Serial.print ("}");
    Serial.println ("");
    irsend.sendRaw(J, 150, 38);
    pinMode (16, OUTPUT);

    client.publish(data1.c_str(), "sent",true);
    Serial.println ("Long command sent.");
    
   }
 }

 if(topics.equals(command2)){

    if((char)payload[0] == 'X'){
     
      
       
        int curr_i = 0;
        int curr_j = 0;
        
            for (int i = 1; i < 600 ; i++){
                if ((char)payload[i] == ','){
                      A = atoi (a1);
                      J [curr_j++] = A; 
                      memset(a1, 0, sizeof a1);
                      curr_i = 0;
                      continue;
                } 
              a1 [curr_i++] = (char)payload[i];
            }
            
             if (curr_j > 0){
                    A = atoi (a1);
                    J [curr_j++] = A;
              }
    Serial.println ("The received array is =");
    Serial.print ("{");
            for (i=0; i<150; i++){
                int a = J[i];
                Serial.print(a);
                Serial.print (","); 
            }
    Serial.print ("}");
    Serial.println ("");
    irsend1.sendRaw(J, 150, 38);
    pinMode (16, OUTPUT);
     
    client.publish(data2.c_str(), "sent",true);
    Serial.println ("Long command sent.");
    
   }

    if((char)payload[0] == 'r')
    {
        client.publish ("reset","Hard Reset in 30 seconds...");
        delay (40000);
    }

 }
  

}

