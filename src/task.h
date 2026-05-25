#include "effects.h"//эффекты

void Task0(void * pvParameters) {
  (void) pvParameters;
  for (;;) {
    //каждые 5 минут получаем новые значения BTC и ETH 
    //ограничение бесплатного ключа api не более 330 запросов в сутки.
    if (cryptoCount >= 5) { 
      ConnectionToServices();
      cryptoCount=0;
      //Меняем анимацию смены режимов 
    }
    if (minsCount >= 30) {
      minsCount = 0;
    }
    vTaskDelay(10000); //Запускаем данную задачу только 1 раз в секунду. Иначе перезагрузка ESP Почему????
  }

}

void Task1(void * pvParameters) {
  (void) pvParameters;
  for (;;) {

    ArduinoOTA.handle(); //Обновление по воздуху
    ui.tick();//Веб интерфейс
    data.tick();//Сохранение настроек
    
    if (ligtSensorTimer.isReady()) AutoBrightness(); //устанавливаем яркость 
    if (TimeTimer.isReady()) calculateTime();

    //Изминение яркости IV13 
    if (SensorTimerI2C.isReady()) {

      //getCharCode(text[1]); 

      //Serial.print("Temp C*: "); Serial.print(bme.readTemperature());
      // Serial.print(" lux: "); Serial.println(veml.readLux());  

      //Считываем данные с bme280

      if (bme_ok) {
      bmetemperature = bme.readTemperature();
      bmepressure = bme.readPressure();
      altitude = pressureToAltitude(bmepressure);
      bmepressure = pressureToMmHg(bmepressure);
      bmehumudity = bme.readHumidity();
      }
      if (bme_ok && bmetemperature != 0 && bmehumudity != 0) {
        byte j = 7;
        while (j <= 10) {
          SensorsAutoShow[j] = ""; //очистка
          SensorsDisplay[j] = ""; //очистка
          j++;
        }

        SensorsAutoShow[7] += ",";
        //SensorsAutoShow[7]+="bme280 ";
        SensorsAutoShow[7] += bmetemperature;
        SensorsAutoShow[7] += "°";

        SensorsAutoShow[8] += ",";
        //SensorsAutoShow[8]+="bme280 ";
        SensorsAutoShow[8] += bmepressure;
        SensorsAutoShow[8] += "mHg";

        SensorsAutoShow[9] += ",";
        //SensorsAutoShow[9]+="bme280 ";
        SensorsAutoShow[9] += bmehumudity;
        SensorsAutoShow[9] += "%";

        SensorsAutoShow[10] += ",";
        //SensorsAutoShow[10]+="bme280 ";
        SensorsAutoShow[10] += altitude;
        SensorsAutoShow[10] += "M";

        SensorsDisplay[7] += ",";
        SensorsDisplay[7] += bmetemperature;
        SensorsDisplay[7] += "°";

        SensorsDisplay[8] += ",";
        SensorsDisplay[8] += bmepressure;
        SensorsDisplay[8] += "mHg";

        SensorsDisplay[9] += ",";
        SensorsDisplay[9] += bmehumudity;
        SensorsDisplay[9] += "%";

        SensorsDisplay[10] += ",";
        SensorsDisplay[10] += altitude;
        SensorsDisplay[10] += "M";
      }
      /*  

           Serial.print(" vemlvalue: "); Serial.println(vemlvalue); 
           Serial.print(" lux: "); Serial.println(veml.readLux()); 
           Serial.print(" PWMvalue: "); Serial.println(brightnessIV13); 
           Serial.print("Temp C*: "); Serial.println(bmetemperature);
      */
      //Serial.println(brightnessIV13);

      SensorsAutoShowSelect2 = "";
      SensorsAutoShowSelect = "";
      byte k = 0;
      while (k <= 13) {
        SensorsAutoShowSelect2 += SensorsAutoShow[k];
        SensorsAutoShowSelect += SensorsDisplay[k];
        k++;
      }

      byte f = 0;
      while (f <= 13) {
        //Serial.println(SensorsDisplay[f]);
        f++;
      }
      log_add('D', "Mode %d anim %d off %d on %d", mydata.display, mydata.anim_change, off_effects, on_effects);
    }

    if (!ap_show_scroll) {
    //Переключение режимов по таймеру
    if (timerTIME.isReady() && mydata.autoshow_slots > 0) {
      mydata.display++;
      if (mydata.display > 0) timeon = false;
      off_effects = 0; on_effects = 0;
      seg_anim_active = false; seg_inited = false;
      byte anim = mydata.anim_change;
      if (anim == 3) anim = random(0, 4);
      if (anim == 0) { off_effects = 1; Counter = 5; }
      if (anim == 1) { off_effects = 2; Counter = 5; }
      if (anim == 2) { off_effects = 3; Counter = 5; }
      if (anim == 3) { off_effects = 5; }
      if (anim == 4) { off_effects = 5; }
      log_add('D', "Timer display=%d off=%d", mydata.display, off_effects);
      if (mydata.display > mydata.autoshow_slots) {
        mydata.display = 0;
        DisplayTimeUpdate();
        timerTIME.stop();
        timerTIME.setInterval(40);
      } else {
        int tim = mydata.autoshow_select_sec[mydata.display] * 1000;
        timerTIME.setInterval(tim + 3000);
        timerstart = true;
      }
    }
    /*
     if (timerstart && on_effects==0 && off_effects==0){
      timerTIME.start();
      timerTIME.setInterval(mydata.autoshow_select_sec[mydata.display]*1000);
      Serial.println("Таймер старт");
      timerstart=false;
     }
      */
    /*

    //Формирование значений для вывода на индикаторы
    if (mydata.display==1) num=pricebtc;
    if (mydata.display==2) num=priceeth;
    if (mydata.display==3) num=TempValue;

    num1=num%10;
    num/=10;
    num2=num%10;
    num/=10;
    num3=num%10;
    num/=10;
    num4=num%10; 
    num/=10;
    num5=num%10; 
    num/=10;
    num6=num%10; 

      newhour = (num6*10)+num5;
      newminute = (num2*10)+num1;
      newsecond = (num4*10)+num3;
     */
//Вклавтопоказ по времени
    if (mydata.autoshow_switch && mydata.autoshow_min!=0) {
      if ((auto_show_counter >= mydata.autoshow_min) && mydata.display==0 && on_effects==0 && off_effects==0) {
        timerTIME.start();
        auto_show_counter=0;
      }
    }

    switch (mydata.display) {
    case 0:
      //Выводим время
      /*
        if (flip && off_effects==0) {
          DisplayTimeUpdate();
          flip=false;
         }
         */
      flip = false;
      if (on_effects == 0 && off_effects == 0) timeon = true;

      if (timeon) {
        DisplayTimeUpdate();
        Counter = 6;
        memset(buffer, 0, sizeof(buffer)); //очистка буфера
        strcpy(buffer, textbuffer);
      }

      break;
    case 1:
      if (flip) {
        AutoShowUpdate(0);
        flip = false;
      }
      break;
    case 2:
      if (flip) {
        AutoShowUpdate(1);
        flip = false;
      }
      break;
    case 3:
      if (flip) {
        AutoShowUpdate(2);
        flip = false;
      }
      break;
    case 4:
      if (flip) {
        AutoShowUpdate(3);
        flip = false;
      }
      break;
    case 5:
      if (flip) {
        AutoShowUpdate(4);
        flip = false;
      }
      break;
    case 6:
      if (flip) {
        AutoShowUpdate(5);
        flip = false;
      }
      break;
    }
    }

    //WiFi disconnect monitoring
    if (WiFi.getMode() == WIFI_STA) {
      if (WiFi.status() != WL_CONNECTED) {
        if (!wifi_dc_state) {
          wifi_dc_state = true;
          ap_show_scroll = true;
          ap_scroll_pos = 0;
          WiFi.softAP("13-AP");
          log_add('W', "WiFi lost, AP started");
        }
      } else {
        if (wifi_dc_state) {
          wifi_dc_state = false;
          ap_show_scroll = false;
          WiFi.softAPdisconnect(true);
          log_add('I', "WiFi reconnected");
          timeon = true;
          flip = true;
          timerTIME.start();
          timerTIME.setInterval(40);
        }
      }
    }

    //AP mode scrolling text
    if (ap_show_scroll) {
      if (apScrollTimer.isReady()) {
        ap_scroll_pos++;
        int len = strlen(ap_scroll_text);
        if (ap_scroll_pos >= len - 6) ap_scroll_pos = 0;
        for (int i = 0; i < 6; i++) {
          buffer[i] = ap_scroll_text[ap_scroll_pos + i];
        }
        buffer[6] = '\0';
        off_effects = 0;
        on_effects = 0;
        Counter = 6;
        timeon = false;
      }
    }

    SwitchEffects(); //Эффекты
    UpdateDisplay();

  }
}