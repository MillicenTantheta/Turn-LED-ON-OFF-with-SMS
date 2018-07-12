/*
 * Millicent Tantheta with Arduino
 */

// Include Software Serial library to communicate with GSM
#include <SoftwareSerial.h>

// Configure software serial port
SoftwareSerial GPSmodule(2, 3);//RX, TX

// Variable to store text message
String textMessage;

// Create a variable to store Lamp state
String lampState = "HIGH";

// Relay connected to pin 12
const int relay = 12;

void setup() {
  // Automatically turn on the GPSmoduleeld
//  digitalWrite(9, HIGH);
//  delay(1000);
//  digitalWrite(9, LOW);
//  delay(5000);
  
  // Set relay as OUTPUT
  pinMode(relay, OUTPUT);

  // By default the relay is off
  digitalWrite(relay, LOW);
  
  // Initializing serial commmunication
  Serial.begin(9600); 
  GPSmodule.begin(9600);

  // Give time to your GSM GPSmoduleeld log on to network
  delay(2000);
  Serial.print("GPSmodule ready...");

  // AT command to set GPSmodule to SMS mode
  GPSmodule.print("AT+CMGF=1\r"); 
  delay(100);
  // Set module to send SMS data to serial out upon receipt 
  GPSmodule.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
}

void loop(){
   if (Serial.available()>0)
 GPSmodule.write(Serial.read());
 if (GPSmodule.available()>0)
 Serial.write(GPSmodule.read());
  if(GPSmodule.available()>0){
    textMessage = GPSmodule.readString();
    Serial.print(textMessage);    
    delay(10);

  } 
  if(textMessage.indexOf("ON")>=0){
    // Turn on relay and save current state
    digitalWrite(relay, HIGH);
    lampState = "on";
    Serial.println("Relay set to ON");  
    textMessage = "";   
  }
  if(textMessage.indexOf("OFF")>=0){
    // Turn off relay and save current state
    digitalWrite(relay, LOW);
    lampState = "off"; 
    Serial.println("Relay set to OFF");
    textMessage = ""; 
  }
  if(textMessage.indexOf("STATE")>=0){
    String message = "Lamp is " + lampState;
    sendSMS(message);
    Serial.println("Lamp state resquest");
    textMessage = "";
  }
}  

// Function that sends SMS
void sendSMS(String message){
  // AT command to set GPSmodule to SMS mode
  GPSmodule.print("AT+CMGF=1\r"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  GPSmodule.println("AT + CMGS = \"XXXXXXXXXX\""); 
  delay(100);
  // Send the SMS
  GPSmodule.println(message); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  GPSmodule.println((char)26); 
  delay(100);
  GPSmodule.println();
  // Give module time to send SMS
  delay(5000);  
}
