#include "SPI.h"
#include "timer2Minim.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <Arduino_JSON.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <Adafruit_VEML7700.h>
#include <GyverBME280.h> 
#include <EEPROM.h>
//#include <Wire.h>
//#include <WiFiClient.h>
//#include <WebServer.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
//#include <Update.h>
#include <LittleFS.h>
#include <FileData.h>

#include <GyverPortal.h>
#include <GyverSegment.h>

// Необходимо форматировать LittleFS только при первом запуске
//#define FORMAT_LITTLEFS_IF_FAILED true

GyverPortal ui;//(&LittleFS); // для проверки файлов

//*******************Переменные которые будут храниться в памяти
struct Data {
  char ssid[20];
  char pass[20];
  char owMapApiKey[60];
  char owCity[40];
  char NTPserver[40];
  byte display, mode, modedots, modetime, lng;
  int GMT;
  boolean sw1,sw2,sw3,sw4,sw5;
  
};
Data mydata;

FileData data(&LittleFS, "/setting.dat", 'B', &mydata, sizeof(mydata));

Adafruit_VEML7700 veml = Adafruit_VEML7700();
GyverBME280 bme;

TaskHandle_t Task_0;
TaskHandle_t Task_1;

//Список часовых поясов  https://en.wikipedia.org/wiki/List_of_UTC_time_offsets
long utcOffsetInSeconds = mydata.GMT; //UTC +7 в секундах Время Барнаул
// Определение NTP-клиента для получения времени
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,mydata.NTPserver, utcOffsetInSeconds);//"pool.ntp.org"

//API ключ от вашего профиля на сайте http://narodmom.ru
const String NarodMonApiKey = "uEvRz28u2aZk3";
//ID профиля устройства (что бы узнать ид необходимо нажать на ссылку профиля например http://narodmom.ru/4548)
const String TempSensorId = "6746";//"4548"; 6678
//MD5 хеш сумма названия (nixieclockin18) вашего приложения в нижнем регистре в вашем профиле на сайте.
const String UUID = "004f8154a800615565d1c35fca92f621"; 

const char* SYMBOLBTC = "BTC";
const char* SYMBOLETH = "ETH";
const char* CONVERT_TO = "usd";

int pricebtc, priceeth;
int TempValue;
//byte mydata.display, mode, modedots, modetime=0;//текущий режим отображение: часы, крипта, дата, температура 
byte anim=0;//текущая анимация Флип эффекты, поезд
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

static const uint8_t _segCharMap[] PROGMEM = {
  0,     //   0x20 32
  0x02,  // ! 0x21 33
  0x22,  // " 0x22 34
  0x36,  // # 0x23 35
  0x69,  // $ 0x24 36
  0x2D,  // % 0x25 37
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
};



char buffer[100] PROGMEM;
char textbuffer[100] PROGMEM;
String lost= "WIFI lost please connect to 192-168-4-1     ";
byte dec_buffer[6];

//const byte cathodeMask[] = {1, 0, 2, 9, 8, 3, 4, 7, 6, 5}; // порядок катодов in14
// ************************** НАСТРОЙКИ **************************
#define DUTY 190        // скважность ШИМ. От скважности зависит напряжение! у меня 175 вольт при значении 180 и 145 вольт при 120

// ======================= ЭФФЕКТЫ =======================
// эффекты перелистывания часов
byte FLIP_EFFECT = 1;
// Выбранный активен при запуске и меняется кнопками
// 0 - нет эффекта
// 1 - плавное угасание и появление (рекомендуемая скорость: 100-150)
// 2 - перемотка по порядку числа (рекомендуемая скорость: 50-80)
// 3 - перемотка по порядку катодов в лампе (рекомендуемая скорость: 30-50)
// 4 - поезд (рекомендуемая скорость: 50-170)
// 5 - резинка (рекомендуемая скорость: 50-150)
byte FLIP_SPEED[] = {0, 20, 40, 100, 9, 100}; // скорость эффектов, мс (количество не меняй) 0, 130, 50, 40, 70, 70

// эффекты подсветки
byte BACKL_MODE = 0;
// Выбранный активен при запуске и меняется кнопками
// 0 - выключена
// 1 - постоянный свет
// 2 - палитра PartyColors
// 3 - палитра RainbowStripeColors
// 4 - палитра CloudColors

// =======================  ЯРКОСТЬ =======================
#define NIGHT_START 22      // час перехода на ночную подсветку (BRIGHT_N)
#define NIGHT_END 8         // час перехода на дневную подсветку (BRIGHT)

#define INDI_BRIGHT 24      // яркость цифр дневная (1 - 24) !на 24 могут быть фантомные цифры!
#define INDI_BRIGHT_N 2     // яркость ночная (1 - 24)

#define DOT_BRIGHT 40       // яркость точки дневная (1 - 255)
#define DOT_BRIGHT_N 30     // яркость точки ночная (1 - 255)

#define BACKL_BRIGHT 120    // макс. яркость подсветки дневная (0 - 255)
#define BACKL_BRIGHT_N 90   // макс. яркость подсветки ночная (0 - 255, 0 - подсветка выключена)

// =======================  ГЛЮКИ =======================
#define GLITCH_MIN 3//30       // минимальное время между глюками, с
#define GLITCH_MAX 6//120      // максимальное время между глюками, с

#define train_MIN 20       // минимальное время между поездом, с 180
#define train_MAX 40      // максимальное время между поездом, с 300

#define test_MIN 180      // минимальное время между перебором индикаторов, с 300
#define test_MAX 300      // максимальное время между перебором индикаторов, с 600

