void Task0( void * pvParameters) {
  (void) pvParameters;
  for(;;){

    
       //каждые 5 минут получаем новые значения BTC и ETH 
   //ограничение бесплатного ключа api не более 330 запросов в сутки.
   if (cryptoCount >= 5) {// ((minute%10==5||minute%10==0) && updateCrypto==true)
      getCrypto(); 
      getTemp(); 
      TimeUpdate();
      //Меняем анимацию смены режимов 
      if (++anim >=3) anim = 0;  
      cryptoCount=0;
     // updateCrypto=false;
      } 
    //if (minute%10==6||minute%10==1) updateCrypto=true;

        if (minsCount >= 30) {   // каждые 30 мин синхронизация с RTC
        minsCount = 0;
        
      }    
  

  vTaskDelay(10000);//Запускаем данную задачу только 1 раз в секунду. Иначе перезагрузка ESP Почему????
  }

}



void Task1( void * pvParameters ){
  (void) pvParameters;
  //Serial.print("Task2 running on core ");
           //  "Задача Task2 выполняется на ядре "
 // Serial.println(xPortGetCoreID());

  for(;;){
    ArduinoOTA.handle();//Обновление по воздуху
    ui.tick();//Работа web интерфейса
  
     //if (ligtSensorTimer.isReady())   NightIn();//считываем датчик света

   //httpServer.handleClient();//Прошивка через web интерфейс

    
     UpdateDisplay(); 
    
 //Изминение яркости IV13 
 if (SensorTimerI2C.isReady()) {

  //getCharCode(text[1]); 
  
     //Serial.print("Temp C*: "); Serial.print(bme.readTemperature());
    // Serial.print(" lux: "); Serial.println(veml.readLux());  

    //Устанавливаем яркость IV13 (подбор на глаз) в зависимости от уровня освещения
    vemllux = veml.readLux();
    vemlvalue = vemllux;
    if (vemlvalue>1000) vemlvalue=1000;
    int brightnessIV13;
    if (vemlvalue<=1000 && vemlvalue>900) brightnessIV13=0; //Максимальная яркость
    if (vemlvalue<895 && vemlvalue>700) brightnessIV13=100;//90%
    if (vemlvalue<695 && vemlvalue>300) brightnessIV13=250;//80%
    if (vemlvalue<295 && vemlvalue>100) brightnessIV13=500;//50%
    if (vemlvalue<95 && vemlvalue>50) brightnessIV13=600;//30%
    if (vemlvalue<45 && vemlvalue>15) brightnessIV13=750;//10%
    if (vemlvalue<10 && vemlvalue>=0) brightnessIV13=800;//5%
    //brightnessIV13 = map(vemlvalue, 0, 800, 800, 0);

    //Считываем данные с bme280
    bmetemperature = bme.readTemperature();
    bmepressure = bme.readPressure();
    altitude = pressureToAltitude(bmepressure);
    bmepressure = pressureToMmHg(bmepressure);
    bmehumudity = bme.readHumidity();

     Serial.print(" vemlvalue: "); Serial.println(vemlvalue); 
     Serial.print(" lux: "); Serial.println(veml.readLux()); 
     Serial.print(" PWMvalue: "); Serial.println(brightnessIV13); 
     Serial.print("Temp C*: "); Serial.println(bmetemperature);

    ledcWrite(PWM_CHANNEL, brightnessIV13);
    }
     
    //Счетчик времени и синхронизация с RTC
    if (dotTimer.isReady()) {
      calculateTime();    
    }

  if(timerTIME.isReady())
 {
   disp++;
  
   if (disp==3) trainTimer.start();
   //Serial.println(disp);
   timerTIME.setInterval(3000);
   if (disp>3) {
   // flipFlag = true; 
     disp=0 ; 
     timerTIME.stop();
     timerTIME.setInterval(40);
     flipInit=false;//    
     }
     //Запускать эффект смены изображения только при смене режима

   // RandomAnodesOn=true;
   // TrainOn=true;

 }
//Формирование значений для вывода на индикаторы
if (disp==1) num=pricebtc;
if (disp==2) num=priceeth;
if (disp==3) num=TempValue;

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
  
switch (disp)
{
case 0:
if(second==30){ timerTIME.start();}//Запускаем таймер для вывода Битка и эфира
//Выводим время
 if (!flipInit) 
      { 
        
        newhour = hour;
        newminute = minute;
        newsecond = second;   
        //Переобразуем из в числа в char 
        sprintf_P(textbuff, (PGM_P)F("%02d%02d%02d"), newhour, newminute, newsecond); 
        setNewTime();  
       // newTime[0] = newhour / 10;
       // newTime[1] = newhour % 10;
      //  newTime[2] = newminute / 10;
       // newTime[3] = newminute % 10;
       // newTime[4] = newsecond / 10;
       // newTime[5] = newsecond % 10;
      } 

  break;

case 1:
//Выводим Биток
if (pricebtc>0){
if (!flipInit) 
      {
        newhour = (num6*10)+num5;
        newminute = (num4*10)+num3;
        newsecond = (num2*10)+num1;
        //Переобразуем из в числа в char 
        sprintf_P(textbuff, (PGM_P)F("%02d%02d%02d"), newhour, newminute, newsecond);
        setNewTime();
      }
}
else disp++;

  break;
  case 2:
  //Выводим Эфириум
  if (priceeth>0){
  if (!flipInit) 
      {
        newhour = (num6*10)+num5;
        newminute = (num4*10)+num3;
        newsecond = (num2*10)+num1;
        //Переобразуем из в числа в char 
        sprintf_P(textbuff, (PGM_P)F("%02d%02d%02d"), newhour, newminute, newsecond);
        setNewTime();
      }
  }
  else disp++;
  break;
    case 3:
  //Выводим Температуру
  //if (TempValue>0){
     
        newhour = (num6*10)+num5;
        newminute = (num2*10)+num1;
        newsecond = (num4*10)+num3;
        sprintf_P(textbuff, (PGM_P)F("%02d%02d%02d"), newhour, newminute, newsecond);
        textbuff[0]=' ';
        textbuff[1]=' ';
        textbuff[4]='*';
        textbuff[5]='C';
        setNewTime();  
  
  //}
     // 
  break;
}
 
 //glitchTick();                    //Глюки
 
// TestTimerI(); 
  }                  
}