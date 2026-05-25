
void setup() { 
  
  Serial.begin(115200);
  delay(2000);
  memset(log_entries, 0, sizeof(log_entries));
  log_write_idx = 0;
  log_count = 0;
  log_add('I', "System boot");
//Подключение файловой системы и чтение настроек
    if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
        log_add('E', "LittleFS Mount Failed");
        Serial.println("LittleFS Mount Failed");
        return;
    }
 
  bool need_defaults = false;
  File file = LittleFS.open("/setting.dat");
  if(!file){
    log_add('W', "No settings file, applying defaults");
    Serial.println("No settings file, applying defaults");
    need_defaults = true;
  } else {
    file.close();
    FDstat_t stat = data.read();
    if (stat == FD_READ) {
      log_add('I', "Settings loaded OK");
      Serial.println("Settings loaded OK");
    } else {
      log_add('W', "Settings read: %d, applying defaults", stat);
      Serial.print("Settings read: "); Serial.println(stat);
      need_defaults = true;
    }
  }
  if (need_defaults) {
    memset(&mydata, 0, sizeof(mydata));
    strcpy(mydata.NTPserver, "pool.ntp.org");
    mydata.GMT = 3;
    mydata.lng = 1;
    mydata.display = 0;
    mydata.mode = 0;
    mydata.modetime = 0;
    mydata.animdots = 1;
    mydata.dots_switch = true;
    mydata.seconds_switch = false;
    for(int i=0; i<7; i++) mydata.dispset[i] = true;
    data.read();  // creates file with defaults if missing, or overwrites if corrupt
  }
/*
  Serial.println("Data read:");
  Serial.println(mydata.ssid);
  Serial.println(mydata.pass);
  Serial.println(mydata.owCity);
  Serial.println(mydata.owMapApiKey);
  Serial.println(mydata.NarodmoonApi);
  Serial.println(mydata.NarodmoonID);
  Serial.println(mydata.NarodmoonApiMD5);
  TempValue=mydata.displaytemperature;
  */
  WiFiConnect_APcreate();

  //Вкл обновление по "воздуху"
  OtaUpdate();

  // подключаем веб интерфейс GiverPortal
  ui.attachBuild(build);
  ui.attach(action);
  ui.uploadAuto(true);
  ui.enableOTA();   // без пароля
  //ui.enableOTA("admin", "pass");  // с паролем
  ui.downloadAuto(true);
  ui.start();
  
 

// подключаем конструктор и запускаем

//Инициализация SPI шины для управления tpic6b595
  pinMode(HSPI_SS, OUTPUT);
  digitalWrite(HSPI_SS, HIGH);
  hspi = new SPIClass(HSPI);
  hspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS);

//PWM контакт для регулировки яркости нуметронов
  pinMode(G, OUTPUT);
  digitalWrite(G, HIGH); 
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(LED_OUTPUT_PIN, PWM_CHANNEL);
  ledcWrite(PWM_CHANNEL, 0);
  
  //Подключение i2c датчиков с выводом значений
 if (bme.begin(0x76));
  veml.begin();
  //veml.setLowThreshold(10000);
  //veml.setHighThreshold(20000);
  //veml.interruptEnable(true);
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  Serial.print("lux: "); 
  Serial.println(veml.readLux());

  timerTIME.stop();
  mydata.display=0;
  timeon=true;
//подключаем 2 ядра esp32
  xTaskCreatePinnedToCore(Task0, "Task_0", 10000, NULL, 2, NULL, 0); 
  xTaskCreatePinnedToCore(Task1, "Task_1", 10000, NULL, 3, NULL, 1);       
}
  void loop() {}