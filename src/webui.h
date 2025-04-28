
// конструктор страницы
void build() {
    GP.BUILD_BEGIN();
    GP.THEME(GP_DARK);

    GP.TITLE("IV13 Numitron clock");//Заголовок
    GP.HR(); //разделительная линия 

    GP.UPDATE("temperature,humudity,pressure,altitude,lux,timesystem,btc,eth,tempsystem"); //динамическое обновленеие данных
    //Кликабельный страницы с обновлением
    GP.NAV_TABS_LINKS("/,/setting,/info,/firmware", "Home,Setting,Info,Firmware", GP_BLUE);
    
// страница setting
    if (ui.uri("/setting")) {
  //Подключение к WiFi
  GP.BLOCK_THIN_BEGIN();
  M_BOX(GP_CENTER, GP.LABEL("Network connection");); 
  GP.HR(); 
  GP.FORM_BEGIN("/setting");
  M_BOX(GP_LEFT, GP.LABEL("WiFi name:");); 
  GP.TEXT("lg", "Login", lp.ssid);
  GP.BREAK();
  M_BOX(GP_LEFT, GP.LABEL("Password:"););
  GP.TEXT("ps", "Password", lp.pass);
 // GP.SUBMIT("Submit", GP_BLUE);
 // GP.FORM_END();
  GP.BLOCK_END();

//Подключение к Openweather
  GP.BLOCK_THIN_BEGIN();
  M_BOX(GP_CENTER, GP.LABEL("Openweathermap.org");); 
  GP.HR(); 
  //GP.FORM_BEGIN("/setting");
  M_BOX(GP_LEFT, GP.LABEL("Api Key:");); 
  GP.TEXT("ap", "ApiKey", ow.owMapApiKey);
  GP.BREAK();
  M_BOX(GP_LEFT, GP.LABEL("City:"););
  GP.TEXT("ct", "City", ow.owCity);
  //GP.FORM_END();
  GP.BLOCK_END();
  GP.SUBMIT("Save", GP_BLUE);
  GP.FORM_END();

  GP.BUTTON("rst", "Restart", "", GP_BLUE, "", 0, 1);
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
  M_BOX(GP_LEFT, GP.RADIO("rad", 0, valRad, GP_BLUE);GP.BREAK();GP.LABEL("Авто смена режимов"); GP.BREAK(););
  M_BOX(GP_LEFT, GP.RADIO("rad", 1, valRad, GP_BLUE);GP.BREAK();GP.LABEL("Время:"); GP.LABEL(" ", "timesystem"); GP.LABEL(" "); GP.BREAK(););
  M_BOX(GP_LEFT, GP.RADIO("rad", 2, valRad, GP_BLUE);GP.BREAK();GP.LABEL("Биток:"); GP.LABEL(" ", "btc"); GP.LABEL("$"); GP.BREAK(););
  M_BOX(GP_LEFT, GP.RADIO("rad", 3, valRad, GP_BLUE);GP.BREAK();GP.LABEL("Эфир:"); GP.LABEL(" ", "eth"); GP.LABEL("$"); GP.BREAK(););
  M_BOX(GP_LEFT, GP.RADIO("rad", 4, valRad, GP_BLUE);GP.BREAK();GP.LABEL("Температура:"); GP.LABEL(" ", "tempsystem"); GP.LABEL("°C"); GP.BREAK(););
  GP.BLOCK_END();
  

  GP.TEXT("txt", "text", textbuff);    GP.BREAK();
 // GP.PASS("pass", "pass", textbuff);   GP.BREAK();
  
    }
    GP.BUILD_END();
  }
    
  //Serial monitor
void action(GyverPortal& p) {
    if (p.form("/setting")) {      // кнопка нажата
        p.copyStr("lg", lp.ssid);  // копируем себе
        p.copyStr("ps", lp.pass);
        p.copyStr("ap", ow.owMapApiKey);  // копируем себе
        p.copyStr("ct", ow.owCity);
        EEPROM.put(0, lp);              // сохраняем
        //EEPROM.commit();                // записываем    
        EEPROM.put(60, ow);              // сохраняем
        EEPROM.commit();                // записываем
        EEPROM.get(60, ow);
        openWeatherMapApiKey=ow.owMapApiKey;
        city=ow.owCity;
      }
      if (p.form("/restart")) { 
        WiFi.softAPdisconnect(); // отключаем AP
        ESP.restart();
      }

      if (ui.click()) {
        if (ui.click("rst")) ESP.restart();//Если нажата кнопка то перезагружаемся.
        if (ui.clickStr("txt", textbuff))  {
         // textbuff = textbuffinput;
        }
        if (ui.clickInt("rad", valRad)){
            if (valRad==0) ;
            if (valRad==1) disp=0;
            if (valRad==2) disp=1;
            if (valRad==3) disp=2;
            if (valRad==4) disp=3;
        } ;
       // if (ui.clickInt("rad1", valRad)) disp=0;
   
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