#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <routerLogin.h>

//#define DEBUG

#ifdef DEBUG
  #define SERIAL_PRINT(x) Serial.print(x)
  #define SERIAL_PRINTLN(x) Serial.println(x)
#else
  #define SERIAL_PRINT(x)
  #define SERIAL_PRINTLN(x)
#endif


const char* ssid = STASSID;
const char* password = STAPSK;
const char* host = "simpleLED";

int led_pin = 2;
#define N_DIMMERS 3
int dimmer_pin[] = {14, 5, 15};

void setup() {
  #ifdef DEBUG
    Serial.begin(115200);
  #endif

  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, HIGH);

  SERIAL_PRINTLN("Booting");
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    SERIAL_PRINTLN("Retrying connection...");
  }

  ArduinoOTA.setHostname(host);
  ArduinoOTA.onStart([]() { // switch off all the PWMs during upgrade
    for (int i = 0; i < N_DIMMERS; i++) {
      analogWrite(dimmer_pin[i], 0);
    }
    analogWrite(led_pin, 0);
  });

  ArduinoOTA.onEnd([]() {});

  ArduinoOTA.onError([](ota_error_t error) {
    (void)error;
    ESP.restart();
  });

  /* setup the OTA server */
  ArduinoOTA.begin();
  SERIAL_PRINTLN("Ready");

}

void loop() {
  ArduinoOTA.handle();

  digitalWrite(led_pin, LOW);
  SERIAL_PRINTLN("loop");
  delay(1000);
  digitalWrite(led_pin, HIGH);
  delay(1000);
}
