/**
 * WARNING: This example needs uEspConfigLib library, https://github.com/Naguissa/uEspConfigLib - available on Arduino IDE libraries
 *
 * Connect to open WiFi called uSevenSegmentLib_device and browse to http://192.168.4.1/
 *
 *
 * Check "void ESPSetup()" function, "configFs" variable if you want to save your config permanently
 *
 *
 * You can use this tool as WiFi text screen
 *
 */

/**
 * Really tiny library to basic 7 segments displays
 *
 *
 * Functionality example
 *
 *
 * *Segments in the display:*
 *
 *    -- a --
 *   |       |
 *   f       b
 *   |       |
 *    -- g --
 *   |       |
 *   e       c
 *   |       |
 *    -- d --  dot
 *
 * *Binary correspondence:*
 *  
 *  0b.gfedcba
 *
 * *Pins definition:*
 *  
 *  Same order as binary representation: {dot, g, f, e, d, c, b, a}
 *
 * *Muxes definition:*
 *  
 *  Left to right: {digit at left, digit at middle, digit at right....}
 *
 *
 *
 * @see <a href="https://github.com/Naguissa/uSevenSegmentLib">https://github.com/Naguissa/uSevenSegmentLib</a>
 * @see <a href="https://www.foroelectro.net/librerias-arduino-ide-f29/usevensegmentlib-libreria-arduino-para-controlar-d-t193.html">https://www.foroelectro.net/librerias-arduino-ide-f29/usevensegmentlib-libreria-arduino-para-controlar-d-t193.html</a>
 * @see <a href="mailto:naguissa@foroelectro.net">naguissa@foroelectro.net</a>
 */
#include "Arduino.h"
#include "uSevenSegmentLib.h"

#include "uEspConfigLib.h"
#include "uEspConfigLibFSInterface.h"
#include "uEspConfigLibFSNone.h"
    
uEspConfigLibFSInterface * configFs;
uEspConfigLib * config;
bool restartConfig = false;

#ifdef ARDUINO_ARCH_ESP32
    WebServer server(80);
#else
    ESP8266WebServer server(80);
#endif


uSevenSegmentLib * sevenSegments = NULL;

unsigned long lastTime = 0;
unsigned long actTime;

void ESPSetup();
void WiFiSetup();
void handleGetConfigHtml();
void handleSaveConfig();
void handleSetText();
void handleDefault();


void sevenSegmentsSetup() {
    int pins[8] = {
        atoi(config->getPointer("pin_0")),
        atoi(config->getPointer("pin_1")),
        atoi(config->getPointer("pin_2")),
        atoi(config->getPointer("pin_3")),
        atoi(config->getPointer("pin_4")),
        atoi(config->getPointer("pin_5")),
        atoi(config->getPointer("pin_6")),
        atoi(config->getPointer("pin_7"))
    };
        
    int muxes[4] = {
        atoi(config->getPointer("mux_0")),
        atoi(config->getPointer("mux_1")),
        atoi(config->getPointer("mux_2")),
        atoi(config->getPointer("mux_3"))
    };
    
    uint8_t nmuxes = atoi(config->getPointer("mux_number"));
    if (nmuxes > 4) {
        nmuxes = 4;
    }
    
    if (sevenSegments != NULL) {
        free(sevenSegments);
    }
    sevenSegments = new uSevenSegmentLib(nmuxes, pins, muxes);
}



