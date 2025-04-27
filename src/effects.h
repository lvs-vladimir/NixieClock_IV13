void flipTick() {
  if (FLIP_EFFECT == 0) {

  if (disp==3){ //Во время отображения температуры гасим не нужные индикаторы в соответствии с анимацией
        anodeStates[0]=0; anodeStates[1]=0; anodeStates[4]=0; anodeStates[5]=0;  
       }
         else {
           anodeStates[0]=1; anodeStates[1]=1; anodeStates[4]=1; anodeStates[5]=1;  
           }

    sendTime(newhour, newminute, newsecond);
    flipInit=false;
    //indiBrightDirection=false;
    //indiBrightCounter = indiMaxBright;
  }
  else if (FLIP_EFFECT == 1) {
    if (!flipInit) {
      flipInit = true;    
     // Time();
      // запоминаем, какие цифры поменялись и будем менять их яркость
      for (byte i = 0; i < 6; i++) {
        if (indiDigits[i] != newTime[i]) flipIndics[i] = true;
        else flipIndics[i] = false;
      }
    }
    if (flipTimer.isReady()) {

      if (!indiBrightDirection) {
        indiBrightCounter--;            // уменьшаем яркость
        if (indiBrightCounter <= 0) {   // если яроксть меньше нуля
          indiBrightDirection = true;   // меняем направление изменения
          indiBrightCounter = 0;        // обнуляем яркость

             if (disp==3){ //Во время отображения температуры гасим не нужные индикаторы в соответствии с анимацией
        anodeStates[0]=0; anodeStates[1]=0; anodeStates[4]=0; anodeStates[5]=0;  
       }
         else {
           anodeStates[0]=1; anodeStates[1]=1; anodeStates[4]=1; anodeStates[5]=1;  
           }
          sendTime(newhour, newminute, newsecond);          // меняем цифры
        
         // flipInit = false;
        }
      } else {
        indiBrightCounter++;                        // увеличиваем яркость
        if (indiBrightCounter >= indiMaxBright) {   // достигли предела
          indiBrightDirection = false;              // меняем направление
          indiBrightCounter = indiMaxBright;        // устанавливаем максимум
          // выходим из цикла изменения
          flipInit = false;
        //  flipFlag = false;
        }
      }
      for (byte i = 0; i < 6; i++) if (flipIndics[i]) indiDimm[i] = indiBrightCounter;   // применяем яркость

    }
  }
  else if (FLIP_EFFECT == 2) {
    if (!flipInit) {
      flipInit = true;
     
      // запоминаем, какие цифры поменялись и будем менять их
      for (byte i = 0; i < 6; i++) {
        if (indiDigits[i] != newTime[i]) flipIndics[i] = true;
        else flipIndics[i] = false;
      }
    }

    if (flipTimer.isReady()) {

    if (disp==3){ //Во время отображения температуры гасим не нужные индикаторы в соответствии с анимацией
        anodeStates[0]=0; anodeStates[1]=0; anodeStates[4]=0; anodeStates[5]=0;  
       }
         else {
           anodeStates[0]=1; anodeStates[1]=1; anodeStates[4]=1; anodeStates[5]=1;  
           }

      byte flipCounter = 0;
      for (byte i = 0; i < 6; i++) {
        if (flipIndics[i]) {
          indiDigits[i]--;
          if (indiDigits[i] < 0) indiDigits[i] = 9;
          if (indiDigits[i] == newTime[i]) flipIndics[i] = false;
        } else {
          flipCounter++;        // счётчик цифр, которые не надо менять
        }
      }
      if (flipCounter == 6) {   // если ни одну из 4 цифр менять не нужно
        // выходим из цикла изменения
        flipInit = false;
      //  flipFlag = false;
      }
    }

    //byte cathodeMask[] = {1, 0, 2, 9, 3, 8, 4, 7, 5, 6};  // порядок катодов in14
  }
  else if (FLIP_EFFECT == 3) {
    if (!flipInit) {
      
      flipInit = true;
      // запоминаем, какие цифры поменялись и будем менять их
      for (byte i = 0; i < 6; i++) {
        if (indiDigits[i] != newTime[i]) {
          flipIndics[i] = true;
          for (byte c = 0; c < 10; c++) {
            if (cathodeMask[c] == indiDigits[i]) startCathode[i] = c;
            if (cathodeMask[c] == newTime[i]) endCathode[i] = c;
          }
        }
        else flipIndics[i] = false;
      }
    }

    if (flipTimer.isReady()) {
       if (disp==3){ //Во время отображения температуры гасим не нужные индикаторы в соответствии с анимацией
        anodeStates[0]=0; anodeStates[1]=0; anodeStates[4]=0; anodeStates[5]=0;  
       }
         else {
           anodeStates[0]=1; anodeStates[1]=1; anodeStates[4]=1; anodeStates[5]=1;  
           }
           
      byte flipCounter = 0;
      for (byte i = 0; i < 6; i++) {
        if (flipIndics[i]) {
          if (startCathode[i] > endCathode[i]) {
            startCathode[i]--;
            indiDigits[i] = cathodeMask[startCathode[i]];
          } else if (startCathode[i] < endCathode[i]) {
            startCathode[i]++;
            indiDigits[i] = cathodeMask[startCathode[i]];
          } else {
            flipIndics[i] = false;
          }
        } else {
          flipCounter++;
        }
      }
      if (flipCounter == 6) {   // если ни одну из 4 цифр менять не нужно
        // выходим из цикла изменения
        flipInit = false;
        indiBrightCounter = indiMaxBright;
       // flipFlag = false;
      }
    }
  }
    else if (FLIP_EFFECT == 4) {
    if (!flipInit) {
      flipInit = true;    
     indiBrightCounterMinus=indiMaxBright;
     indiBrightCounterPlus=0;     
      //Таймер должен быть в 2 раза быстрее
      /*Сначала мы делаем проход по уменьшению текущего значения индикатора на 1 еденицу яркости задем меняем
      направление для увеличения на 1 еденицу следующей цифры в этом же индикаторе и так далее 
      пока не достигнем предела по яркости для разных значений одного индикатора*/
      for (byte i = 0; i < 6; i++) {
        //буферизируем старые значения. 
        oldTime[i]=indiDigits[i];
        // запоминаем, какие цифры поменялись и будем менять их яркость
        if (indiDigits[i] != newTime[i]) flipIndics[i] = true;
        else flipIndics[i] = false;
      }
    }
    if (flipTimer.isReady()) {

      if (!indiBrightDirection) {
        for (byte i = 0; i < 6; i++) indiDigits[i]=oldTime[i];//возвращаем старые значения.  
        indiBrightCounterMinus--;            // уменьшаем яркость
      //  if (indiBrightCounterMinus <= 0) {   // если яроксть меньше нуля
      //    indiBrightCounterMinus = 0;        // обнуляем яркость
      //  }
        for (byte i = 0; i < 6; i++) if (flipIndics[i]) indiDimm[i] = indiBrightCounterMinus;   // применяем яркость      
      } 
      if (indiBrightDirection) {
        for (byte i = 0; i < 6; i++) indiDigits[i]=newTime[i];//возвращаем новые значения. 
        indiBrightCounterPlus++;                        // увеличиваем яркость
      //  if (indiBrightCounterPlus >= indiMaxBright) {   // достигли предела
       //   indiBrightDirection = false;              // меняем направление
        //  indiBrightCounter = indiMaxBright;        // устанавливаем максимум
          
         for (byte i = 0; i < 6; i++) if (flipIndics[i]) indiDimm[i] = indiBrightCounterPlus;   // применяем яркость
        //  flipFlag = false;
        }
        //Если мы достигли пределов яркостей то завершаем изменение и возвращаем значения
        if (indiBrightCounterPlus >= indiMaxBright ){ // && indiBrightCounterMinus <= 0
        //  indiBrightCounterMinus=indiMaxBright;
        //  indiBrightCounterPlus=0;
          // выходим из цикла изменения
          flipInit = false;
          }
/*
          if (indiBrightDirection){
           indiBrightDirection = false;
          }   
          else{
            indiBrightDirection = true;
            }
            */
         indiBrightDirection=!indiBrightDirection;//меняем направление для следующей итерации
      }

     
      
    }
  
  // --- train --- //
  else if (FLIP_EFFECT == 5) {
    if (!flipInit4) {
  
      flipInit4 = true;
      flipInit = true;
      currentLamp = 0;
      trainLeaving = true;
      flipTimer.reset();
    }
    if (flipTimer.isReady()) {
      if (trainLeaving) {
        for (byte i = 5; i > currentLamp; i--) {
          indiDigits[i] = indiDigits[i - 1];
        }
        anodeStates[currentLamp] = 0;
        currentLamp++;
        if (currentLamp >= 6) {
          trainLeaving = false; //coming
          currentLamp = 0;
          //sendTime(hrs, mins);
        }
      }
      else { //trainLeaving == false
        for (byte i = currentLamp; i > 0; i--) {
          indiDigits[i] = indiDigits[i - 1];
        }
        indiDigits[0] = newTime[5 - currentLamp];
        anodeStates[currentLamp] = 1;
        currentLamp++;
        if (currentLamp >= 6) {
         flipInit=false; 
         // flipFlag = false;
        }
      }
    }
  }


  // --- elastic band --- //
  else if (FLIP_EFFECT == 6) {
    if (!flipInit) {
      
      flipInit = true;
      flipEffectStages = 0;
      flipTimer.reset();
    }
    if (flipTimer.isReady()) {
      switch (flipEffectStages++) {
        case 1:
          anodeStates[3] = 0; break;
        case 2:
          anodeStates[2] = 0;
          indiDigits[3] = indiDigits[2];
          anodeStates[3] = 1; break;
        case 3:
          anodeStates[3] = 0; break;
        case 4:
          anodeStates[1] = 0;
          indiDigits[2] = indiDigits[1];
          anodeStates[2] = 1; break;
        case 5:
          anodeStates[2] = 0;
          indiDigits[3] = indiDigits[1];
          anodeStates[3] = 1; break;
        case 6:
          anodeStates[3] = 0; break;
        case 7:
          anodeStates[0] = 0;
          indiDigits[1] = indiDigits[0];
          anodeStates[1] = 1; break;
        case 8:
          anodeStates[1] = 0;
          indiDigits[2] = indiDigits[0];
          anodeStates[2] = 1; break;
        case 9:
          anodeStates[2] = 0;
          indiDigits[3] = indiDigits[0];
          anodeStates[3] = 1; break;
        case 10:
          anodeStates[3] = 0;
          //sendTime(hrs,mins);
          break;
        case 11:
          indiDigits[0] = newTime[3];
          anodeStates[0] = 1; break;
        case 12:
          anodeStates[0] = 0;
          indiDigits[1] = newTime[3];
          anodeStates[1] = 1; break;
        case 13:
          anodeStates[1] = 0;
          indiDigits[2] = newTime[3];
          anodeStates[2] = 1; break;
        case 14:
          anodeStates[2] = 0;
          indiDigits[3] = newTime[3];
          anodeStates[3] = 1; break;
        case 15:
          indiDigits[0] = newTime[2];
          anodeStates[0] = 1; break;
        case 16:
          anodeStates[0] = 0;
          indiDigits[1] = newTime[2];
          anodeStates[1] = 1; break;
        case 17:
          anodeStates[1] = 0;
          indiDigits[2] = newTime[2];
          anodeStates[2] = 1; break;
        case 18:
          indiDigits[0] = newTime[1];
          anodeStates[0] = 1; break;
        case 19:
          anodeStates[0] = 0;
          indiDigits[1] = newTime[1];
          anodeStates[1] = 1; break;
        case 20:
          indiDigits[0] = newTime[0];
          anodeStates[0] = 1; break;
        case 21:
          flipInit = false;
         // flipFlag = false;
      }
    }
  }
}

