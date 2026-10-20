/*
 * Portable Luggage Monitor and Tracker
 * ESP32 web dashboard firmware, based on the project report.
 *
 * Configure credentials locally before uploading. Never commit real credentials.
 * Serial2 defaults below assume the Arduino sends data to ESP32 GPIO16 (RX2).
 */

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

String latestGPS = "Waiting for GPS...";
String tamperStatus = "Checking...";
float latitude = 0.0;
float longitude = 0.0;
bool hasValidLocation = false;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX2, TX2

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi. IP address:");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    String html = "<!DOCTYPE html><html><head>"
                  "<meta http-equiv='refresh' content='5'>"
                  "<meta name='viewport' content='width=device-width,initial-scale=1'>"
                  "<style>body{font-family:Arial;text-align:center;padding:24px;background:#f4f7fb}"
                  ".box{border:1px solid #ccd3dc;padding:18px;border-radius:10px;"
                  "background:white;display:inline-block;margin:10px;max-width:90%}"
                  "iframe{max-width:100%}</style>"
                  "<title>Luggage Tracker</title></head><body>"
                  "<h1>Portable Luggage Monitor</h1>"
                  "<div class='box'><h2>GPS Location</h2><p><strong>" +
                  latestGPS + "</strong></p></div>"
                  "<div class='box'><h2>Motion Status</h2><p><strong>" +
                  tamperStatus + "</strong></p></div>"
                  "<div class='box'><h2>Map</h2>";

    if (hasValidLocation) {
      html += "<iframe width='300' height='300' frameborder='0' "
              "style='border:0' src='https://maps.google.com/maps?q=" +
              String(latitude, 6) + "," + String(longitude, 6) +
              "&z=15&output=embed' allowfullscreen></iframe>";
    } else {
      html += "<p>Waiting for valid location...</p>";
    }

    html += "</div></body></html>";
    server.send(200, "text/html", html);
  });

  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  server.handleClient();

  if (Serial2.available()) {
    String incoming = Serial2.readStringUntil('\n');
    incoming.trim();

    int statusIndex = incoming.indexOf("Status:");
    if (statusIndex != -1) {
      tamperStatus = incoming.substring(statusIndex + 7);
      tamperStatus.trim();
    }

    int latIndex = incoming.indexOf("Lat:");
    int lngIndex = incoming.indexOf("Lng:");

    if (latIndex != -1 && lngIndex != -1) {
      int latEnd = incoming.indexOf(",", latIndex);
      int lngEnd = incoming.indexOf(",", lngIndex + 1);
      if (lngEnd == -1) lngEnd = incoming.length();

      if (latEnd != -1) {
        latitude = incoming.substring(latIndex + 4, latEnd).toFloat();
        longitude = incoming.substring(lngIndex + 4, lngEnd).toFloat();
        hasValidLocation = true;
        latestGPS = "Lat: " + String(latitude, 6) +
                    ", Lng: " + String(longitude, 6);
      }
    } else if (incoming.indexOf("Location: INVALID") != -1) {
      latestGPS = "Location: INVALID";
      hasValidLocation = false;
    }

    Serial.println(latestGPS);
    Serial.println("Status: " + tamperStatus);
  }
}
