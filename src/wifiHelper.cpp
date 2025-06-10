#include "wifiHelper.h"
#include "displayHelper.h"
#include "config.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <esp_wifi.h>

const char *softAP_ssid = WIFI_SSID;

// The access points IP address and net mask
// It uses the default Google DNS IP address 8.8.8.8 to capture all 
// Android dns requests
IPAddress apIP(8, 8, 8, 8);
IPAddress netMsk(255, 255, 255, 0);

// DNS server 
const byte DNS_PORT = 53; 
DNSServer dnsServer;


void wifiSetup()
{
  // Ensure WiFi is completely deinitialized before setting up again
  WiFi.mode(WIFI_OFF); // Deinitialize the WiFi driver
  delay(100); // Small delay after deinit

  // Turn On WiFi
  WiFi.mode(WIFI_AP);
  delay(50);
  // Set the WiFi access point IP address and net mask
  WiFi.softAPConfig(apIP, apIP, netMsk);
  if(WiFi.softAP(softAP_ssid)) {
    Serial.println(F("AP Configured"));
    /* Setup the DNS server redirecting all the domains to the apIP */
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(DNS_PORT, "*", apIP);
    Serial.println(F("Wifi Started"));
  } else {
    Serial.println(F("Failed to configure AP"));
  }
  // its an open WLAN access point without a password parameter

}

void wifiLoop()
{
  dnsServer.processNextRequest();
}

void wifiStop()
{
  // Stop the DNS server
  dnsServer.stop();
  
  // Stop the WiFi access point
  WiFi.softAPdisconnect(true);
  delay(200); // Wait for the AP to disconnect
  // Disable WiFi
  WiFi.mode(WIFI_OFF);
  delay(100);
  Serial.println(F("Wifi Stopped"));
}
