#include "effects.h"//эффекты

void Task0(void * pvParameters) {
  (void) pvParameters;
  for (;;) {
    //каждые 5 минут получаем новые значения BTC и ETH 
    //ограничение бесплатного ключа api не более 330 запросов в сутки.
    if (cryptoCount >= 5) { 
      ConnectionToServices();
      //Меняем анимацию смены режимов 
    }
    if (minsCount >= 30) { // каждые 30 мин синхронизация с RTC
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

      bmetemperature = bme.readTemperature();
      bmepressure = bme.readPressure();
      altitude = pressureToAltitude(bmepressure);
      bmepressure = pressureToMmHg(bmepressure);
      bmehumudity = bme.readHumidity();
      if (bmetemperature != 0 && bmehumudity != 0) {
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
      while (k <= 12) {
        SensorsAutoShowSelect2 += SensorsAutoShow[k];
        SensorsAutoShowSelect += SensorsDisplay[k];
        k++;
      }

      byte f = 0;
      while (f <= 13) {
        //Serial.println(SensorsDisplay[f]);
        f++;
      }
      Serial.println(buffer);
      Serial.print("OFF ");
      Serial.println(off_effects);
      Serial.print("ON ");
      Serial.println(on_effects);
      Serial.print("Disp ");
      Serial.println(mydata.display);
    }

    //Переключение режимов по таймеру
    if (timerTIME.isReady()) {
      mydata.display++;
      if (mydata.display > 0) timeon = false; //замораживаем дисплей со временем для анимации выключения
      off_effects = 1; //включаем первую анимацию выключения
      int tim = mydata.autoshow_select_sec[mydata.display] * 1000;
      timerTIME.setInterval(tim + 3000);
      //timerTIME.stop();//останавоиваем таймер после смены режима
      timerstart = true;
      if (mydata.display > 6) {
        mydata.display = 0;
        DisplayTimeUpdate();
        // flip=true; 
        timerTIME.stop();
        timerTIME.setInterval(40);
      }
      //Запускать эффект смены изображения только при смене режима
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

    if (mydata.autoshow_switch) {
      if (second == 30) timerTIME.start();
    };
    switch (mydata.display) {
    case 0:
      //Выводим время
      /*
        if (flip && off_effects==0) {
          DisplayTimeUpdate();
          flip=false;
         }
         */
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
      //  Serial.print("OFF ");
      //  Serial.println(off_effects);
      //  Serial.print("ON ");
      //  Serial.println(on_effects);
      //  Serial.print("Disp ");
      //  Serial.println(mydata.display);
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
      //sprintf_P(buffer,PSTR( "The column header is %S") ,(char)text2);
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

    SwitchEffects(); //Эффекты

    UpdateDisplay();
    //Запускаем таймера переключения режимов только после завершения анимации
    //if (flip && on_effects==0 && mydata.display>0){
    // timerTIME.start();
    // timerTIME.setInterval(mydata.autoshow_select_sec[mydata.display]*1000);
    //}
    /*
    switch (mydata.mode)
    {
    case 0:
    if (second==30) timerTIME.start();
    break;
    case 1: mydata.display=0; break;
    case 2: mydata.display=1; break;
    case 3: mydata.display=2; break;
    case 4: mydata.display=3; break;
      }  
    */
  }
}