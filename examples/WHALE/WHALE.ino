#include "Bluewhale.h"
#include <Servo.h>
// Define a Gamecube Console
//デジタル5,6番ピンを使う
CGamecubeConsole GamecubeConsole1(5);
Servo myservo;
const int SV_PIN = 6;
Gamecube_Data_t d = defaultGamecubeData;

void setup()
{
  //SG90用初期セットアップ
  myservo.attach(SV_PIN, 500, 2400);
  delay(500);
  myservo.detach();

  d.report.a = 0;
  d.report.b = 0;
  d.report.x = 0;
  d.report.y = 0;
  d.report.start = 0;
  d.report.dleft = 0;
  d.report.dright = 0;
  d.report.ddown = 0;
  d.report.dup = 0;
  d.report.z = 0;
  d.report.r = 0;
  d.report.l = 0;
  d.report.xAxis = 128;
  d.report.yAxis = 128;
  d.report.cxAxis = 128;
  d.report.cyAxis = 128;
  d.report.left = 0;
  d.report.right = 0;

  //コントローラーを認識させるおまじない
  d.report.start = 1;
  GamecubeConsole1.write(d);  //press start

  d.report.start = 0;
  GamecubeConsole1.write(d);  //release start
  // Start debug serial
  //4800にしてるけど9600とかでも良いかも
  Serial.begin(4800);
}

void update_AT12806379(char m){
  switch (m) {
      case 'a':
        d.report.a = 1;
        Serial.println("KeyDown.A");
        break;
      case 'b':
        d.report.b = 1;
        Serial.println("KeyDown.B");
        break;
      case 'c':
        d.report.x = 1;
        Serial.println("KeyDown.X");
        break;
      case 'd':
        d.report.y = 1;
        Serial.println("KeyDown.Y");
        break;
      case 'e':
        d.report.l = 1;
        Serial.println("KeyDown.L");
        break;
      case 'f':
        d.report.r = 1;
        Serial.println("KeyDown.R");
        break;
      case 'g':
        d.report.z = 1;
        Serial.println("KeyDown.Z");
        break;
      case 'h':
        d.report.start = 1;
        Serial.println("KeyDown.Start");
        break;
      case 'i':
        d.report.dleft = 1;
        Serial.println("KeyDown.Left");
        break;
      case 'j':
        d.report.dright = 1;
        Serial.println("KeyDown.Right");
        break;
      case 'k':
        d.report.ddown = 1;
        Serial.println("KeyDown.Down");
        break;
      case 'l':
        d.report.dup = 1;
        Serial.println("KeyDown.Up");
        break;
      case 'm':
        d.report.a = 0;
        Serial.println("KeyUp.A");
        break;
      case 'n':
        d.report.b = 0;
        Serial.println("KeyUp.B");
        break;
      case 'o':
        d.report.x = 0;
        Serial.println("KeyUp.X");
        break;
      case 'p':
        d.report.y = 0;
        Serial.println("KeyUp.Y");
        break;
      case 'q':
        d.report.l = 0;
        Serial.println("KeyUp.L");
        break;
      case 'r':
        d.report.r = 0;
        Serial.println("KeyUp.R");
        break;
      case 's':
        d.report.z = 0;
        Serial.println("KeyUp.Z");
        break;
      case 't':
        d.report.start = 0;
        Serial.println("KeyUp.Start");
        break;
      case 'u':
        d.report.dleft = 0;
        Serial.println("KeyUp.Left");
        break;
      case 'v':
        d.report.dright = 0;
        Serial.println("KeyUp.Right");
        break;
      case 'w':
        d.report.ddown = 0;
        Serial.println("KeyUp.Down");
        break;
      case 'x':
        d.report.dup = 0;
        Serial.println("KeyUp.Up");
        break;
      case 'y':
        d.report.xAxis = 0;
        Serial.println("xAxis.0");
        break;
      case 'z':
        d.report.xAxis = 128;
        Serial.println("xAxis.128");
        break;
      case '1':
        d.report.xAxis = 255;
        Serial.println("xAxis.255");
        break;
      case '2':
        d.report.yAxis = 1;
        Serial.println("yAxis.0");
        break;
      case '3':
        d.report.yAxis = 128;
        Serial.println("yAxis.128");
        break;
      case '4':
        d.report.yAxis = 255;
        Serial.println("yAxis.255");
        break;
      case '5':
        d.report.cxAxis = 0;
        Serial.println("cxAxis.0");
        break;
      case '6':
        d.report.cxAxis = 128;
        Serial.println("cxAxis.128");
        break;
      case '7':
        d.report.cxAxis = 255;
        Serial.println("cxAxis.255");
        break;
      case '8':
        d.report.cyAxis = 1;
        Serial.println("cyAxis.0");
        break;
      case '9':
        d.report.cyAxis = 128;
        Serial.println("cyAxis.128");
        break;
      case '0':
        d.report.cyAxis = 255;
        Serial.println("cyAxis.255");
        break;
    }
}

void press_gc_reset(){
  //角度は各々要調整
  Serial.println("Reset");
  myservo.attach(SV_PIN, 500, 2400);
  myservo.write(65);
  delay(500);
  myservo.write(90);
  delay(500);
  myservo.detach();
}

void update_data(){
  if(!Serial.available()) return;

  uint8_t head = Serial.read();
  //@が送られてきたらサーボでリセットボタンを押す
  if(head == 0x40) {
      press_gc_reset();
      return;
  }

  // コントローラ操作を送信する際は先頭を0x80にする.
  // 文字が1つだけ送られてきたらみずようかんさんの方式で処理する.
  if(head != 0x80) {
    update_AT12806379(head);
    return;
  }
  
  uint8_t buttons_state_1;
  uint8_t buttons_state_2;
  // 2byte分送信される.
  while((buttons_state_1 = Serial.read()) == 0xFF) {}
  while((buttons_state_2 = Serial.read()) == 0xFF) {}

  if(buttons_state_1 == 0x80){
    d.report.a = 0;
    d.report.b = 0;
    d.report.x = 0;
    d.report.y = 0;
    d.report.l = 0;
    d.report.r = 0;
  } else {
    d.report.a =  buttons_state_1       & 1;
    d.report.b = (buttons_state_1 >> 1) & 1;
    d.report.x = (buttons_state_1 >> 2) & 1;
    d.report.y = (buttons_state_1 >> 3) & 1;
    d.report.l = (buttons_state_1 >> 4) & 1;
    d.report.r = (buttons_state_1 >> 5) & 1;
  }

  if(buttons_state_2 == 0x80){
    d.report.z      = 0;
    d.report.start  = 0;
    d.report.dleft  = 0;
    d.report.dright = 0;
    d.report.dup    = 0;
    d.report.ddown  = 0;
  } else {
    d.report.z      =  buttons_state_2       & 1;
    d.report.start  = (buttons_state_2 >> 1) & 1;
    d.report.dleft  = (buttons_state_2 >> 2) & 1;
    d.report.dright = (buttons_state_2 >> 3) & 1;
    d.report.dup    = (buttons_state_2 >> 4) & 1;
    d.report.ddown  = (buttons_state_2 >> 5) & 1;
  }
}

void loop()
{
  update_data();
  if(!GamecubeConsole1.write(d))
    Serial.println("GCが起動していないか、接続されていません。");
}
