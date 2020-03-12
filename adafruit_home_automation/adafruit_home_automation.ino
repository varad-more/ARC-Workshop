#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Setup ****************************************/
#define WLAN_SSID       "******"              // Your SSID
#define WLAN_PASS       "*********"          // Your password

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"       //Adafruit Server
#define AIO_SERVERPORT  1883                    //Server Port
#define AIO_USERNAME    "*********"          // Username
#define AIO_KEY         "**********"           // Auth Key

//WIFI CLIENT
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe localFeedObjectName = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/led");  //Enter Your Feed Name According to Adafruit.io's Feed. Create local feed object by renaming "localFeedObjectName" 




void MQTT_connect();

void setup() {
  Serial.begin(9600);

  pinMode(D1, OUTPUT);                                 //Set Pinmode of all the pins Used
  

  // Connect to WiFi access point.
  
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);


  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();


  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  mqtt.subscribe(&localFeedObjectName);  //Subscribe to all your Feed

}

void loop() {

  MQTT_connect();


  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000)))
  {
    if (subscription == &localFeedObjectName)
    {
      Serial.print(F("Got: "));
      Serial.println((char *)localFeedObjectName.lastread);
      int State = atoi((char *)localFeedObjectName.lastread);
      digitalWrite(D1, State);
    }
  }
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;

  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");

}
