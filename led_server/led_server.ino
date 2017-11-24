//#define DEBUG
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include "util.hpp"
#define WIFI_SSID "7308"
#define WIFI_PWD "mokemoke"

enum LEDStateType{
  OFF,ALL_ON,RIGHT,LEFT,BLINK,SM_BLINK
};
ESP8266WebServer server(80);
Ticker timer_event_ticker;

LED* led;
#define HTML_HEADER "<!doctype html>"\
  "<html><head><meta charset=\"UTF-8\"/>"\
  "<meta name=\"viewport\" content=\"width=device-width\"/>"\
  "</head><body>"
#define HTML_FOOTER "</body></html>"

LEDStateType LEDState = OFF;

void send_text_to_client(String str){
  String html = HTML_HEADER + str + HTML_FOOTER;
  server.send(200, "text/html", html);
}
void timer_call_back(){
  switch(LEDState){
    case OFF:
      break;
    case ALL_ON:
      break;
    case RIGHT:
      led->shift_right();
      break;
    case LEFT:
      led->shift_left();
      break;
    case BLINK:
      led->toggle();
      break;
    case SM_BLINK:
      led->blink_smooth();
      break;
  }
}
void led_off(){
  LEDState = OFF;
  led->all_off();
  timer_event_ticker.detach();
}
void led_all_on(Color color){
  LEDState = ALL_ON;
  led->set_all_color(color);
  timer_event_ticker.detach();
}
void led_right(Color color){
  LEDState = RIGHT;
  led->set_odd_color(color);
  timer_event_ticker.attach_ms(100,timer_call_back);
}
void led_left(Color color){
  LEDState = LEFT;
  led->set_odd_color(color);
  timer_event_ticker.attach_ms(100,timer_call_back);
}
void led_blink(Color color){
  //LEDState = BLINK;
  LEDState = SM_BLINK;
  //led->set_all_color(color);
  led->blink_color = color;
  timer_event_ticker.attach_ms(20,timer_call_back);
}
void led_smooth_blink(Color color){
  LEDState = SM_BLINK;
  led->blink_color = color;
  timer_event_ticker.attach_ms(100,timer_call_back);
}
String clientGETParamater(String& key){
  for(int i = 0; i < server.args(); i ++){
    #ifdef DEBUG
    Serial.println("Arg" + (String)i + "->" + server.argName(i) + ":" + server.arg(i));
    #endif
    if(server.argName(i) == key){
      return server.arg(i);
    }
  }
  //ここまで来たときはkeyが見つからなかったとき
  return String("0");
}
Color parseRGBParamater(){
  Color color(0,0,0);
  for(int i = 0; i < server.args(); i ++){
    #ifdef DEBUG
    Serial.println("Arg" + (String)i + "->" + server.argName(i) + ":" + server.arg(i));
    #endif
    if(server.argName(i) == "red"){
      color.r = server.arg(i).toInt();
    }
    if(server.argName(i) == "green"){
      color.g = server.arg(i).toInt();
    }
    if(server.argName(i) == "blue"){
      color.b = server.arg(i).toInt();
    }
  }
  return color;
}
void setup() {
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
   led = new  LED(5, 250);
  // Setup WebServer Handlers
  server.on("/", [](){
    String html = String(HTML_HEADER) + "<form action=\"http://" + WiFi.localIP().toString() + "/led/0/all_on\" method=\"get\">\n" ;
    html += "<p>red <input type=\"text\" name=\"red\"/></p>\n</form>\n";
    html += HTML_FOOTER;
    server.send(200, "text/html", html);
  });
  server.on("/led/0/off",[]{
    send_text_to_client("<h1>LED 0 OFF</h1>");
    led_off();
  });
  server.on("/led/0/all_on",[]{
    send_text_to_client("<h1>LED 0 ALL ON</h1>");
    led_all_on(parseRGBParamater());
  });
  server.on("/led/0/right",[]{
    send_text_to_client("<h1>LED 0 RIGHT</h1>");
    led_right(parseRGBParamater());
  });
  server.on("/led/0/left",[]{
    send_text_to_client("<h1>LED 0 LEFT</h1>");
    led_left(parseRGBParamater());
  });
  server.on("/led/0/blink",[]{
    send_text_to_client("<h1>LED 0 BLINK</h1>");
    led_blink(parseRGBParamater());
  });
  server.begin();
}

void loop() {
  static unsigned char i = 0;
  server.handleClient();
  //Serial.println(i++);
}

