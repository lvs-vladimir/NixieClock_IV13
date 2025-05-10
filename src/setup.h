
void setup() { 
  
  Serial.begin(115200);  
//Подключение файловой системы и чтение настроек
  LittleFS.begin();
  //data.addWithoutWipe(true);
  
  FDstat_t stat = data.read();
  switch (stat) {
    case FD_FS_ERR: Serial.println("FS Error");
      break;
    case FD_FILE_ERR: Serial.println("Error");
      break;
    case FD_WRITE: Serial.println("Data Write");
      break;
    case FD_ADD: Serial.println("Data Add");
      break;
    case FD_READ: Serial.println("Data Read");
      break;
    default:
      break;
  }

  Serial.println("Data read:");
  Serial.println(mydata.ssid);
  Serial.println(mydata.pass);
  Serial.println(mydata.owCity);
  Serial.println(mydata.owMapApiKey);
  Serial.println(mydata.NarodmoonApi);
  Serial.println(mydata.NarodmoonID);
  Serial.println(mydata.NarodmoonApiMD5);
  TempValue=mydata.displaytemperature;
  WiFiConnect_APcreate();

  //Вкл обновление по "воздуху"
  OtaUpdate();

  // подключаем веб интерфейс GiverPortal
  ui.attachBuild(build);
  ui.attach(action);
  ui.start();
  
  ui.enableOTA();   // без пароля
  //ui.enableOTA("admin", "pass");  // с паролем
 
  if (!LittleFS.begin()) Serial.println("FS Error");
  ui.downloadAuto(true);

// подключаем конструктор и запускаем

//Инициализация SPI шины для управления tpic6b595
  pinMode(HSPI_SS, OUTPUT);
  digitalWrite(HSPI_SS, HIGH);
  hspi = new SPIClass(HSPI);
  hspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS);

//PWM на gpio4 для регулировки яркости нуметронов
  pinMode(G, OUTPUT);
  digitalWrite(G, HIGH); 
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(LED_OUTPUT_PIN, PWM_CHANNEL);
  ledcWrite(PWM_CHANNEL, 200);
  
  //Подключение i2c датчиков с выводом значений
 if (bme.begin(0x76)) ;
  veml.begin();
  veml.setLowThreshold(10000);
  veml.setHighThreshold(20000);
  veml.interruptEnable(true);
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  Serial.print("lux: "); 
  Serial.println(veml.readLux());

  timerTIME.stop();
  mydata.display=0;
  timeon=true;
//подключаем 2 ядра esp32
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
        3,           /* Приоритет задачи 0-низкий 3-высокий */
        NULL,      /* &Task1 Идентификатор задачи, чтобы ее можно было отслеживать */
        1);          /* Ядро для выполнения задачи (1) */
      }
      void loop() {}