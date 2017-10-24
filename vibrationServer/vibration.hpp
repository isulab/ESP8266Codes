#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
//                                          ------------          --------       ←
//                                          |          |          |               |
//                                          |          |          |               |   strength
//                                          |          |          |               |
//                     ----------------------          ------------              ←
//                     ↑-------------------↑--------↑----------↑-------
//state                         OFF              ON         WAIT      ON
//                                          ↑--------------------↑
//                                                 interval
                    //オン,オフ,オンの時の2次PWMでオフの時
enum VibrationState { ON, OFF, WAIT };
class Vibration{
public:
  //バイブレーションがオフかオンか保存しておく変数
  VibrationState state;
  //バイブレーションがオンの時に次に状態を変化させるべき時間
  int toggle_time;
  float ratio;  //オンとオフの時間の比 : 0.0~1.0
  int interval; //LEDの点滅周期,周波数の逆数 : 0~65535 [ms]
  float strength; //バイブレーションの強さ : 0.0~1.0
  Vibration();
  Vibration(int pin);
  void update(); //定期的(1msとか)に呼ぶ関数
  void setting(float strength, int interval, float duty); //LEDの状態を変えるときによぶ.
  void on();
  void off();
private:
  int pin;
  int pwm_duty; //strengthから計算するpwm比
  int on_time; //LEDがオンの時間,ratioとintervalから計算する
  int off_time;//LEDがオフの時間,ratioとintervalから計算する
};

