//**********************************Подключаемся к coingecko и получаем значение btc и eth**********************************
void getCrypto(){
   
   byte j=11;
    while (j<=12) {
      SensorsAutoShow[j]="";
      SensorsDisplay[j]="";
      j++;
    }
   String serverPath = "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin,ethereum&vs_currencies=USD";
   String jsonBuffer = httpGETRequest(serverPath.c_str());
   JSONVar myObject = JSON.parse(jsonBuffer);
   pricebtc = myObject["bitcoin"]["usd"]; 
   priceeth = myObject["ethereum"]["usd"];
   
   SensorsAutoShow[11]+=",";
   SensorsAutoShow[11]+="BTC ";
   SensorsAutoShow[11]+=pricebtc;
   SensorsAutoShow[11]+=" $";

   SensorsAutoShow[12]+=",";
   SensorsAutoShow[12]+="ETH ";
   SensorsAutoShow[12]+=priceeth;
   SensorsAutoShow[12]+=" $";

   SensorsDisplay[11]+="B";
   SensorsDisplay[11]+=pricebtc;
 
   SensorsDisplay[12]+="E";
   SensorsDisplay[12]+=priceeth;


  
}
