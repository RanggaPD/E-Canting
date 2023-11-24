#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <EEPROM.h>
#include <DallasTemperature.h>
Servo servo;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define tombol_kiri    2
#define tombol_tengah  3
#define ONE_WIRE_BUS   4
#define tombol_kanan   6
#define pemanas        7
#define tombol_servo  10
#define pin_servo     11
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define OLED_RESET     -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int menu,batas_bawah,batas_atas,status_pemanas;
int data_adc;
double suhu,kekentalan;

void setup() {
  Serial.begin(9600);
  pinMode(tombol_kiri,INPUT_PULLUP);
  pinMode(tombol_tengah,INPUT_PULLUP);
  pinMode(tombol_kanan,INPUT_PULLUP);
  pinMode(tombol_servo,INPUT_PULLUP);
  pinMode(pemanas,OUTPUT);
  sensors.begin();
  servo.attach(pin_servo);
  servo.write(0); delay(1000);
  servo.write(0); delay(500);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();  
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0); 
  display.println("");
  display.println(" E-CANTING");
  display.setTextSize(1);
  display.println("");
  display.println("    By: PKM UNIMED   ");
  display.println("      TAHUN 2022     ");
  display.display();
  delay(3000);
  display.clearDisplay();

}

void loop() {
  batas_bawah = EEPROM.read(0);
  batas_atas  = EEPROM.read(1);
  sensors.requestTemperatures();
  suhu = sensors.getTempCByIndex(0);
  kekentalan = (suhu-60.0)*(1.92-6.89)/(135.0-60.0)+6.89;
  
  display.clearDisplay();
  if(suhu<batas_bawah) {
    digitalWrite(pemanas,HIGH);
    status_pemanas = 1;
  }
  if(suhu>batas_atas) {
    digitalWrite(pemanas,LOW);  
    status_pemanas = 0;
  }

  if(digitalRead(tombol_servo)==0){
    servo.write(45);
  }
  else servo.write(-5);
  
  if(menu==0) {
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0); 
    display.println(" E-CANTING");
    display.setCursor(0,18); 
    display.setTextSize(1);
    if(status_pemanas == 0) display.println("----------OFF--------");  
    else display.println("---------AKTIF-------");   
    display.setTextSize(1);
    display.setCursor(0,28); 
    display.print("  :SUHU:    :KENTAL: ");
    display.setTextSize(2); display.setCursor(0,39);
    display.print(suhu,1);   // Nilai sensor SUHU
    display.write(247); 
    display.print(" "); 
    display.print(kekentalan);  // Nilai Kekentalan
    display.setTextSize(1); display.setCursor(4,57);
    display.print(" Celcius     Pascal    ");
  }
  else if(menu==1 || menu==2) {
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0); 
    display.println(" E-CANTING");
    display.setCursor(24,20); 
    display.setTextSize(1);
    display.println("--------------");
    
    display.setTextSize(1);
    display.setCursor(0,30); 
    display.println("     MENU SETTING     ");
    display.setCursor(0,45);
    display.print("  Batas Bawah : ");
    display.print(batas_bawah); display.write(247); display.println("C");
    display.print("  Batas Atas  : ");
    display.print(batas_atas); display.write(247); display.println("C");    
  }
  
  if(digitalRead(tombol_kiri)==0) {
    if(menu==1) {
      batas_bawah = batas_bawah - 1;
      EEPROM.write(0,batas_bawah);
    }
    else if(menu==2) {
      batas_atas = batas_atas - 1;
      EEPROM.write(1,batas_atas);
    }
  }

  else if(digitalRead(tombol_kanan)==0) {
    if(menu==1) {
      batas_bawah = batas_bawah + 1;
      EEPROM.write(0,batas_bawah);
    }
    else if(menu==2) {
      batas_atas = batas_atas + 1;
      EEPROM.write(1,batas_atas);
    }
  }
  
  else if(digitalRead(tombol_tengah)==0) {
    while(digitalRead(tombol_tengah)==0) {}
    menu = menu + 1;
  }
  
  if(menu>2) menu=0;
  
  display.display();
}
