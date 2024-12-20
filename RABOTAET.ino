
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>

//Конфигурация пинов на ESP32
#define RST_PIN 5
#define SS_PIN 21

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Создаем подключение с MFRC522

//Подключение к Wi-Fi
const char *ssid = "DECO222";            // Имя вайфай точки доступа
const char *pass = "66065401!";          // Пароль от точки доступа
                                         //Подключение к MQTT серверу
const char *mqtt_server = "m7.wqtt.ru";  // Имя сервера MQTT
const int mqtt_port = 15128;             // Порт для подключения к серверу MQTT
const char *mqtt_user = "u_QPTT9R";      // Логи от сервер
const char *mqtt_pass = "fAhauOpC";      // Пароль от сервера


//Функция получения данных от сервера
WiFiClient wclient;
PubSubClient client(wclient, mqtt_server, mqtt_port);



int faza = 0;
byte readCard[4];
String cardID = "1234";  // замените на ID своей метки
String tagID = "";
String newID = "";


// Открытие двери через Relay
void open_lock() {
  pinMode(22, OUTPUT);  // ini пин для Relay
  digitalWrite(22, 0);
  delay(20);
  digitalWrite(22, 1);
  delay(4000);
}
void callback(const MQTT::Publish &pub) {


  String payload = pub.payload_string();

  if (String(pub.topic()) == "new/button")  // проверяем из нужного ли нам топика пришли данные
  {
    Serial.println("#");
    Serial.println(payload);
    Serial.println("$");

    if (payload == "45") {
      open_lock();
    } else {
      cardID = payload;
    }
  }
  if (String(pub.topic()) == "test/button")  // проверяем из нужного ли нам топика пришли данные
  {
  }
}
void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);                           // Дополнительный дилей
  mfrc522.PCD_DumpVersionToSerial();  // Показываем информацию о MFRC522
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    while (getID()) {
      if (tagID == cardID) {
        Serial.println("ДОСТУП ПРЕДОСТАВЛЕН!");
        open_lock();
      } else {
        Serial.println("КАРТА ОБНАРУЖЕНА!");
      }
      Serial.print("ID: ");
      Serial.println(tagID);
      client.publish("new/tag", tagID);
      delay(2000);
      digitalWrite(6, LOW);
    }
    Serial.print("ПОДКЛЮЧЕНИЕ К ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi ПОДКЛЮЧЕН");
  }

  // подключаемся к MQTT серверу
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      Serial.println("ПОДКЛЮЧЕНИЕ К MQTT СЕРВЕРУ");
      if (client.connect(MQTT::Connect("Dver")
                           .set_auth(mqtt_user, mqtt_pass))) {
        Serial.println("ПОДКЛЮЧЕН К MQTT СЕРВЕРУ");
        client.set_callback(callback);
        client.subscribe("new/button");  // подписывааемся по топик с данными для светодиода
      } else {
        Serial.println("НЕ УДОЛОСЬ ПОДКЛЮЧИТСЯ К MQTT СЕРВЕРУ :(");
        while (getID()) {
          if (tagID == cardID) {
            Serial.println("ДОСТУП ПРЕДОСТАВЛЕН!");
            open_lock();
          } else {
            Serial.println("!!! ДОСТУП ЗАПРЕЩЕН !!!");
          }
          Serial.print("ID: ");
          Serial.println(tagID);
          client.publish("new/tag", tagID);
          delay(2000);
          digitalWrite(6, LOW);
        }
      }
    }

    if (client.connected()) {
      client.loop();
      while (getID()) {
        if (tagID == cardID) {
          Serial.println("ДОСТУП ПРЕДОСТАВЛЕН!");
          open_lock();
        } else {
          Serial.println("!!! ДОСТУП ЗАПРЕЩЕН !!!");
        }
        Serial.print("ID: ");
        Serial.println(tagID);
        client.publish("new/tag", tagID);
        delay(2000);
        digitalWrite(6, LOW);
      }
    }
  }
}

boolean getID() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return false;
  }

  tagID = "";

  for (uint8_t i = 0; i < 4; i++) {
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  tagID.toUpperCase();
  mfrc522.PICC_HaltA();
  return true;
}
