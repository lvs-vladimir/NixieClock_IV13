void getCrypto(){//Подключаемся к coingecko и получаем значение btc и eth

HTTPClient crypto;
//https://api.open-meteo.com/v1/forecast?latitude=53.36&longitude=83.76&current=temperature
//https://api.weatherapi.com/v1/current.json?key=d20d799046844f8f85465416240902&q=Barnaul
 crypto.begin("https://api.coingecko.com/api/v3/simple/price?ids=bitcoin,ethereum&vs_currencies=USD"); //Specify the URL and certificate
    int httpCode = crypto.GET(); 
 if (httpCode > 0) { //Check for the returning code
  String response = crypto.getString();
  JSONVar myObject = JSON.parse(response);
  pricebtc = myObject["bitcoin"]["usd"]; 
  priceeth = myObject["ethereum"]["usd"];
  //Serial.println(pricebtc);
  //Serial.println(priceeth);
 }
crypto.end();

}
