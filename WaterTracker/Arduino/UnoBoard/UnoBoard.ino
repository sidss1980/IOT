/*-----( Import needed libraries )-----*/
#include <SoftwareSerial.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define SSerialRX 8
#define SSerialTX 9
#define Pin13LED  13

/*-----( Declare objects )-----*/
SoftwareSerial mySerial(SSerialRX, SSerialTX); // RX, TX
/*-----( Declare Variables )-----*/
int byteReceived;
int byteSend;
String in,out;
unsigned long lastSend1,lastSend2,lastSend3;

/*************************** Pressure Sensor ***********************/
byte sensorFlowPin1       = 2;
byte sensorFlowPin2       = 3;

// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 4.5;

volatile byte pulseCount1; 
volatile byte pulseCount2;  

float flowRate1;
unsigned int flowMilliLitres1;
unsigned long totalMilliLitres1;

float flowRate2;
unsigned int flowMilliLitres2;
unsigned long totalMilliLitres2;

unsigned long oldTime1;
unsigned long oldTime2;

void setup()   /****** SETUP: RUNS ONCE ******/
{
  // Start the built-in serial port, probably to Serial Monitor
  Serial.begin(9600);
  Serial.println("WATER CONTROLLER EXAMPLE - USING SERIAL AND ESP8266");  
  pinMode(Pin13LED, OUTPUT);    

  // Start the software serial port, to another device
  mySerial.begin(9600);   // set the data rate 
  lastSend1 = 0;
  lastSend2 = 0;
  lastSend3 = 0;


  // Set up the status LED line as an output  
  digitalWrite(13, HIGH);  // We have an active-low LED attached
  
  pinMode(sensorFlowPin1, INPUT);
  pinMode(sensorFlowPin2, INPUT);
  digitalWrite(sensorFlowPin1, HIGH);
  digitalWrite(sensorFlowPin2, HIGH);

  pulseCount1        = 0;
  pulseCount2        = 0;
  
  flowRate1          = 0.0;
  flowMilliLitres1   = 0;
  
  flowRate2          = 0.0;
  flowMilliLitres2   = 0;  
  
  totalMilliLitres2 = 0;
  
  oldTime1           = 0;
  oldTime2           = 0;

  // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(digitalPinToInterrupt(sensorFlowPin1), pulseCounter1, FALLING);
  attachInterrupt(digitalPinToInterrupt(sensorFlowPin2), pulseCounter2, FALLING);

}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  
//  if ( millis() - lastSend3 > 6000 ) { // Update and send only after 5 seconds
//    mySerial.print("2TTSensorDat");
//    lastSend3 = millis();
//  }
  

  if (Serial.available())
  {
    in = Serial.readString();
    mySerial.print(in);  // Send byte to Remote Arduino
    
    delay(25);
  }
  
  if (mySerial.available())  //Look for data from other Arduino
   {
    out = mySerial.readString();    // Read received byte
    //First character from the Serial is 1 then it is command to take action on devices connected
    if(out.charAt(0)=='1')
    {
      if(out.charAt(1)=='1')//BLUE
      {
        digitalWrite(12, HIGH);
        digitalWrite(Pin13LED, HIGH);
      }
      if(out.charAt(1)=='2')//YLW
      {
        digitalWrite(12, HIGH);
      }
      if(out.charAt(1)=='3')//RED
      {
        digitalWrite(11, HIGH);
      }
      Serial.println("GOT COMMAND FROM ESP8266");
      Serial.println(out);        // Show on Serial Monitor
      digitalWrite(Pin13LED, HIGH);  // Show activity    
    }
    else
    {
      if(out.charAt(1)=='1')//BLUE
      {
        digitalWrite(Pin13LED, LOW);
      }
      if(out.charAt(1)=='2')//YLW
      {
        digitalWrite(12, LOW);
      }
      if(out.charAt(1)=='3')//RED
      {
        digitalWrite(11, LOW);
      }      
      Serial.println("Got print statements from ESP8266");
      Serial.println(out);        // Show on Serial Monitor      
      digitalWrite(Pin13LED, LOW);  // Show activity    
    }
    delay(10);
   }  



   if((millis() - oldTime1) > 1000)    // Only process counters once per second
  { 
    // Disable the interrupt while calculating flow rate and sending the value to
    // the host
    detachInterrupt(digitalPinToInterrupt(sensorFlowPin1));
        
    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate1 = ((1000.0 / (millis() - oldTime1)) * pulseCount1) / calibrationFactor;
    
    // Note the time this processing pass was executed. Note that because we've
    // disabled interrupts the millis() function won't actually be incrementing right
    // at this point, but it will still return the value it was set to just before
    // interrupts went away.
    oldTime1 = millis();
    
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres1 = (flowRate1 / 60) * 1000;
    
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres1 += flowMilliLitres1;
      
    unsigned int frac;



    // Print the flow rate for this second in litres / minute
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate1));  // Print the integer part of the variable
    Serial.print(".");             // Print the decimal point
    // Determine the fractional part. The 10 multiplier gives us 1 decimal place.
    frac = (flowRate1 - int(flowRate1)) * 10;
    Serial.print(frac, DEC) ;      // Print the fractional part of the variable
    Serial.print("L/min");
    // Print the number of litres flowed in this second
    Serial.print("  Current Liquid Flowing: ");             // Output separator
    Serial.print(flowMilliLitres1);
    Serial.print("mL/Sec");

    // Print the cumulative total of litres flowed since starting
    Serial.print("  Output Liquid Quantity: ");             // Output separator
    Serial.print(totalMilliLitres1);
    Serial.println("mL"); 


