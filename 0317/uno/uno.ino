#include <ArduinoJson.h>
#include "DHT.h"
#include <Thread.h>
#define DHTTYPE    DHT11
#define DHTPIN  2  // 温湿度传感器引脚
int buzzerPin = 3; //蜂鸣器引脚
int mq2Pin = A0; //烟雾传感器引脚
int pirPin = 7; //人体传感器引脚
DHT dht(DHTPIN, DHTTYPE);
int ledPin = LED_BUILTIN;
Thread myThread = Thread();
int mq2MaxValue = 400; //默认阈值为400
void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  pinMode(pirPin, INPUT);
  dht.begin();
  digitalWrite(buzzerPin, HIGH);
  pinMode(ledPin, OUTPUT);
  myThread.onRun(niceCallback);
  myThread.setInterval(5000);
}
int mq2Value;
int pirVlue;
void niceCallback() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  mq2Value = analogRead(mq2Pin);

  pirVlue = digitalRead(pirPin); //等于1则为有人
  const int capacity = JSON_OBJECT_SIZE(5);
  StaticJsonDocument<capacity> doc;//声明一个JsonDocument对象
  doc["temperature"] = t; //温度
  doc["humidity"] = h; //湿度
  doc["mq2"] = mq2Value; //烟雾
  doc["pir"] = pirVlue; //是否有人
  doc["max"] = mq2MaxValue; //阈值
  String output = "";
  serializeJson(doc, output);
  Serial.print(output);
}
String readString;
void loop() {

  if (myThread.shouldRun())
  {
    myThread.run();
  }
  while (Serial.available()) { //返回串口缓冲区中当前剩余的字符个数
    char c = Serial.read(); //读取数据
    readString += c;
    delay(2);
  }

  if (readString.length() > 0) { //串口接收到了数据，可以读取
    //Serial.println(readString);

    if (readString == "open") {
      digitalWrite(ledPin, HIGH);
      
    }

    if (readString == "close") {
      digitalWrite(ledPin, LOW);
     

    }

    if(readString!="open" && readString!="close"){
      mq2MaxValue = readString.toInt();
    }

    readString = "";
  }


  if (mq2Value > mq2MaxValue || pirVlue == 1) {
    digitalWrite(buzzerPin, LOW);
  }

  if (mq2Value < mq2MaxValue && pirVlue == 0) {
    digitalWrite(buzzerPin, HIGH);

  }





}
