
// конструктор страницы
void build() {
    GP.BUILD_BEGIN();
    GP.THEME(GP_DARK);

    GP.TITLE("IV13 Numitron clock");//Заголовок
    GP.HR(); //разделительная линия 

    GP.UPDATE("temperature,humudity,pressure,altitude,lux"); //динамическое обновленеие данных
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






  
    }

    GP.NAV_TABS("Home,Setting,Info,Firmware", GP_BLUE);//Вкладки
    GP.NAV_BLOCK_BEGIN();
  // через макрос
  M_BLOCK_TAB(
    "MANUAL",
    M_BOX(GP.LABEL("Duty"); GP.SLIDER("spd"););
    M_BOX(GP.BUTTON("bkw", "◄"); GP.BUTTON("frw", "►"););
  );
  GP.FORM_BEGIN("имяЭ");                 // начать форму с именем (имя)
    GP.FORM_END();                      // завершить форму
    //GP.LABEL("Tab1");
    GP.NAV_BLOCK_END();
//Конец вкладки Home
//Начало вкладки Setting
    GP.NAV_BLOCK_BEGIN();
    //GP.LABEL("Tab2");
 // список имён компонентов на обновление
/*
 // обновление случайным числом
 GP.TITLE("Title", "t1");
 GP.HR();
 GP.LABEL("Label: ");
 GP.LABEL("NAN", "num");        GP.BREAK();
 GP.LABEL("Label Block: ");
 GP.LABEL_BLOCK("NAN", "lbb"); GP.BREAK();
 GP.LABEL("Check: ");
 GP.CHECK("ch");         GP.BREAK();
 GP.LABEL("Led: ");
 GP.LED("led");          GP.BREAK();
 GP.LABEL("Switch: ");
 GP.SWITCH("sw");        GP.BREAK();
 GP.TEXT("txt", "text"); GP.BREAK();

 // обновление из переменной (храним значение)
 GP.NUMBER("num", "number", valNum);   GP.BREAK();
 GP.PASS("pass", "pass", valPass);     GP.BREAK();
 GP.SPINNER("spn");
 GP.SLIDER("sld", valSlider, 0, 10);   GP.BREAK();
 GP.DATE("date", valDate);             GP.BREAK();
 GP.TIME("time", valTime);             GP.BREAK();
 GP.COLOR("col", valCol);              GP.BREAK();
 GP.SELECT("sel", "val 1,val 2,val 3", valSelect);  GP.BREAK();
 GP.RADIO("rad", 0, valRad); GP.LABEL("Value 0"); GP.BREAK();
 GP.RADIO("rad", 1, valRad); GP.LABEL("Value 1"); GP.BREAK();
 GP.RADIO("rad", 2, valRad); GP.LABEL("Value 2"); GP.BREAK();
 GP.RADIO("rad", 3, valRad); GP.LABEL("Value 3"); GP.BREAK();
 GP.BREAK();
*/
    GP.NAV_BLOCK_END();
 //Информация 
    GP.NAV_BLOCK_BEGIN();
   // GP.LABEL("Tab3");
//Информация об устройстве и прошивки
   // GP.SYSTEM_INFO();                   // таблица с системной информацией
  // + версия вашей программы (в таблице появится строка Version с указанным текстом), [строка]
    GP.NAV_BLOCK_END();
  //Начало вкдадка Firmware 
    GP.NAV_BLOCK_BEGIN();
   // GP.LABEL("Tab4");
    GP.NAV_BLOCK_END();
   //Rjytw вкдадка Firmware

/*
    // второй блок навигации
  GP.NAV_TABS("KEK,PUK,4EBUREK", GP_RED);

  // эти блоки сделаем макросами
  M_NAV_BLOCK(
    GP.LABEL("block1");
    GP.BUTTON("", "Btn");
  );

  M_NAV_BLOCK(
    GP.LABEL("block2");
    GP.LED("");
  );

  M_NAV_BLOCK(
    GP.LABEL("block3");
    GP.BREAK();
    GP.TEXT("");
  );
*/
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

        WiFi.softAPdisconnect();        // отключаем AP
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