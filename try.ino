#include "SoftwareSerial.h"
String ssid ="GoogleFibre"; 
String password="google100";
SoftwareSerial esp(2,3);// RX, TX 
int port=13;
String data; 
String server = "blows.comli.com" ;
String uri = "/index.html";// our example is /esppost.php
String led;
String ands="&";
String under="_";
void setup() { 
esp.begin(9600); 
Serial.begin(9600); 
pinMode(port,OUTPUT);
reset(); 
esp.println("AT+CWMODE=3\r\n");
connectWifi(); 
}
//reset the esp8266 module 
void reset() { 
esp.println("AT+RST\r\n"); 
delay(1000);
if(esp.find("OK") ) {
  //Serial.println(esp.read());
  Serial.println("Module Reset");}
} 
//connect to your wifi network
void connectWifi() { 
String cmd = "AT+CWJAP=\"" +ssid+"\",\"" + password + "\""; 
esp.println(cmd); 
delay(4000); 
if(esp.find("OK")) { 
Serial.println("Connected!"); 
} 
else { 
  Serial.println("Cannot connect to wifi");
connectWifi(); 
 } 
}

void loop () { 
data = "led=led";// data sent must be under this form //name1=value1&name2=value2.
httppost(); 
delay(1000); 
}
void httppost () { 
esp.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection. 
if( esp.find("OK")) { 
Serial.println("TCP connection ready");
} delay(1000); 
String postRequest =
"GET " + uri + " HTTP/1.0\r\n" + 
"Host: " + server + "\r\n" + 
"Accept: *" + "/" + "*\r\n" + 
"Content-Length: " + data.length() + "\r\n" + 
"Content-Type: application/x-www-form-urlencoded\r\n" + 
"\r\n" + data; 

String sendCmd = "AT+CIPSEND=";//determine the number of characters to be sent.
esp.print(sendCmd); 
esp.println(postRequest.length() ); 
delay(500); 
if(esp.find(">")) { Serial.println("Sending.."); esp.print(postRequest); 
if( esp.find("SEND OK")) { Serial.println("Packet sent"); 
while (esp.available()) { 
String tmpResp = esp.readString(); 
delay(1000);
Serial.println(tmpResp);
//Serial.println(tmpResp);
if(tmpResp.substring((tmpResp.length()-11),(tmpResp.length()-10))=="_")
{
  digitalWrite(port,HIGH);
}
else if (tmpResp.substring((tmpResp.length()-11),(tmpResp.length()-10))=="&")
{
  digitalWrite(port,LOW);
}
}
} 
// close the connection
esp.println("AT+CIPCLOSE");
} 
}
