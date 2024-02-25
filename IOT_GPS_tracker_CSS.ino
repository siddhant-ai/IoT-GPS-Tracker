#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
TinyGPSPlus gps;
SoftwareSerial ss(2, 13);
const char* ssid = "Vivo";
const char* password = "12345678";
float latitude, longitude;
int year, month, date, hour, minute, second;
String date_str, time_str, lat_str, lng_str;
int pm;
String latarray[20];
String lngarray[20];
unsigned int i = 0;
const unsigned long Interval = 13000;
unsigned long previousTime = 0;
WiFiServer server(80);

String htmlContent = "";

void setup() {
    Serial.begin(115200);
    ss.begin(9600);
    delay(2000);
    Serial.println();

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    // Display "Welcome" "To the IoT GPS tracker" "Mini Project" one by one
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(40, 40);
    display.println("Starting...");
    display.display();
    delay(3000);

    display.clearDisplay();
    display.setCursor(40, 40);
    display.println("About to Go...");
    display.display();
    delay(3000);

    display.clearDisplay();
    display.setCursor(40, 40);
    display.println("Welcome");
    display.display();
    delay(2000);

    display.clearDisplay();
    display.setCursor(40, 40);
    display.println("To the IoT GPS Tracker");
    display.display();
    delay(2000);

    display.clearDisplay();
    display.setCursor(40, 40);
    display.println("Mini Project 4");
    display.display();
    delay(2000);

    display.clearDisplay();
    display.setCursor(40, 40);
    display.println("Developed By");
    display.display();
    delay(2000);

    display.clearDisplay();
    display.setCursor(40, 40);
    display.println("Sidhant");
    display.display();
    delay(2000);

    display.clearDisplay();
    display.setCursor(40, 40);
    display.println("Keval");
    display.display();
    delay(2000);

    display.clearDisplay();
    display.setCursor(40, 40);
    display.println("Sakshi");
    display.display();
    delay(2000);

    display.clearDisplay();
    display.setCursor(40, 40);
    display.println("Subhashree");
    display.display();
    delay(2000);

    display.clearDisplay();

    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Connecting...");
        display.display();
        delay(500);
        display.clearDisplay();
        delay(500);
    }

    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Connected :D");
    display.display();

    Serial.println("WiFi connected");
    server.begin();
    Serial.println("Server started");

    // Print the IP address at the bottom of the display
    display.setCursor(0, 40);
    display.println("IP Address:");
    display.setCursor(0, 55);
    display.println(WiFi.localIP());
    display.display();

    Serial.println("WiFi connected");
    server.begin();
    Serial.println("Server started");
    Serial.println(WiFi.localIP());

    htmlContent = generateHtmlContent(); // Generate the initial HTML content
}

void loop() {
    unsigned long currentTime = millis();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    while (ss.available() > 0) {
        if (gps.encode(ss.read())) {
            if (currentTime - previousTime >= Interval) {
                if (gps.location.isValid()) {
                    // Update GPS coordinates
                    latitude = gps.location.lat();
                    lat_str = String(latitude, 6);
                    longitude = gps.location.lng();
                    lng_str = String(longitude, 6);
                    latarray[i] = lat_str;
                    lngarray[i] = lng_str;
                    i++;
                    if (i >= 20) {
                        i = 0; // Reset to the beginning of the array
                    }
                
                    Serial.println("Latitude:");
                    Serial.println(latarray[i]);
                    Serial.println("Longitude:");
                    Serial.println(lngarray[i]);
                    Serial.println();
                
                    display.setCursor(0, 0);
                    display.println("Mini Project 4");
                    display.setCursor(0, 30);
                    display.println("Lat:");
                    display.setCursor(27, 30);
                    display.println(lat_str);
                    display.setCursor(0, 40);
                    display.println("Lng:");
                    display.setCursor(27, 40);
                    display.println(lng_str);
                    display.display(); 
                    display.setCursor(0, 55);
                    display.println(WiFi.localIP());
                    display.display();
                     
                    previousTime = currentTime;
                }
                if (gps.date.isValid()) {
                    // Update date and time
                    date_str = formatDate(gps.date.day(), gps.date.month(), gps.date.year());
                    time_str = formatTime(gps.time.hour(), gps.time.minute(), gps.time.second());
                }
            }
        }
    }

    // Check if a client has connected
    WiFiClient client = server.available();
    if (client) {
        String response = generateHtmlContent();
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println();
        client.println(response);
        delay(200);
        client.stop();
    }
}

