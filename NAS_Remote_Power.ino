// ********************************************************************************
// ESP8266 with BLYNK
// NAS wake on lan with NAS ping to know if alive
//
// Run on wemos D1 mini pro
// Compiled with arduino 1.8.13 and ESP8266 package 2.7.4
// Need https://arduino.esp8266.com/stable/package_esp8266com_index.json added to Arduino preference and ESP8266 installed in board manager
// ********************************************************************************
// https://github.com/Loicandre
// Copyright Loic Andre
// ********************************************************************************

#include <BlynkSimpleEsp8266.h>
#include <WiFiUdp.h>
#include "ESP8266Ping.h"
#define BLYNK_PRINT Serial


// Change Settings here :
// ********************************************************************************

// Blynk App settings
char auth[] = "Blynk token here ";   // blynk API token
// Wifi App settings
char ssid[] = "Wifi Name here";			 // Wifi Name	
char pass[] = "Wifi Pass here";			 // Wifi WPA Password
// Your NAS MAC adress
byte macAddr[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06}; // put the NAS MAC adress here for wake on lan
// Your NAS ip adress
IPAddress NASAddr(192, 168, 0, 50);					// put the NAS IP adress to know if NAS is running
// LAN Broadcast address
IPAddress bcastAddr(192, 168, 0, 255);		// put your LAN broadcast adress (usually router ip with 255 for the last digit (mask=255.255.255.0)

// ********************************************************************************



// Some Definition and declaration
// ********************************************************************************

WiFiUDP udp;
SimpleTimer timer;

#define MAGIC_PACKET_LENGTH 102
#define PORT_WAKEONLAN 9
byte magicPacket[MAGIC_PACKET_LENGTH];
//unsigned int localPort = 9;

// Virtual pin number to wake pc
#define APP_BUTTON_WAKEONLAN  V1
#define APP_LED_NAS_ON        V2

WidgetLED led_nas(APP_LED_NAS_ON);



// Setup all things !
// ********************************************************************************
void setup()
{
  // Init Blue LED on the wemos mini pro
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  
  Serial.begin(9600);  // initialise UART for logging
  
  // start BLYNK API
  Blynk.begin(auth, ssid, pass);
  if (udp.begin(PORT_WAKEONLAN) == 1) {
    BLYNK_LOG("udp begin OK");
    buildMagicPacket();
  }
  doConnect();
  
  // show RSSI info on log
  long rssi = WiFi.RSSI();
  Serial.print("RSSI:");
  Serial.println(rssi);

  // start timer for Ping task
  timer.setInterval(100L, pingIP); //(100ms) do pingIP 10 time/seconde
  
}

void loop()
{
  Blynk.run();
  timer.run();
}

// Application
// ********************************************************************************


void pingIP(void) // go here 10 time/seconde 
{  
  static uint8_t counter =0 ;

  if(counter >= 100) // do inside each 10 sec, change Ping test time here
  {
    if(Ping.ping(NASAddr)) {
      led_nas.on();
      BLYNK_LOG("NAS IP responded");
    } else {
      led_nas.off();
      BLYNK_LOG("NAS IP ping timeout");
    }
    counter =0; // reset counter
  }
  else
  {
    counter ++;
  }
}

// Generate wake on lan magic packet
void buildMagicPacket()
{
  memset(magicPacket, 0xFF, 6);
  for (int i = 0; i < 16; i++) {
    int ofs = i * sizeof(macAddr) + 6;
    memcpy(&magicPacket[ofs], macAddr, sizeof(macAddr));
  }
}

// BOOT PC button handler of application
BLYNK_WRITE(APP_BUTTON_WAKEONLAN)
{
  if(param.asInt()==1)
  {
    digitalWrite(2, HIGH); // Set led off when sending WOL
	 
    BLYNK_LOG("AppButtonWakeOnLan: value=%d", param.asInt());
	
	//send WOL request
    udp.beginPacket(bcastAddr, PORT_WAKEONLAN);
    udp.write(magicPacket, MAGIC_PACKET_LENGTH);
    udp.endPacket();
	
    delay(200);		// wait some time to see the LED toggling
    digitalWrite(2, LOW);   // Set led ON after sending WOL
  }
}


// BLYNK API specifics 
// ********************************************************************************

// Blynk Connect to server
void doConnect()
{
  BLYNK_LOG("doConnect start");
  bool sts = false;
  Blynk.disconnect();
  sts = Blynk.connect(10000);
  BLYNK_LOG("Blynk.connect=%d", sts);
  if (!sts) {
    BLYNK_LOG("connect timeout. restarting..");
    system_restart(); // ESP8266 API
  }
}
