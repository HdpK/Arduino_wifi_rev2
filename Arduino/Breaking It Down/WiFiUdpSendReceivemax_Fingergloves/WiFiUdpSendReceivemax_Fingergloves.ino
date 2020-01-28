
/*
  WiFi UDP Send and Receive String

  This sketch wait an UDP packet on localPort using the WiFi module.
  When a packet is received an Acknowledge packet is sent to the client on port remotePort

  created 30 December 2012
  by dlf (Metodo2 srl)

*/

const char RECV_IP_ADDRESS[] = "10.7.2.75";
const int PORT = 10001;
char ssid[] = "hdpk-student";        // your network SSID (name) hdpk-student        / Soundsgood01
char pass[] = "drUse300";    // your network password (use for WPA, or use as key for WEP) drUse300

#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <Arduino_LSM6DS3.h>

//IP 10.7.21.191

int status = WL_IDLE_STATUS;
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int localPort = 2390;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

WiFiUDP Udp;

const int pingPin = 7;


void setup() {
  //Initialize serial and wait for port to open:
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
}



void loop() {

  float accelX, accelY, accelZ, pitch, roll;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(accelX, accelY, accelZ);

    pitch = 180 * atan2(accelX, sqrt(accelY * accelY + accelZ * accelZ)) / PI;
    roll = 180 * atan2(accelY, sqrt(accelX * accelX + accelZ * accelZ)) / PI;

  }
// 3x Pot, 2x Button,1x Switch Button, 1x Ultrasonic, 1x Flex
  int pot00_value = analogRead(A0);
  int pot01_value = analogRead(A1);
  int pot02_value = analogRead(A2);
  int button00_value = digitalRead(9);
  int button01_value = digitalRead(8);
  int flexpin00_value = analogRead(A5);
  int switch00_value = digitalRead(6);
  
  //int ultra00_value = analogRead(7);

  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

// The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
    cm = microsecondsToCentimeters(duration);

  


  sendToMax("Pot00 ", pot00_value);
  sendToMax("Pot01 ", pot01_value);
  sendToMax("Pot02 ", pot02_value);
  sendToMax("Button00 ", button00_value);
  sendToMax("Button01 ", button01_value);
  sendToMax("flexpin00 ", flexpin00_value);
  sendToMax("switch00 ", switch00_value);
  
  sendToMax("ultra ", cm);
  


  sendToMax("pitch ",pitch);
  sendToMax("roll ",roll);


}


long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}












void sendToMax(char* identifier, float value){
  
   char float_string[30];
  //dtostrf(floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, charBuf);
  dtostrf(value, 2, 2, float_string );
  
  char data[30];
  strcpy(data,identifier);
  strcat(data, float_string);

  // send a reply, to the IP address and port that sent us the packet we received
  Udp.beginPacket(RECV_IP_ADDRESS, PORT);
  Udp.write(data);
  Udp.endPacket();
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
