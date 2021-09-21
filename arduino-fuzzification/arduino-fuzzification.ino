#include <DHT.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

#define DHTPIN 5
#define DHTTYPE DHT11

WiFiMulti wifiMulti;
DHT dht(DHTPIN, DHTTYPE);

float suhu;
float kelembaban;
String lama_waktu;
String cepat = "Cepat";
String sedang = "Sedang";
String lama = "Lama";
String temp_after;
String hum_after;

void setup() {
  Serial.begin(115200);
  for(uint8_t t=4; t>0; t--){
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  wifiMulti.addAP("Your SSID", "Your SSID Password");
  Serial.println("Network is connected...");
  dht.begin();
}

float fuzzy_cold(){
  float cold;

  if (suhu <= 0){
    cold = 1;
  } else if (suhu >= 0 && suhu <= 3){
    cold = (3 - suhu) / 3;
    //cold = cold;
  } else if (suhu >= 3){
    cold = 0;
  }

  return cold;
}

float fuzzy_cool(){
  float cool;

  if(suhu <= 0 || suhu >= 15){
    cool = 0;
  } else if (suhu >= 0 && suhu <= 7.5){
    cool = (suhu - 0) / 7.5;
  } else if (suhu >= 7.5 && suhu <= 15){
    cool = (15 - suhu) / 7.5;
  }
  return cool;
}

float fuzzy_normal(){
  float normal;

  if (suhu <= 12 || suhu >= 27){
    normal = 0;
  } else if (suhu >= 12 && suhu <= 19.5){
    normal = (suhu - 12) / 7.5;
  } else if (suhu >= 19.5 && suhu <= 27){
    normal = (27 - suhu) / 7.5;
  }
  return normal;
}

float fuzzy_warm(){
  float warm;

  if (suhu <= 24 || suhu >= 39){
    warm = 0;
  } else if (suhu >= 24 && suhu <= 31.5){
    warm = (suhu - 24) / 7.5;
  } else if (suhu >= 31.5 && suhu <= 39){
    warm = (39 - suhu) / 7.5;
  }
  return warm;
}

float fuzzy_hot(){
  float hot;

  if (suhu <= 36){
    hot = 0;
  } else if (suhu >= 36 && suhu <= 39){
    hot = (36 - suhu) / 3;
    hot = -hot;
  } else if (suhu >= 39){
    hot = 1;
  }
  return hot;
}

float fuzzy_dry(){
  float dry;

  if (kelembaban <= 10){
    dry = 1;
  } else if (kelembaban >= 10 && kelembaban <= 20){
    dry = (20 - kelembaban) / 10;
  } else if (kelembaban >= 20){
    dry = 0;
  }
  return dry;
}

float fuzzy_moist(){
  float moist;

  if (kelembaban <= 10 || kelembaban >= 50){
    moist = 0;
  } else if (kelembaban >= 10 && kelembaban <= 30){
    moist = (kelembaban - 10) / 20;
  } else if (kelembaban >= 30 && kelembaban <= 50){
    moist = (50 - kelembaban) / 20;
  }
  return moist;
}

float fuzzy_wet(){
  float wet;

  if (kelembaban <= 40){
    wet = 0;
  } else if (kelembaban >= 40 && kelembaban <= 50){
    wet = (kelembaban - 40) / 10;
  } else if (kelembaban >= 50){
    wet = 1;
  }
  return wet;
}

void loop() {
  suhu = dht.readTemperature();
  kelembaban = dht.readHumidity();
  float cold = fuzzy_cold();
  float cool = fuzzy_cool();
  float normal = fuzzy_normal();
  float warm = fuzzy_warm();
  float hot = fuzzy_hot();
  float dry = fuzzy_dry();
  float moist = fuzzy_moist();
  float wet = fuzzy_wet();

  //Menentukan label kelembaban
  if (dry > moist && dry > wet){
    hum_after = "dry";
  } else if (moist > dry && moist > wet){
    hum_after = "moist";
  } else if (wet > dry && wet > moist){
    hum_after = "wet";
  }

  //Menentukan label suhu
  if (cold > cool && cold > normal && cold > warm && cold > hot){
    temp_after = "cold";
  } else if (cool > cold && cool > normal && cool > warm && cool > hot){
    temp_after = "cool";
  } else if (normal > cold && normal > cool && normal > warm && normal > hot){
    temp_after = "normal";
  } else if (warm > cold && warm > cool && warm > normal && warm > hot){
    temp_after = "warm";
  } else if (hot > cold && hot > cool && hot > normal && hot && warm){
    temp_after = "hot";
  }

  //Menentukan Perbandingan Label Suhu dan Kelembaban
  if (temp_after == "cold" && hum_after == "dry"){
    lama_waktu = lama;
  } else if (temp_after == "cold" && hum_after == "moist"){
    lama_waktu = lama;
  } else if (temp_after == "cold" && hum_after == "wet"){
    lama_waktu = cepat;
  } else if (temp_after == "cool" && hum_after == "dry"){
    lama_waktu = lama;
  } else if (temp_after == "cool" && hum_after == "moist"){
    lama_waktu = sedang;
  } else if (temp_after == "cool" && hum_after == "wet"){
    lama_waktu = cepat;
  } else if (temp_after == "normal" && hum_after == "dry"){
    lama_waktu = lama;
  } else if (temp_after == "normal" && hum_after == "moist"){
    lama_waktu = sedang;
  } else if (temp_after == "normal" && hum_after == "wet"){
    lama_waktu = cepat;
  } else if (temp_after == "warm" && hum_after == "dry"){
    lama_waktu = lama;
  } else if (temp_after == "warm" && hum_after == "moist"){
    lama_waktu = sedang;
  } else if (temp_after == "warm" && hum_after == "wet"){
    lama_waktu = cepat;
  } else if (temp_after == "hot" && hum_after == "dry"){
    lama_waktu = lama;
  } else if (temp_after == "hot" && hum_after == "moist"){
    lama_waktu = sedang;
  } else if (temp_after == "hot" && hum_after == "wet"){
    lama_waktu = cepat;
  }

  Serial.println();
  Serial.print("Suhu : " + (String)suhu + "\t");
  Serial.println("Kelembaban : " + (String)kelembaban + "\t");
  Serial.println("Fuzzy Logic Suhu");
  Serial.print("Cold : " + (String)cold + "\t");
  Serial.print("Cool : " + (String)cool + "\t");
  Serial.print("Normal : " + (String)normal + "\t");
  Serial.print("Warm : " + (String)warm + "\t");
  Serial.println("Hot : " + (String)hot);
  Serial.println("Fuzzy Logic Kelembaban");
  Serial.print("Dry : " + (String)dry + "\t");
  Serial.print("Moist : " + (String)moist + "\t");
  Serial.println("Wet : " + (String)wet);
  Serial.print("Kategori Suhu : " + temp_after + "\t");
  Serial.println("Kategori Kelembaban : " + hum_after + "\t");
  Serial.println("Lama Waktu : " + lama_waktu);
  
  if ((wifiMulti.run() == WL_CONNECTED)){
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");

    String urlsend = "http://YOUR IP ADDRESS/fuzzification/insert.php?suhu=" + 
                     (String)suhu + "&kelembaban=" + (String)kelembaban +
                     "&lama_waktu=" + lama_waktu + "";

    Serial.println("URL to send: " + urlsend);
    http.begin(urlsend);

    Serial.print("[HTTP] GET...\n");

    int httpCode = http.GET();

    if (httpCode > 0){
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK){
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n",
      http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  delay(5000);
}
