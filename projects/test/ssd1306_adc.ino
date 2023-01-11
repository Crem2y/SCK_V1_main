#include <Wire.h>                   //i2c통신을 사용하기 때문에 아두이노의 i2c 통신용 라이브러리가 필요
#include <Adafruit_GFX.h>           // adafruit의 그래픽 관련 라이브러리
#include <Adafruit_SSD1306.h>        // ssd1306 제어용 라이브러리

#define SCREEN_WIDTH 128              // OLED 디스플레이의 가로 픽셀수
#define SCREEN_HEIGHT 64              // OLED 디스플레이의 세로 픽셀수
#define OLED_RESET    99             // 리셋핀이 있는 oled의 리셋핀에 연결할 아두이노의 핀 번호, 리셋핀이 없는 모듈은 임의의 숫자를 넣어도 됨.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);    // 디스플레이 객체 생성

#define SCREENDELAY 10

unsigned char graph_buffer[120] = {0,};
unsigned char graph_buffer_2[120] = {0,};

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Arduino Mega I2C OLED");
  display.println("            by Cremmy");
  display.println("     Z               ");
  display.println("      z A___A        ");
  display.println("       / U.U \\       ");
  display.println("      >(  w  )<      ");
  display.println("        /   \\        ");
  display.println("       Now Loading...");
  display.display();
  delay(5000);


}

void loop() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(40,56);
  display.print("by Cremmy");

  display.setCursor(0, 13);
  display.print("5");
  display.setCursor(0, 45);
  display.print("0");

  display.drawLine(8, 8, 8, 55, WHITE);
  display.drawLine(8, 55, 119, 55, WHITE);
  display.drawPixel(7, 47, WHITE);
  display.drawPixel(6, 47, WHITE);
  display.drawPixel(7, 16, WHITE);
  display.drawPixel(6, 16, WHITE);
  display.drawPixel(19, 54, WHITE);
  display.drawPixel(39, 54, WHITE);
  display.drawPixel(59, 54, WHITE);
  display.drawPixel(79, 54, WHITE);
  display.drawPixel(99, 54, WHITE);

  int analog_raw = 0;
  int analog_mv = 0;
  int analog_avr = 0;
  char graph_pos = 0;

  analog_raw = analogRead(A0);
  analog_avr += (int)((float)analog_raw * (5000.0/1023.0));
  analog_raw = analogRead(A0);
  analog_avr += (int)((float)analog_raw * (5000.0/1023.0));
  analog_raw = analogRead(A0);
  analog_avr += (int)((float)analog_raw * (5000.0/1023.0));
  analog_raw = analogRead(A0);
  analog_avr += (int)((float)analog_raw * (5000.0/1023.0));
  analog_mv = analog_avr >> 2;

  display.setCursor(0,0);
  display.print("ADC0 ");
  display.print(analog_mv);
  display.print("mV");

  graph_buffer[111] = analog_raw >> 5;
  for(unsigned char i=0; i<111; i++) {
    display.drawLine(8+i, 47-graph_buffer[i], 8+i, 47-graph_buffer[i+1], WHITE);
    graph_buffer[i] = graph_buffer[i+1];
  }
  display.drawPixel(8+111, 47-graph_buffer[111], WHITE);

  analog_raw = 0;
  analog_mv = 0;
  analog_avr = 0;
  graph_pos = 0;

  analog_raw = analogRead(A1);
  analog_avr += (int)((float)analog_raw * (5000.0/1023.0));
  analog_raw = analogRead(A1);
  analog_avr += (int)((float)analog_raw * (5000.0/1023.0));
  analog_raw = analogRead(A1);
  analog_avr += (int)((float)analog_raw * (5000.0/1023.0));
  analog_raw = analogRead(A1);
  analog_avr += (int)((float)analog_raw * (5000.0/1023.0));
  analog_mv = analog_avr >> 2;
  
  display.setCursor(64,0);
  display.print("ADC1 ");
  display.print(analog_mv);
  display.print("mV");

  graph_buffer_2[111] = analog_raw >> 5;
  for(unsigned char i=0; i<111; i++) {
    display.drawLine(8+i, 47-graph_buffer_2[i], 8+i, 47-graph_buffer_2[i+1], WHITE);
    graph_buffer_2[i] = graph_buffer_2[i+1];
  }
  display.drawPixel(8+111, 47-graph_buffer_2[111], WHITE);

  display.display();
  delay(SCREENDELAY);
}