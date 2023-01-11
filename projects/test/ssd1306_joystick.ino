#include <Wire.h>                   //i2c통신을 사용하기 때문에 아두이노의 i2c 통신용 라이브러리가 필요
#include <Adafruit_GFX.h>           // adafruit의 그래픽 관련 라이브러리
#include <Adafruit_SSD1306.h>        // ssd1306 제어용 라이브러리

#define SCREEN_WIDTH 128              // OLED 디스플레이의 가로 픽셀수
#define SCREEN_HEIGHT 64              // OLED 디스플레이의 세로 픽셀수
#define OLED_RESET    99             // 리셋핀이 있는 oled의 리셋핀에 연결할 아두이노의 핀 번호, 리셋핀이 없는 모듈은 임의의 숫자를 넣어도 됨.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);    // 디스플레이 객체 생성

#define SCREENDELAY 10

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();

}

void loop() {
  int analog_raw;
  int joy_x;
  int joy_y;
  char calc_joy_x;
  char calc_joy_y;
  // 문자표시하기
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("ADC0      ADC1");

  analog_raw = analogRead(A0);
  display.setCursor(6*5,0);
  display.print(analog_raw);
  joy_x = analog_raw;
  analog_raw = analogRead(A1);
  display.setCursor(6*15,0);
  display.print(analog_raw);
  joy_y = analog_raw;

  calc_joy_x = (joy_x / 25) - 20;
  calc_joy_y = (joy_y / 25) - 20;

  display.drawPixel(64, 42, WHITE);
  display.drawCircle(64, 42, 20, WHITE);
  display.drawCircle(64 + calc_joy_x, 42 + calc_joy_y, 5, WHITE);
  display.stopscroll();
  display.display();
  delay(200);
  //delay(SCREENDELAY);

  display.startscrollleft(0x01, 0x07);
  
  display.display();
  delay(1000);

}