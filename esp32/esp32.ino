#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "Dost";
const char* password = "Dost2023";

// MQTT settings
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* subscribe_topic = "esp32/led";
const char* publish_topic = "esp32/status";

WiFiClient espClient;
PubSubClient client(espClient);

const int ledPin = 2;

void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected. IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("Message: ");
  Serial.println(msg);

  if (msg == "1") {
    digitalWrite(ledPin, HIGH);
    client.publish(publish_topic, "LED is ON");
  } else if (msg == "0") {
    digitalWrite(ledPin, LOW);
    client.publish(publish_topic, "LED is OFF");
  } else {
    client.publish(publish_topic, "Invalid command");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Trying MQTT...");
    if (client.connect("ESP32S3Client")) {
      Serial.println("Connected.");
      client.subscribe(subscribe_topic);
    } else {
      Serial.print("Failed, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
