/*
 WiFi Web Server Servo controller 

 A simple web server that lets you do simple control a stepper motor. 
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off 

 This example is written for a network using WPA2 encryption. For insecure
 WEP or WPA, change the Wifi.begin() call and use Wifi.setMinSecurity() accordingly. 

 Circuit:
 * WiFi shield attached

 based of blink sketch by Tom Igoe, 25 Nov 2012
 
 Made by NibaliDev Engineering, Karlskoga, Örebro Län, 
 Monarchy of Sweden, European Union. 
 
 */

#include <WiFi.h> 

const char* ssid     = "Hemma"; 
const char* password = "3r1kC@r1"; 

#define stepPin 12 
#define dirPin 14 
#define relayPin 27 

WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    pinMode(stepPin, OUTPUT); 
    pinMode(dirPin, OUTPUT); 
    pinMode(relayPin, OUTPUT); 

    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

} 




void stepper (bool direction) { 
  
  digitalWrite(dirPin, direction); 

  Serial.println("Started"); 
  
  for (int i = 0; i < ((200*10)+10); i++) { // steps/ degrees = 200 / 360 => steps/ degrees * degrees = steps. 
    digitalWrite(stepPin, HIGH); 
    delayMicroseconds(1000);    // Only changing rotation speed 
    
    digitalWrite(stepPin, LOW); 
    delayMicroseconds(1000); 

    if (i > 10) {
      digitalWrite(relayPin, HIGH); 
    } 
  } 

  digitalWrite(relayPin, LOW); 
} 


void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<a href=\"/H\" style=\"display: inline-block; padding: 10px 20px; text-decoration: none; background-color: #1bc0f2; color: white; border-radius: 4px; margin: 10px; \" >Down</a><br>"); 
            client.print("<a href=\"/L\" style=\"display: inline-block; padding: 10px 20px; text-decoration: none; background-color: #1bc0f2; color: white; border-radius: 4px; margin: 10px; \" >Up</a><br>"); 

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          stepper(LOW); 
        } 
        if (currentLine.endsWith("GET /L")) {
          stepper(HIGH); 
        } 
      } 
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
} 
