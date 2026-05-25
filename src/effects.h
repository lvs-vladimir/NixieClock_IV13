//Во время смены режима должны старый дисплей помощью анимации стереть, занести новые данные буфер включить анимацию появления.
void SwitchEffects(){
  if (mooveNixie.isReady()){
    // **********Анимация выключения изображения справо лево (скролл)*********************
    if (off_effects == 1){
      for(byte i=0; i<Counter; i++){
        buffer[i] = textbuffer[(5-Counter)+1+i];
      }
      buffer[Counter] = ' ';
      Counter--;
      if (Counter < 0){
        off_effects = 0;
        flip = true;
        on_effects = 1;
        Counter = 0;
      }
    }
    //Анимация включения изображения справо лево (скролл)
    if (on_effects == 1 && !flip){
      for(byte i=0; i<=Counter; i++){
        buffer[(5-Counter)+i] = textbuffer[i];
      }
      Counter++;
      if (Counter > 5){
        on_effects = 0;
      }
    }

    // **********Каскад справа налево (выключение)*********************
    if (off_effects == 2){
      buffer[Counter] = ' ';
      if (Counter == 0){
        off_effects = 0;
        flip = true;
        on_effects = 2;
        Counter = 0;
      } else {
        Counter--;
      }
    }
    //Каскад справа налево (включение)
    if (on_effects == 2 && !flip){
      buffer[5-Counter] = textbuffer[5-Counter];
      Counter++;
      if (Counter == 6){
        on_effects = 0;
      }
    }

    // **********Каскад слева направо (выключение)*********************
    if (off_effects == 3){
      buffer[5-Counter] = ' ';
      if (Counter == 0){
        off_effects = 0;
        flip = true;
        on_effects = 3;
        Counter = 0;
      } else {
        Counter--;
      }
    }
    //Каскад слева направо (включение)
    if (on_effects == 3 && !flip){
      buffer[Counter] = textbuffer[Counter];
      Counter++;
      if (Counter == 6){
        on_effects = 0;
      }
    }
  }
}
