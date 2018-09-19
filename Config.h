#ifndef CONFIG_INCLUDED
#define CONFIG_INCLUDED

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Config{
	
public:
	   void loadClientID();
	   void loadIp();
	   void loadWifi();
     void loadInterval();
	   void loadMqtt();
	
};


#endif
