#include <lang.h>
// конструктор страницы
void build() {

    GP.BUILD_BEGIN(400);
    GP.THEME(GP_DARK);
    GP.TITLE(DEVICE_NAME[mydata.lng]);//Заголовок
    GP.HR(); //разделительная линия 
    //динамическое обновленеие данных на странице из переменных
    GP.UPDATE("temperature,humudity,pressure,altitude,lux,timesystem,btc,eth,tempsystem"); 
    //Кликабельный страницы с обновлением
    GP.NAV_TABS_LINKS("/,/setting,/info,/firmware", TAB_LINKS_NAMES[mydata.lng], GP_BLUE);
    
// страница setting
    if (ui.uri("/setting")) {
  GP.FORM_BEGIN("/setting");
//Настройка NTP сервера********************************************
  GP.BLOCK_THIN_BEGIN();
    M_BOX(GP_CENTER, GP.LABEL(SETTING_NTP_NAME[mydata.lng]);); 
    GP.HR(); 
    GP.TEXT("ntp", "Ntp", mydata.NTPserver); 
    M_BOX(GP_CENTER, GP.BUTTON_MINI("sntp_btn", SETTING_NTP_SYNC_BTN[mydata.lng], "", GP_BLUE, "", 0, 1); );
    //GP.BREAK(); //Делает отступ
    GP.BLOCK_END();

  //Подключение к WiFi***********************************************
  GP.BLOCK_THIN_BEGIN();
  M_BOX(GP_CENTER, GP.LABEL(SETTING_NETWORK_CONNECTION[mydata.lng]);); 
  GP.HR(); 
  M_BOX(GP_LEFT, GP.LABEL(SETTING_NETWORK_WIFI_NAME[mydata.lng]);); 
  GP.TEXT("lg", "Login", mydata.ssid);
  GP.BREAK();
  M_BOX(GP_LEFT, GP.LABEL(SETTING_NETWORK_WIFI_PASSWORD[mydata.lng]););
  GP.TEXT("ps", "Password", mydata.pass); 
  GP.BREAK();
  M_BOX(GP_CENTER, GP.BUTTON_MINI("wifi_btn", SETTING_WIFI_CONNECT_BTN[mydata.lng], "", GP_BLUE, "", 0, 1); );
 // GP.SUBMIT("Submit", GP_BLUE);
 // GP.FORM_END();
  GP.BLOCK_END();

//Подключение к Openweather********************************************
  GP.BLOCK_THIN_BEGIN();
  M_BOX(GP_CENTER, GP.LABEL(SETTING_OP_NAME[mydata.lng]);); 
  GP.HR(); 
  M_BOX(GP_LEFT, GP.LABEL(SETTING_OP_APIKEY[mydata.lng]);); 
  GP.TEXT("ap", "ApiKey", mydata.owMapApiKey);
  GP.BREAK();
  M_BOX(GP_LEFT, GP.LABEL(SETTING_OP_CITY[mydata.lng]););
  GP.TEXT("ct", "City", mydata.owCity);
  M_BOX(GP_CENTER, GP.BUTTON_MINI("op_btn", SETTING_OP_SYNC_BTN[mydata.lng], "", GP_BLUE, "", 0, 1); );
  GP.BLOCK_END();

//Выбор Языка***********************************************************
  GP.BLOCK_THIN_BEGIN();
  M_BOX(GP_LEFT, GP.LABEL(SETTING_LANGUAGE[mydata.lng]); 
  M_BOX(GP_RIGHT, GP.SELECT("lng", SETTING_LANGUAGE_ARRAY, mydata.lng, 0);); GP.BREAK(););   
  GP.BLOCK_END();

  GP.BUTTON_MINI("rst", "Restart", "", GP_BLUE, "", 0, 1);
  //GP.SUBMIT(SETTING_SAVE_BUTTON[mydata.lng], GP_BLUE);
  GP.FORM_END();

  
      // страница INFO
    } else if (ui.uri("/info")) {
        GP.SYSTEM_INFO("1.0");
  
      // страница firmware
    } else if (ui.uri("/firmware")) {
        GP.BLOCK_THIN_BEGIN();
        GP.OTA_FIRMWARE("OTA firmware", GP_BLUE);
        GP.OTA_FILESYSTEM("OTA filesystem", GP_BLUE); 
        GP.BLOCK_END();
  
      // главная страница, корень, "/"
    } else {
    //Начало вкладки HOME
    GP.BLOCK_THIN_BEGIN();
    GP.HR(); 

    M_BOX(GP_LEFT, GP.LABEL(SETTING_TIMEZONE_NAME[mydata.lng]); 
    M_BOX(GP_RIGHT,GP.SELECT("timeZone", SETTING_TIMEZONE_GMT, mydata.GMT + 12, 0);); GP.BREAK(););  
    GP.BLOCK_END();
    //Блок для вывода BME280
  GP.BLOCK_THIN_BEGIN();
  M_BOX(GP_CENTER, GP.LABEL("BME280");); 
  GP.HR(); 
  M_BOX(GP_LEFT, GP.LABEL("Температура:"); GP.LABEL(" ", "temperature"); GP.LABEL("*C"); GP.BREAK(););
  M_BOX(GP_LEFT, GP.LABEL("Влажность:"); GP.LABEL(" ", "humudity"); GP.LABEL("%"); GP.BREAK(););
  M_BOX(GP_LEFT, GP.LABEL("Давление:"); GP.LABEL(" ", "pressure"); GP.LABEL("mm Hg"); GP.BREAK(););
  M_BOX(GP_LEFT, GP.LABEL("Высота:"); GP.LABEL(" ", "altitude"); GP.LABEL("M"); GP.BREAK(););
  GP.BLOCK_END();

//Блок для вывода VEML7700
  GP.BLOCK_THIN_BEGIN();
  M_BOX(GP_CENTER, GP.LABEL("VEML7700");); 
  GP.HR(); 
  M_BOX(GP_LEFT, GP.LABEL("LUX:"); GP.LABEL(" ", "lux"); GP.BREAK(););
  GP.BLOCK_END();

  
     //Блок для вывода данных дисплея
  GP.BLOCK_THIN_BEGIN();
  M_BOX(GP_CENTER, GP.LABEL("Данные дисплея");); 
  GP.HR(); 
  M_BOX(GP_LEFT, GP.RADIO("rad", 0, mydata.mode, GP_BLUE);GP.LABEL("Время:"); GP.LABEL("", "timesystem"); GP.LABEL(" "); );
  M_BOX(GP_LEFT, GP.RADIO("rad", 1, mydata.mode, GP_BLUE);GP.LABEL("Биток:"); GP.LABEL("", "btc"); GP.LABEL("$"); );
  M_BOX(GP_LEFT, GP.RADIO("rad", 2, mydata.mode, GP_BLUE);GP.LABEL("Эфир:"); GP.LABEL("", "eth"); GP.LABEL("$"); );
  M_BOX(GP_LEFT, GP.RADIO("rad", 3, mydata.mode, GP_BLUE);GP.LABEL("Температура:"); GP.LABEL("", "tempsystem"); GP.LABEL("°C"););
  M_BOX(GP_LEFT, GP.RADIO("rad", 4, mydata.mode, GP_BLUE);GP.LABEL("Авто смена режимов");)
  GP.BLOCK_END();
  
  GP.BLOCK_THIN_BEGIN();
  M_BOX(GP_CENTER, GP.LABEL("Режим точек");); 
  GP.HR(); 
  M_BOX(GP_LEFT, GP.RADIO("dot", 0, mydata.modedots, GP_BLUE);GP.LABEL("1 последовательно"););
  M_BOX(GP_LEFT, GP.RADIO("dot", 1, mydata.modedots, GP_BLUE);GP.LABEL("2 одновременно"); );
  M_BOX(GP_LEFT, GP.RADIO("dot", 2, mydata.modedots, GP_BLUE);GP.LABEL("2 попеременно"); );
  //M_BOX(GP_LEFT, GP.RADIO("dot", 3, mydata.modedots, GP_BLUE);GP.LABEL("Режим 4"); );
  //M_BOX(GP_LEFT, GP.RADIO("dot", 4, mydata.modedots, GP_BLUE);GP.LABEL("Режим 5"); );
  GP.HR(); 

  M_BOX(GP_LEFT, GP.SWITCH("sw1", mydata.sw1, GP_BLUE); GP.LABEL("Случ. перек."););
  //M_BOX(GP_LEFT, GP.SWITCH("sw1", mydata.sw1, GP_BLUE););
  
  GP.BLOCK_END();
  GP.BLOCK_THIN_BEGIN();
  M_BOX(GP_CENTER, GP.LABEL("Отображение секунд");); 
  GP.HR(); 
  M_BOX(GP_LEFT, GP.RADIO("sec", 0, mydata.modetime, GP_BLUE);GP.LABEL("Вкл"););
  M_BOX(GP_LEFT, GP.RADIO("sec", 1, mydata.modetime, GP_BLUE);GP.LABEL("Выкл"); );
  GP.BLOCK_END();
  GP.TEXT("txt", "text", buffer);    GP.BREAK();
 // GP.PASS("pass", "pass", textbuff);   GP.BREAK();
  
    }
    GP.BUILD_END();
  }
    
  //Serial monitor
