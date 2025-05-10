//Во время смены режима мы должны старый дисплей с помощью анимации стереть, занести новые данные в буфер и включить анимацию появления.
void SwitchEffects(){
if (mooveNixie.isReady()){
  // **********Анимация выключения изображения справо на лево*********************
  if (off_effects == 1)
  { 
    for (byte i = 0; i <= Counter; i++)
    {
      buffer[i] = textbuffer[(5 - Counter) + (i + 1)];
    }
    buffer[Counter] = ' '; // Заполняем последний символ
    Counter--;
  }
  //Анимация включения изображения справо на лево
  if (on_effects == 1){
      for(byte i=0; i<=Counter; i++){
        buffer[(5-Counter)+i] = textbuffer[i];
      }
      Counter++;
      Serial.println("TEXT moove: ");
      Serial.print(Counter);
      Serial.println(buffer);
      }
  
      if (Counter < 0){
      off_effects=0;//Выключаем анимацию
      flip=true;
      //if (mydata.display!=0) 
      on_effects=1;//Включаем первуюанимацию включения
      }
  
      if (Counter == 6)
      { 
        on_effects = 0;
      //  if (on_effects == 0) 
        //timeon = true;  
      }
    }
}