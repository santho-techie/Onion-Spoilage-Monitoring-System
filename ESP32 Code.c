#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "DHT.h"

/* ---------- SENSOR PINS ---------- */

#define DHTPIN 4
#define DHTTYPE DHT22
#define MQ135_PIN 34

#define GREEN_LED 25
#define YELLOW_LED 26
#define RED_LED 27

DHT dht(DHTPIN, DHTTYPE);

/* ---------- WIFI ---------- */

const char* ssid = "";
const char* password = "";

/* ---------- AWS ENDPOINT ---------- */

const char* aws_endpoint = "";

WiFiClientSecure net;
PubSubClient client(net);

/* ---------- CERTIFICATES ---------- */

const char* AmazonRootCA1 = R"EOF(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)EOF";

const char* device_cert = R"EOF(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----

)EOF";

const char* private_key = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
-----END RSA PRIVATE KEY-----

)EOF";


/* ---------- WIFI CONNECT ---------- */

void connectWiFi() {

  Serial.println("\nConnecting to WiFi...");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(2000);

  WiFi.begin(ssid, password);

  int attempts = 0;

  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi connection timeout");
  }
}


/* ---------- AWS CONNECT ---------- */

void connectAWS() {

  if (client.connected()) return;

  Serial.println("Connecting to AWS IoT...");

  net.setCACert(AmazonRootCA1);
  net.setCertificate(device_cert);
  net.setPrivateKey(private_key);

  client.setServer(aws_endpoint, 8883);

  int retries = 0;

  while (!client.connected() && retries < 5) {

    String clientID = "ESP32-" + String(random(0xffff), HEX);

    if (client.connect(clientID.c_str())) {

      Serial.println("AWS IoT Connected");

    } else {

      Serial.print("AWS failed: ");
      Serial.println(client.state());

      retries++;
      delay(2000);
    }
  }
}


bool spoilageDetected = false;
bool ventilationActive = false;
String status = "SAFE";


/* ---------- SETUP ---------- */

void setup() {

  Serial.begin(115200);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  dht.begin();

  connectWiFi();

  if (WiFi.status() == WL_CONNECTED) {
    connectAWS();
  }
}


/* ---------- LOOP ---------- */

void loop() {

  if (WiFi.status() != WL_CONNECTED) {

    connectWiFi();
    delay(5000);
    return;
  }

  if (!client.connected()) {

    connectAWS();
  }

  client.loop();


  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  int gas = analogRead(MQ135_PIN);

  float gasRisk = gas / 150.0;
  float tempRisk = temp / 40.0;
  float humRisk = humidity / 100.0;

  float OSRI = (0.6 * gasRisk) + (0.2 * tempRisk) + (0.2 * humRisk);


 bool spoilageDetected = false;
bool ventilationActive = false;

if (OSRI < 0.45 && !spoilageDetected) {

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  status = "SAFE";

  Serial.println("SAFE STORAGE");

}

else if (OSRI >= 0.45 && OSRI < 0.60) {

  spoilageDetected = true;

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(RED_LED, LOW);

  status = "SELL SOON";

  Serial.println("WARNING: VOC GAS DETECTED - SELL SOON");

}

else if (OSRI >= 0.60) {

  spoilageDetected = true;
  ventilationActive = true;

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, HIGH);

  status = "IMMEDIATE SALE";

  Serial.println("CRITICAL: VENTILATION ACTIVE - IMMEDIATE SALE");

}

if (spoilageDetected && OSRI < 0.45) {

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(RED_LED, LOW);

  status = "SELL SOON";

  Serial.println("GAS REDUCED AFTER VENTILATION - SELL BATCH SOON");

}

  String payload = "{";
payload += "\"gas\":" + String(gas) + ",";
payload += "\"temperature\":" + String(temp) + ",";
payload += "\"humidity\":" + String(humidity) + ",";
payload += "\"osri\":" + String(OSRI) + ",";
payload += "\"status\":\"" + status + "\"";
payload += "}";

  Serial.println(payload);

  if (client.connected()) {
  client.publish("onion/storage/data", payload.c_str());
}

  delay(5000);
}