void TestTimer_Event() { // тест индикаторов
  for (byte i = 0; i < 10; i++) {
    indiDigits[0] = i;
    indiDigits[1] = i;
    indiDigits[2] = i;
    indiDigits[3] = i;
    indiDigits[4] = i;
    indiDigits[5] = i;
    delay(300);
  }
}
void RandomAnodes (){ 
  if (RandomAnodesOn){
    flipFlag=false;
    flipInit=false;
  if (RandomAnodesFlag) {
    // инициализация генератора случайных чисел
    srand(random(10));
    //Перемешиваем массив анодов 
    for (byte i = 5; i >= 1; i--)
    {
        byte j = rand() % (i + 1);
        byte tmp = RandomOnOff[j];
        RandomOnOff[j] = RandomOnOff[i];
        RandomOnOff[i] = tmp;
    }
    RandomAnodesFlag=false;
    CounterAn=0;
  }

if (RandomTimer.isReady()){
//Гасим аноды
if (RandomDirection){
  anodeStates[RandomOnOff[CounterAn]]=0;
}
//Включаем аноды
else{
  if (disp==3){anodeStates[RandomOnOff[CounterAn]]=anodeStatesAnim1[5][RandomOnOff[CounterAn]];}//маска включения температуры 00 11 00
  else {anodeStates[RandomOnOff[CounterAn]]=1;} 
}

if (CounterAn>=5&&!RandomDirection){
  //Вырубаем эффект, включаем часы.
  RandomAnodesFlag=true;
  RandomDirection=true;
  RandomAnodesOn=false;
  if (disp==0) flipFlag = true;   
}  

if (CounterAn>=5&&!RandomAnodesFlag){
  sendTime(newhour, newminute, newsecond);//актульное значение дисплея
  RandomDirection=false;//теперь будем включать аноды
  RandomAnodesFlag=true;//еще раз перемешиваем
}

CounterAn++;//следующий элемент массива

  }
}
}
void Train() { //убегание цифр по индикаторам, ака поезд!
 
  if (trainTimer.isReady()) {
    if (TrainFlag) { flipFlag = false; flipInit=false; currentLamp = 6;}
    TrainFlag = false;
   if (TrainDirection){
    currentLamp--;
    for (byte i = 0; i < currentLamp; i++) {
      indiDigits[i] = indiDigits[i + 1];
      indiDigits[currentLamp]=10;
    }
    //Во время отображения температуры гасим не нужные индикаторы в соответствии с анимацией 
       if (disp==0){ 
      for (byte i = 0; i <= currentLamp; i++) {
      anodeStates[i]=anodeStatesAnim2[currentLamp][i];
    }
    }
    else anodeStates[currentLamp] = 0;
   }
   else{    
     for (byte i = 0; i <= currentLamp; i++) {
      indiDigits[i] = newTime[(5-currentLamp)+i];   
    }

    if (disp==3){ //Во время отображения температуры гасим не нужные индикаторы в соответствии с анимацией
      for (byte i = 0; i <= currentLamp; i++) {
      anodeStates[i]=anodeStatesAnim1[currentLamp][i];
    }
    }
    else anodeStates[currentLamp]=1;
    currentLamp++;
   }

    if (currentLamp == 0) {
      TrainDirection=false;    
    }
    if (currentLamp == 6 && !TrainDirection){    
      TrainDirection=true;
      TrainFlag = true;
      if (disp==0) flipFlag = true;       
      TrainOn=false;
    }
  }

}

