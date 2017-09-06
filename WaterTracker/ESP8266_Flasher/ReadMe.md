1. Flush the ESP8266:
   Connect ESP-RX to UNO-RX
   Connect ESP-TX to UNO-TX
   Connect GPIO0 to GRND (puts in flash mode)
   Take 5v from UNO to breadboard
   Put two diodes in parallel to above 5v in breadboard (starting with black end and then silver ends of the diodes).
   Connect the second end of the diode to the + rail of breadboard.
   Take graound from UNO and put it on the - rail of breadboard.
   Next to these + and - cross a 100 microF capacitor with long tail in + rail and short in - rail.
   Next to capacitor put the both volt lines of ESP in + rail and ground of ESP to - rail of breadboard
   Open the EXE esp8266_flasher
   Select first the v1.3.0.2 AT Firmware.exe and start flush
   Then select the AiThinker_ESP8266_DIO_8M_8M_20160615_V1.5.4.bin and flush the ESP
   Remember to join Ground and RESET of UNO board
   
   After flush remove GROUND + RESET join on UNO and remove the GPIO0 of ESP from Ground
   From IDE upload plain sketch setup / reset
   Open CMD on baud 115200
   Issue CMD to change baud to 95600 using: AT+UART_DEF=9600,8,1,0,0
   
   After flash connect the TX to 6 and RX to 7 of UNO
   Download: WiFiEsp.h from GITHUB and try the example sketch


Configuring to access over router to localIP
http://www.instructables.com/id/Accessing-Arduino-over-internet/

https://medium.com/@lelylan/how-to-build-an-high-availability-mqtt-cluster-for-the-internet-of-things-8011a06bd000

https://blog.risingstack.com/getting-started-with-nodejs-and-mqtt/