#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>

#define WIFI_SSID "7308"
#define WIFI_PWD "mokemoke"

ESP8266WebServer server(80);
Ticker ticker1;
Ticker led_loop_ticker;
int led_pwm_duty = 300; //PWMのduty比 0~1023(brightness)
int led_duty = 127; //LEDの点滅duty比(0~255)
int led_interval = 500; //LEDの点滅周期(0~65535ms)
// HTML
#define HTML_HEADER "<!doctype html>"\
  "<html><head><meta charset=\"UTF-8\"/>"\
  "<meta name=\"viewport\" content=\"width=device-width\"/>"\
  "</head><body>"
#define HTML_FOOTER "</body></html>"
void L_chika(){
  //Serial.println("I am L chika");
  static bool toggle = false;
  if(toggle){
    digitalWrite(2,HIGH);
    toggle = false;
  }else{
    digitalWrite(2,LOW);
    toggle = true;
  }
}
void led_tiemr_interrupt(){
  static bool led_state = false; //true:on false:off
  static unsigned int toggle_time = 0;
  static float ratio = (float)led_duty / (float)255;
  static int on_time = (int)(ratio * (float)led_interval); 
  static int off_time = led_interval - on_time;
  
  int now_time = millis();
  //Serial.println(now_time);
  //ledの状態を変更すべき時間が来たら
  if(toggle_time <= now_time){
    if(led_state){ //onだったらoffにする
      analogWrite(5,0);
      toggle_time = now_time + off_time;
      led_state = false;
    }else{
      analogWrite(5,led_pwm_duty);
      toggle_time = now_time + on_time;
      led_state = true;
    }
    ratio = (float)led_duty / (float)255;
    on_time = (int)(ratio * (float)led_interval);
    off_time = led_interval - on_time;
  }
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
    String html = HTML_HEADER "<h1>I am the vibration server.</h1>" HTML_FOOTER;
    server.send(200, "text/html", html);
  });

  server.on("/vibration/0/on", [](){
    String html = HTML_HEADER "<h1>Vibration ON</h1>" HTML_FOOTER;
    server.send(200, "text/html", html);
    for(int i = 0; i < server.args(); i ++){
      Serial.println("Arg" + (String)i + "->" + server.argName(i) + ":" + server.arg(i));
      if(server.argName(i).compareTo("duty") == 0){
        led_duty = server.arg(i).toInt();
      }
      if(server.argName(i).compareTo("strength") == 0){
        led_pwm_duty = server.arg(i).toInt();
      }
      if(server.argName(i).compareTo("interval") == 0){
        led_interval = server.arg(i).toInt();
      }
    }
    //ticker1.attach_ms(500,L_chika);
    //digitalWrite(2,HIGH);
    //analogWrite(2,led_pwm_duty);
    led_loop_ticker.attach_ms(1,led_tiemr_interrupt);
  });

  server.on("/vibration/0/off", [](){
    String html = HTML_HEADER "<h1>Vibration OFF</h1>" HTML_FOOTER;
    server.send(200, "text/html", html);
    led_loop_ticker.detach();
    //digitalWrite(2,LOW);
    analogWrite(5,0);
  });

  server.begin();
}

void loop() {
  server.handleClient();
  //Serial.println("hi");
}
