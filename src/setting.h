#include "SPI.h"
#include "timer2Minim.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Arduino_JSON.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <Adafruit_VEML7700.h>
#include <GyverBME280.h> 
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <LittleFS.h>
#include <FileData.h>
#include <GyverPortal.h>
#include <MD5.h>
// Необходимо форматировать LittleFS только при первом запуске
#define FORMAT_LITTLEFS_IF_FAILED true

GyverPortal ui;//(&LittleFS); // для проверки файлов

//*******************Переменные которые будут храниться в памяти
struct Data {
  char ssid[40];
  char pass[20];
  char owMapApiKey[60];
  char owCity[40];
  char NTPserver[40];
  char NarodmoonApi[20];
  char NarodmoonApiMD5[40];
  char NarodmoonID[10];
  char NameSensor[5][20];
  byte display, mode, animdots, modetime, lng, 
  displaypressure, displayhumidity, displaytemperature;
  int GMT;
  boolean dots_switch, seconds_switch, autoshow_switch, random_autoshow_switch, sw1,sw2,sw3,sw4,sw5;
  bool dispset[7] = {true, true, true, true, true, true, true};
  byte nrd_sens0,nrd_sens1,nrd_sens2;
  byte nrd_sens[5];//номер датчика narodmon
  byte nrd_type_sensor[6];
  byte autoshow_min,autoshow_select[6],autoshow_select_sec[6], autoshow_animations_select;
   
};
Data mydata;
FileData data(&LittleFS, "/setting.dat", 'B', &mydata, sizeof(mydata));

Adafruit_VEML7700 veml = Adafruit_VEML7700();
GyverBME280 bme;

TaskHandle_t Task_0;
TaskHandle_t Task_1;

// Определение NTP-клиента для получения времени
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,mydata.NTPserver, mydata.GMT);//"pool.ntp.org"


const char* SYMBOLBTC = "BTC";
const char* SYMBOLETH = "ETH";
const char* CONVERT_TO = "usd";
int pricebtc, priceeth;
int TempValue;
int num, num1, num2, num3, num4, num5,num6;

SPIClass * hspi = NULL;
#define CS_ON_HSPI (GPIO.out_w1ts = ((uint32_t)1 << 15))  
#define CS_OFF_HSPI (GPIO.out_w1tc = ((uint32_t)1 << 15))
#define HSPI_MISO   12
#define HSPI_MOSI   13
#define HSPI_SCLK   14
#define HSPI_SS     15
#define G 16

const int PWM_CHANNEL = 0;    // ESP32 имеет 16 каналов, которые могут генерировать 16 независимых сигналов.
const int PWM_FREQ = 15000;     // Официальный пример ESP32 использует частоту 5000 Гц.
const int PWM_RESOLUTION = 10; // Мы будем использовать то же разрешение, что и Uno (8 бит, 0-255), но ESP32 может достигать 16 бит.

// Максимальное значение рабочего цикла, основанное на разрешении ШИМ (будет 255, если разрешение составляет 8 бит)
const int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1);
const int LED_OUTPUT_PIN = 16;

byte second, minute, hour,newsecond, newminute, newhour, dayOfWeek, day, dayOfMonth, month, year, minsCount, hourCount, cryptoCount;
boolean updateCrypto = true;  

