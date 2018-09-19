#include "Config.h"

//Loading client file
void Config::loadClientID(){
	 if (SPIFFS.exists("/clientID.json")) {
    //file exists, reading and loading
    Serial.println("reading clientID file");
    File clientIDFile = SPIFFS.open("/clientID.json", "r");
    if (clientIDFile) {
      Serial.println("opened clientID file");
      size_t size =  clientIDFile.size();
      // Allocate a buffer to store contents of the file.
      std::unique_ptr<char[]> buf(new char[size]);

      clientIDFile.readBytes(buf.get(), size);
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(buf.get());
      json.printTo(Serial);

      if (json.success()) {
        Serial.println("\nparsed json");
        json.printTo(Serial);
        
          Serial.println("setting custom clientID");
         
          
          strcpy(clientID, json["clientID"]);
          strcpy(newHostName, json["hostID"]);
          strcpy(device_id, json["deviceID"]);


      } else {
        Serial.println("failed to load json clientID");
      }
    }
  }
}

//Loading network settings
void Config::loadIp() {
  if (SPIFFS.exists("/ip.json")) {
    //file exists, reading and loading
    Serial.println("reading ip file");
    File ipFile = SPIFFS.open("/ip.json", "r");
    if (ipFile) {
      Serial.println("opened ip file");
      size_t size =  ipFile.size();
      // Allocate a buffer to store contents of the file.
      std::unique_ptr<char[]> buf(new char[size]);

      ipFile.readBytes(buf.get(), size);
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(buf.get());
      json.printTo(Serial);

      if (json.success()) {
        Serial.println("\nparsed json");
        json.printTo(Serial);
        
          Serial.println("setting custom network settings");
         
          strcpy(ip1,json["ip1"]);
          strcpy(ip2,json["ip2"]);
          strcpy(ip3,json["ip3"]);
          strcpy(ip4,json["ip4"]);

          strcpy(gw1,json["gw1"]);
          strcpy(gw2,json["gw2"]);
          strcpy(gw3,json["gw3"]);
          strcpy(gw4,json["gw4"]);

          strcpy(sn1,json["sn1"]);
          strcpy(sn2,json["sn2"]);
          strcpy(sn3,json["sn3"]);
          strcpy(sn4,json["sn4"]);

          
      } else {
        Serial.println("failed to load json ip");
      }
    }
  }
}

//Loading Wifi settings
void Config::loadWifi() {
  if (SPIFFS.exists("/wifi.json")) {
    //file exists, reading and loading
    Serial.println("reading config file");
    File configFile = SPIFFS.open("/wifi.json", "r");
    if (configFile) {
      Serial.println("opened wifi file");
      size_t size = configFile.size();
      // Allocate a buffer to store contents of the file.
      std::unique_ptr<char[]> buf(new char[size]);

      configFile.readBytes(buf.get(), size);
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(buf.get());
      json.printTo(Serial);

      if (json.success()) {
        Serial.println("\nparsed json");
        json.printTo(Serial);

          Serial.println("setting custom wifi settings");
          strcpy(ssid, json["ssid"]);
          strcpy(password, json["password"]);
          
      } else {
        Serial.println("failed to load json wifi");
      }
    } wd_timer_id = timer.setInterval((Interval*1000), repeatCT);  
  }
}

//Load Logging interval
void Config::loadInterval() {
  if (SPIFFS.exists("/interval.json")) {
    //file exists, reading and loading
    Serial.println("reading interval file");
    File intervalFile = SPIFFS.open("/interval.json", "r");
    if (intervalFile) {
      Serial.println("opened interval file");
      size_t size = intervalFile.size();
      // Allocate a buffer to store contents of the file.
      std::unique_ptr<char[]> buf(new char[size]);

      intervalFile.readBytes(buf.get(), size);
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(buf.get());
      json.printTo(Serial);

      if (json.success()) {
        Serial.println("\nparsed json");
        json.printTo(Serial);

          Serial.println("setting interval settings");
          Interval = json["Interval"];
        
          
      } else {
        Serial.println("failed to load json wifi");
      }
    }
  }
}


void Config::loadMqtt() {
  if (SPIFFS.exists("/Mqtt.json")) {
    //file exists, reading and loading
    Serial.println("reading mqtt file");
    File mqttFile = SPIFFS.open("/Mqtt.json", "r");
    if (mqttFile) {
      Serial.println("opened mqtt file");
      size_t size = mqttFile.size();
      // Allocate a buffer to store contents of the file.
      std::unique_ptr<char[]> buf(new char[size]);

      mqttFile.readBytes(buf.get(), size);
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(buf.get());
      json.printTo(Serial);

      if (json.success()) {
        Serial.println("\nparsed json");
        json.printTo(Serial);

          Serial.println("setting mqtt settings");
         
          strcpy(port,json["port"]);
          strcpy(broker, json["broker"]);
          
      } else {
        Serial.println("failed to load json mqtt");
      }
    }
  }
}
