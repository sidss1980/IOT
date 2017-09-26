#include <ESP8266WiFi.h>
#include <PubSubClient.h>


//#define SlaveDeviceId 9
//int value;
//int pinValue;
//String a;

String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

// Update these with values suitable for your network.
const char* ssidO = "sid";
const char* passwordO = "1234567890";

const char* ssid = "sid";
const char* password = "1234567890";

#define ORG "70dktj" // "quickstart" or use your organisation
    
#define DEVICE_TYPE "ESPGateway" // your device type or not used for "quickstart"

#define DEVICE_ID "ESP8266"  
#define TOKEN "0YbRq01jROlZbjM3@4" // your device token or not used for "quickstart"
//-------- Customise the above values --------

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";

char authMethod[] = "use-token-auth";

char token[] = TOKEN;

char clientId[] = "g:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

WiFiClientSecure espClient;
PubSubClient client(server, 8883, espClient);

long lastMsg = 0;
float temp = 0;
int inPin = 5;

void setup_wifi() {
  delay(10);
  //Serial.println(ssid);
  WiFi.begin(ssidO, passwordO);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    //Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void reconnect() {
  // Loop until we're reconnected
  while (!!!client.connected()) {
    //Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientId, authMethod, token)) {
      //Serial.println("connected");
    } else {
      //Serial.print("failed, rc=");
      //Serial.print(client.state());
      //Serial.println(" try again in 2 seconds");
      // Wait 5 seconds before retrying
      delay(500);
    }
  }
}

 
void setup()
{
  Serial.begin(9600);
  setup_wifi(); 
  //client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);
  delay(10);
  inputString.reserve(200); 
}


void loop()
{
  if (!!!client.connected()) {
    reconnect();
  }
  client.loop(); 
  
//       {
//          String payload = "{\"d\":{\"myName\":\"ESP8266.U\",\"counter\":1";
//          //payload += String(inputString).c_str();
//          payload += "}}";
//          client.publish("iot-2/type/MQTTDevices/id/IRSensor-1/evt/status/fmt/json", (char*) payload.c_str());
//       }    
//
//       {
//          String payload = "{\"d\":{\"myName\":\"ESP8266.IR\",\"counter\":2";
//          //payload += String(inputString).c_str();
//          payload += "}}";
//          client.publish("iot-2/type/MQTTDevices/id/UltraSonic-1/evt/status/fmt/json", (char*) payload.c_str());
//       }
//
//       {
//          String payload = "{\"d\":{\"myName\":\"ESP8266.IR\",\"counter\":3";
//          //payload += String(inputString).c_str();
//          payload += "}}";
//          client.publish("iot-2/type/MQTTDevices/id/AddDynamic/evt/status/fmt/json", (char*) payload.c_str());
//       }


  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    
    if (inChar == '\n') {
        
       if(inputString.charAt(0) == '1')//first char of string is 1... then publish to topic-A... the meesage content are next set of strings)
       {
          String payload = "{\"d\":{\"myName\":\"ESP8266.IR\",\"counter\":";
          payload += String(inputString).substring(2).c_str();
          payload += "}}";
          client.publish("iot-2/type/MQTTDevices/id/IRSensor-1/evt/status/fmt/json", (char*) payload.c_str());
       }
       else if(inputString.charAt(0) == '2')//first char of string is 1... then publish to topic-A... the meesage content are next set of strings)
       {
          String payload = "{\"d\":{\"myName\":\"ESP8266.U\",\"counter\":";
          payload += String(inputString).substring(2).c_str();
          payload += "}}";
          client.publish("iot-2/type/MQTTDevices/id/UltraSonic-1/evt/status/fmt/json", (char*) payload.c_str());
       }
       inputString = "";
    }
  }
//delay(5000);
}


void callback(char* topic, byte* payload, unsigned int length) {
  // Switch on the LED if an 1 was received as first character
  //send this back to Arduino so it can take device specific actions
  //1 means open the walve
  if ((char)payload[0] == '1') {
    Serial.println("1-C");
  } 
  //else - 0 or other, close the valve
  else {
    Serial.println("0-C");
  }
}





