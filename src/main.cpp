/*
 * Copyright (C) 2017 Tomas Nilsson (joekickass)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at

 *    http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Dummy main file - to be deleted
 */
#include <Arduino.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>

#include "config/ConnectionParams.h"
#include "mqtt/MqttClient.h"
#include "aws/AwsIotSigv4.h"
#include "aws/ESP8266DateTimeProvider.h"

#include "motor/motor.h"

WiFiManager wifiManager;

ESP8266DateTimeProvider dtp;
AwsIotSigv4 sigv4(&dtp);
ConnectionParams cp(sigv4);
WebSocketClientAdapter adapter(cp);
MqttClient client(adapter, cp);
Motor motor;
DynamicJsonBuffer jsonBuffer(400);
char reply[40];

void callback(const char* topic, const char* msg)
{
  // Update state
  Serial.printf("Got msg '%s' on topic %s\n", msg, topic);
	JsonObject& root = jsonBuffer.parseObject(msg);
	if (!root.success()) {
    Serial.println("parseObject() failed");
		return;
  }
	long speed = root["state"]["speed"];
  if (speed == 0) {
    motor.stop();
    Serial.printf("Stop\n");
  } else if (speed < 0 && speed >= -100) {
    uint16_t vel = speed * -10.23;
    Serial.printf("Back: %d\n", vel);
    motor.backward(vel);
  } else if (speed > 0 && speed <= 100) {
    uint16_t vel = speed * 10.23;
    Serial.printf("Forward: %d\n", vel);
    motor.forward(vel);
  } else {
    Serial.printf("Illegal speed %d\n", speed);
    return;
  }

  // Report back
  sprintf(reply, "{\"state\":{\"reported\":{\"speed\":%d}}}", speed);
  Serial.printf("Reporting back: %s\n", reply);
  client.publish("$aws/things/train/shadow/update", reply, 0, false);
  client.yield();
}

void setup()
{
  // Set up and wait for serial port to connect
  Serial.begin (115200);

  // Set up wifi
  wifiManager.autoConnect("trainAP");
  Serial.println("connected to wifi...");

  // Setup motor
  motor.init();
  Serial.println("motor initialized...");

  // Connect mqtt client
  int res = client.connect();
  if (res == 0) {
    Serial.println("connected to aws iot...");
    client.subscribe("$aws/things/train/shadow/update/delta", 0, callback);
  } else {
    Serial.printf("Failed to connect to AWS IoT service. res=%d\n", res);
  }
}

void loop()
{
  if (client.isConnected()) {
    client.yield();
  } else {

    Serial.println("Not connected...");
    delay(10000);

    int res = client.connect();
    if (res == 0) {
      Serial.println("connected to aws iot...");
      client.subscribe("$aws/things/train/shadow/update/delta", 0, callback);
    } else {
      Serial.printf("Failed to connect to AWS IoT service. res=%d\n", res);
    }
  }
}
