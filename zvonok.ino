#include <WiFi.h>
#include <PubSubClient.h>
#include <time.h> 
int pelmeni = 2; 
int LED = 13;
    //Настройка Wi-Fi 
const char *ssid = "ZTE MF920RU_6FC112";    // Имя вайфай точки доступа 
const char *pass = "YE5YG7B5ZH";            // Пароль от точки доступа 
    //Настройка MQTT
const char *mqtt_server = "m7.wqtt.ru";     // Имя сервера MQTT 
const int mqtt_port = 15128;                // Порт для подключения к серверу MQTT 
const char *mqtt_user = "u_QPTT9R";         // Логи от сервер 
const char *mqtt_pass = "fAhauOpC";         // Пароль от сервера 
    //Настройка Интернет-времени
const char* ntpServer = "pool.ntp.org"; 
const long gmtOffset_sec = 3600; 
const int daylightOffset_sec = 3600; 
 
int Faza = 0; 
int chasi = 3; 
 
WiFiClient wclient; 
PubSubClient client(wclient, mqtt_server, mqtt_port); 
 
void ring(){ 
    Serial.println("RRRR"); 
    pinMode(pelmeni, OUTPUT); 
    digitalWrite(pelmeni, 0); 
    delay(5000); 
    digitalWrite(pelmeni, 1); 
} 
 
int zvonki(){ 
    struct tm timeinfo; 
    if (!getLocalTime(&timeinfo)) { 
        Serial.println("Неудалось синхронизировать время :("); 
        return 1; 
    } 
    int time_wday = timeinfo.tm_wday; 
    int time_hr = timeinfo.tm_hour + chasi; 
    int time_min = timeinfo.tm_min; 
    int time_sec = timeinfo.tm_sec; 
    Serial.println("tm_wday: " + String(time_wday)); 
    Serial.println("tm_hr: " + String(time_hr)); 
    Serial.println("tm_min: " + String(time_min)); 
    Serial.println("tm_sec: " + String(time_sec)); 
    String val = "tm_wday: " + String(time_wday) + "; hr: " + String(time_hr) + ":" + String(time_min) + ":" + String(time_sec); 
    client.publish("test/timenow", val); 
    if (Faza == 0) { 
        if ((time_wday == 2) or (time_wday == 3) or (time_wday == 5)) { 
        if ((time_hr == 8 ) and (time_min == 0 ) and (time_sec < 3)) ring(); 
        if ((time_hr == 8 ) and (time_min == 40) and (time_sec < 3)) ring(); 
        if ((time_hr == 8 ) and (time_min == 50) and (time_sec < 3)) ring(); 
        if ((time_hr == 9 ) and (time_min == 30) and (time_sec < 3)) ring(); 
        if ((time_hr == 9 ) and (time_min == 40) and (time_sec < 3)) ring(); 
        if ((time_hr == 10) and (time_min == 20) and (time_sec < 3)) ring(); 
        if ((time_hr == 10) and (time_min == 40) and (time_sec < 3)) ring(); 
        if ((time_hr == 11) and (time_min == 20) and (time_sec < 3)) ring(); 
        if ((time_hr == 11) and (time_min == 30) and (time_sec < 3)) ring(); 
        if ((time_hr == 12) and (time_min == 10) and (time_sec < 3)) ring(); 
        if ((time_hr == 12) and (time_min == 20) and (time_sec < 3)) ring(); 
        if ((time_hr == 13) and (time_min == 0 ) and (time_sec < 3)) ring(); 
        if ((time_hr == 13) and (time_min == 10) and (time_sec < 3)) ring(); 
        if ((time_hr == 13) and (time_min == 50) and (time_sec < 3)) ring(); 
        if ((time_hr == 14) and (time_min == 0 ) and (time_sec < 3)) ring(); 
        if ((time_hr == 14) and (time_min == 40) and (time_sec < 3)) ring(); 
        if ((time_hr == 14) and (time_min == 50) and (time_sec < 3)) ring(); 
        if ((time_hr == 15) and (time_min == 30) and (time_sec < 3)) ring(); 
        if ((time_hr == 15) and (time_min == 50) and (time_sec < 3)) ring(); 
        if ((time_hr == 16) and (time_min == 30) and (time_sec < 3)) ring(); 
        if ((time_hr == 16) and (time_min == 40) and (time_sec < 3)) ring(); 
        if ((time_hr == 17) and (time_min == 20) and (time_sec < 3)) ring(); 
        if ((time_hr == 17) and (time_min == 30) and (time_sec < 3)) ring(); 
        if ((time_hr == 18) and (time_min == 10) and (time_sec < 3)) ring(); 
        if ((time_hr == 18) and (time_min == 20) and (time_sec < 3)) ring(); 
        if ((time_hr == 19) and (time_min == 0 ) and (time_sec < 3)) ring(); 
    } 
        if ((time_wday == 1) or (time_wday == 4)) { 
            if ((time_hr == 8 ) and (time_min == 0 ) and (time_sec < 3)) ring(); 
            if ((time_hr == 8 ) and (time_min == 30) and (time_sec < 3)) ring(); 
            if ((time_hr == 8 ) and (time_min == 35) and (time_sec < 3)) ring(); 
            if ((time_hr == 9 ) and (time_min == 10) and (time_sec < 3)) ring(); 
            if ((time_hr == 9 ) and (time_min == 20) and (time_sec < 3)) ring(); 
            if ((time_hr == 9 ) and (time_min == 55) and (time_sec < 3)) ring(); 
            if ((time_hr == 10) and (time_min == 5 ) and (time_sec < 3)) ring(); 
            if ((time_hr == 10) and (time_min == 40) and (time_sec < 3)) ring(); 
            if ((time_hr == 11) and (time_min == 0 ) and (time_sec < 3)) ring(); 
            if ((time_hr == 11) and (time_min == 35) and (time_sec < 3)) ring(); 
            if ((time_hr == 11) and (time_min == 45) and (time_sec < 3)) ring(); 
            if ((time_hr == 12) and (time_min == 20) and (time_sec < 3)) ring(); 
            if ((time_hr == 12) and (time_min == 30) and (time_sec < 3)) ring(); 
            if ((time_hr == 13) and (time_min == 05) and (time_sec < 3)) ring(); 
            if ((time_hr == 13) and (time_min == 15) and (time_sec < 3)) ring(); 
            if ((time_hr == 13) and (time_min == 50) and (time_sec < 3)) ring(); 
            if ((time_hr == 14) and (time_min == 0 ) and (time_sec < 3)) ring(); 
            if ((time_hr == 14) and (time_min == 30) and (time_sec < 3)) ring(); 
            if ((time_hr == 14) and (time_min == 35) and (time_sec < 3)) ring(); 
            if ((time_hr == 15) and (time_min == 10) and (time_sec < 3)) ring(); 
            if ((time_hr == 15) and (time_min == 20) and (time_sec < 3)) ring(); 
            if ((time_hr == 15) and (time_min == 55) and (time_sec < 3)) ring(); 
            if ((time_hr == 16) and (time_min == 5 ) and (time_sec < 3)) ring(); 
            if ((time_hr == 16) and (time_min == 40) and (time_sec < 3)) ring(); 
            if ((time_hr == 17) and (time_min == 0 ) and (time_sec < 3)) ring(); 
            if ((time_hr == 17) and (time_min == 35) and (time_sec < 3)) ring(); 
            if ((time_hr == 17) and (time_min == 45) and (time_sec < 3)) ring(); 
            if ((time_hr == 18) and (time_min == 20) and (time_sec < 3)) ring(); 
            if ((time_hr == 18) and (time_min == 25) and (time_sec < 3)) ring(); 
            if ((time_hr == 19) and (time_min == 0 ) and (time_sec < 3)) ring(); 
        } 
    } 
} 
void callback(const MQTT::Publish &pub) { 
    Serial.print(pub.topic());             // выводим в сериал порт название топика 
    Serial.print(" => "); 
    Serial.println(pub.payload_string());  // выводим в сериал порт значение полученных данных 
    
    String payload = pub.payload_string(); 
    
    if (String(pub.topic()) == "test/button") { 
        if (payload == "1") ring(); 
        if (payload == "On") Faza = 1; 
        if (payload == "Off") Faza = 0; 
    } 
    if (String(pub.topic()) == "test/chasi") { 
        chasi = payload.toInt(); 
    } 
} 
 
 
 
