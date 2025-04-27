#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

TaskHandle_t Task1;



const char* host = "esp32";
const char* ssid = "BOBKA";
const char* password = "lukjanow";
const char* logi = "admin";
const char* pass = "admin";
WebServer server(80);


const char* loginIndex = 
 "<form name='loginForm'>"
    "<table width='20%' bgcolor='A09F9F' align='center'>"
        "<tr>"
            "<td colspan=2>"
                "<center><font size=4><b>ESP32 Login Page</b></font></center>"
                "<br>"
            "</td>"
            "<br>"
            "<br>"
        "</tr>"
        "<td>Username:</td>"
        "<td><input type='text' size=25 name='userid'><br></td>"
        "</tr>"
        "<br>"
        "<br>"
        "<tr>"
            "<td>Password:</td>"
            "<td><input type='Password' size=25 name='pwd'><br></td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
            "<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
        "</tr>"
    "</table>"
"</form>"
"<script>"
    "function check(form)"
    "{"
    "if(form.userid.value=='admin' && form.pwd.value=='admin')"
    "{"
    "window.open('/serverIndex')"
    "}"
    "else"
    "{"
    " alert('Error Password or Username')/*displays error message*/"
    "}"
    "}"
"</script>";
 
/*
 * Server Index Page
 */
 
const char* serverIndex = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')" 
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";

void Task1code( void * parameter) {
  for(;;){

/*if(pin < 34) {
 GPIO.out1_w1ts.val = ((uint32_t)1 << (pin - 32));
 */
GPIO.out_w1ts = ((uint32_t)1 << 2);//вкл
//digitalWrite(2, HIGH);
    delay(1000);
 //   digitalWrite(2, LOW);
 GPIO.out_w1tc = ((uint32_t)1 << 2);//Выкл
    delay(1000);
  }

}
void setup() {
  pinMode(2, OUTPUT);
  xTaskCreatePinnedToCore(
 Task1code, /* Функция, содержащая код задачи */
 "Task1", /* Название задачи */
 10000, /* Размер стека в словах */
 NULL, /* Параметр создаваемой задачи */
 0, /* Приоритет задачи */
 &Task1, /* Идентификатор задачи */
 0); /* Ядро, на котором будет выполняться задача */


  Serial.begin(9600);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
 // server.setup(&httpServer,path,username,pass);
  server.begin();
}

void loop() {
   //каждые 5 минут получаем новые значения BTC и ETH 
   //ограничение бесплатного ключа api не более 330 запросов в сутки.
   if (cryptoCount >= 5) {// ((minute%10==5||minute%10==0) && updateCrypto==true)
      getCrypto(); getTemp(); 
      //Меняем анимацию смены режимов 
      if (++anim >=3) anim = 0;  
      cryptoCount=0;
     // updateCrypto=false;
      } 
    //if (minute%10==6||minute%10==1) updateCrypto=true;

        if (minsCount >= 30) {   // каждые 30 мин синхронизация с RTC
        minsCount = 0;
        TimeUpdate();
       // getDateDs3231(); 
      }

  vTaskDelay(10000);//Запускаем данную задачу только 1 раз в секунду. Иначе перезагрузка ESP Почему????
  }

}

void Task1( void * pvParameters ){
  (void) pvParameters;
  //Serial.print("Task2 running on core ");
           //  "Задача Task2 выполняется на ядре "
 // Serial.println(xPortGetCoreID());

  for(;;){

     if (ligtSensorTimer.isReady())   NightIn();//считываем датчик света

   httpServer.handleClient();//Прошивка через web интерфейс

     //обновление дисплея с заданной частотой
    if (micros() - previousMillis > 100)  //42-9 50-10
        { 
          UpdateDisplay(); 
            previousMillis = micros();
               
        }

    

    //Счетчик времени и синхронизация с RTC
    if (dotTimer.isReady()) calculateTime();

//(byte)minute == 0&&
//меняем эффект ламп каждый час
 if ( second == 0 && minute==0 && secondtimer.isReady()) {
     if (++FLIP_EFFECT >=4) FLIP_EFFECT = 0;
       flipTimer.setInterval(FLIP_SPEED[FLIP_EFFECT]);
      
      for (byte i = 0; i < 6; i++) {
        indiDimm[i] = indiMaxBright;
        anodeStates[i] = 1;
      }
      secondtimer.reset();
    }  
//меняем эффект ламп каждую минуту
 //if ( (byte)second == 0&& secondtimer.isReady()) {
     
   //   secondtimer.reset();
  //  }
  
    if (flipFlag) flipTick();   

    switch (anim)
    {
    case 0:
    flipFlag=true; 
    break;

    case 1:
    //эффект поезд только при смене режимов
      if (TrainOn) Train();
    break;
   
    case 2:
    //эффект случайное исчезновение и появление ламп 
      RandomAnodes();
    break;
    }

  if(timerTIME.isReady())
 {
   disp++;
   timerTIME.setInterval(5000);
   if (disp>3) {
   // flipFlag = true; 
     disp=0 ; 
     timerTIME.stop();
     timerTIME.setInterval(40);
     flipInit=false;//    
     }
     //Запускать эффект смены изображения только при смене режима

    RandomAnodesOn=true;
    TrainOn=true;

 }

if (disp==1) num=pricebtc;
if (disp==2) num=priceeth;
if (disp==3) num=TempValue;

num1=num%10;
num/=10;
num2=num%10;
num/=10;
num3=num%10;
num/=10;
num4=num%10; 
num/=10;
num5=num%10; 
num/=10;
num6=num%10; 

switch (disp)
{
case 0:
if(second==30){ timerTIME.start();}//Запускаем таймер для вывода Битка и эфира
//Выводим время
 if (!flipInit) 
      { 
        newhour = hour;
        newminute = minute;
        newsecond = second;     
        setNewTime();  
      } 
  /*
  indiDigits[0] = (byte)hour / 10;
  indiDigits[1] = (byte)hour % 10;
  indiDigits[2] = (byte)minute / 10;
  indiDigits[3] = (byte)minute % 10;
  indiDigits[4] = (byte)second / 10;
  indiDigits[5] = (byte)second % 10;
  */
  break;

case 1:
//Выводим Биток
if (pricebtc>0){
if (!flipInit) 
      {
        newhour = (num6*10)+num5;
        newminute = (num4*10)+num3;
        newsecond = (num2*10)+num1;
        setNewTime();
      }
}
else disp++;
/*
  indiDigits[0] = (byte)num6;
  indiDigits[1] = (byte)num5;
  indiDigits[2] = (byte)num4;
  indiDigits[3] = (byte)num3;
  indiDigits[4] = (byte)num2;
  indiDigits[5] = (byte)num1;
*/
  break;
  case 2:
  //Выводим Эфириум
  if (priceeth>0){
  if (!flipInit) 
      {
        newhour = (num6*10)+num5;
        newminute = (num4*10)+num3;
        newsecond = (num2*10)+num1;
        setNewTime();
      }
  }
  else disp++;
  break;
    case 3:
  //Выводим Эфириум
  if (!flipInit) 
      {
        newhour = (num6*10)+num5;
        newminute = (num2*10)+num1;
        newsecond = (num4*10)+num3;
        setNewTime();
      }
     // 
  break;
}
 
 //glitchTick();                    //Глюки
 
// TestTimerI(); 
                      
                      
}