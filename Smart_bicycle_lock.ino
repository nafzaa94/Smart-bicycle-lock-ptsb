#define BLYNK_TEMPLATE_ID           "TMPLj-f8Oiso"
#define BLYNK_DEVICE_NAME           "Smart bicycle lock"
#define BLYNK_AUTH_TOKEN            "fLdlufPV0GGYElKpJrH8SDAl5NXmlD05"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

int vibsensor = 34;
int resett = 12;

int valuevibsensor = 0;
int valuereset = 0;
int state = 0;

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "HP";
char pass[] = "nafza9494";

BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  valuevibsensor = digitalRead(vibsensor); // 0 1;
  valuereset = digitalRead(resett);

  if (valuevibsensor == LOW && state == 0){
    Blynk.logEvent("notification", "ada pencuri");
    state = 1;
    }

  if (valuereset == HIGH){
    state = 0;
    }
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(vibsensor, INPUT);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
