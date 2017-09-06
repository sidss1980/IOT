#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define SlaveDeviceId 9
int value;
int pinValue;
String a;

String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

// Update these with values suitable for your network.
//const char* ssid = "Estuate_Accounts1";
//const char* password = "F1r3w@ll";
const char* ssid = "Soni";
const char* password = "chanusingh12";

const char* mqtt_server = "192.168.1.3";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
float temp = 0;
int inPin = 5;

void setup_wifi() {
  delay(10);
  ///Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    ///Serial.print(".");
  }
  ///Serial.println("WiFi connected");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    ///Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient_temperature_sensor")) {
      ///Serial.println("connected");
    } else {
      ///Serial.print("failed, rc=");
      ///Serial.print(client.state());
      ///Serial.println(" try again in 2 seconds");
      // Wait 5 seconds before retrying
      delay(2000);
    }
  }
}
 
void setup()
{
  Serial.begin(9600);
  setup_wifi(); 
  client.setServer(mqtt_server, 1883);
  //pinMode(inPin, INPUT);
  client.setCallback(callback);
  delay(10);
  inputString.reserve(200); 
}


void loop()
{
  if (!client.connected()) {
    reconnect();
    client.subscribe("v1/devices/me/command");
  }
  client.loop();

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
          client.publish("v1/devices/flow/1", String(inputString).c_str());
       }
       else if(inputString.charAt(0) == '2')//first char of string is 1... then publish to topic-A... the meesage content are next set of strings)
       {
          client.publish("v1/devices/flow/2", String(inputString).c_str());
       }
       inputString = "";
    }
  }
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





