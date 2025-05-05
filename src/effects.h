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