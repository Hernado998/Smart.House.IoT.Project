# Smart.House.IoT.Project
This project was made with a NodeMCU ESP8266 WiFi module, Raspberry Pi4, Temp and Humidity sensor, Flame sensor, Photoresistor, RGB LED and a Buzzer.

This project use the gRPC protocol for RPC calls between the client (PC) and the server (Raspberry P4). It's the fastest protocol because it transfer the data with the Protocol buffers, the REST protocol (REST use JSON serialization) is 4 times slower than the gRPC.

The Mosquitto broker is installed by default in the custom Linux distro (I have specified some details in the link below).

Communications between the PC and the NodeMCU ESP8266 module are using the MQTT protocol via the MOSQUITTO broker (I am using QoS0 of the MQTT protocol).

This link will take you to the tutorial explaining how to manipulate the Yocto Project and how to build the custom Linux distro:
https://github.com/Hernado998/Linux.Qemu-Raspberrypi.distro.gRPC.Server.MQTT.Paho.CPP.Yocto.Custom.App .

You can replace the home router with a SmartPhone Hotspot.

I got the sensors in the 37 arduino sensors kit from 2bTrading.

![Architecture](arch.png)
 
