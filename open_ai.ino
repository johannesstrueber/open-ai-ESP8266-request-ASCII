#include <ESP8266WiFi.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char *openAIUrl = "https://api.openai.com/v1/completions";

Adafruit_SSD1306 display(128, 64);

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.println("Connecting to WiFi...");
  display.display();
  display.clearDisplay();
  Serial.begin(115200);
  WiFi.begin(yourSsid, yourPassword);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.println("WiFi connected");
  display.println("IP address: ");
  display.println(WiFi.localIP());
  display.display();
}

void loop()
{
  WiFiClientSecure client;
  HTTPClient http;

  client.setInsecure();
  http.begin(client, openAIUrl);

  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", yourBearerKey);
  http.addHeader("OpenAI-Organization", yourOrganizationKey);

  display.println("init client...");

  String jsonPayLoad = "{ \"model\": \"text-davinci-003\", \"prompt\": \"ASCII Image of todays EU news.Line length must be 21 and line height must be 7.\", \"max_tokens\": 100, \"temperature\": 0.75 }";

  int httpCode = http.POST(jsonPayLoad);
  if (httpCode > 0)
  {
    Serial.print(httpCode);
    Serial.print(jsonPayLoad);
    display.clearDisplay();
    display.println("httpCode okay...");
    display.display();
    String postResponse = http.getString();
    DynamicJsonDocument postDoc(4096);
    deserializeJson(postDoc, postResponse);
    String encoded = postDoc["choices"][0]["text"].as<String>();
    encoded = encoded.substring(2);
    Serial.println("--------------------");
    Serial.println("START OF ASCII IMAGE:");
    Serial.print(encoded);
    Serial.println("--------------------");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextWrap(true);
    display.print(encoded);
    display.display();
  }
  else
  {
    Serial.println("first request failed");
  }
  http.end();

  // repeat every 24 hours
  delay(86400000);
}
