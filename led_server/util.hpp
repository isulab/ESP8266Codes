#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define DEFAULT_LED_NUM 32
#define DEFAULT_LED_PIN 2
/**
* 色を定義するクラス
* @auther Takishima
*/
class Color{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
  Color(): r(0),g(0),b(0){};
  Color(unsigned char r,unsigned char g,unsigned char b):r(r),g(g),b(g){}
  const Color operator*(const float& rhs){
    Color tmp;
    tmp.r = (int)((float)this->r * rhs);
    tmp.g = (int)((float)this->g * rhs);
    tmp.b = (int)((float)this->b * rhs);
    return tmp;
  }
};

/**
* テープLEDの制御を行うクラス
* @auther Takishima
*/
class LED{
private:
	//LEDの数
	const int led_num;

	//LEDの現在の色を示す配列(LEDの数だけ確保されている)

	//LEDの制御に使用するピン
	unsigned int led_pin;
  Color* temp_c;
  Adafruit_NeoPixel pixels;
  //now_led配列の色をLEDに適用する
  void apply_now_led();
  void init();
  
public:
  LED(unsigned int led_pin);
  LED(unsigned int led_pin,unsigned int led_num);
  LED();
	//number番目（0から）のLEDの色を変更する.
	void set_color(int number,Color color);
	//すべてのLEDの色を変更する.
	void set_all_color(Color color);
	//now_color配列を右にシフトしてあふれた分は左に入れて循環させる.
	void shift_right();
	//now_color配列を左にシフトしてあふれた分は右に入れて循環させる.
	void shift_left();
	//now_colorの奇数番目の色を指定する.
  //↑は間違い　いまは4こごとになった
	void set_odd_color(Color color);
  void gradually_blink();
  //すべてのLEDの色を(0,0,0)にする.set_all_color(Color(0,0,0))と同じ.
	void all_off();
  //色情報を保持したまますべてのLEDの色について(0,0,0)と指定した色を呼び出すたびに交互に変更する
  void toggle();
  void blink_smooth();
  Color* now_color;
  Color blink_color;
};
