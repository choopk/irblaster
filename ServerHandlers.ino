#include "ServerHandlers.h"

void ServerHandlers::handleClientID(){
 String mclientID;
 String mhostID;
 String mdeviceID;

   
  for (int i = 0; i < server.args(); i++) {
  
      if(i == 0){
          mclientID = server.arg(i);
          Serial.println(mclientID);
      }

      if(i == 1){
          mhostID = server.arg(i);
          Serial.println(mhostID);
      }

       if(i == 2){
          mdeviceID = server.arg(i);
          Serial.println(mdeviceID);
      }
  } 

      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.createObject();
      json["clientID"] = mclientID;
      json["hostID"] = mhostID;
      json["deviceID"] = mdeviceID;
      File historyFile =  SPIFFS.open("/clientID.json", "w");
      json.printTo(historyFile);
      historyFile.close();  
      
      char JSONmessageBuffer[300];
      json.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
   
      
      server.send(200, "application/json", JSONmessageBuffer); 
     
}

void ServerHandlers::handleReset(){
  server.send(200, "text/plain", "Resetting please wait for 10 seconds"); 
  delay(10000);
  ESP.restart();
}

void ServerHandlers::handleIP(){
      String mip1;
      String mip2;
      String mip3;
      String mip4;
      
      String mgw1;
      String mgw2;
      String mgw3;
      String mgw4;
      
      String msn1;
      String msn2;
      String msn3;
      String msn4;

  for (int i = 0; i < server.args(); i++) {
  
  
 
      if(i == 0){
          mip1 = server.arg(i);
          Serial.println(mip1);
      }

      if(i == 1){
          mip2 = server.arg(i);
          Serial.println(mip2);
      }

      if(i == 2){
          mip3 = server.arg(i);
          Serial.println(mip3);
      }

       if(i == 3){
          mip4 = server.arg(i);
          Serial.println(mip4);
      }

       if(i == 4){
          mgw1 = server.arg(i);
          Serial.println(mgw1);
      }

      if(i == 5){
          mgw2 = server.arg(i);
          Serial.println(mgw2);
      }

      if(i == 6){
          mgw3 = server.arg(i);
          Serial.println(mgw3);
      }

       if(i == 7){
          mgw4 = server.arg(i);
          Serial.println(mgw4);
      }

      
       if(i == 8){
          msn1 = server.arg(i);
          Serial.println(msn1);
      }

      if(i == 9){
          msn2 = server.arg(i);
          Serial.println(msn2);
      }

      if(i == 10){
          msn3 = server.arg(i);
          Serial.println(msn3);
      }

       if(i == 11){
          msn4 = server.arg(i);
          Serial.println(msn4);
      }

     
  } 

      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.createObject();
      json["ip1"] = mip1;
      json["ip2"] = mip2;
      json["ip3"] = mip3;
      json["ip4"] = mip4;

      json["gw1"] = mgw1;
      json["gw2"] = mgw2;
      json["gw3"] = mgw3;
      json["gw4"] = mgw4;

      json["sn1"] = msn1;
      json["sn2"] = msn2;
      json["sn3"] = msn3;
      json["sn4"] = msn4;
      File historyFile =  SPIFFS.open("/ip.json", "w");
      json.printTo(historyFile);
      historyFile.close();  
      
      char JSONmessageBuffer[300];
      json.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
   
      
      server.send(200, "application/json", JSONmessageBuffer); 
     
   
}


void ServerHandlers::handleWifi() { 
 
 String mssid;
 String mpassword;
 
 
  for (int i = 0; i < server.args(); i++) {
  

      if(i == 0){
          mssid = server.arg(i);
          Serial.println(mssid);
      }

       if(i == 1){
          mpassword = server.arg(i);
          Serial.println(mpassword);
      }
  } 

      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.createObject();
      json["ssid"] = mssid;
      json["password"] = mpassword;

      File historyFile = SPIFFS.open("/wifi.json", "w");
      json.printTo(historyFile);
      historyFile.close();  

      char JSONmessageBuffer[300];
      json.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
   
      
      server.send(200, "application/json", JSONmessageBuffer); 
   

}



void ServerHandlers::handleInterval() { 
 
 String mInterval;

 
 
  for (int i = 0; i < server.args(); i++) {
  

      if(i == 0){
          mInterval = server.arg(i);
          Serial.println(mInterval);
      }

  } 

      Interval = mInterval.toInt();   
      timer.deleteTimer(wd_timer_id);
      wd_timer_id = timer.setInterval((Interval*1000), repeatCT);

      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.createObject();
      json["Interval"] = mInterval.toInt();
      

      File historyFile = SPIFFS.open("/interval.json", "w");
      json.printTo(historyFile);
      historyFile.close();  

      char JSONmessageBuffer[300];
      json.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
   
      
      server.send(200, "application/json", JSONmessageBuffer); 
   

}



void ServerHandlers::handleVersion() { 
 

 char JSONmessageBuffer[300];
 
 
 if (SPIFFS.exists("/version.json")) {
    //file exists, reading and loading
    Serial.println("reading version file");
    File versionFile = SPIFFS.open("/version.json", "r");
    if (versionFile) {
      Serial.println("opened version file");
      size_t size =  versionFile.size();
      // Allocate a buffer to store contents of the file.
      std::unique_ptr<char[]> buf(new char[size]);

      versionFile.readBytes(buf.get(), size);
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(buf.get());
      json.printTo(Serial);

      if (json.success()) {
        Serial.println("\nparsed json");
          json.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));


      } else {
        Serial.println("failed to load json clientID");
      }
    }
  }
      
      server.send(200, "application/json", JSONmessageBuffer); 
   

}


void ServerHandlers::handleMqtt() { 
 
 String mbroker;
 String mport;
 
 
  for (int i = 0; i < server.args(); i++) {
  

      if(i == 0){
          mbroker = server.arg(i);
          Serial.println(mbroker);
      }

       if(i == 1){
          mport = server.arg(i);
          Serial.println(mport);
      }
  } 

      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.createObject();
      json["port"] = mport;
      json["broker"] = mbroker;

      File historyFile = SPIFFS.open("/Mqtt.json", "w");
      json.printTo(historyFile);
      historyFile.close();  

      char JSONmessageBuffer[300];
      json.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
   
      
      server.send(200, "application/json", JSONmessageBuffer); 
   

}
