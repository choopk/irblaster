#ifndef SERVER_HANDLER_INCLUDED
#define SERVER_HANDLER_INCLUDED

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class ServerHandlers{
	
public:
	  static void handleWifi();
	  static void handleIP();
	  static void handleReset();
	  static void handleClientID();
    static void handleInterval();
	  static void handleVersion();
    static void handleMqtt();
	
};




#endif