//Массив с десятичным представлением символов из библиотеки AlexGiver
static const uint8_t _segCharMap[] PROGMEM = {
  0,     //   0x20 32
  0x02,  // ! 0x21 33
  0x22,  // " 0x22 34
  0x36,  // # 0x23 35
  0x69,  // $ 0x24 36
  0xC5,  // % 0x25 37
  0x7B,  // & 0x26 38
  0x20,  // ' 0x27 39
  0x39,  // ( 0x28 40
  0x0F,  // ) 0x29 41
  0x93,  // * 0x2a 42
  0x70,  // + 0x2b 43
  0x0C,  // , 0x2c 44
  0x10,  // - 0x2d 45
  0,     // . 0x2e 46 !
  0x42,  // / 0x2f 47

  0xE7,  // 0 0x30 48
  0x06,  // 1 0x31 49
  0x73,  // 2 0x32 50
  0x57,  // 3 0x33 51
  0x96,  // 4 0x34 52
  0xD5,  // 5 0x35 53
  0xF5,  // 6 0x36 54
  0x07,  // 7 0x37 55
  0xF7,  // 8 0x38 56
  0xD7,  // 9 0x39 57

  0x09,  // : 0x3a 58
  0x0D,  // ; 0x3b 59
  0x58,  // < 0x3c 60
  0x48,  // = 0x3d 61
  0x4C,  // > 0x3e 62
  0x53,  // ? 0x3f 63
  0x5F,  // @ 0x40 64

  0xB7,  // A 0x41 65
  0xF4,  // B 0x42 66
  0xE1,  // C 0x43 67
  0x76,  // D 0x44 68
  0xF1,  // E 0x45 69
  0xB1,  // F 0x46 70
  0xE5,  // G 0x47 71
  0xB4,  // H 0x48 72
  0xA0,  // I 0x49 73
  0x66,  // J 0x4a 74
  0xB5,  // K 0x4b 75
  0xE0,  // L 0x4c 76
  0x25,  // M 0x4d 77
  0xA7,  // N 0x4e 78
  0xE7,  // O 0x4f 79
  0xB3,  // P 0x50 80
  0x97,  // Q 0x51 81
  0xA3,  // R 0x52 82
  0xD5,  // S 0x53 83
  0xF0,  // T 0x54 84
  0xE6,  // U 0x55 85
  0xC6,  // V 0x56 86
  0xC2,  // W 0x57 87
  0xB6,  // X 0x58 88
  0xD6,  // Y 0x59 89
  0x53,  // Z 0x5a 90

  0x39,  // [ 0x5b 91
  0x60,  // \ 0x5c 92
  0x0F,  // ] 0x5d 93
  0x23,  // ^ 0x5e 94
  0x08,  // _ 0x5f 95
  0x02,  // ` 0x60 96

  // 97..122 lowercase

  0x39,  // { 0x7b 123
  0x06,  // | 0x7c 124
  0x0F,  // } 0x7d 125
  0x40,  // ~ 0x7e 126
//127....175
  0x93,  // ° 176 знак градуса 69 место
};

char textbuffer[7]="";//6 символов и нулевой байт
//char SensorsAutoShowSelect[100];
char buffer[7]="";//6 символов и нулевой байт
String lost= "WIFI lost please connect to 192-168-4-1      ";
//String SensorsNarodMon[20];
String SensorsAutoShow[20];
String SensorsDisplay[20];
String SensorsDisplayBme[3];
String SensorsDisplayOp[3];
String SensorsDisplayNarod[3];
String SensorsAutoShowSelect;
String SensorsAutoShowSelect2;

String WiFI_List;
char WiFI_ssid;

byte dec_buffer[6];

unsigned long previousMillis;

timerMinim TimeTimer(1000); //Таймер для рассчета времени
timerMinim timerTIME(40);
timerMinim ligtSensorTimer(100); // таймер мигания цифры в настройках
timerMinim RandomTimer(150);
timerMinim SensorTimerI2C(3000); // Время обновления датчиков и яркости индикаторов
timerMinim DotTimer(1000);//Таймер для переключение точек
timerMinim DotRandomTimer(60000);//Таймер для переключение точек
timerMinim mooveNixie(100);//движение массива

int vemlvalue, vemllux;
int bmevalue, bmehumudity, bmepressure, bmetemperature, altitude;
int  oppressure, ophumidity, optemperature, narodpressure, narodhumidity, narodtemperature;
int bvs[4];//Buffer Value sensors

int brightnessIV13;
byte brightnesscount;

//Переменные для анимации
int Counter = 6;
byte off_effects = 0;
byte on_effects = 0;
boolean flip;
boolean timerstart = false;
boolean timeon = true;//Вспомогательное булевое значение для включение анимации точек
byte dmoove = 0;
boolean dmooveright = true;
boolean dmooveleft = false;
int auto_show_counter;
byte scan_list_idx;
/////////////////////////