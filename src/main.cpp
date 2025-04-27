#include <Arduino.h>
#include "SPI.h"

#include "timer2Minim.h"

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <Arduino_JSON.h>
#include <HTTPClient.h>
#include <WiFi.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BME280.h>
#include <Adafruit_VEML7700.h>
#include <GyverBME280.h> 
//#include <Wire.h>
//#include <WiFiClient.h>
//#include <WebServer.h>
//#include <ESPmDNS.h>
//#include <Update.h>

//BME280
//GyverBME280 bme;

//VEML7700
//TwoWire I2CBME = TwoWire(0);
Adafruit_VEML7700 veml = Adafruit_VEML7700();
//Adafruit_BME280 bme;
GyverBME280 bme;

TaskHandle_t Task_0;
TaskHandle_t Task_1;


//Openweather 
String openWeatherMapApiKey = "d5489beb80bf49ffbe818eb9a8eb261c";
String city = "Barnaul";
String countryCode = "RU";

char ssid[] = "WAY";//WAY2G       // your network SSID (name)
char password[] = "lukjanow";  // your network key

#include "html.h"

//Список часовых поясов  https://en.wikipedia.org/wiki/List_of_UTC_time_offsets
const long utcOffsetInSeconds = 25200; //UTC +7 в секундах Время Барнаул

// Определение NTP-клиента для получения времени
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

//Ключ API pro coinmarketcup.com
const String API_KEY = "68b5269b-464d-46d8-90fe-bcbbb262cf46";

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
byte disp=0;//текущий режим отображение: часы, крипта, дата, температура 
byte anim=0;//текущая анимация Флип эффекты, поезд
int num, num1, num2, num3, num4, num5,num6;

//WiFiClientSecure httpsClient;    
//WiFiClientSecure client;
//CoinMarketCapApi api(client);



SPIClass * hspi = NULL;

#define CS_ON_HSPI (GPIO.out_w1ts = ((uint32_t)1 << 15))  
#define CS_OFF_HSPI (GPIO.out_w1tc = ((uint32_t)1 << 15))

#define G 16

#define HSPI_MISO   12
#define HSPI_MOSI   13
#define HSPI_SCLK   14
#define HSPI_SS     15


const int PWM_CHANNEL = 0;    // ESP32 имеет 16 каналов, которые могут генерировать 16 независимых сигналов.
const int PWM_FREQ = 15000;     // Официальный пример ESP32 использует частоту 5000 Гц.
const int PWM_RESOLUTION = 10; // Мы будем использовать то же разрешение, что и Uno (8 бит, 0-255), но ESP32 может достигать 16 бит.

// Максимальное значение рабочего цикла, основанное на разрешении ШИМ (будет 255, если разрешение составляет 8 бит)
const int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1);
const int LED_OUTPUT_PIN = 16;

byte second, minute, hour,newsecond, newminute, newhour, dayOfWeek, day, dayOfMonth, month, year, minsCount, hourCount, cryptoCount;
boolean updateCrypto = true;  


int32_t numbersArrayIv13[]{
  0b11100111, //0
  0b00000110, //1
  0b01110011, //2
  0b01010111, //3
  0b10010110, //4
  0b11010101, //5
  0b11110101, //6
  0b00000111, //7
  0b11110111, //8
  0b11010111,//9
  0b00000000, //10 чисто
  0b00001000, //11 точка
  0b11100001, //12 С
  0b10010011, //13 Градус
  0b00010000  //14 Минус
};


const byte cathodeMask[] = {1, 0, 2, 9, 8, 3, 4, 7, 6, 5}; // порядок катодов in14
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
timerMinim SensorTimerI2C(5000); //  тестирование индикаторов. 3000





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
int vemlvalue;
float bmelvalue;
byte decToBcd(byte val) {
  return ( (val / 10 * 16) + (val % 10) );
} 
byte bcdToDec(byte val) {
  return ( (val / 16 * 10) + (val % 16) );
}


// отправить на индикаторы
void sendTime(byte hours, byte minutes, byte seconds) {

  indiDigits[0] = (byte)hours / 10;
  indiDigits[1] = (byte)hours % 10;

  indiDigits[4] = (byte)seconds / 10;
  indiDigits[5] = (byte)seconds % 10;
  
  indiDigits[2] = (byte)minutes / 10;
  indiDigits[3] = (byte)minutes % 10;
}

void setNewTime() {
  newTime[0] = newhour / 10;
  newTime[1] = newhour % 10;
  newTime[2] = newminute / 10;
  newTime[3] = newminute % 10;
  newTime[4] = newsecond / 10;
  newTime[5] = newsecond % 10;
}

void TimeUpdate(){
    timeClient.update();
    if (timeClient.getHours()>0||(timeClient.getMinutes()>0&&timeClient.getSeconds()>0)){
    hour = timeClient.getHours();
    minute = timeClient.getMinutes();
    second = timeClient.getSeconds();
  
 
    }
}

String httpGETRequest(const char* serverName) {
  HTTPClient http;

  // Your IP address with path or Domain name with URL path 
  http.begin(serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}"; 

  if (httpResponseCode>0) {
   // Serial.print("HTTP Response code: ");
   // Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
   // Serial.print("Error code: ");
   // Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

//#include "sensor.h"
#include "getCrypto.h"
#include "getTemp.h"
#include "calcTime.h"
//#include "effects.h"

// динамическая индикация 
void UpdateDisplay() {
/*
  NewTime[5] = sec % 10;
  //NewTime[4] = sec % 100 / 10;
  NewTime[4] = sec / 10;
  NewTime[3] = min % 10;
  NewTime[2] = min / 10;
  NewTime[1] = hor % 10;
  NewTime[0] = hor / 10;
*/  

hspi->beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE3));
CS_OFF_HSPI;

//for(byte i=6; i>=0; i--){
hspi->transfer(numbersArrayIv13[newTime[5]]);
hspi->transfer(numbersArrayIv13[newTime[4]]);
hspi->transfer(numbersArrayIv13[newTime[3]]);
hspi->transfer(numbersArrayIv13[newTime[2]]);
hspi->transfer(numbersArrayIv13[newTime[1]]);
hspi->transfer(numbersArrayIv13[newTime[0]]);
//}

hspi->endTransaction();
CS_ON_HSPI;
//for (int i=0; i < 6; i++) newTime[i]=10;
}

void Task0( void *pvParameters );
void Task1( void *pvParameters );

#include "setup.h"

#include "loop.h"

#include "task.h"