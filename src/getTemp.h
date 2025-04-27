

void getTemp(){

String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&APPID=" + openWeatherMapApiKey +"&units=metric";

//String jsonBuffer;
String jsonBuffer = httpGETRequest(serverPath.c_str());
//Serial.println(jsonBuffer);
JSONVar myObject = JSON.parse(jsonBuffer);
//String tempstring1 = myObject["main"]["temp"]; 
int temperature = myObject["main"]["feels_like"]; 

//float tempstring1 = Serial.println(myObject["main"]["temp"]);
//Serial.println(tempstring1);
//float temperature = tempstring;
//Serial.println(temperature);
 if (temperature>=0) {TempValue = int(temperature); minus=false;}
 else {TempValue = int(temperature*-1); minus=true;}//Если отрицательное преобразуем в положительное значение для корректного отображения.

      //Serial.println(myObject["main"]["pressure"]);
      //Serial.println(myObject["main"]["humidity"]);
      //Serial.println(myObject["wind"]["speed"]);
/* 
HTTPClient temp;
temp.begin("http://narodmon.ru/api/sensorsOnDevice?id="+TempSensorId+"&uuid="+UUID+"&api_key="+NarodMonApiKey+"&lang=ru");
//http.begin("http://narodmon.ru/api/sensorsOnDevice?id=6678&uuid=004f8154a800615565d1c35fca92f621&api_key=eyQhbFzr8XBmm&lang=ru");
int httpCode = temp.GET();                                                                  //Send the request
 
if (httpCode > 0) { //не пустое
 
String payload = temp.getString();   //Get the request response payload   

  //https://arduinojson.org/v5/assistant/
  
  //const size_t capacity = JSON_ARRAY_SIZE(3) + 3*JSON_OBJECT_SIZE(11) + JSON_OBJECT_SIZE(15) + 580;
  DynamicJsonBuffer jsonBuffer;//(capacity);
  JsonObject& root = jsonBuffer.parseObject(payload);
  JsonArray& sensors = root["sensors"];
  JsonObject& sensor1 = sensors[0];
  float temperature = round(sensor1["value"]); 

 if (temperature>=0) {TempValue = int(temperature); minus=false;}
 else {TempValue = int(temperature*-1); minus=true;}//Если отрицательное преобразуем в положительное значение для корректного отображения.
}
 
temp.end();   //
*/ 
}