// ======================  МИГАНИЕ =======================
#define DOT_TIME 500        // время мигания точки, мс
#define DOT_TIMER 20        // шаг яркости точки, мс

// ==================  АНТИОТРАВЛЕНИЕ ====================
#define BURN_TIME 10        // период обхода индикаторов в режиме очистки, мс
#define BURN_LOOPS 3        // количество циклов очистки за каждый период
#define BURN_PERIOD 15      // период антиотравления, минут
// *********************** ДЛЯ РАЗРАБОТЧИКОВ ***********************
byte FLIP_EFFECT_NUM = 4;//sizeof(FLIP_SPEED);   // количество эффектов
//boolean flipIndics[6];
//byte newTime[6];
unsigned long previousMillis;

timerMinim dotTimer(1000);                      // полсекундный таймер для часов
//timerMinim dotBrightTimer(60);          // таймер шага яркости точки
//timerMinim ReadTime(100);                // полсекундный таймер для часов
timerMinim secondtimer(2000); //Таймер для смены эффектов
timerMinim timerTIME(40);
timerMinim Datetimer(5000); //Таймер для вывода даты
timerMinim dotBrightTimer(DOT_TIMER);    // таймер шага яркости точки
timerMinim backlBrightTimer(30);         // таймер шага яркости подсветки
//timerMinim almTimer((long)ALM_TIMEOUT * 1000); // таймер работы будильника 30сек если ALM_TIMEOUT=30
timerMinim flipTimer(FLIP_SPEED[FLIP_EFFECT]); // таймер резинки
timerMinim glitchTimer(1000); //таймер для глюков
timerMinim blinkTimer(250); // таймер мигания цифры в настройках
timerMinim ligtSensorTimer(100); // таймер мигания цифры в настройках
timerMinim RandomTimer(150);
timerMinim TestTimer(3000); //  тестирование индикаторов. 3000
timerMinim trainTimer(300); // и для поезда
timerMinim modeTimer((long)60 * 1000);
timerMinim modeTimerP((long)202 * 1000);
timerMinim SensorTimerI2C(3000); // Время обновления датчиков и яркости индикаторов
timerMinim DotTimer(1000);//Таймер для переключение точек
timerMinim DotRandomTimer(60000);//Таймер для переключение точек
timerMinim mooveNixie(300);//движение массива



volatile int8_t indiDimm[24];      // величина диммирования (0-24)
volatile int8_t indiCounter[6];   // счётчик каждого индикатора (0-24)
volatile int8_t indiDigits[6];    // цифры, которые должны показать индикаторы (0-10)
volatile int8_t curIndi;          // текущий индикатор (0-5)
boolean dotFlag, TrainFlag = true, TestFlag = true; //!
//int8_t hrs, mins, secs, mnth, days, year, weekdays;
//int8_t alm_hrs, alm_mins;
//int8_t mode = Clock;    // 0 часы, 1 температура, 2 настройка будильника, 3 настройка часов, 4 аларм
boolean changeFlag;
boolean blinkFlag;
boolean minus;
byte indiMaxBright = INDI_BRIGHT, dotMaxBright = DOT_BRIGHT, backlMaxBright = BACKL_BRIGHT;
boolean alm_flag = 0;
boolean dotBrightFlag, dotBrightDirection, backlBrightFlag, backlBrightDirection, indiBrightDirection;
int dotBrightCounter, backlBrightCounter, indiBrightCounter, indiBrightCounterMinus, indiBrightCounterPlus;
byte dotBrightStep;
boolean newTimeFlag = true;
boolean flipFlag = true;
boolean flipIndics[6];
int32_t newTime[6];
int32_t Bufer[6];
byte oldTime[6];
boolean flipInit, flipInit4;
byte startCathode[4], endCathode[4];
byte glitchCounter, glitchMax, glitchIndic;
boolean glitchFlag, indiState;
byte curMode = 0;
byte currentDigit = 0;
int8_t changeHrs, changeMins, changeSecs,  changeDays, changeMonth, changeYear;
boolean lampState = false;
boolean anodeStates[] = {1, 1, 1, 1, 1, 1};
boolean dotsFlag = false;
//////////////////////////
boolean timeon = true;//Вспомогательное булевое значение для включение анимации точек
byte dmoove = 0;
boolean dmooveright = true;
boolean dmooveleft = false;
/////////////////////////
byte RandomOnOff[6] = {0, 1, 2, 3, 4, 5};//Храним вычислиные значение порядка появления анодов
byte lightValue[4] = {21, 28, 24, 21};
byte brightValue[4] = {2,10,18,24};
byte is,js;
boolean RandomAnodesFlag, RandomDirection=true;
boolean RandomAnodesOn=false;
byte currentLamp, flipEffectStages, testIndicator;
byte CounterAn=0;
bool trainLeaving = true;
uint8_t colorIndex;
unsigned long effect_timer;
boolean ZoneCounter = false; // для "перевода часов"
int8_t LampDots;
byte DotsLR=1;
byte k = 1;
boolean DotRun, DotDirection = false;
boolean TrainOn = false;
boolean TrainDirection = true;
byte al,bl,cl;//борьба с дребезгом сенсора
int vemlvalue, vemllux;
float bmevalue, bmehumudity, bmepressure, bmetemperature, altitude;

// переменные
int valNum;
String valPass;
int valSlider;
float valSpin;
GPdate valDate;
GPtime valTime;
GPcolor valCol;
int valSelect;
byte valRad, valDot, valSec;

byte Counter = 6;