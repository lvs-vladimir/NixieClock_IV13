void calculateTime() {
  dotsFlag=!dotsFlag;
    second++;
    
    if (second > 59) {
     // flipFlag = true;   // флаг что нужно поменять время
      second = 0;
      minute++;
      minsCount++;
      cryptoCount++;
    }
      
    //if (hour>=0 && hour<=8) {indiMaxBright=INDI_BRIGHT_N; for (byte i = 0; i < 6; i++) indiDimm[i] = indiMaxBright;}
   // else {indiMaxBright=INDI_BRIGHT; for (byte i = 0; i < 6; i++) indiDimm[i] = indiMaxBright;}
   
    //Каждые 23 часа синхронизируем врем с сервером UDP
    if (hourCount>=23){
        hourCount=0;
        TimeUpdate();
    }
  
  
    if (minute > 59) {
      minute = 0;
      hourCount++;
      hour++;
     // if (hour >= 8 && hrs <= 22) TimerFreeTone(PIEZO_PORT, 500, 200, 10); // сигнал каждый час
      if (hour > 23) hour = 0;
     // changeBright();//Изменить яркость индикаторов в зависимости от времени
    }
    //if (flipFlag) setNewTime();         // обновляем массив времени
  
  
}

