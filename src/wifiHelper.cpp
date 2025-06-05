#include "wifiHelper.h"
#include "displayHelper.h"
#include "config.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>

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
  WiFi.softAPConfig(apIP, apIP, netMsk);
  // its an open WLAN access point without a password parameter
  WiFi.softAP(softAP_ssid);

  /* Setup the DNS server redirecting all the domains to the apIP */
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

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
  
  // Disable WiFi
  WiFi.mode(WIFI_OFF);
}
