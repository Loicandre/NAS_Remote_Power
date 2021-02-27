# NAS_Remote_Power
Use Blynk and a ESP8266 (WEMOS D1 mini pro) to send Wake On Lan frame to wake a Synology NAS from outside.
With also a Blynk Led to show Ping NAS state

## Why this project

A friend of mine, has a synology NAS plugged into his local network. He would like the NAS to be turned off most of the time so that it doesn't waste power.
But when he's not at home, he'd like to be able to turn on the NAS when he needs it.

That's why this project has started.

With Blynk it is extremely easy to create an "application" on the smartphone. And since the application and the Blynk library communicate through an external server (hosted by Blynk), there is no need to configure the router.

However, the NAS should have a static IP address (no DHCP), to avoid that the address changes.
https://jarrodstech.net/tip-synology-nas-basics-static-ip-address/

I use the Wemos D1 mini-pro board, wich integrate the ESP8266 and the CP2104 Chip for USB-serial programming.

## 3D box

I also put the 3D files for the board in the folder https://github.com/Loicandre/NAS_Remote_Power/tree/main/3D.
This protects the circuit and gives a nice look !
 
![alt text](https://github.com/Loicandre/NAS_Remote_Power/blob/main/Blynk_Config/BlynkApp.jpeg "Blynk App")

## Arduino firmware

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

- Virtual pin V1 : When 1, Send Wake On Lan magic paquet to predifined MAC address
- Virtual pin V2 : A timer send a ping every 10sec. The result is saved in V2 (1=ping returned; 0=ping lost) 


Compiled with arduino 1.8.13 and ESP8266 package 2.7.4

## Blynk APP

Create an blynk app and paste received token in the NAS_Remote_Power.ino settings

Blynk App: 
![alt text](https://github.com/Loicandre/NAS_Remote_Power/blob/main/Blynk_Config/BlynkApp.jpeg "Blynk App")

Create Button for Virtual pin V1
![alt text](https://github.com/Loicandre/NAS_Remote_Power/blob/main/Blynk_Config/ButtonSettings.jpeg "Button Settings")

Create LED for Virtual pin V2
![alt text](https://github.com/Loicandre/NAS_Remote_Power/blob/main/Blynk_Config/LedSettings.jpeg "LED Settings")


## Install

### Arduino

- Install Arduino IDE from : https://www.arduino.cc/en/software

### Arduino ESP8266

- Go to Arduino IDE Settings, "File" menu --> "Settings"
- Enter http://arduino.esp8266.com/stable/package_esp8266com_index.json into the “Additional Boards Manager URLs” field. Then, validate with the “OK” button
- go to "Tools" menu -> "Board" -> "Boards manager" and download esp8266 package
- Select your board in the "Tools" menu -> "Board" -> "ESP8266 module" and select your board (in my case, i choosed "WEMOS D1 mini pro"

Reference : https://projetsdiy.fr/programmer-esp8266-ide-arduino-librairies-gpio-web-serveur-client/
            https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/

### Blynk

https://blynk.io/en/getting-started

- Install "Blynk" library from Arduino IDE library manager.
- Install Blynk app on your smartphone


Thanks to **https://github.com/dancol90/ESP8266Ping**
