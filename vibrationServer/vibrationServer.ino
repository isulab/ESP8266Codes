#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include "vibration.hpp"
#define WIFI_SSID "7308"
#define WIFI_PWD "mokemoke"

ESP8266WebServer server(80);
Ticker loop_ticker;
Vibration v0(5),v1(12);
// HTML
#define HTML_HEADER "<!doctype html>"\
  "<html><head><meta charset=\"UTF-8\"/>"\
  "<meta name=\"viewport\" content=\"width=device-width\"/>"\
  "</head><body>"
#define HTML_FOOTER "</body></html>"
void tiemr_interrupt(){
  //Serial.println("int");
  v0.update();
  v1.update();
}
void setup() {
  pinMode(5,OUTPUT);
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PWD);

  // Wait until WiFi is connected
  Serial.println("");
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Setup WebServer Handlers
  server.on("/", [](){
    String html = HTML_HEADER "<h1>I am vibrations server.</h1>" HTML_FOOTER;
    server.send(200, "text/html", html);
  });

  server.on("/vibration/0/on", [](){
    float duty = 0.5;
    float pwm_duty = 0.8;
    int interval = 100;
    String html = HTML_HEADER "<h1>Vibration 0 ON</h1>" HTML_FOOTER;
    server.send(200, "text/html", html);
    for(int i = 0; i < server.args(); i ++){
      //Serial.println("Arg" + (String)i + "->" + server.argName(i) + ":" + server.arg(i));
      if(server.argName(i).compareTo("duty") == 0){
        duty = server.arg(i).toFloat();
      }
      if(server.argName(i).compareTo("strength") == 0){
        pwm_duty = server.arg(i).toFloat();
      }
      if(server.argName(i).compareTo("interval") == 0){
        interval = server.arg(i).toInt();
      }
    }
    v0.setting(pwm_duty,interval,duty);
    v0.on();
    loop_ticker.attach_ms(10,tiemr_interrupt);
  });
  server.on("/vibration/1/on", [](){
    float duty = 0.5;
    float pwm_duty = 0.8;
    int interval = 100;
    String html = HTML_HEADER "<h1>Vibration 1 ON</h1>" HTML_FOOTER;
    server.send(200, "text/html", html);
    for(int i = 0; i < server.args(); i ++){
      //Serial.println("Arg" + (String)i + "->" + server.argName(i) + ":" + server.arg(i));
      if(server.argName(i).compareTo("duty") == 0){
        duty = server.arg(i).toFloat();
      }
      if(server.argName(i).compareTo("strength") == 0){
        pwm_duty = server.arg(i).toFloat();
      }
      if(server.argName(i).compareTo("interval") == 0){
        interval = server.arg(i).toInt();
      }
    }
    v1.setting(pwm_duty,interval,duty);
    v1.on();
    loop_ticker.attach_ms(10,tiemr_interrupt);
  });
  server.on("/vibration/0/off", [](){
    String html = HTML_HEADER "<h1>Vibration 0 OFF</h1>" HTML_FOOTER;
    server.send(200, "text/html", html);
    v0.off();
    if(v0.state == OFF && v1.state == OFF ){
      loop_ticker.detach();
    }
  });
  server.on("/vibration/1/off", [](){
    String html = HTML_HEADER "<h1>Vibration 1 OFF</h1>" HTML_FOOTER;
    server.send(200, "text/html", html);
    v1.off();
    if(v0.state == OFF && v1.state == OFF ){
      loop_ticker.detach();
    }
  });
  server.begin();
}

void loop() {
  server.handleClient();
  //Serial.println("hi");
}
