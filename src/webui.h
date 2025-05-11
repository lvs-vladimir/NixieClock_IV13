#include <lang.h>
 // конструктор страницы
void build() {

  GP.BUILD_BEGIN(500);
  GP.THEME(GP_DARK);
  GP.TITLE(DEVICE_NAME[mydata.lng]); //Заголовок
  GP.HR(); //разделительная линия 
  //динамическое обновленеие данных на странице из переменных
  GP.UPDATE("temperature,humudity,pressure,altitude,lux,timesystem,btc,eth,tempsystem,timedisp,sens0,sens1,sens2,sens3,lg");
  //Кликабельный страницы с обновлением
  GP.NAV_TABS_LINKS("/,/setting,/info,/firmware", TAB_LINKS_NAMES[mydata.lng], GP_BLUE);

  // страница setting
  if (ui.uri("/setting")) {
    GP.FORM_BEGIN("/setting");
    //**************************************Настройка времени********************************************
    GP.BLOCK_THIN_BEGIN();
    M_BOX(GP_CENTER, GP.LABEL(SETTING_TIME_NAME[mydata.lng]););
    GP.HR();
    M_BOX(GP_LEFT, GP.LABEL(SETTING_NTP_NAME[mydata.lng]); M_BOX(GP_RIGHT, GP.TEXT("ntp", "Ntp", mydata.NTPserver, "100%");););
    M_BOX(GP_LEFT, GP.LABEL(SETTING_TIMEZONE_NAME[mydata.lng]); M_BOX(GP_RIGHT, GP.SELECT("timeZone", SETTING_TIMEZONE_GMT, mydata.GMT + 12, 0);););
    M_BOX(GP_CENTER, GP.BUTTON_MINI("sntp_btn", SETTING_NTP_SYNC_BTN[mydata.lng], "", GP_BLUE, "", 0, 1););
    //GP.BREAK(); //Делает отступ
    GP.BLOCK_END();

    //*****************************************Подключение к WiFi***********************************************
    GP.BLOCK_THIN_BEGIN();
    M_BOX(GP_CENTER, GP.LABEL(SETTING_NETWORK_CONNECTION[mydata.lng]););
    GP.HR();
    M_BOX(GP_LEFT, GP.BUTTON_MINI("scan_btn", SETTING_NETWORK_SCAN_BTN[mydata.lng], "", GP_BLUE, "", 0, 1); M_BOX(GP_RIGHT, GP.SELECT("WiFi_List_Select", WiFI_List, scan_list_idx, 0, 0, 0); ););
    GP.HR();
    M_BOX(GP_LEFT, GP.LABEL(SETTING_NETWORK_WIFI_NAME[mydata.lng]); M_BOX(GP_RIGHT, GP.TEXT("lg", "Login", mydata.ssid, "100%");););
    M_BOX(GP_LEFT, GP.LABEL(SETTING_NETWORK_WIFI_PASSWORD[mydata.lng]); M_BOX(GP_RIGHT, GP.TEXT("ps", "Password", mydata.pass, "100%");));
    GP.BREAK();
    M_BOX(GP_CENTER, GP.BUTTON_MINI("wifi_btn", SETTING_WIFI_CONNECT_BTN[mydata.lng], "", GP_BLUE, "", 0, 1););
    // GP.SUBMIT("Submit", GP_BLUE);
    // GP.FORM_END();
    GP.BLOCK_END();

    //*****************************************Подключение к Openweather********************************************
    GP.BLOCK_THIN_BEGIN();
    M_BOX(GP_CENTER, GP.LABEL(SETTING_OP_NAME[mydata.lng]););
    GP.HR();
    M_BOX(GP_LEFT, GP.LABEL(SETTING_OP_APIKEY[mydata.lng]); M_BOX(GP_RIGHT, GP.TEXT("ap", "ApiKey", mydata.owMapApiKey, "100%");););
    M_BOX(GP_LEFT, GP.LABEL(SETTING_OP_CITY[mydata.lng]); M_BOX(GP_RIGHT, GP.TEXT("ct", "City", mydata.owCity, "100%");););
    GP.BREAK();
    M_BOX(GP_CENTER, GP.BUTTON_MINI("op_btn", SETTING_OP_SYNC_BTN[mydata.lng], "", GP_BLUE, "", 0, 1););
    GP.BLOCK_END();
    //*********************************************Подключение к Narodmon********************************************
    GP.BLOCK_THIN_BEGIN();
    M_BOX(GP_CENTER, GP.LABEL(SETTING_NAROD_NAME[mydata.lng]););
    GP.HR();
    M_BOX(GP_LEFT, GP.LABEL(SETTING_NAROD_APIKEY[mydata.lng]); M_BOX(GP_RIGHT, GP.TEXT("api_narod", "ApiKey", mydata.NarodmoonApi, "100%");););
    M_BOX(GP_LEFT, GP.LABEL(SETTING_NAROD_ID[mydata.lng]); M_BOX(GP_RIGHT, GP.TEXT("id_narod", "ID", mydata.NarodmoonID, "45%", 4);););
    //GP.TEXT("", "", "", "", 0, "", false)
    GP.BREAK();
    M_BOX(GP_CENTER, GP.LABEL(SETTING_NAROD_SENSORS_LABEL[mydata.lng]););
    GP.HR();
    //GP.SPINNER(имя, значение, мин, макс, шаг, знаков, цвет, ширина, откл);  // + режим "только чтение" - true [bool] (умолч. false)
    //Прототип: GP.SPINNER("", 0, NAN, NAN, 1, 0, GP_GREEN, "", 0); GP.NUMBER("sens0_narod", "", mydata.nrd_sens0, "60px", false);
    M_BOX(GP_LEFT, GP.LABEL("", "sens0"); M_BOX(GP_RIGHT, GP.SELECT("type_sensor0", SETTING_NAROD_SENSORS[mydata.lng], mydata.nrd_type_sensor[0], 0, 0, 1); GP.SPINNER("sens0_narod", mydata.nrd_sens[0], 0, 20, 1, 0, GP_BLUE, "40px", 0);););
    M_BOX(GP_LEFT, GP.LABEL("", "sens1"); M_BOX(GP_RIGHT, GP.SELECT("type_sensor1", DATA_TYPE_SENSOR, mydata.nrd_type_sensor[1], 0, 0, 1); GP.SPINNER("sens1_narod", mydata.nrd_sens[1], 0, 20, 1, 0, GP_BLUE, "40px", 0);););
    M_BOX(GP_LEFT, GP.LABEL("", "sens2"); M_BOX(GP_RIGHT, GP.SELECT("type_sensor2", DATA_TYPE_SENSOR, mydata.nrd_type_sensor[2], 0, 0, 1); GP.SPINNER("sens2_narod", mydata.nrd_sens[2], 0, 20, 1, 0, GP_BLUE, "40px", 0);););
    M_BOX(GP_LEFT, GP.LABEL("", "sens3"); M_BOX(GP_RIGHT, GP.SELECT("type_sensor3", DATA_TYPE_SENSOR, mydata.nrd_type_sensor[3], 0, 0, 1); GP.SPINNER("sens3_narod", mydata.nrd_sens[3], 0, 20, 1, 0, GP_BLUE, "40px", 0);););
    //M_BOX(GP_LEFT, GP.LABEL(SETTING_NAROD_SENSOR_1[mydata.lng]); GP.LABEL(" ", "sens0"); GP.LABEL("°C"); M_BOX(GP_RIGHT, GP.SPINNER("sens0_narod", mydata.nrd_sens0, 0, 20, 1, 0, GP_BLUE, "50px", 0); ); );
    //M_BOX(GP_LEFT, GP.LABEL(SETTING_NAROD_SENSOR_2[mydata.lng]); GP.LABEL(" ", "sens1"); GP.LABEL("%"); M_BOX(GP_RIGHT, GP.SPINNER("sens1_narod", mydata.nrd_sens1, 0, 20, 1, 0, GP_BLUE, "50px", 0);); );
    //M_BOX(GP_LEFT, GP.LABEL(SETTING_NAROD_SENSOR_3[mydata.lng]); GP.LABEL(" ", "sens2"); GP.LABEL("mmHg"); M_BOX(GP_RIGHT, GP.SPINNER("sens2_narod", mydata.nrd_sens2, 0, 20, 1, 0, GP_BLUE, "50px", 0);); );
    M_BOX(GP_CENTER, GP.BUTTON_MINI("narod_btn", SETTING_NAROD_SYNC_BTN[mydata.lng], "", GP_BLUE, "", 0, 1););
    GP.BLOCK_END();
    //******************************************Выбор Языка***********************************************************
    GP.BLOCK_THIN_BEGIN();
    M_BOX(GP_LEFT, GP.LABEL(SETTING_LANGUAGE[mydata.lng]); M_BOX(GP_RIGHT, GP.SELECT("lng", SETTING_LANGUAGE_ARRAY, mydata.lng, 0, 0, 1);); GP.BREAK(););
    GP.BLOCK_END();

    GP.BUTTON_MINI("rst", SETTING_RESTART_BTN[mydata.lng], "", GP_BLUE, "", 0, 1);
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
    //*******************Начало вкладки HOME***********************************************************
    //Блок для вывода BME280
    GP.BLOCK_THIN_BEGIN();
    M_BOX(GP_CENTER, GP.LABEL("BME280"););
    GP.HR();
    M_BOX(GP_LEFT, GP.LABEL("Температура:"); GP.LABEL(" ", "temperature"); GP.LABEL("°C"); GP.BREAK(););
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
    M_BOX(GP_CENTER, GP.LABEL(DISPLAY_DATA_LABEL[mydata.lng]););

    GP.HR();
    M_BOX(GP_LEFT, GP.LABEL(DISPLAY_DATA_SHOW_SWITCH[mydata.lng]); M_BOX(GP_RIGHT, GP.SWITCH("auto_show_switch", mydata.autoshow_switch, GP_BLUE); GP.SPINNER("autoshow_min", mydata.autoshow_min, 0, 20, 1, 0, GP_BLUE, "50px", 0);););
    GP.BREAK();
    M_BOX(GP_LEFT, GP.LABEL("1:"); GP.SELECT("s_a_s0", SensorsAutoShowSelect2, mydata.autoshow_select[0], 0, 0, 1); M_BOX(GP_RIGHT, GP.SPINNER("autoshow_select_sec0", mydata.autoshow_select_sec[1], 0, 30, 1, 0, GP_BLUE, "50px", 0);););
    M_BOX(GP_LEFT, GP.LABEL("2:"); GP.SELECT("s_a_s1", SensorsAutoShowSelect2, mydata.autoshow_select[1], 0, 0, 1); M_BOX(GP_RIGHT, GP.SPINNER("autoshow_select_sec1", mydata.autoshow_select_sec[2], 0, 30, 1, 0, GP_BLUE, "50px", 0);););
    M_BOX(GP_LEFT, GP.LABEL("3:"); GP.SELECT("s_a_s2", SensorsAutoShowSelect2, mydata.autoshow_select[2], 0, 0, 1); M_BOX(GP_RIGHT, GP.SPINNER("autoshow_select_sec2", mydata.autoshow_select_sec[3], 0, 30, 1, 0, GP_BLUE, "50px", 0);););
    M_BOX(GP_LEFT, GP.LABEL("4:"); GP.SELECT("s_a_s3", SensorsAutoShowSelect2, mydata.autoshow_select[3], 0, 0, 1); M_BOX(GP_RIGHT, GP.SPINNER("autoshow_select_sec3", mydata.autoshow_select_sec[4], 0, 30, 1, 0, GP_BLUE, "50px", 0);););
    M_BOX(GP_LEFT, GP.LABEL("5:"); GP.SELECT("s_a_s4", SensorsAutoShowSelect2, mydata.autoshow_select[4], 0, 0, 1); M_BOX(GP_RIGHT, GP.SPINNER("autoshow_select_sec4", mydata.autoshow_select_sec[5], 0, 30, 1, 0, GP_BLUE, "50px", 0);););
    M_BOX(GP_LEFT, GP.LABEL("6:"); GP.SELECT("s_a_s5", SensorsAutoShowSelect2, mydata.autoshow_select[5], 0, 0, 1); M_BOX(GP_RIGHT, GP.SPINNER("autoshow_select_sec5", mydata.autoshow_select_sec[6], 0, 30, 1, 0, GP_BLUE, "50px", 0);););
    GP.BREAK();
    M_BOX(GP_LEFT, GP.LABEL(DISPLAY_ANIMATION_AUTOSHOW_NAME[mydata.lng]); M_BOX(GP_RIGHT, GP.SELECT("autoshow_animations_select", DISPLAY_AUTOSHOW_ANIMATION_SELECT[mydata.lng], mydata.autoshow_animations_select, 0, 0, 1);););
    M_BOX(GP_LEFT, GP.LABEL(DISPLAY_DOTS_RANDOM_SWITCH[mydata.lng]); M_BOX(GP_RIGHT, GP.SWITCH("random_autoshow_switch", mydata.random_autoshow_switch, GP_BLUE);););
    GP.HR();
    M_BOX(GP_LEFT, GP.LABEL(DISPLAY_DOTS_NAME[mydata.lng]); M_BOX(GP_RIGHT, GP.SELECT("animations_dots", DISPLAY_DOTS_ANIMATIONS[mydata.lng], mydata.animdots, 0, 0, 1);););
    M_BOX(GP_LEFT, GP.LABEL(DISPLAY_DOTS_RANDOM_SWITCH[mydata.lng]); M_BOX(GP_RIGHT, GP.SWITCH("random_dots_switch", mydata.dots_switch, GP_BLUE);););
    GP.HR();
    M_BOX(GP_LEFT, GP.LABEL(DISPLAY_SECONDS_SWITCH[mydata.lng]); M_BOX(GP_RIGHT, GP.SWITCH("seconds_switch", mydata.seconds_switch, GP_BLUE);););

    GP.BLOCK_END();

    //Блок для вывода данных дисплея
    GP.BLOCK_THIN_BEGIN();
    M_BOX(GP_CENTER, GP.LABEL("Данные дисплея"););
    GP.HR();
    sprintf_P(SET_TEMPERATURE_SENSOR_ARRAY, (PGM_P) F("%S"), "BME280,Narodmon,Openweather");
    M_BOX(GP_LEFT, GP.RADIO("rad", 0, mydata.mode, GP_BLUE); GP.LABEL("Время:"); GP.LABEL("", "timesystem"); GP.LABEL(" "););
    M_BOX(GP_LEFT, GP.RADIO("rad", 1, mydata.mode, GP_BLUE); GP.LABEL("Биток:"); GP.LABEL("", "btc"); GP.LABEL("$"););
    M_BOX(GP_LEFT, GP.RADIO("rad", 2, mydata.mode, GP_BLUE); GP.LABEL("Эфир:"); GP.LABEL("", "eth"); GP.LABEL("$"););
    M_BOX(GP_LEFT, GP.RADIO("rad", 3, mydata.mode, GP_BLUE); GP.LABEL("Температура:"); GP.LABEL("", "tempsystem"); GP.LABEL("°C");

      M_BOX(GP_RIGHT, GP.SELECT("timedisp", SET_TEMPERATURE_SENSOR_ARRAY, mydata.displaytemperature, 0, 0, 1);); GP.BREAK();
    );
    M_BOX(GP_LEFT, GP.RADIO("rad", 4, mydata.mode, GP_BLUE); GP.LABEL("Авто смена режимов");)
    GP.BLOCK_END();

    GP.BLOCK_THIN_BEGIN();

    //GP.HR(); 
    //M_BOX(GP_LEFT, GP.RADIO("dot", 0, mydata.modedots, GP_BLUE);GP.LABEL("1 последовательно"););
    //M_BOX(GP_LEFT, GP.RADIO("dot", 1, mydata.modedots, GP_BLUE);GP.LABEL("2 одновременно"); );;
    //M_BOX(GP_LEFT, GP.SWITCH("sw1", mydata.sw1, GP_BLUE););
    GP.BLOCK_END();

    /*
  GP.BLOCK_THIN_BEGIN();
  M_BOX(GP_CENTER, GP.LABEL("Отображение секунд");); 
  GP.HR(); 
  M_BOX(GP_LEFT, GP.RADIO("sec", 0, mydata.modetime, GP_BLUE);GP.LABEL("Вкл"););
  M_BOX(GP_LEFT, GP.RADIO("sec", 1, mydata.modetime, GP_BLUE);GP.LABEL("Выкл"); );
  GP.BLOCK_END();
  GP.TEXT("txt", "text", buffer);    GP.BREAK();
*/

  }
  GP.BUILD_END();
}

