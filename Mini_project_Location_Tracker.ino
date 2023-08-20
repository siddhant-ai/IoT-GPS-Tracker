#include <TinyGPS++.h>

#include <SoftwareSerial.h>

#include <ESP8266WiFi.h>

#include <Wire.h>

#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels

#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

TinyGPSPlus gps;  // The TinyGPS++ object

SoftwareSerial ss(2, 0); // The serial connection to the GPS device

const char* ssid = "ADITYA";

const char* password = "03071975";

float latitude , longitude;

int year , month , date, hour , minute , second;

String date_str , time_str , lat_str , lng_str;

int pm;

//int numreadings = 10;

String latarray[20];

String lngarray[20];

//String current_latarray[10], current_lngarray[10], previous_latarray[10], previous_lngarray[10] ;

unsigned int i = 0;

const unsigned long Interval = 13000;

unsigned long previousTime = 0;

WiFiServer server(80);

void setup()

{

  Serial.begin(115200);

  ss.begin(9600);

  delay(2000); // Pause for 2 seconds

  Serial.println();

  Serial.print("Connecting to ");

  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)

  {

    delay(500);

    Serial.print(".");

  }

  Serial.println("");

  Serial.println("WiFi connected");

  server.begin();

  Serial.println("Server started");

  // Print the IP address

  Serial.println(WiFi.localIP());

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64

    Serial.println(F("SSD1306 allocation failed"));

    for(;;);

  }

}

void loop()

{

  unsigned long currentTime = millis();

  display.clearDisplay();

  display.setTextSize(1);

  display.setTextColor(WHITE);


  while (ss.available() > 0)

    if (gps.encode(ss.read()))

    { 

     if (currentTime - previousTime >= Interval) { 

      if (gps.location.isValid())

      {

        //Serial.print("Getting Data");

        latitude = gps.location.lat();

        lat_str = String(latitude , 6);

        longitude = gps.location.lng();

        lng_str = String(longitude , 6);

        latarray[i] = lat_str;

        lngarray[i]= lng_str;

        i++;

        Serial.print(i);

        if (i>=20)

       // {

          i=0; //reset to beginning of array, so you don't try to save readings outside of the bounds of the array

        // } 
        Serial.println("Latitude:");

        Serial.println(latarray[i]);

        Serial.println("Longitude:");

        Serial.println(lngarray[i]);

        Serial.println();
        
        display.setCursor(0, 0);

        display.println("Mini Project 5");

        display.display();

        display.setCursor(0, 20);

        display.println("Lat:");

        display.setCursor(27, 20);

        display.println(lat_str);

        display.setCursor(0, 35);

        display.println("Lng:");

        display.setCursor(27, 35);

        display.println(lng_str);

        display.setCursor(0, 55);

        display.println("IP:");

        display.setCursor(27, 55);

        display.println(WiFi.localIP());

        display.display(); 

        }

        previousTime = currentTime;

       }       

      //}

      if (gps.date.isValid())

      {

        //Serial.print("Getting Time");

        date_str = "";

        date = gps.date.day();

        month = gps.date.month();

        year = gps.date.year();

        if (date < 10)

          date_str = '0';

        date_str += String(date);

        date_str += " / ";

        if (month < 10)

          date_str += '0';

        date_str += String(month);

        date_str += " / ";

        if (year < 10)

          date_str += '0';

        date_str += String(year);

      }

      if (gps.time.isValid())

      {

        time_str = "";

        hour = gps.time.hour();

        minute = gps.time.minute();

        second = gps.time.second();

        minute = (minute + 30);

        if (minute > 59)

        {

          minute = minute - 60;

          hour = hour + 1;

        }

        hour = (hour + 5) ;

        if (hour > 23)

          hour = hour - 24;

        if (hour >= 12)

          pm = 1;

        else

          pm = 0;

        hour = hour % 12;

        if (hour < 10)

          time_str = '0';

        time_str += String(hour);

        time_str += " : ";

        if (minute < 10)

          time_str += '0';

        time_str += String(minute);

        time_str += " : ";

        if (second < 10)

          time_str += '0';

        time_str += String(second);

        if (pm == 1)

          time_str += " PM ";

        else

          time_str += " AM ";

      }

    }

  // Check if a client has connected

  WiFiClient client = server.available();

  if (!client)

  {

    return;

  }

  // Prepare the response

  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title>IoT based Location tracker using ESP8266-12F</title> <style>";

  s += "a:link {background-color: YELLOW;text-decoration: none;}";

  s += "table, th, td {border: 1px solid black;} </style> </head> <body> <h1  style=";

  s += "font-size:300%;";

  s += " ALIGN=CENTER> IoT based GPS system using ESP8266-12F</h1>";

  s += "<p ALIGN=CENTER style=""font-size:150%;""";

  s += "> <b>Location Details</b></p> <table ALIGN=CENTER style=";

  s += "width:50%";

  s += "> <tr> <th>Latitude</th>";

  s += "<td ALIGN=CENTER >";

  s += lat_str;

  s += "</td> </tr> <tr> <th>Longitude</th> <td ALIGN=CENTER >";

  s += lng_str;

  s += "</td> </tr> <tr>  <th>Date</th> <td ALIGN=CENTER >";

  s += date_str;

  s += "</td></tr> <tr> <th>Time</th> <td ALIGN=CENTER >";

  s += time_str;

  s += "</td>  </tr> </table> ";

   if (gps.location.isValid())

  {

    // s += "<p align=center><a style=""color:RED;font-size:125%;"" href=""http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=";

     s += "<p align=center><a style=""color:RED;font-size:125%;"" href=""https://www.google.com/maps/dir/";

 //https://www.google.com/maps/dir/26.8172985,75.8286322/26.8181889,75.830…;         

    for (int j=0; j<20; j++)

    {

    s += latarray[j];                           

    s += ",";

    s += lngarray[j];

    if (j<10)

    s += "/"; 

    }

    s += """ target=""_top"">Click here!</a> To check the location in Google maps.</p>";

  }

  s += "</body> </html> \n";

  client.print(s);

  delay(200);  

 }

