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

    // **********Случайное исчезновение сегментов*********************
    if (off_effects == 5){
      seg_anim_active = true;
      byte any = 0;
      for (byte i = 0; i < 6; i++){
        byte pat = dec_buffer[i] & 0x7F;
        if (pat){
          any = 1;
          byte bit = random(0, 7);
          while (!(pat & (1 << bit))){
            bit = random(0, 7);
          }
          pat &= ~(1 << bit);
          dec_buffer[i] = pat;
        }
      }
      if (!any){
        off_effects = 0;
        flip = true;
        on_effects = 5;
        seg_inited = false;
        memset(dec_buffer, 0, 6);
      }
    }
    //Случайное появление сегментов
    if (on_effects == 5 && !flip){
      seg_anim_active = true;
      if (!seg_inited){
        for (byte i = 0; i < 6; i++){
          seg_current[i] = 0;
          seg_target[i] = getCharCode(textbuffer[i]) & 0x7F;
          dec_buffer[i] = 0;
        }
        seg_inited = true;
      }
      byte done = 1;
      for (byte i = 0; i < 6; i++){
        if (seg_current[i] != seg_target[i]){
          done = 0;
          byte diff = seg_target[i] & ~seg_current[i];
          byte bit = random(0, 7);
          while (!(diff & (1 << bit))){
            bit = random(0, 7);
          }
          seg_current[i] |= (1 << bit);
          dec_buffer[i] = seg_current[i];
        }
      }
      if (done){
        memcpy(buffer, textbuffer, 6);
        seg_anim_active = false;
        on_effects = 0;
      }
    }
  }
}
