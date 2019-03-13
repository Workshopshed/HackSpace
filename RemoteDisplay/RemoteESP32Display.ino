#include <WiFi.h>
#include <PubSubClient.h>
#include <GxEPD2_3C.h>
#include <U8g2_for_Adafruit_GFX.h>

#include "arduino_secrets.h"

const char ssid[] = SECRET_SSID;
const char password[] = SECRET_PWD;
const char mqtt_server[] = "broker.mqttdashboard.com";
const int mqtt_port = 1883;
const char mqtt_topic[] = "automation";
const char mqtt_room[] = "mainroom";

WiFiClient espClient;
PubSubClient mqttClient (espClient);
long lastSend = 0;
String clienttopic = "";

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
GxEPD2_3C<GxEPD2_154c, GxEPD2_154c::HEIGHT> display(GxEPD2_154c(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
String roomName = "Main Room";
String temperature = "";
String statusMsg = "Status: Init";

void setup_wifi() {
  delay(10);
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

void setup_Display() {
  display.init(115200);
  u8g2Fonts.begin(display); // connect u8g2 procedures to Adafruit GFX
  u8g2Fonts.setFontMode(1);                 // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);            // left to right (this is default)
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);         // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);         // apply Adafruit GFX color
}

void setup_MQTT() {
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(on_Msg);  
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  setup_Display();
  setup_MQTT();

  randomSeed(micros());
}

void drawLine(int y, int h)
{
   int linemargin = 4;
   for (int i = 0; i <= h; i++) {
      display.drawFastHLine(linemargin, y + i, display.width() - (2*linemargin),GxEPD_RED);
   }
}

void printCentredMsg(uint16_t y,String msg) {
    uint16_t x = (display.width() - u8g2Fonts.getUTF8Width(msg.c_str())) / 2;
    u8g2Fonts.setCursor(x, y);
    u8g2Fonts.print(msg.c_str());
}

void updateDisplay()
{

  display.firstPage();
  do
  {
    int margin = 3;
    int lineheight = 3;
    int linespace = 6;
    display.fillScreen(GxEPD_WHITE);

    u8g2Fonts.setFont(u8g2_font_logisoso20_tf); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
    uint16_t y = u8g2Fonts.getFontAscent() + margin;   
    printCentredMsg(y,roomName);

    drawLine(y + linespace,lineheight);

    u8g2Fonts.setFont(u8g2_font_logisoso58_tf);
    uint16_t yt = (display.height() + u8g2Fonts.getFontAscent()) / 2;
    printCentredMsg(yt,temperature);
    
    u8g2Fonts.setFont(u8g2_font_logisoso20_tf);  
    uint16_t ys = display.height() - margin;
    printCentredMsg(ys,statusMsg);

    drawLine(ys - u8g2Fonts.getFontAscent() - linespace - lineheight,lineheight);  
  }
  while (display.nextPage());
  
}

void on_Msg(char* topic, byte* payload, unsigned int length) {
  String sTopic;
  for (int i = 0; topic[i] != 0; i++) {
    sTopic += topic[i];
  }
    
  String sPayload;
  for (int i = 0; i < length; i++) {
    sPayload += char(payload[i]);
  }

  if (sTopic.endsWith("/temperature")) {
    temperature = sPayload;
  }

  if (sTopic.endsWith("/status")) {
    statusMsg = sPayload;
  }

  updateDisplay();
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      clienttopic = mqtt_topic;
      clienttopic += "/";
      clienttopic += clientId;
      Serial.println(clienttopic);
      mqttClient.publish(clienttopic.c_str(), "Connected");
      // ... and subscribe to our status 
      String subtopic = mqtt_topic;
      subtopic += "/";
      subtopic += mqtt_room;
      subtopic += "/#";
      mqttClient.subscribe(subtopic.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  long now = millis();
  if (now - lastSend > 30000) {
    lastSend = now;
    mqttClient.publish(clienttopic.c_str(), "Heartbeat");
  }
}
