#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid  = "sanghoon's a52s";  // 연결할 와이파이 공유기(AP)의 SSID명칭
const char* password = "w2f2password";  // 연결한 와이파이 공유기(AP)의 비밀번호
const char* server_ip = "192.168.102.87";  // MQTT Broker의 접속주소
const int server_port = 1883;           // MQTT Broker 접속 포트번호

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

/**
  아두이노 보드에 전원이 켜지거나 리셋버튼을 눌러 리셋될 때 실행되는 함수
*/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

/**
  와이파이 공유기에 접속하는 기능을 구현한 함수
*/
void connect(){
  WiFi.begin(ssid, password);

  while(WiFi.status()!=WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }

  Serial.println("Connected"); // 연결이 완료되면 Connected 문자열을 시리얼로 출력
}

/**
  MQTT Broker에 연결하는 기능을 구현한 함수
*/
void mqtt_connect(){
  mqtt_client.setServer(server_ip,server_port);

  while(!mqtt_client.connected()){
    String client_id = String(random(0xffff),HEX);
    if(mqtt_client.connect(client_id.c_str())){
      Serial.println("MQTT Broker is connected");
    } else{
      Serial.print("failed, rc=");
      Serial.println(mqtt_client.state());
      delay(10000); // 10초 후 재 접속
    }
  }
}

/**
  아두이노 보드에서 setup 함수의 실행 후 무한 반복해서 호출되는 함수
*/
void loop() {
  // put your main code here, to run repeatedly:
  while(true){
    // 와이파이 공유기 연결이 되어 있지 않으면 와이파이 공유기 연결을 실행
    if(WiFi.status()!=WL_CONNECTED){
      connect();
    }

    // MQTT Broker연결이 되어 있지 않으면 MQTT Broker 연결
    if(!mqtt_client.connected()){
      mqtt_connect();
    }
  
    // MQTT Broker에 메시지 발행
    mqtt_client.publish("myTopic","Hello,World");
    Serial.println("Message is published.");
    delay(1000); // 1초 대기
  }
}
