void Task0( void * pvParameters) {
  (void) pvParameters;
  for(;;){
    while (ui.tick());//Работа web интерфейса

       //каждые 5 минут получаем новые значения BTC и ETH 
   //ограничение бесплатного ключа api не более 330 запросов в сутки.
   if (cryptoCount >= 5) {// ((minute%10==5||minute%10==0) && updateCrypto==true)
    // ConnectionToServices();
      //Меняем анимацию смены режимов 
      if (++anim >=3) anim = 0;  
      cryptoCount=0;
     // updateCrypto=false;
      } 
    //if (minute%10==6||minute%10==1) updateCrypto=true;

        if (minsCount >= 30) {   // каждые 30 мин синхронизация с RTC
        minsCount = 0;
        
      }    
 // vTaskDelay(10000);//Запускаем данную задачу только 1 раз в секунду. Иначе перезагрузка ESP Почему????
  }

}



void Task1( void * pvParameters ){
  (void) pvParameters;
  //Serial.print("Task2 running on core ");
           //  "Задача Task2 выполняется на ядре "
 // Serial.println(xPortGetCoreID());

  for(;;){
    ArduinoOTA.handle();//Обновление по воздуху
    
    data.tick();
   
     //if (ligtSensorTimer.isReady())   NightIn();//считываем датчик света

   //httpServer.handleClient();//Прошивка через web интерфейс

   //GPlistIdx()
     UpdateDisplay(); 
    
 //Изминение яркости IV13 
 if (SensorTimerI2C.isReady()) {

  //getCharCode(text[1]); 
  
     //Serial.print("Temp C*: "); Serial.print(bme.readTemperature());
    // Serial.print(" lux: "); Serial.println(veml.readLux());  

    //Устанавливаем яркость IV13 (подбор на глаз) в зависимости от уровня освещения
    vemllux = veml.readLux();
    vemlvalue = vemllux;
    int brightnessIV13;
    //Так как яркость на нуметронах меняется не линейно, то меняем ее путем подбора
    if (vemlvalue>900) brightnessIV13 = map(vemlvalue, 900, 1000, 99, 0);//brightnessIV13=0; //Максимальная яркость
    if (vemlvalue<899 && vemlvalue>700) brightnessIV13 = map(vemlvalue, 700, 899, 199, 100);//brightnessIV13=100;//90%
    if (vemlvalue<699 && vemlvalue>300) brightnessIV13 = map(vemlvalue, 300, 699, 249, 200);//brightnessIV13=250;//80%
    if (vemlvalue<299 && vemlvalue>100) brightnessIV13 = map(vemlvalue, 100, 299, 499, 250);//brightnessIV13=500;//50%
    if (vemlvalue<99 && vemlvalue>50) brightnessIV13 =   map(vemlvalue, 50, 98, 599, 500);//brightnessIV13=600;//30%
    if (vemlvalue<49 && vemlvalue>15) brightnessIV13 =   map(vemlvalue, 15, 48, 749, 600);//brightnessIV13=750;//10%
    if (vemlvalue<14 && vemlvalue>=0) brightnessIV13 =   map(vemlvalue, 0, 14, 800, 750);//brightnessIV13=800;//5%
    
    //brightnessIV13 = map(vemllux, 0, 800, 800, 0);

    //Считываем данные с bme280
    
    bmetemperature = bme.readTemperature();
    bmepressure = bme.readPressure();
    altitude = pressureToAltitude(bmepressure);
    bmepressure = pressureToMmHg(bmepressure);
    bmehumudity = bme.readHumidity();
   if (bmetemperature!=0 && bmehumudity!=0){
    byte j=7;
    while (j<=10) {
      SensorsAutoShow[j]="";//очистка
      SensorsDisplay[j]="";//очистка
      j++;
    }

    SensorsAutoShow[7]+=",";
   //SensorsAutoShow[7]+="bme280 ";
    SensorsAutoShow[7]+=bmetemperature;
    SensorsAutoShow[7]+="°";
  
    SensorsAutoShow[8]+=",";
    //SensorsAutoShow[8]+="bme280 ";
    SensorsAutoShow[8]+=bmepressure;
    SensorsAutoShow[8]+="mHg";
   
    SensorsAutoShow[9]+=",";
    //SensorsAutoShow[9]+="bme280 ";
    SensorsAutoShow[9]+=bmehumudity;
    SensorsAutoShow[9]+="%";

    SensorsAutoShow[10]+=",";
    //SensorsAutoShow[10]+="bme280 ";
    SensorsAutoShow[10]+=altitude;
    SensorsAutoShow[10]+="M";

    SensorsDisplay[7]+=",";
    SensorsDisplay[7]+=bmetemperature;
    SensorsDisplay[7]+="°";
    
    SensorsDisplay[8]+=",";
    SensorsDisplay[8]+=bmepressure;
    SensorsDisplay[8]+="mHg";

    SensorsDisplay[9]+=",";
    SensorsDisplay[9]+=bmehumudity;
    SensorsDisplay[9]+="%";

    SensorsDisplay[10]+=",";
    SensorsDisplay[10]+=altitude;
    SensorsDisplay[10]+="M";
  }
/*  

     Serial.print(" vemlvalue: "); Serial.println(vemlvalue); 
     Serial.print(" lux: "); Serial.println(veml.readLux()); 
     Serial.print(" PWMvalue: "); Serial.println(brightnessIV13); 
     Serial.print("Temp C*: "); Serial.println(bmetemperature);
*/
    Serial.println(brightnessIV13);

    ledcWrite(PWM_CHANNEL, brightnessIV13);

    SensorsAutoShowSelect2="";
    SensorsAutoShowSelect="";
    k=0;
     while (k<=12)
     {
      SensorsAutoShowSelect2+=SensorsAutoShow[k];
      SensorsAutoShowSelect+=SensorsDisplay[k];
      k++;
     }

     byte f=0;
     while (f<=13) {
      Serial.println(SensorsDisplay[f]);
       f++;
     }
     
    }
     
    //Счетчик времени и синхронизация с RTC
    if (dotTimer.isReady()) {
      calculateTime();    
    }

  if(timerTIME.isReady())
 {
  mydata.display++;
  timerTIME.setInterval(mydata.autoshow_select_sec[mydata.display]*1000);
   if (mydata.display==3) trainTimer.start();
   //Serial.println(display);
   
   if (mydata.display>6) {
   // flipFlag = true; 
   mydata.display=0 ; 
     timerTIME.stop();
     timerTIME.setInterval(40);
     flipInit=false;//    
     }
     //Запускать эффект смены изображения только при смене режима

   // RandomAnodesOn=true;
   // TrainOn=true;

 }
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
  

  if (mydata.autoshow_switch) {
    if (second==30) timerTIME.start();
  }; 
switch (mydata.display)
{
case 0:
//Выводим время
 if (!flipInit) 
      {        
        newhour = hour;
        newminute = minute;
        newsecond = second;   
        //Режим отображение времени с секундами или без
        switch (mydata.seconds_switch)
        {
        case true:
        //Переобразуем из в числа в char 
        sprintf_P(buffer, (PGM_P)F("%02d%02d%02d"), newhour, newminute, newsecond); 
        break;    
        case false:
        sprintf_P(buffer, (PGM_P)F(" %02d%02d "), newhour, newminute);  
          break;
        }
        set_dec_buffer();  
      } 
  break;
 // GPlistIdx(SensorsAutoShowSelect2,mydata.autoshow_select[0]);
case 1:
//Выводим Биток если непустое значение
AutoShowUpdate(0);

/*
if (pricebtc>0){
if (!flipInit) 
      {
        //Переобразуем из в числа в char 
        sprintf_P(buffer, (PGM_P)F("%06d"), pricebtc);
        set_dec_buffer();
      }
       
}
else mydata.display++;
 */
  break;
  case 2:
  AutoShowUpdate(1);
  /*
  //Выводим Эфириум если не пустое значение
  if (priceeth>0){
  if (!flipInit) 
      {
        //Переобразуем из в числа в char       
        sprintf_P(buffer, (PGM_P)F("%06d"), priceeth);
        set_dec_buffer();
      }
  }
  else mydata.display++;
  */
  break;
    case 3:
  //Выводим Температуру
  AutoShowUpdate(2);
  /*
  //if (TempValue>0){
        sprintf_P(buffer, (PGM_P)F(" %02d*C "), TempValue);
        set_dec_buffer();  
  //}
     // 
     */
  break;
  case 4:
    //text='NoWiFi';
   // byte i= sizeof(IP);
   //char s1[] = "hello";
    //String text2 = "kjdvbdjvlasnccfbfcbdcbdfbdfbdfbdfbdfb";
    //buffer=strcat(buffer, text);
  //sprintf_P(buffer, (PGM_P)F("%S"), textbuffer);
  //sprintf_P(buffer,PSTR( "The column header is %S") ,(char)text2);
  AutoShowUpdate(3);
  break;
  case 5:
  AutoShowUpdate(4);
  break;
  case 6:
  AutoShowUpdate(5);
  /*
  sprintf_P(buffer, (PGM_P)F("%6S"), GPlistIdx(SensorsAutoShowSelect2,mydata.autoshow_select[5]).c_str());
  set_dec_buffer();
  */
  break;
}


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