String formatDate(int day, int month, int year) {
    String formattedDate = String(day);
    formattedDate += " / ";
    formattedDate += (month < 10 ? "0" : "") + String(month);
    formattedDate += " / ";
    formattedDate += (year < 10 ? "0" : "") + String(year);
    return formattedDate;
}

String formatTime(int hour, int minute, int second) {
    int pm = 0;
    minute += 30;
    if (minute > 59) {
        minute -= 60;
        hour += 1;
    }
    hour += 5;
    if (hour > 23) {
        hour -= 24;
    }
    if (hour >= 12) {
        pm = 1;
    }
    else {
        pm = 0;
    }
    hour = hour % 12;
    String formattedTime = (hour < 10 ? "0" : "") + String(hour);
    formattedTime += " : ";
    formattedTime += (minute < 10 ? "0" : "") + String(minute);
    formattedTime += " : ";
    formattedTime += (second < 10 ? "0" : "") + String(second);
    formattedTime += (pm == 1) ? " PM" : " AM";
    return formattedTime;
}
String generateHtmlContent() {
    String content = "<!DOCTYPE html><html><head><title>IoT based GPS tracker</title>";
    content += "<style>";
    content += "body { font-family: Arial, sans-serif; margin: 0; padding: 0; overflow: hidden; background-image: url('https://images.unsplash.com/photo-1451187580459-43490279c0fa?q=80&w=1772&auto=format&fit=crop&ixlib=rb-4.0.3&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D'); background-size: cover; color: skyblue; }"; // Set your desired image file
    content += "video { position: fixed; right: 0; bottom: 0; min-width: 100%; min-height: 100%; z-index: -1; }";
    content += "body, html { height: 100%; }";
    content += "body { display: flex; flex-direction: column; justify-content: center; align-items: center; }";
    content += "table { border-collapse: collapse; margin-bottom: 20px; background-color: rgba(0, 0, 255, 0.3); }"; // Set table background color with transparency
    content += "th, td { border: 1px solid black; padding: 8px; color: white; }"; // Set text color in table cells
    content += "a:link { background-color: yellow; text-decoration: none; }";
    content += "</style>";
    content += "<script>";
    content += "function refreshPage() { location.reload(); }";
    content += "setInterval(refreshPage, 10000);"; // Refresh page every 10 seconds
    content += "</script>";
    content += "</head><body>";
    content += "<video autoplay muted loop>";
    content += "<source src='earth.mp4' type='video/mp4'>";
    content += "</video>";
    content += "<h1>IoT based GPS tracker</h1>";
    content += "<p><b>Location Details</b></p>";
    content += "<table>";
    content += "<tr><th>Latitude</th><td>" + lat_str + "</td></tr>";
    content += "<tr><th>Longitude</th><td>" + lng_str + "</td></tr>";
    content += "<tr><th>Date</th><td>" + date_str + "</td></tr>";
    content += "<tr><th>Time</th><td>" + time_str + "</td></tr>";
    content += "</table>";
    if (gps.location.isValid()) {
        content += "<p><a style='color:red;' href='https://www.google.com/maps/dir/";
        for (int j = 0; j < 20; j++) {
            content += latarray[j];
            content += ",";
            content += lngarray[j];
            if (j < 19) {
                content += "/";
            }
        }
        content += "' target='_top'>Click here</a> to check the location in Google maps.</p>";
    }
    content += "</body></html>\n";
    return content;
}
