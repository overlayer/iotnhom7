

#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6RDtwgMBK"
#define BLYNK_TEMPLATE_NAME "TEST"
#define BLYNK_AUTH_TOKEN "wo-F6FyU_cajnKQ6-B6BWDfiPRHeWbBj"


#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <FirebaseESP8266.h>
#include <ArduinoJson.h>

char ssid[] = "A";
char pass[] = "12345678";
#define WIFI_SSID "A"
#define WIFI_PASSWORD "12345678"

#define Led D6
#define Button D7
#define Led2 D2
#define Button2 D3
bool previousButtonState = HIGH; 
bool previousButtonState2 = HIGH; // Trạng thái trước đó của nút bấm

FirebaseData firebaseData;
String path = "/";
FirebaseJson json;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(Led, OUTPUT);
  pinMode(Button, INPUT_PULLUP);
  pinMode(Led2, OUTPUT);
  pinMode(Button2, INPUT_PULLUP);  // Thiết lập chân nút bấm với chế độ INPUT_PULLUP
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());

  config.database_url = "https://iot-nhom-7-d70d7-default-rtdb.firebaseio.com/";
  config.signer.tokens.legacy_token = "QNZWTt0SoQivgDeFSL5imCXQxGr3gwMEl9Cq49O2";
  
  Firebase.begin(&config, &auth);
}

BLYNK_WRITE(V2) {
  int button = param.asInt();
  if (button == 1) {
    digitalWrite(Led, HIGH);
  } else {
    digitalWrite(Led, LOW);
  }
}
BLYNK_WRITE(V7) {
  int button = param.asInt();
  if (button == 1) {
    digitalWrite(Led2, HIGH);
  } else {
    digitalWrite(Led2, LOW);
  }
}

void buttonPressed(int buttonPin, int ledPin, bool &state) {
  bool currentButtonState = digitalRead(buttonPin);

  if (currentButtonState != state) {
    delay(50);

    bool finalButtonState = digitalRead(buttonPin);

    if (finalButtonState == LOW && finalButtonState != state) {
      digitalWrite(ledPin, !digitalRead(ledPin));

      int ledState = digitalRead(ledPin);

      if (ledPin == Led) {
        Blynk.virtualWrite(V2, ledState);
      } else if (ledPin == Led2) {
        Blynk.virtualWrite(V7, ledState);
      }
    } 

    state = finalButtonState;
  }
}



void loop() {
  Blynk.run();
  buttonPressed(Button, Led, previousButtonState);
  buttonPressed(Button2, Led2, previousButtonState2);
  Firebase.setString(firebaseData, path+"/led1", "ON");//chay thu ket noi firebase
}
