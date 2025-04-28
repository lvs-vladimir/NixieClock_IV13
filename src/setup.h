void setup() { 

  //I2CBME.begin(17, 16);
 // Wire.begin(21, 23);
  Serial.begin(115200);  
  // читаем логин пароль WiFi из памяти
  EEPROM.begin(300);
  EEPROM.get(0, lp);

  WiFi.begin(lp.ssid, lp.pass);
  //WiFi.begin(ssid, password); //Wifi  
  while (WiFi.status() != WL_CONNECTED) {    
    delay (500);    
    Serial.print (".");  
  }

  //Читаем данные openWeather из памяти
  EEPROM.get(60, ow);
  openWeatherMapApiKey=ow.owMapApiKey;
  city=ow.owCity;

  //Обновление по воздуху   
  ArduinoOTA.setHostname("ESP32");

  ArduinoOTA.onStart([]() {
  	String type;
  	if (ArduinoOTA.getCommand() == U_FLASH)
    	type = "sketch";
  	else // U_SPIFFS
    	type = "filesystem";

  	// NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
  	Serial.println("Start updating " + type);
	})
	.onEnd([]() {
  	Serial.println("\nEnd");
	})
	.onProgress([](unsigned int progress, unsigned int total) {
  	Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	})
	.onError([](ota_error_t error) {
  	Serial.printf("Error[%u]: ", error);
  	if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
  	else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
  	else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
  	else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
  	else if (error == OTA_END_ERROR) Serial.println("End Failed");
	});
     
  ArduinoOTA.begin();
                
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //Обновление по воздуху 

  // подключаем конструктор и запускаем

  ui.attachBuild(build);
  ui.start();
  ui.enableOTA();   // без пароля
  //ui.enableOTA("admin", "pass");  // с паролем
  ui.attach(action);
  ui.log.start(30);   // размер буфера
  

  if (!LittleFS.begin()) Serial.println("FS Error");
  ui.downloadAuto(true);

  ui.start();
  ui.attachBuild(build);

// подключаем конструктор и запускаем

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