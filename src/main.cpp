#include <Arduino.h>
#include <setting.h>

uint8_t getCharCode(char symb) {
  if (symb < 32 || symb > 126) return 0;
  if (symb >= 97) {
      if (symb <= 122) symb -= 32;  // to lowercase
      else symb -= 26;              // 123..126
  }
  return pgm_read_byte(_segCharMap + (symb - 32));
}

void TimeUpdate(){
    timeClient.update();
    if (timeClient.getHours()>0||(timeClient.getMinutes()>0&&timeClient.getSeconds()>0)){
    hour = timeClient.getHours();
    minute = timeClient.getMinutes();
    second = timeClient.getSeconds();
    }
}

//обновляем часовой пояс 
void NTPClientUpdate(){
  utcOffsetInSeconds=3600*mydata.GMT;
  timeClient.setPoolServerName(mydata.NTPserver);
  timeClient.setTimeOffset(utcOffsetInSeconds);
  TimeUpdate();
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

#include "getCrypto.h"
#include "getTemp.h"
#include "calcTime.h"

bool initWiFi() { //Функция инициализации wifi
  if(mydata.ssid==""){//|| ip==""
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
  while(WiFi.status() != WL_CONNECTED) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= 100) {
      Serial.println("Ошибка подключения к WIFI");
      return false;
    }
  }
  Serial.println(WiFi.localIP());
  return true;
}
void ConnectionToServices(){
  timeClient.begin();
  NTPClientUpdate();
  getTemp();
  getCrypto();
  }
//Инициализация WiFI
void WiFiConnect_APcreate(){
if(initWiFi()) {
  Serial.print ("WiFi успешно подключен");
  //подключаемся к сервисам
  ConnectionToServices();
}
else{
// запускаем точку доступа если нет подключения
  Serial.println ("WiFi не подключен, создаем точку");
  WiFi.mode(WIFI_AP);
  WiFi.softAP("IV13-CONNECT");
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP); 
  timerTIME.stop();
  mydata.display=4;
 
  //bufferer=text2.c_str();
  //text2.toCharArray(bufferer, sizeof(text2));
  sprintf_P(textbuffer, (PGM_P)F("%S"), lost.c_str());//Заносим в буфер

  Serial.println(lost.length());
}
}

void OtaUpdate(){
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
  for(byte i=0; i<=5; i++){
  dec_buffer[i] = getCharCode(buffer[i]); 
  }
}

void RunDots(){
//Случайное переключение точек раз в минуту
if (mydata.sw1==true&&DotRandomTimer.isReady()) mydata.modedots = random(0, 2);

switch (mydata.modedots)
{
case 0:
//Первый режим последовательное переключение точек по лампам вперед и обратно
if ((mydata.display == 0) && timeon)
{
  if (DotTimer.isReady())
  {
    if (dmooveright) dmoove++;
    if (dmooveleft)  dmoove--;
    if (dmoove >= 5)
    {
      dmoove = 5;
      dmooveleft = true;
      dmooveright = false;
    }
    if (dmoove <= 0)
    {
      dmoove = 0;
      dmooveleft = false;
      dmooveright = true;
    }
  }
  dec_buffer[dmoove]|=0b00001000;
}
  break;

case 1:
//Второй режим одновременное мигание в 3 и 4 индикаторе при увеличении на 1 сек
if ((mydata.display == 0) && timeon)
{
    if (second & 0x01){//если не четная секунда
    dec_buffer[3]|=0b00001000;
    dec_buffer[2]|=0b00001000;
    }
}
  break;
  case 2:
//Третий режим попеременное мигание в 3 и 4 индикаторе при увеличении на 1 сек
if ((mydata.display == 0) && timeon)
{
    if (second & 0x01) dec_buffer[2]|=0b00001000;
    else dec_buffer[3]|=0b00001000;
}
  break;
}

}

// Отпраляем данные по SPI на tpic6b595
void UpdateDisplay() {
  RunDots();
  
hspi->beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE3));
CS_OFF_HSPI;

for(byte i=5; i>0; i--){
  hspi->transfer(dec_buffer[i]);
}
hspi->endTransaction();
CS_ON_HSPI;

}

void Task0( void *pvParameters );
void Task1( void *pvParameters );


#include "webui.h"//Веб интерфейс
#include "setup.h"

#include "loop.h"

#include "task.h"