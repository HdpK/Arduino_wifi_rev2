#include <ArduinoOSC.h>

// WiFi stuff
const char* ssid = "hdpk-student";
const char* pwd = "drUse300";
const IPAddress ip(10, 2, 21, 191);
const IPAddress gateway(10, 7, 0, 1);
const IPAddress subnet(255, 255, 0, 0);

// for ArduinoOSC
OscWriter osc;
//const char* host = "10.2.21.192";
const int recv_port = 10000;
const int send_port = 12000;

void setup()
{
    Serial.begin(115200);

    // WiFi stuff
    WiFi.begin(ssid, pwd);
    WiFi.config(ip, gateway, subnet);
    while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(500); }
    Serial.print("WiFi connected, IP = "); Serial.println(WiFi.localIP());

    // ArduinoOSC
    //osc.begin(recv_port);

    // TODO: TBD
    // osc.subscribe("/int32", i);
    // osc.subscribe("/float", f);
    // osc.subscribe("/string", s);
    // osc.subscribe("/blob", b);


    // TODO: TBD
    // osc.publish(host, send_port, "/value", value);
    // osc.publish(host, send_port, "/millis", &millis);
}

void loop()
{
    osc.parse(); // should be called
    osc.send(host, send_port, "/send", 1, 2.2F, 3.3, "string"); // send osc packet in one line
}