void action(GyverPortal& p) {
    if (p.form("/setting")) {      // кнопка нажата
        p.copyStr("lg", mydata.ssid);  // копируем себе
        p.copyStr("ps", mydata.pass);
        p.copyStr("ap", mydata.owMapApiKey);  // копируем себе
        p.copyStr("ct", mydata.owCity);
        p.copyStr("ntp",mydata.NTPserver);
        
        //EEPROM.put(0, mydata);              // сохраняем
        //EEPROM.commit();                // записываем    
       // EEPROM.put(60, mydata);              // сохраняем
       // EEPROM.commit();                // записываем
       // EEPROM.get(0, mydata);
        data.update();
       // data.updateNow();
      }
      if (p.form("/restart")) { 
        WiFi.softAPdisconnect(); // отключаем AP
        ESP.restart();
      }

      if (ui.click()) {
        if (ui.click("rst")) ESP.restart();//Перезагрузка по кнопке.
        if (ui.click("sntp_btn")) NTPClientUpdate();//Синхранизация NTP сервера
        if (ui.click("op_btn")) getTemp();//Синхранизация OP сервера
        if (ui.click("wifi_btn")) {//Подключение к WiFi
          WiFi.softAPdisconnect(); // отключаем AP
          WiFiConnect_APcreate(); // Подключаемся или создаем точку доступа
          ConnectionToServices(); //Синхронизируем время, погоду, крипту
        }
        if (ui.clickStr("ntp", mydata.NTPserver));  
        if (ui.clickStr("lg", mydata.ssid));   
        if (ui.clickStr("ps", mydata.pass)); 
        if (ui.clickStr("ap", mydata.owMapApiKey)); 
        if (ui.clickStr("ct", mydata.owCity));
        if (ui.clickStr("txt", buffer))  {
         // textbuff = textbuffinput;
        }
        if (ui.clickInt("rad", mydata.mode)){
          for (byte i=0; i<=4; i++) { 
            if (mydata.mode==i) {
              mydata.display=i;
              if (mydata.mode==4) mydata.display=0;
            }
          }
          //если автосмена режима то 
          
          /*
            if (valRad==0) {mydata.mode=0; mydata.modedots=0;}//Запускаем таймер для вывода Битка и эфира;
            if (valRad==1) {mydata.mode=1; }
            if (valRad==2) {mydata.mode=2; mydata.modedots=1;}
            if (valRad==3) mydata.mode=3;
            if (valRad==4) mydata.mode=4;
            */
        } 
        if (ui.click("timeZone")){
          mydata.GMT = ui.getInt("timeZone") - 12;
          Serial.println(mydata.GMT);
          NTPClientUpdate();
         
        }
        if (ui.click("lng")){
          mydata.lng = ui.getInt("lng");
          Serial.println(mydata.lng);   
        }
        
        if (ui.clickInt("dot", mydata.modedots)){
          /*
          if (valDot==0) {mydata.modedots=0;}//Запускаем таймер для вывода Битка и эфира;
          if (valDot==1) {mydata.modedots=1;}
          if (valDot==2) {mydata.modedots=2;}
          if (valDot==3) mydata.modedots=3;
          if (valDot==4) mydata.modedots=4;
          */
      } ;
      if (ui.clickInt("sec", mydata.modetime)){
        if (mydata.modetime==0); //{mydata.modetime=0;}//Запускаем таймер для вывода Битка и эфира;
        if (mydata.modetime==1); //{mydata.modetime=1;}
    } ;
       // if (ui.clickInt("rad1", valRad)) disp=0;
      // data.update();
       data.updateNow();
      }
/*
      if (p.form("/ow")) {      // кнопка нажата
        p.copyStr("ap", ow.owMapApiKey);  // копируем себе
        p.copyStr("ct", ow.owCity);
        EEPROM.put(60, ow);              // сохраняем
        EEPROM.commit();                // записываем
        EEPROM.get(60, ow);
        openWeatherMapApiKey=ow.owMapApiKey;
        city=ow.owCity;
      }
        */
    // было обновление
  if (ui.update()) {
    // 1. ищем, какой компонент запрашивает обновление
    // и вручную отправляем рандомное значение 
    if (ui.update("temperature")) ui.answer(bmetemperature);
    if (ui.update("humudity")) ui.answer(bmehumudity);
    if (ui.update("pressure")) ui.answer(bmepressure);
    if (ui.update("altitude")) ui.answer(altitude);
    if (ui.update("lux")) ui.answer(vemllux);
    char a[16];
    sprintf_P(a, (PGM_P)F("%02d:%02d:%02d"), hour, minute, second);
    if (ui.update("timesystem")) ui.answer(a);
    if (ui.update("btc")) ui.answer(pricebtc);
    if (ui.update("eth")) ui.answer(priceeth);
    if (ui.update("tempsystem")) ui.answer(TempValue);
    /*temperature,humudity,pressure,lux
    if (ui.update("t1")) ui.answer(random(100));
    //if (ui.update("t1")) ui.answer(bmetemperature);
    if (ui.update("lb")) ui.answer(random(1000));
    if (ui.update("lbb")) ui.answer(random(1000));
    if (ui.update("ch")) ui.answer(random(2));
    if (ui.update("led")) ui.answer(random(2));
    if (ui.update("sw")) ui.answer(random(2));
    if (ui.update("txt")) ui.answer(random(1000));

    // 2. автоматическое обновление из переменной
    ui.updateInt("num", bmetemperature);
    ui.updateString("pass", valPass);
    ui.updateInt("spn", valSpin);
    ui.updateInt("sld", valSlider);
    ui.updateDate("date", valDate);
    ui.updateTime("time", valTime);
    ui.updateColor("col", valCol);
    ui.updateInt("sel", valSelect);
    ui.updateInt("rad", valRad);
    */
}
    if (ui.click("btn")) {
      // отправляем обратно в "монитор" содержимое поля, оно пришло по клику кнопки
      ui.log.println(ui.getString("btn"));
    }
  }