void TestTimerI() { //тест индикаторов  
  if (TestTimer.isReady()) {
    if (TestFlag) {flipFlag = false; testIndicator = 10;}
    TestFlag = false;
    modeTimer.reset();
    modeTimerP.reset();
    testIndicator--;
    indiDigits[0] = testIndicator;
    indiDigits[1] = testIndicator;
    indiDigits[2] = testIndicator;
    indiDigits[3] = testIndicator;
    indiDigits[4] = testIndicator;
    indiDigits[5] = testIndicator;
    TestTimer.setInterval(200);
    if (testIndicator == 0) {
      sendTime(newhour, newminute, newsecond);
      TestTimer.setInterval(random(test_MIN * 1000L, test_MAX * 1000L));
      TestFlag = true;
      flipFlag = true;
    }
  }
}
void glitchTick() {
  if (!glitchFlag && second > 7 && second < 55) {
    if (glitchTimer.isReady()) {
      glitchFlag = true;
      indiState = 0;
      glitchCounter = 0;
      glitchMax = random(2, 6);
      glitchIndic = random(0, 6);
      glitchTimer.setInterval(random(1, 6) * 20);
    }
  } else if (glitchFlag && glitchTimer.isReady()) {
    indiDimm[glitchIndic] = indiState * indiMaxBright;
    indiState = !indiState;
    glitchTimer.setInterval(random(1, 6) * 20);
    glitchCounter++;
    if (glitchCounter > glitchMax) {
      glitchTimer.setInterval(random(GLITCH_MIN * 1000L, GLITCH_MAX * 1000L));
      glitchFlag = false;
      indiDimm[glitchIndic] = indiMaxBright;
    }
  }
}
