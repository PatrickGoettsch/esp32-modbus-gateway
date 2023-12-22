#include <WiFi.h>
#include <AsyncTCP.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include <Logging.h>

#include "config.h"
#include "modbusmanager.h"
#include "pages.h"

AsyncWebServer webServer(80);
Config config;
Preferences prefs;
ModbusManager *modbusmanager;
WiFiManager wm;

void setup()
{
  debugSerial.begin(115200);
  dbgln();
  dbgln("[config] load");
  prefs.begin("modbusRtuGw");
  config.begin(&prefs);
  debugSerial.end();
  debugSerial.begin(config.getSerialBaudRate(), config.getSerialConfig());

  dbgln("[wifi] start");
  WiFi.mode(WIFI_STA);
  wm.setClass("invert");
  auto reboot = false;
  wm.setAPCallback([&reboot](WiFiManager *wifiManager)
                   { reboot = true; });
  wm.autoConnect();
  if (reboot)
  {
    ESP.restart();
  }
  dbgln("[wifi] finished");

  dbgln("[modbus] start");
  modbusmanager->start(&config);
  dbgln("[modbus] finished");

  dbgln("Start webserver");
  setupPages(&webServer, MBclient, &MBbridge, &config, &wm);
  webServer.begin();
  dbgln("[setup] finished");
}

void loop()
{
  // put your main code here, to run repeatedly:
}