void setup() { 
    //pinMode(pelmeni, OUTPUT); 
    Serial.begin(115200); 
    pinMode(LED, OUTPUT); 
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); 
} 
 
 
void loop() { 
    // подключаемся к wi-fi 
    if (WiFi.status() != WL_CONNECTED) { 
        Serial.print("Подключение к "); 
        Serial.print(ssid); 
        Serial.println("..."); 
        delay(1000);
        WiFi.begin(ssid, pass); 
        if (WiFi.waitForConnectResult() != WL_CONNECTED) 
            return; 
        Serial.println("WiFi подключен!"); 
    } 
    
    // подключаемся к MQTT серверу 
    if (WiFi.status() == WL_CONNECTED) { 
        if (!client.connected()) { 
        Serial.println("Подключение к MQTT серверу"); 
        if (client.connect(MQTT::Connect("Zvonok") 
            .set_auth(mqtt_user, mqtt_pass))) { 
            Serial.println("Подключен к MQTT серверу"); 
            client.set_callback(callback); 
            client.subscribe("test/button"); // подписывааемся по топик с данными для светодиода 
            client.subscribe("test/chasi"); 
    } else { 
            Serial.println("Не удалось подключиться к MQTT серверу"); 
        } 
    } 
    
        if (client.connected()) { 
            digitalWrite(LED, 1); 
            client.loop(); 
            delay(500); 
            digitalWrite(LED, 0); 
            zvonki(); 
        } 
    } 
}
