#include "mqttWrapper.h"

mqttWrapper::mqttWrapper()  {
 
}

uint8_t mqttWrapper::init(const char* mqtt_server, const char* client_id){
  this->mqtt_server = mqtt_server;
  this->client_id = client_id;
  client.begin(mqtt_server, 1883, net);
  int i=0;
  while (!client.connect(client_id)) {
    Serial.print(".");
    delay(10);
    i++;
    if(i>10){
      return 1;
    }
  }
  return 0;
}

uint8_t mqttWrapper::sendMQTT(float temp, float humi){
  client.loop();
  int i=0;
  while (!client.connect(client_id)) {
    Serial.print(".");
    delay(10);
    i++;
    if(i>10){
      return 1;
    }

  }
  char buff[10];
  snprintf (buff, sizeof(buff), "%.2f", temp);
  client.publish("/wordclock/readings/temp", buff);
  snprintf (buff, sizeof(buff), "%.2f", humi);
  client.publish("/wordclock/readings/humi", buff);
  return 0;
}
