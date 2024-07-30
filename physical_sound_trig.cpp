// Load Wi-Fi library
#include <WiFi.h>
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
const char* ssid = "Your ssid";
const char* password = "Your password";
WiFiServer server(80);
String header;
String output12State = "off";
String output14State = "off";
String output26State =  "off";
const int trigPin = 5;
const int echoPin = 18;
const int output12 = 12;
const int output14 = 14;
const int output26 = 26;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
const long timeoutTime = 2000;
const long measureInterval = 1000; // Measure interval in milliseconds

long duration;
float distanceCm;
float distanceInch;
bool objectDetected = false;

void setup() {
  Serial.begin(115200);
  pinMode(output12, OUTPUT);
  pinMode(output14, OUTPUT);
  pinMode(output26, OUTPUT);
  digitalWrite(output12, LOW);
  digitalWrite(output14, LOW);
  digitalWrite(output26, LOW);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  
  // Setup ultrasonic sensor
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
}

void loop(){
  WiFiClient client = server.available();   
  if (client) {                             
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");         
    String currentLine = "";                
    while (client.connected() && currentTime - previousTime <= timeoutTime) { 
      currentTime = millis();
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            if (header.indexOf("GET /12/on") >= 0) {
              Serial.println("GPIO 12 on");
              output12State = "on";
              digitalWrite(output12, HIGH);
            } else if (header.indexOf("GET /12/off") >= 0) {
              Serial.println("GPIO 12 off");
              output12State = "off";
              digitalWrite(output12, LOW);
            } else if (header.indexOf("GET /14/on") >= 0) {
              Serial.println("GPIO 14 on");
              output14State = "on";
              digitalWrite(output14, HIGH);
            } else if (header.indexOf("GET /14/off") >= 0) {
              Serial.println("GPIO 14 off");
              output14State = "off";
              digitalWrite(output14, LOW);
            } else if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(output26, HIGH);
            }else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            }
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Calibri; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            client.println("<body><h1>Apsara GPIO Web Server</h1>");
            client.println("<p>GPIO 12 - State " + output12State + "</p>");     
            if (output12State=="off") {
              client.println("<p><a href=\"/12/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/12/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            client.println("<p>GPIO 14 - State " + output14State + "</p>");     
            if (output14State=="off") {
              client.println("<p><a href=\"/14/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/14/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>GPIO 26 - State " + output14State + "</p>");
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else { 
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display object detection status and distance
            client.print("<p>Object Detected: ");
            client.print(objectDetected ? "Yes" : "No");
            client.println("</p>");

            client.print("<p>Distance (cm): ");
            client.print(distanceCm);
            client.println("</p>");

            client.print("<p>Distance (inch): ");
            client.print(distanceInch);
            client.println("</p>");

            client.println("</body></html>");
            client.println();
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  // Check for object detection
  objectDetected = detectObject();
}

bool detectObject() {
  static unsigned long previousMeasureTime = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMeasureTime >= measureInterval) {
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  }
}