void ESPSetup() {
    // Configuration File system. You can choose among following options:

    // None (lost on reboot): configFs = new uEspConfigLibFSNone("/config.ini", true);
    // SPIFFS: configFs = new uEspConfigLibFSSpiffs("/config.ini", true);
    // LittleFS: configFs = new uEspConfigLibFSLittlefs("/config.ini", true);
    // SD-card: configFs = new uEspConfigLibFSSd("/config.ini", true);

    configFs = new uEspConfigLibFSNone("/config.ini", true);
    if (configFs->status() == uEspConfigLibFS_STATUS_FATAL) {
        //DEBUGLN("  * Error initializing FS");
    }

    config = new uEspConfigLib(configFs);

    // Define your variables: Name, Description, Default value [, OPTIONS]
    
    // WiFi
    config->addOption("wifi_mode", "WiFi mode (C=Client, other=Access Point)", "A");
    config->addOption("wifi_ssid", "SSID of your WiFi", "uSevenSegmentLib_device", uEspConfigLib_OPTION_SCANNER);
    config->addOption("wifi_password", "Password of your WiFi", "");

    // 7 segmant pins:
    config->addOption("pin_0", "7 segment displays pin 0 GPIO index (dot)", "16", uEspConfigLib_OPTION_NUMERIC);
    config->addOption("pin_1", "7 segment displays pin 1 GPIO index (g)", "5", uEspConfigLib_OPTION_NUMERIC);
    config->addOption("pin_2", "7 segment displays pin 2 GPIO index (f)", "4", uEspConfigLib_OPTION_NUMERIC);
    config->addOption("pin_3", "7 segment displays pin 3 GPIO index (e)", "0", uEspConfigLib_OPTION_NUMERIC);
    config->addOption("pin_4", "7 segment displays pin 4 GPIO index (d)", "2", uEspConfigLib_OPTION_NUMERIC);
    config->addOption("pin_5", "7 segment displays pin 5 GPIO index (c)", "14", uEspConfigLib_OPTION_NUMERIC);
    config->addOption("pin_6", "7 segment displays pin 6 GPIO index (b)", "12", uEspConfigLib_OPTION_NUMERIC);
    config->addOption("pin_7", "7 segment displays pin 7 GPIO index (a)", "13", uEspConfigLib_OPTION_NUMERIC);

    // Muxes:    
    config->addOption("mux_number", "Number of muxes, max 4", "3", uEspConfigLib_OPTION_NUMERIC);
    config->addOption("mux_0", "Mux 0", "15", uEspConfigLib_OPTION_NUMERIC);
    config->addOption("mux_1", "Mux 1", "3", uEspConfigLib_OPTION_NUMERIC);
    config->addOption("mux_2", "Mux 2", "10", uEspConfigLib_OPTION_NUMERIC);
    config->addOption("mux_3", "Mux 3", "16", uEspConfigLib_OPTION_NUMERIC); // I ran out of usable pings on NodeMCU
    
    // Not useful on FS type none:
    config->loadConfigFile();
}
    
    

void WiFiSetup() {
    char *mode, *ssid, *pass;
    
    server.stop();
    WiFi.disconnect();
    #ifndef ARDUINO_ARCH_ESP32
        WiFi.setAutoConnect(true);
    #endif
    WiFi.setAutoReconnect(true);
    
    mode = config->getPointer("wifi_mode");
    ssid = config->getPointer("wifi_ssid");
    pass = config->getPointer("wifi_password");

    if (strcmp(mode, "S") == 0) {
	    WiFi.mode(WIFI_STA);
	    WiFi.begin(ssid, pass);
	    // Wait for connection
	    uint8_t i = 0;
	    while (WiFi.status() != WL_CONNECTED && i++ < 30) { //wait 30*2 seconds
		    delay(2000);
	    }
	    if (i == 31) {
		    return;
	    }
    } else { // Default mode, 'A' (AP)
	    WiFi.mode(WIFI_AP);
	    WiFi.softAP(ssid, pass);
    }

    server.on("/config", HTTP_GET, handleGetConfigHtml);
    server.on("/saveConfig", HTTP_POST, handleSaveConfig);
    server.on("/text", HTTP_POST, handleSetText);
    
    server.onNotFound(handleDefault);
    yield();
    server.begin();
    yield();
}

void handleGetConfigHtml() {
    yield();
    config->handleConfigRequestHtml(&server, "/saveConfig");
}

void handleSaveConfig() {
    yield();
    config->handleSaveConfig(&server);
    restartConfig = true;
}


void handleSetText() {
    sevenSegments->setText(server.arg("text").c_str());
    yield();
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.sendHeader("Location", "/", true);
    server.send(302, "text/html","");
}

void handleDefault() {
    String line;
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    yield();
    server.send(200, "text/html", "");
    yield();
    server.sendContent("<html><head><title>uSevenSegmentLib ESP example</title></head><body>");
    yield();

    server.sendContent("<h1>uSevenSegmentLib ESP example</h1>");
    yield();
    server.sendContent("<p>Available items:</p>");
    yield();
    server.sendContent("<p><a href=\"/config\">WiFi, pins and muxes configuration</a></p>");
    yield();
    server.sendContent("<form action=\"/text\" method=\"POST\"><p>Text to show: <input type=\"text\" name=\"text\"><button type=\"submit\">Set</button><p></form>");
    yield();
    server.sendContent("</body>");
}
    



void setup() {
    ESPSetup();
    WiFiSetup();
    sevenSegmentsSetup();
    WiFiSetup();
    char *mode = config->getPointer("wifi_mode");
    char *ssid = config->getPointer("wifi_ssid");
    String ip;
    if (strcmp(mode, "S") == 0) {
	    ip = WiFi.localIP().toString();
    } else {
	    ip = WiFi.softAPIP().toString();
    }
    String message = "Connect to WiFi ssid ";
    message += ssid;
    message += " and access to ip ";
    message += ip;
    sevenSegments->setText(message.c_str());
    
	lastTime = millis();
}


void loop() {
	sevenSegments->interruptLoop();
    yield();
    server.handleClient();
    yield();
    if (restartConfig) {
        restartConfig = false;    
        WiFiSetup();
        yield();
        sevenSegmentsSetup();
        yield();
    }
}



