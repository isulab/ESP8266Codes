#include "util.hpp"
//#define DEBUG
void LED::init(){
  pinMode(led_pin,OUTPUT);
  pixels = Adafruit_NeoPixel(led_num, led_pin, NEO_GRB + NEO_KHZ400);
  now_color = new Color[led_num];
  temp_c = new Color[led_num];
  blink_color = Color(0,0,0);
  pixels.begin();
  Serial.println(led_num);
}
LED::LED(unsigned int led_pin,unsigned int led_num): led_pin(led_pin), led_num(led_num){
  init();
}

LED::LED(unsigned int led_pin): led_pin(led_pin), led_num(DEFAULT_LED_NUM){
  init();
}

LED::LED(): led_pin(DEFAULT_LED_PIN), led_num(DEFAULT_LED_NUM){
  init();
}

void LED::set_color(int number,Color color){
  #ifdef DEBUG
  if(number < 0 || number > led_num){
      Serial.println("LED::set_color -> out of range");
    return;
  }
  #endif
  now_color[number] = color;
  apply_now_led();
  //pixels.setPixelColor(number, pixels.Color(color.r, color.g, color.b));
  //pixels.show();
}

void LED::apply_now_led(){
  for(int i = 0; i < led_num; i ++){
    pixels.setPixelColor(i, pixels.Color(now_color[i].r, now_color[i].g, now_color[i].b));
    //pixels.show();
  }
  pixels.show();
}

void LED::set_all_color(Color color){
  for(int i = 0; i < led_num; i++){
    now_color[i] = color;
    #ifdef DEBUG
    Serial.print("led ");
    Serial.print(i);
    Serial.print(" : ");
    Serial.println(now_color[i].r);
    #endif
  }
  apply_now_led();
}

void LED::shift_right(){
  for(int i = 0; i < led_num; i++){
    temp_c[i].r = now_color[i].g;
    temp_c[i].g = now_color[i].r;
    temp_c[i].b = now_color[i].b;
  }
  for(int i = 1; i < led_num; i++){
    #ifdef DEBUG
    Serial.print("led ");
    Serial.print(i);
    Serial.print(" : ");
    Serial.println(now_color[i].r);
    #endif
    now_color[i] = temp_c[i-1];
  }
  now_color[0] = temp_c[led_num -1];
  apply_now_led();
}

void LED::shift_left(){
  for(int i = 0; i < led_num; i++){
    temp_c[i].r = now_color[i].g;
    temp_c[i].g = now_color[i].r;
    temp_c[i].b = now_color[i].b;
  }
  for(int i = 0; i < led_num - 1; i++){
    now_color[i] = temp_c[i+1];
  }
  now_color[led_num - 1] = temp_c[0];
  apply_now_led();
}

void LED::set_odd_color(Color color){
  //Serial.println("set color");
  //Serial.println(color.b);
  //Serial.println((int)(((float)color.b) * 0.66));
  for(int i = 0; i < led_num; i++){
    
    if(i % 4 == 0){
      now_color[i] = color;
    }else{
      now_color[i] = Color(0,0,0);
    }
    
    /*
    switch (i % 4){
      case 0:
        now_color[i] = color;
        break;
      case 1:
        //now_color[i] = Color((int)(((float)color.r) * 0.66), (int)(((float)color.g) * 0.66), (int)(((float)color.b) * 0.66));
        now_color[i].r = (int)(((double)color.r) * 0.1);
        now_color[i].g = (int)(((double)color.g) * 0.1);
        now_color[i].b = (int)(((double)color.b) * 0.1);
        break;
      case 2:
        //now_color[i] = Color((int)(((float)color.r) * 0.33), (int)(((float)color.g) * 0.33), (int)(((float)color.b) * 0.33));
        now_color[i].r = (int)(((double)color.r) * 0.05);
        now_color[i].g = (int)(((double)color.g) * 0.05);
        now_color[i].b = (int)(((double)color.b) * 0.05);
        break;
      case 3:
        now_color[i] = Color(0,0,0);
        break;
    }
    */
  }
  apply_now_led();
}

void LED::all_off(){
  set_all_color(Color(0,0,0));
}
void LED::toggle(){
  //static Color color = now_color[0];
  static bool led_on = false;
  if(led_on){
    all_off();
    led_on = false;
  }else{
    set_all_color(blink_color);
    led_on = true;
  }
}

