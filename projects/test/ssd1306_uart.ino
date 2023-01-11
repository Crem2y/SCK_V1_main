#include <Wire.h>                   //i2c통신을 사용하기 때문에 아두이노의 i2c 통신용 라이브러리가 필요
#include <Adafruit_GFX.h>           // adafruit의 그래픽 관련 라이브러리
#include <Adafruit_SSD1306.h>        // ssd1306 제어용 라이브러리

#define SCREEN_WIDTH 128              // OLED 디스플레이의 가로 픽셀수
#define SCREEN_HEIGHT 64              // OLED 디스플레이의 세로 픽셀수
#define OLED_RESET    99             // 리셋핀이 있는 oled의 리셋핀에 연결할 아두이노의 핀 번호, 리셋핀이 없는 모듈은 임의의 숫자를 넣어도 됨.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);    // 디스플레이 객체 생성

#define SCREENDELAY 1000

unsigned char graph_buffer[120] = {0,};

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();

  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
}

void loop() {
  String str0 = "(none)";
  String str1 = "(none)";
  String str2 = "(none)";
  String str3 = "(none)";

  if(Serial.available()) {
    str0 = Serial.readStringUntil('\n');
  }
  if(Serial1.available()) {
    str1 = Serial1.readStringUntil('\n');
  }
  if(Serial2.available()) {
    str2 = Serial2.readStringUntil('\n');
  }
  if(Serial3.available()) {
    str3 = Serial3.readStringUntil('\n');
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("UART0:");
  display.print(str0);
  display.setCursor(0,16);
  display.print("UART1:");
  display.print(str1);
  display.setCursor(0,32);
  display.print("UART2:");
  display.print(str2);
  display.setCursor(0,48);
  display.print("UART3:");
  display.print(str3);

  display.display();
  delay(SCREENDELAY);
}