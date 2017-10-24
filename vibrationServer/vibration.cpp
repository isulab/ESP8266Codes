#include "vibration.hpp"

Vibration::Vibration(){
  //strength,interval,duty
  this->setting(0.8,100,0.5);
  pin = 0;
}
Vibration::Vibration(int pin):pin(pin){
  this->setting(0.8,100,0.5);
}
void Vibration::setting(float _strength, int _interval, float duty){
  strength = _strength;
  interval = _interval;
  ratio = duty;
  pwm_duty = (int)(1024.0 * strength);
  on_time = (int)(ratio * (float)interval);
  off_time = interval - on_time;
  toggle_time = 0;
}

void Vibration::update(){
  if(state == OFF)return;
  int now_time = millis();
  if(toggle_time <= now_time){
    switch(state){
      case ON: //オンだったらオフにする
        analogWrite(pin,0);
        toggle_time = now_time + off_time;
        state = WAIT;
        break;
      case WAIT: //オフだったらオンにする
        analogWrite(pin,pwm_duty);
        toggle_time = now_time + on_time;
        state = ON;
        break;
    }
  }
}
void Vibration::on(){
  state = ON;
  toggle_time = 0;
}

void Vibration::off(){
  state = OFF;
  analogWrite(pin,0);
  toggle_time = 0;
}

