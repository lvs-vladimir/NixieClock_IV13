#include <Arduino.h>
#include <setting.h>

//Рассчет md5 ключа api для narodmon обязательный параметр для GET запроса
void md5(char * apimd5) {
  unsigned char * hash = MD5::make_hash(apimd5);
  char * md5str = MD5::make_digest(hash, 16);
  sprintf_P(mydata.NarodmoonApiMD5, (PGM_P) F("%S"), md5str);
}
//Функция возврата десятичного представления символа
uint8_t getCharCode(char symb) {
  if (symb < 32 || symb > 176) return 0;
  if (symb >= 97 && symb <= 175) {
    if (symb <= 122) symb -= 32; // to lowercase
    else symb -= 26; // 123..126       
  }
  if (symb == 176) symb -= 75;
  return pgm_read_byte(_segCharMap + (symb - 32));
}

void TimeUpdate() {
  timeClient.update();
  if (timeClient.getHours() > 0 || (timeClient.getMinutes() > 0 && timeClient.getSeconds() > 0)) {
    hour = timeClient.getHours();
    minute = timeClient.getMinutes();
    second = timeClient.getSeconds();
  }
}
void ValueTempUpdate() {
  if (mydata.displaytemperature == 0) TempValue = bmetemperature;
  if (mydata.displaytemperature == 1) TempValue = narodtemperature;
  if (mydata.displaytemperature == 2) TempValue = optemperature;
}
//обновляем часовой пояс 
void NTPClientUpdate() {
  timeClient.setPoolServerName(mydata.NTPserver);
  timeClient.setTimeOffset(3600 * mydata.GMT);
  TimeUpdate();
}

String httpGETRequest(const char * serverName) {
  HTTPClient http;
  String a;
  http.begin(serverName);
  //if (!http.connected()) return a;
  int httpResponseCode = http.GET();
  
  String payload = "{}";
  if (httpResponseCode == HTTP_CODE_OK) {
    payload = http.getString();
  } else { 
    http.end();
   return a;
  }
  // Free resources
  http.end();
  return payload;
}

#include "getCrypto.h"
#include "getTemp.h"
#include "calcTime.h"

bool initWiFi() { //Функция инициализации wifi
  if (mydata.ssid == "") { //|| ip==""
    Serial.println("Не определен SSID ");
    return false;
  }

  WiFi.mode(WIFI_STA);
  // localIP.fromString(ip.c_str());
  // localGateway.fromString(gateway.c_str());

  /*
    if (!WiFi.config(localIP, localGateway, subnet)){
      Serial.println("STA Failed to configure");
      return false;
    }
  */
  WiFi.begin(mydata.ssid, mydata.pass);
  Serial.println("Подключаемся к WiFi...");

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;
  //Если в течении 10сек нет подключения к вайфай создаем точку доступа
  while (WiFi.status() != WL_CONNECTED) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= 10000) {
      Serial.println("Ошибка подключения к WIFI");
      return false;
    }
  }
  Serial.println(WiFi.localIP());
  return true;
}
void ConnectionToServices() {
  timeClient.begin();
  NTPClientUpdate();
  if (strlen(mydata.owMapApiKey) > 0 && strlen(mydata.owCity) > 0) getTemp2(0);
  getCrypto();
  if (strlen(mydata.NarodmoonApi) > 0 && strlen(mydata.NarodmoonID) > 0) getTemp2(1);
}
//Инициализация WiFI
void WiFiConnect_APcreate() {
  if (initWiFi()) {
    Serial.println("WiFi успешно подключен");
    //подключаемся к сервисам
    ConnectionToServices();
  } else {
    // запускаем точку доступа если нет подключения
    Serial.println("WiFi не подключен, создаем точку");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("IV13-CONNECT");
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    timerTIME.stop();
    //mydata.display=4;

    //bufferer=text2.c_str();
    //text2.toCharArray(bufferer, sizeof(text2));
    //sprintf_P(textbuffer, (PGM_P)F("%S"), lost.c_str());//Заносим в буфер

    Serial.println(lost.length());
  }
}

void OtaUpdate() {
  //Обновление по воздуху   
  ArduinoOTA.setHostname("ESP32");

  ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //Обновление по воздуху 
}
//переводим символы из char в десятичные
void set_dec_buffer() {
  for (byte i = 0; i <= 5; i++) {
    dec_buffer[i] = getCharCode(buffer[i]);
  }
}

void DisplayTimeUpdate() {
  if (mydata.seconds_switch) {
    sprintf_P(textbuffer, (PGM_P) F("%02d%02d%02d"), hour, minute, second);
  } else {
    // numbers+=hour;
    // numbers+=minute;
    // int len=numbers.length()+1;
    // numbers.toCharArray(textbuffer, len);
    sprintf_P(textbuffer, (PGM_P) F(" %02d%02d "), hour, minute);
  }
}

