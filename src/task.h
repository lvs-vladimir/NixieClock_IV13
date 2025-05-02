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
    data.tick();
  
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
    int brightnessIV13=100;
    /*
    if (vemlvalue<=1000 && vemlvalue>900) brightnessIV13=0; //Максимальная яркость
    if (vemlvalue<898 && vemlvalue>700) brightnessIV13=100;//90%
    if (vemlvalue<698 && vemlvalue>300) brightnessIV13=250;//80%
    if (vemlvalue<298 && vemlvalue>100) brightnessIV13=500;//50%
    if (vemlvalue<98 && vemlvalue>50) brightnessIV13=600;//30%
    if (vemlvalue<48 && vemlvalue>15) brightnessIV13=750;//10%
    if (vemlvalue<13 && vemlvalue>=0) brightnessIV13=800;//5%
    //brightnessIV13 = map(vemlvalue, 0, 800, 800, 0);
*/
    //Считываем данные с bme280
    bmetemperature = bme.readTemperature();
    bmepressure = bme.readPressure();
    altitude = pressureToAltitude(bmepressure);
    bmepressure = pressureToMmHg(bmepressure);
    bmehumudity = bme.readHumidity();
/*
     Serial.print(" vemlvalue: "); Serial.println(vemlvalue); 
     Serial.print(" lux: "); Serial.println(veml.readLux()); 
     Serial.print(" PWMvalue: "); Serial.println(brightnessIV13); 
     Serial.print("Temp C*: "); Serial.println(bmetemperature);
*/
    Serial.println(buffer);
    ledcWrite(PWM_CHANNEL, brightnessIV13);
    }
     
    //Счетчик времени и синхронизация с RTC
    if (dotTimer.isReady()) {
      calculateTime();    
    }

  if(timerTIME.isReady())
 {
  mydata.display++;
  
   if (mydata.display==3) trainTimer.start();
   //Serial.println(display);
   timerTIME.setInterval(3000);
   if (mydata.display>3) {
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
  

  if (mydata.mode==4) {
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
        switch (mydata.modetime)
        {
        case 0:
        //Переобразуем из в числа в char 
        sprintf_P(buffer, (PGM_P)F("%02d%02d%02d"), newhour, newminute, newsecond); 
        break;    
        case 1:
        sprintf_P(buffer, (PGM_P)F(" %02d%02d "), newhour, newminute);  
          break;
        }
        set_dec_buffer();  
      } 

  break;

case 1:
//Выводим Биток если непустое значение
if (pricebtc>0){
if (!flipInit) 
      {
        //Переобразуем из в числа в char 
        sprintf_P(buffer, (PGM_P)F("%06d"), pricebtc);
        set_dec_buffer();
      }
}
else mydata.display++;

  break;
  case 2:
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
  break;
    case 3:
  //Выводим Температуру
  //if (TempValue>0){
        
        sprintf_P(buffer, (PGM_P)F(" %02d*C "), TempValue);
       // buffer[0]=' ';
       // buffer[1]=' ';
       // buffer[4]='*';
       // buffer[5]='C';
        set_dec_buffer();  
  
  //}
     // 
  break;
  case 4:
    //text='NoWiFi';
   // byte i= sizeof(IP);
   //char s1[] = "hello";
    
    //String text2 = "kjdvbdjvlasnccfbfcbdcbdfbdfbdfbdfbdfb";
    //buffer=strcat(buffer, text);
  //sprintf_P(buffer, (PGM_P)F("%S"), textbuffer);
  //sprintf_P(buffer,PSTR( "The column header is %S") ,(char)text2);
 
  
  set_dec_buffer();
  break;

}

if (mooveNixie.isReady()){

  for(byte i=0; i<=Counter; i++){
    buffer[(5-Counter)+i] = textbuffer[i];
  }
  Counter++;
  
  if (Counter > lost.length())
  { 
    Counter = 0;
    
    //sprintf_P(buffer, (PGM_P)F("%S"), "     ");//Заносим в буфер
  }
  Serial.println("TEXT moove: ");
  Serial.print(Counter);
  Serial.println(buffer);
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