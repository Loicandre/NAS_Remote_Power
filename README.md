# NAS_Remote_Power
Use Blynk and a ESP8266 (WEMOS D1 mini pro) to send Wake On Lan frame to wake a Synology NAS from outside.
With also a Blynk Led to show Ping NAS state

##Arduino APP

Please Add your settings in NAS_Remote_Power.ino

```cpp
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
```

Virtual pin V1 : When 1, Send Wake On Lan magic paquet to predifined MAC address
Virtual pin V2 : A timer send a ping every 10sec. The result is saved in V2 (1=ping returned; 0=ping lost) 


Compiled with arduino 1.8.13 and ESP8266 package 2.7.4

##Blynk APP

Create an blynk app and paste received token in the NAS_Remote_Power.ino settings

Blynk App: 
![alt text](https://github.com/Loicandre/NAS_Remote_Power/Blynk_Config/BlynkApp.jpeg "Blynk App")

Create Button for Virtual pin V1
![alt text](https://github.com/Loicandre/NAS_Remote_Power/Blynk_Config/ButtonSettings.jpeg "Button Settings")

Create LED for Virtual pin V2
![alt text](https://github.com/Loicandre/NAS_Remote_Power/Blynk_Config/LedSettings.jpeg "LED Settings")




Thanks to **https://github.com/dancol90/ESP8266Ping**