void AutoShowUpdate(byte i) {
  memset(textbuffer, 0, sizeof(textbuffer)); //очистка буфера
  if (GPlistIdx(SensorsAutoShowSelect2, mydata.autoshow_select[i]).length() == 0) {
    mydata.display = 0;
    DisplayTimeUpdate();
    timerTIME.stop();
    timerTIME.setInterval(40);
    // Serial.print("Reset"); Serial.println(textbuffer);
    return;
  }
  if (GPlistIdx(SensorsAutoShowSelect2, mydata.autoshow_select[i]).length() <= 3) {
    sprintf_P(textbuffer, (PGM_P) F("  %S"), GPlistIdx(SensorsAutoShowSelect2, mydata.autoshow_select[i]).c_str());
  } else if (GPlistIdx(SensorsAutoShowSelect2, mydata.autoshow_select[i]).length() == 4) {
    sprintf_P(textbuffer, (PGM_P) F(" %S"), GPlistIdx(SensorsAutoShowSelect2, mydata.autoshow_select[i]).c_str());
  } else sprintf_P(textbuffer, (PGM_P) F("%S"), GPlistIdx(SensorsAutoShowSelect2, mydata.autoshow_select[i]).c_str());

  // set_dec_buffer();
}

void RunDots() {
  //Случайное переключение точек раз в минуту
  if (mydata.dots_switch == true && DotRandomTimer.isReady()) mydata.animdots = random(0, 2);
  byte first, last;
  if (mydata.seconds_switch) {first=0; last=5;}
  else {first=1; last=4;}
  switch (mydata.animdots) {
  case 0:
    
    //Первый режим последовательное переключение точек по лампам вперед и обратно
    if ((mydata.display == 0) && timeon) {
      if (DotTimer.isReady()) {
        if (dmooveright) dmoove++;
        if (dmooveleft) dmoove--;
        if (dmoove >= last) {
          dmoove = last;
          dmooveleft = true;
          dmooveright = false;
        }
        if (dmoove <= first) {
          dmoove = first;
          dmooveleft = false;
          dmooveright = true;
        }
      }
      dec_buffer[dmoove] |= 0b00001000;
    }
    break;

  case 1:
    //Второй режим одновременное мигание в 3 и 4 индикаторе при увеличении на 1 сек
    if ((mydata.display == 0) && timeon) {
      if (second & 0x01) { //если не четная секунда
        dec_buffer[3] |= 0b00001000;
        dec_buffer[2] |= 0b00001000;
      }
    }
    break;
  case 2:
    //Третий режим попеременное мигание в 3 и 4 индикаторе при увеличении на 1 сек
    if ((mydata.display == 0) && timeon) {
      if (second & 0x01) dec_buffer[2] |= 0b00001000;
      else dec_buffer[3] |= 0b00001000;
    }
    break;
  }

}

// Отпраляем данные по SPI на tpic6b595
void UpdateDisplay() {
  set_dec_buffer();
  RunDots();

  hspi -> beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE3));

  int8_t i = 5;
  while (i > -1) {
    hspi -> transfer(dec_buffer[i]);
    i--;
  }

  hspi -> endTransaction();
  CS_ON_HSPI;
  CS_OFF_HSPI;
}

void AutoBrightness() {
  vemllux = veml.readLux();
  vemlvalue = vemllux;
  //Устанавливаем яркость IV13 (подбор на глаз) в зависимости от уровня освещения
  //Так как яркость на нуметронах меняется не линейно, то меняем ее путем подбора
  //int с = (a > b) ? 10 : -20;
  //unsigned int brightnessIV13; 
  if (vemlvalue > 900) brightnessIV13 = map(vemlvalue, 900, 1000, 99, 0);
  if (vemlvalue < 899 && vemlvalue > 700) brightnessIV13 = map(vemlvalue, 700, 899, 199, 100); //brightnessIV13=100;//90%
  if (vemlvalue < 699 && vemlvalue > 300) brightnessIV13 = map(vemlvalue, 300, 699, 249, 200); //brightnessIV13=250;//80%
  if (vemlvalue < 299 && vemlvalue > 100) brightnessIV13 = map(vemlvalue, 100, 299, 499, 250); //brightnessIV13=500;//50%
  if (vemlvalue < 99 && vemlvalue > 50) brightnessIV13 = map(vemlvalue, 50, 98, 599, 500); //brightnessIV13=600;//30%
  if (vemlvalue < 49 && vemlvalue > 15) brightnessIV13 = map(vemlvalue, 15, 48, 749, 600); //brightnessIV13=750;//10%
  if (vemlvalue < 14 && vemlvalue >= 0) brightnessIV13 = map(vemlvalue, 0, 14, 800, 750); //brightnessIV13=800;//5%
  ledcWrite(PWM_CHANNEL, brightnessIV13); //brightnessIV13 = map(vemllux, 0, 800, 800, 0);
}

void DisplayTime() {
  switch (mydata.seconds_switch) {
  case true:
    //Переобразуем из в числа в char 
    sprintf_P(buffer, (PGM_P) F("%02d%02d%02d"), newhour, newminute, newsecond);
    break;
  case false:
    sprintf_P(buffer, (PGM_P) F(" %02d%02d "), newhour, newminute);
    break;
  }
  set_dec_buffer();
}

void Task0(void * pvParameters);
void Task1(void * pvParameters);

#include "webui.h"//Веб интерфейс
#include "setup.h"//Настройки
#include "task.h"//основная программа