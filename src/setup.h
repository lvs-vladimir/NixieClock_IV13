void setup() { 

  //I2CBME.begin(17, 16);
 // Wire.begin(21, 23);
  Serial.begin(9600);  

  WiFi.begin(ssid, password); //Wifi  
  while (WiFi.status() != WL_CONNECTED) {    
    delay (500);    
    Serial.print (".");  
  }  
  
  
  pinMode(HSPI_SS, OUTPUT);
  pinMode(G, OUTPUT);
  
  digitalWrite(HSPI_SS, HIGH);
  digitalWrite(G, HIGH);  

  hspi = new SPIClass(HSPI);
  hspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS); //SCLK, MISO, MOSI, SS

//PWM на gpio4 для регулировки яркости пинов G TPIC6B595
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(LED_OUTPUT_PIN, PWM_CHANNEL);
  ledcWrite(PWM_CHANNEL, 200);
  
  bme.begin(0x76);
  veml.begin();
  veml.setLowThreshold(10000);
  veml.setHighThreshold(20000);
  veml.interruptEnable(true);
  
  timeClient.begin();
  TimeUpdate();
  getTemp();
  getCrypto();
 // Serial.print("Temperature = ");
 // Serial.print(bme.readTemperature());
 // Serial.println(" *C");
 // Serial.print("lux: "); Serial.println(veml.readLux());

   xTaskCreatePinnedToCore(
        Task0, /* Функция, содержащая код задачи */
        "Task_0", /* Название задачи */
        10000, /* Размер стека в словах */
        NULL, /* Параметр создаваемой задачи */
        2, /* Приоритет задачи 0-низкий 3-высокий */
        NULL, /* &Task0 Идентификатор задачи */
        0); /* Ядро, на котором будет выполняться задача */
  
  xTaskCreatePinnedToCore(
        Task1,   /* Функция задачи */
        "Task_1",     /* Название задачи */
        10000,       /* Размер стека задачи */
        NULL,        /* Параметр задачи */
        2,           /* Приоритет задачи 0-низкий 3-высокий */
        NULL,      /* &Task1 Идентификатор задачи, чтобы ее можно было отслеживать */
        1);          /* Ядро для выполнения задачи (1) */
}