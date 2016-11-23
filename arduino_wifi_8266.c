#include <SoftwareSerial.h>

#define DEBUG true

SoftwareSerial esp8266(2,3); // make TX Arduino line is pin 2, make RX Arduino line is pin 3.

boolean alreadyConnected = false;

// This means that you need to connect the TX line from the esp to the Arduino's pin 2
                             // and the RX line from the esp to the Arduino's pin 3
void setup()
{

  Serial.begin(9600);
  esp8266.begin(9600); // your esp's baud rate might be different
  
  pinMode(6,OUTPUT);
  digitalWrite(6,LOW);
  
  pinMode(7,OUTPUT);
  digitalWrite(7,LOW);

  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configure as access point
  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80

}

void loop()
{

  String inString = "";  
  if(esp8266.available()) // check if the esp is sending a message 
  {
   if (!alreadyConnected) {
      // clear out the input buffer:
    esp8266.flush();
    Serial.println("We have a new client");
    esp8266.println("Hello, client!");
    alreadyConnected = true;
  }

  if(esp8266.find("+IPD,"))
  {
     delay(10); // wait for the serial buffer to fill up (read all the serial data)

    int pinNumber = 0;
    String tempS = "";
    esp8266.find("pin="); // advance cursor to "pin="
    char temp = esp8266.read();
    tempS = temp;
     
    pinNumber = tempS.toInt();
    Serial.println(pinNumber);
    digitalWrite(pinNumber, !digitalRead(pinNumber)); // toggle pin    

   }
 }
 
}


/*
* Name: sendData
* Description: Function used to send data to ESP8266.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/
String sendData(String command, const int timeout, boolean debug)
{
  String response = "";

    esp8266.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {

        // The esp has data so display its output to the serial window 
        char c = esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
  }