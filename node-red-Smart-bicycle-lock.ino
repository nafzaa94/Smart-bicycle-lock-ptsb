#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

Servo myservo;

// Replace the next variables with your SSID/Password combination
const char* ssid = "HP";
const char* password = "nafza9494";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.43.228";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// LED Pin
const int ledPin = 4;
int servolock = 27;
int vibsensor = 34;
int buzz = 2;


int valuevibsensor = 0;
int state = 0;
int statelock = 0;
int count = 0;

void setup() {
  Serial.begin(115200);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);// Standard 50hz servo
  myservo.attach(servolock, 500, 2400);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(vibsensor, INPUT);
  myservo.write(0);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  String messageStart;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
    messageStart += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/lock") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      myservo.write(90);
      statelock = 1;
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      myservo.write(0);
      statelock = 0;
    }
  }

  else if (String(topic) == "esp32/start"){
    if(messageStart == "start"){
      Serial.println("start");
      state = 0;
      }
    }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/lock");
      client.subscribe("esp32/start");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  valuevibsensor = digitalRead(vibsensor);

  if (valuevibsensor == HIGH && state == 0){
    if (statelock == 0){
      for (int i = 0; i < 5; i++){
      client.publish("esp32/notification", "notification");
      }
      digitalWrite(buzz, HIGH);
      Serial.println("buzz on");
      delay(5000);
      digitalWrite(buzz, LOW);
      Serial.println("buzz off");
      state = 1;
      }
    }
}
