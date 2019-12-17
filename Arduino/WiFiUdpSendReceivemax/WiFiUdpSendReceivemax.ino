
/*
  WiFi UDP Send and Receive String

  This sketch wait an UDP packet on localPort using the WiFi module.
  When a packet is received an Acknowledge packet is sent to the client on port remotePort

  created 30 December 2012
  by dlf (Metodo2 srl)

*/


#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <Arduino_LSM6DS3.h>

//IP 10.7.21.191

int status = WL_IDLE_STATUS;
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "hdpk-student";        // your network SSID (name)
char pass[] = "drUse300";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int localPort = 2390;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

WiFiUDP Udp;

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


    Serial.print(pitch);
    Serial.print('\t');
    Serial.println(roll);

  }

  int rand_num = 128; //random(10, 100);
  char cstr[32];
  //ftoa(rand_num, cstr, 10);
  //char src[] = "Pitch ";
  //char dest[32];
  //strcat(src, cstr); //src = "Pitch 128";

  char float_string[30];
  //dtostrf(floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, charBuf);
  dtostrf(pitch, 2, 2, float_string );

  char keyword[] = "Pitch ";
  strcat(keyword, float_string);
  

  Serial.println(keyword);



  //strcpy(dest,cstr);

  // send a reply, to the IP address and port that sent us the packet we received
  Udp.beginPacket("10.7.21.191", 10001);
  Udp.write(keyword);
  Udp.endPacket();

  /*
    // if there's data available, read a packet
    int packetSize = Udp.parsePacket();
    if (packetSize) {
      Serial.print("Received packet of size ");
      Serial.println(packetSize);
      Serial.print("From ");
      IPAddress remoteIp = Udp.remoteIP();
      Serial.print(remoteIp);
      Serial.print(", port ");
      Serial.println(Udp.remotePort());

      // read the packet into packetBufffer
      int len = Udp.read(packetBuffer, 255);
      if (len > 0) {
        packetBuffer[len] = 0;
      }
      Serial.println("Contents:");
      Serial.println(packetBuffer);


    }
  */
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