//    if ( millis() - lastSend1 > 1000 ) { // Update and send only after 1 seconds
      mySerial.print("1-"+String(flowRate1,1)+"-"+String(totalMilliLitres1)+"\n");
//      //mySerial.flush();
//      lastSend1 = millis();
//    }
    
    
    // Reset the pulse counter so we can start incrementing again
    pulseCount1 = 0;
    
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(digitalPinToInterrupt(sensorFlowPin1), pulseCounter1, FALLING);
  }


     if((millis() - oldTime2) > 1000)    // Only process counters once per second
  { 
    // Disable the interrupt while calculating flow rate and sending the value to
    // the host
    detachInterrupt(digitalPinToInterrupt(sensorFlowPin2));
        
    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate2 = ((1000.0 / (millis() - oldTime2)) * pulseCount2) / calibrationFactor;
    
    // Note the time this processing pass was executed. Note that because we've
    // disabled interrupts the millis() function won't actually be incrementing right
    // at this point, but it will still return the value it was set to just before
    // interrupts went away.
    oldTime2 = millis();
    
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres2 = (flowRate2 / 60) * 1000;
    
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres2 += flowMilliLitres2;
      
    unsigned int frac;
    
    // Print the flow rate for this second in litres / minute
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate2));  // Print the integer part of the variable
    Serial.print(".");             // Print the decimal point
    // Determine the fractional part. The 10 multiplier gives us 1 decimal place.
    frac = (flowRate2 - int(flowRate2)) * 10;
    Serial.print(frac, DEC) ;      // Print the fractional part of the variable
    Serial.print("L/min");
    // Print the number of litres flowed in this second
    Serial.print("  Current Liquid Flowing: ");             // Output separator
    Serial.print(flowMilliLitres2);
    Serial.print("mL/Sec");

    // Print the cumulative total of litres flowed since starting
    Serial.print("  Output Liquid Quantity: ");             // Output separator
    Serial.print(totalMilliLitres2);
    Serial.println("mL"); 



//    if ( millis() - lastSend2 > 1000 ) { // Update and send only after 1 seconds
      mySerial.print("2-"+String(flowRate2,1)+"-"+String(totalMilliLitres2)+"\n");
//      mySerial.flush();
//      lastSend2 = millis();
//    }
    
    // Reset the pulse counter so we can start incrementing again
    pulseCount2 = 0;
    
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(digitalPinToInterrupt(sensorFlowPin2), pulseCounter2, FALLING);
  }

  delay(2000);
}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/

/*
Insterrupt Service Routine
 */
void pulseCounter1()
{
  // Increment the pulse counter
  pulseCount1++;
}

/*
Insterrupt Service Routine
 */
void pulseCounter2()
{
  // Increment the pulse counter
  pulseCount2++;
}

//NONE
//*********( THE END )***********


