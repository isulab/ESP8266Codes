#include "vibration.hpp"

Vibration::Vibration(){
  //strength,interval,duty
  this->setting(0.8,100,0.5);
  pin = 0;
  state = OFF;
}
Vibration::Vibration(int pin):pin(pin){
  this->setting(0.8,100,0.5);
  state = OFF;
}
void Vibration::setting(float _strength, int _interval, float duty){
  strength = _strength < 0.0 ? 0.0 : _strength > 1.0 ? 1.0 : _strength;
  interval = (unsigned int)_interval;
  ratio = duty < 0.0 ? 0.0 : duty > 1.0 ? 1.0 : duty;
  pwm_duty = (int)(1024.0 * strength);
  on_time = (int)(ratio * (float)interval);
  off_time = interval - on_time;
  toggle_time = 0;
  //Serial.println(String() + "ratio : " + ratio + ",pwm : " + pwm_duty + ",on : " + on_time + ",off : " + off_time);
}

void Vibration::update(){
  //Serial.println(state);
  if(state == OFF)return;
  int now_time = millis();
  //Serial.println(String() + "now : " + now_time);
  if(toggle_time <= now_time){
    switch(state){
      case ON: //オンだったらオフにする
        analogWrite(pin,0);
        toggle_time = now_time + off_time;
        state = WAIT;
        //Serial.println("OFF");
        break;
      case WAIT: //オフだったらオンにする
        analogWrite(pin,pwm_duty);
        toggle_time = now_time + on_time;
        state = ON;
        //Serial.println("ON");
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