//Serial monitor
void action(GyverPortal & p) {
  
    if (p.form("/setting")) {      // кнопка нажата
      /*
        p.copyStr("lg", mydata.ssid);  // копируем себе
        p.copyStr("ps", mydata.pass);
        p.copyStr("ap", mydata.owMapApiKey);  // копируем себе
        p.copyStr("ct", mydata.owCity);
        p.copyStr("ntp",mydata.NTPserver);
        
        data.update();
       // data.updateNow();
      }
      if (p.form("/restart")) { 
        WiFi.softAPdisconnect(); // отключаем AP
        ESP.restart();
      }
        */
    }
  if (ui.click()) {
    if (ui.click("rst")) ESP.restart(); //Перезагрузка по кнопке.
    if (ui.click("sntp_btn")) NTPClientUpdate(); //Синхранизация NTP сервера
    //Синхранизация OP сервера
    if (ui.click("op_btn")) {
      //если есть ключь API и гоород
      if (strlen(mydata.owMapApiKey) > 0 && strlen(mydata.owCity) > 0) getTemp2(0);
      else Serial.println("Нет настроек OP");

    }
    if (ui.click("scan_btn"))
    {
      ScanWiFi();
    }
    //Синхранизация narodmon.ru
    if (ui.click("narod_btn")) {
      //если есть ключь API и ID то запрашиваем данные
      if (strlen(mydata.NarodmoonApi) > 0 && strlen(mydata.NarodmoonID) > 0) getTemp2(1);
      else Serial.println("Нет настроек Narodmon");
    }
    if (ui.click("wifi_btn")) { //Подключение к WiFi
      WiFi.softAPdisconnect(); // отключаем AP
      WiFiConnect_APcreate(); // Подключаемся или создаем точку доступа
      ConnectionToServices(); //Синхронизируем время, погоду, крипту

    }
    if (ui.clickStr("nameSens0", mydata.NameSensor[mydata.nrd_sens[0]]));
    if (ui.clickStr("nameSens1", mydata.NameSensor[mydata.nrd_sens[1]]));
    if (ui.clickStr("nameSens2", mydata.NameSensor[mydata.nrd_sens[2]]));
    if (ui.clickStr("nameSens3", mydata.NameSensor[mydata.nrd_sens[3]]));
    if (ui.clickStr("ntp", mydata.NTPserver));
    if (ui.clickStr("lg", mydata.ssid));
    if (ui.clickStr("ps", mydata.pass));
    if (ui.clickStr("ap", mydata.owMapApiKey));
    if (ui.clickStr("ct", mydata.owCity));
    if (ui.clickStr("api_narod", mydata.NarodmoonApi)) {
      md5(mydata.NarodmoonApi);
      Serial.println(mydata.NarodmoonApiMD5);
      //Serial.println(md5str);    
    }
    //if (ui.clickStr("md5_narod", mydata.NarodmoonApiMD5));
    if (ui.clickStr("id_narod", mydata.NarodmoonID));
    if (ui.clickInt("sens0_narod", mydata.nrd_sens[0]));
    if (ui.clickInt("sens1_narod", mydata.nrd_sens[1]));
    if (ui.clickInt("sens2_narod", mydata.nrd_sens[2]));
    if (ui.clickInt("sens3_narod", mydata.nrd_sens[3]));

    if (ui.clickInt("autoshow_min", mydata.autoshow_min));
    if (ui.clickInt("autoshow_select_sec0", mydata.autoshow_select_sec[1]));
    if (ui.clickInt("autoshow_select_sec1", mydata.autoshow_select_sec[2]));
    if (ui.clickInt("autoshow_select_sec2", mydata.autoshow_select_sec[3]));
    if (ui.clickInt("autoshow_select_sec3", mydata.autoshow_select_sec[4]));
    if (ui.clickInt("autoshow_select_sec4", mydata.autoshow_select_sec[5]));
    if (ui.clickInt("autoshow_select_sec5", mydata.autoshow_select_sec[6]));

    if (ui.clickStr("txt", buffer)) {
      // textbuff = textbuffinput;
    }
    if (ui.clickInt("rad", mydata.mode)) {
      for (byte i = 0; i <= 4; i++) {
        if (mydata.mode == i) {
          mydata.display = i;
          if (mydata.mode == 4) mydata.display = 0;
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
    if (ui.click("WiFi_List_Select")) {
      ui.copyInt("WiFi_List_Select", scan_list_idx);
     // int num = GPinList(const String& s, const String& list);
        sprintf_P(mydata.ssid, (PGM_P) F("%S"), GPlistIdx(WiFI_List, scan_list_idx).c_str());
  }
    if (ui.click("type_sensor0")) mydata.nrd_type_sensor[0] = ui.getInt("type_sensor0");
    if (ui.click("type_sensor1")) mydata.nrd_type_sensor[1] = ui.getInt("type_sensor1");
    if (ui.click("type_sensor2")) mydata.nrd_type_sensor[2] = ui.getInt("type_sensor2");
    if (ui.click("type_sensor3")) mydata.nrd_type_sensor[3] = ui.getInt("type_sensor3");

    if (ui.click("s_a_s0")) mydata.autoshow_select[0] = ui.getInt("s_a_s0");
    if (ui.click("s_a_s1")) mydata.autoshow_select[1] = ui.getInt("s_a_s1");
    if (ui.click("s_a_s2")) mydata.autoshow_select[2] = ui.getInt("s_a_s2");
    if (ui.click("s_a_s3")) mydata.autoshow_select[3] = ui.getInt("s_a_s3");
    if (ui.click("s_a_s4")) mydata.autoshow_select[4] = ui.getInt("s_a_s4");
    if (ui.click("s_a_s5")) mydata.autoshow_select[5] = ui.getInt("s_a_s5");

    if (ui.click("timeZone")) {
      mydata.GMT = ui.getInt("timeZone") - 12;
      Serial.println(mydata.GMT);
      NTPClientUpdate();

    }
    if (ui.click("timedisp")) {
      mydata.displaytemperature = ui.getInt("timedisp");
      Serial.println(mydata.displaytemperature);
      // sprintf_P(SET_TEMPERATURE_SENSOR_ARRAY, (PGM_P)F("BME280: %02d*C,Narod: %02d*C,OP: %02d*C"), bmetemperature, narodtemperature, optemperature);
      ValueTempUpdate(); //Обновляем глобальную переменную температуры в зависимости от выбранного датчики

    }
    if (ui.click("lng")) {
      mydata.lng = ui.getInt("lng");
      Serial.println(mydata.lng);
    }

    if (ui.clickInt("animations_dots", mydata.animdots)) {}
    if (ui.clickInt("random_dots_switch", mydata.dots_switch)) {}

    if (ui.clickInt("random_autoshow_switch", mydata.random_autoshow_switch)) {}
    if (ui.clickInt("autoshow_animations_select", mydata.autoshow_animations_select)) {}

    if (ui.clickInt("seconds_switch", mydata.seconds_switch)) {
      // if (mydata.modetime==0); //{mydata.modetime=0;}//Запускаем таймер для вывода Битка и эфира;
      //if (mydata.modetime==1); //{mydata.modetime=1;}
    };
    if (ui.clickInt("auto_show_switch", mydata.autoshow_switch));

    // if (ui.clickInt("rad1", valRad)) disp=0;
    // data.update();
    data.updateNow();
  }
  // было обновление
  if (ui.update()) {
    // 1. ищем, какой компонент запрашивает обновление
    // и вручную отправляем рандомное значение 

    if (ui.update("sens0")) {
      // String updbuffer=""; 
      //updbuffer+=SensorsNarodMon[mydata.nrd_sens[0]];
      //updbuffer.remove(0,4);
      ui.answer(SensorsDisplay[0]);
    } //myString.
    if (ui.update("sens1")) {
      //String updbuffer=""; 
      //updbuffer+=SensorsNarodMon[mydata.nrd_sens[1]];
      //updbuffer.remove(0,4);
      ui.answer(SensorsDisplay[1]);
    } //myString.
    if (ui.update("sens2")) {
      //String updbuffer=""; 
      //updbuffer+=SensorsNarodMon[mydata.nrd_sens[2]];
      //updbuffer.remove(0,4);
      ui.answer(SensorsDisplay[2]);
    } //myString.
    if (ui.update("sens3")) {
      //String updbuffer=""; 
      //updbuffer+=SensorsNarodMon[mydata.nrd_sens[3]];
      //updbuffer.remove(0,4);
      ui.answer(SensorsDisplay[3]);
    } //myString.
    if (ui.update("lg")) ui.answer(mydata.ssid);
    if (ui.update("temperature")) ui.answer(bmetemperature);
    if (ui.update("humudity")) ui.answer(bmehumudity);
    if (ui.update("pressure")) ui.answer(bmepressure);
    if (ui.update("altitude")) ui.answer(altitude);
    if (ui.update("lux")) ui.answer(vemllux);

    //if (ui.update("timedisp")) ui.answer(SET_TEMPERATURE_SENSOR_ARRAY);
    char a[16];
    //sprintf_P(a, (PGM_P)F("%02d:%02d:%02d"), hour, minute, second);
    if (ui.update("timesystem")) ui.answer(a);
    if (ui.update("btc")) ui.answer(pricebtc);
    if (ui.update("eth")) ui.answer(priceeth);
    if (ui.update("tempsystem")) {
      ui.answer(TempValue);
      if (mydata.displaytemperature == 0) TempValue = bmetemperature;
      if (mydata.displaytemperature == 1) TempValue = narodtemperature;
      if (mydata.displaytemperature == 2) TempValue = optemperature;
    }
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