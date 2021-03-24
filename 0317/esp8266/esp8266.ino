#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>


const char* ssid = "移动热点";  //家里的wifi账号
const char* password = "1311902735"; //家里的wifi密码
#define IP  "183.230.40.39" //onenet 服务器地址
#define MQTT_PORT  6002 //端口
#define PRODUCT_ID    "407688" //产品ID
#define DEVICE_ID    "691338057" //设备ID
#define API_KEY    "uTGrygpSqAZcK5LXjxkgVlsxCmc=" //API_KEY
#define HOST  "api.heclouds.com" //上传数据流的地址


WiFiClient espClient; //WIFI客户端对象
PubSubClient client(espClient); //定义MQTT客户端
HTTPClient http; //http请求对象

void callback(char *topic, byte * payload, unsigned int length) {
  String str = "";
  for (int i = 0; i < length; i++) {
    str += (char) payload[i];
  }

  Serial.print(str);  //发送控制的命令到UNO


}
//*订阅主题后收到信息的回调函数（用于进行远程控制）

void setup() {
  Serial.begin(115200); //设置波特率


  WiFi.mode(WIFI_STA); //设置WiFi的模式为STA
  WiFi.begin(ssid, password); //连接wifi
  




  //如果wifi正确连接就退出循环
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
   // Serial.print(".");
  }

  client.setServer(IP, MQTT_PORT);//设置MQTT服务器的地址端口
  client.setCallback(callback); //接收服务器发来的消息事件
  while (!client.connected()) {

    client.connect(DEVICE_ID, PRODUCT_ID, API_KEY); //连接

  }

  client.publish("esp8266", "hello"); //发送消息到MQTT
}

String readString;
void loop() {



  client.loop();

  while (Serial.available()) { //返回串口缓冲区中当前剩余的字符个数
    char c = Serial.read(); //读取数据
    readString += c;
    delay(2);
  }

  if (readString.length() > 0) { //串口接收到了数据，可以读取
    //Serial.println(readString);



    DynamicJsonDocument doc(1024);  //定义一个动态JSON对象 大小1024
    deserializeJson(doc, readString); //反序列化JSON
    float temperature = doc["temperature"]; //解析uno发过来的温度
    int humidity = doc["humidity"]; //解析uno发过来的温度
    int mq2 = doc["mq2"];
    int pir = doc["pir"];
    int maxV = doc["max"];

    uploadDht("温度", temperature); //上传温度至onenet
    uploadDht("湿度", humidity);
    uploadDht("烟雾", mq2);
    uploadDht("是否有人", pir); //等于1则为有人
    uploadDht("阈值", maxV); 
    readString = "";
  }

}


void uploadDht(String dataName, float dataValue) {
  
  DynamicJsonDocument doc(100); //定义动态json文档


  JsonArray arr1 = doc.createNestedArray("datastreams"); //创建一个嵌套Json数组

  JsonObject node_1 = arr1.createNestedObject(); //创建一个嵌套Json对象

  node_1["id"] = dataName.c_str(); //String转char

  JsonArray arr2 = node_1.createNestedArray("datapoints"); //创建一个嵌套Json数组

  JsonObject node_1_1 = arr2.createNestedObject(); //创建一个嵌套Json对象
  node_1_1["value"] = dataValue;

  String output = "";
  serializeJson(doc, output); //序列化Json
  //Serial.println(output);

  http.begin("http://api.heclouds.com/devices/" + String(DEVICE_ID) + "/datapoints");  //onenet上报数据点api
  http.addHeader("Content-Type", "application/json"); //设置请求头为application/json
  http.addHeader("api-key", API_KEY); //设置api-key
  int httpCode = http.POST(output);  //提交请求后的状态码
  String payload = http.getString(); //获取onenet服务器响应

 
  http.end(); //关闭连接

}
