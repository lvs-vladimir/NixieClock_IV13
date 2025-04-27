void NightIn()  //обработка датчика внутри таймера
{
    
   int sensorValue = analogRead(A6); // Чтение показаний датчика;
    
    //диапазоны
   if (sensorValue<=400){
       if(al++>10) is=brightValue[0];//Если это значение считывается 10 раз подрят
       }
    else al=0;
   
   if (sensorValue>500&&sensorValue<1600){
      if(bl++>10) is=brightValue[1];
       }
    else bl=0;
   if (sensorValue>1800){
      if(cl++>10) is=brightValue[3];
      }//&&sensorValue<2800
   //if (sensorValue>3100){is=brightValue[3];}
    else cl=0;

    if (is!=js) {
        indiMaxBright=is; 
        FLIP_SPEED[1]=lightValue[is];  //меняем время таймера и шаг для эффекта плавной смены    
        if (FLIP_EFFECT == 1){flipTimer.setInterval(FLIP_SPEED[FLIP_EFFECT]);}      
        js=is;
        for (byte j = 0; j < 6; j++) indiDimm[j] = indiMaxBright;//применяем яркость
        }
    
}