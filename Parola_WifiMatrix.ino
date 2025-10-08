
/*
 * Parola WiFi Matrix Display Controller
 * 
 * Author: @dedsec (GitHub)
 * Project: ESP8266 WiFi LED Matrix Display with Web Interface
 * Description: Professional WiFi-controlled LED matrix display system with 
 *              AP mode, web interface, OTA updates, and security features
 * 
 * Hardware: ESP8266 + 4x MAX7219 LED Matrix Modules
 * Features: - Automatic AP mode when no WiFi configured
 *           - Modern responsive web interface with Material Design
 *           - 28 visual effects and animations
 *           - Real-time clock display with NTP sync
 *           - Secure authentication with session management
 *           - Over-the-air firmware updates
 *           - EEPROM settings persistence
 * 
 * Default Access:
 * - AP Mode: Connect to "Parola" network (password: parola123)
 * - Web Interface: http://192.168.4.1 (admin/admin)
 * 
 * Repository: https://github.com/dedsec/Parola_
 * License: MIT
 */

#include <ArduinoOTA.h>
#include <EEPROM.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <MD_MAX72xx.h>
#include <ArduinoOTA.h>
#include <MD_Parola.h>
#include <NTPClient.h>
#include <SPI.h>
#include <WiFiUdp.h>
#include <map>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN D5
#define DATA_PIN D7
#define CS_PIN D8

MD_Parola myDisplay =
  MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
ESP8266HTTPUpdateServer httpUpdater;

textEffect_t effects[] = {
  PA_NO_EFFECT,
  PA_PRINT,
  PA_SCROLL_UP,
  PA_SCROLL_DOWN,
  PA_SCROLL_LEFT,
  PA_SCROLL_RIGHT,
  PA_SLICE,
  PA_MESH,
  PA_FADE,
  PA_DISSOLVE,
  PA_BLINDS,
  PA_RANDOM,
  PA_WIPE,
  PA_WIPE_CURSOR,
  PA_SCAN_HORIZ,
  PA_SCAN_HORIZX,
  PA_SCAN_VERT,
  PA_SCAN_VERTX,
  PA_OPENING,
  PA_OPENING_CURSOR,
  PA_CLOSING,
  PA_CLOSING_CURSOR,
  PA_SCROLL_UP_LEFT,
  PA_SCROLL_UP_RIGHT,
  PA_SCROLL_DOWN_LEFT,
  PA_SCROLL_DOWN_RIGHT,
  PA_GROW_UP,
  PA_GROW_DOWN
};

const char * effect_names[] = {
  "No Effect",
  "Print",
  "Scroll Up",
  "Scroll Down",
  "Scroll Left",
  "Scroll Right",
  "Slice",
  "Mesh",
  "Fade",
  "Dissolve",
  "Blinds",
  "Random",
  "Wipe",
  "Wipe Cursor",
  "Scan Horiz",
  "Scan HorizX",
  "Scan Vert",
  "Scan VertX",
  "Opening",
  "Opening Cursor",
  "Closing",
  "Closing Cursor",
  "Scroll Up Left",
  "Scroll Up Right",
  "Scroll Down Left",
  "Scroll Down Right",
  "Grow Up",
  "Grow Down"
};

const char * effect_icons[] = {
  "block",
  "print",
  "arrow_upward",
  "arrow_downward",
  "arrow_back",
  "arrow_forward",
  "animation",
  "grid_on",
  "blur_on",
  "grain",
  "view_week",
  "shuffle",
  "waterfall_chart",
  "keyboard_tab",
  "deselect",
  "table_chart",
  "height",
  "align_vertical_bottom",
  "open_in_full",
  "keyboard_return",
  "fullscreen_exit",
  "undo",
  "north_east",
  "north_west",
  "south_east",
  "south_west",
  "vertical_align_top",
  "vertical_align_bottom"
};

#define NUM_EFFECTS (sizeof(effects) / sizeof(effects[0]))
#define EEPROM_SIZE 256
#define EEPROM_ADDR_BRIGHTNESS 0
#define EEPROM_ADDR_SPEED 1
#define EEPROM_ADDR_EFFECT 2#include <ArduinoOTA.h>
#define EEPROM_ADDR_MESSAGE 4
#define EEPROM_ADDR_WIFI_SSID 68
#define EEPROM_ADDR_WIFI_PASS 132

struct Settings {
  uint8_t brightness, speed, effect_idx;
  char message[64];
  char wifi_ssid[64];
  char wifi_password[64];
};
Settings settings;

const char * ssid = "...";
const char * password = "...";
bool apMode = false;ii
const char * adminUser = "...";
const char * adminPass = "...";
ESP8266WebServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);
bool wifiConnected = true;
bool displayDisabled = false;
unsigned long lastWifiCheck = 0;
const unsigned long wifiRetryInterval = 15000;

// TRACK LOGIN ATTEMPTS
struct LoginAttempt {
  uint8_t attempts;
  unsigned long lastAttemptTime;
  unsigned long lastSeen;
};

std::map < String, LoginAttempt > loginAttempts;
const int MAX_ATTEMPTS = 3;
const unsigned long BLOCK_TIME = 30000;

String sessionToken = "";
unsigned long sessionTimestamp = 0;
const unsigned long SESSION_TIMEOUT = 10 * 60 * 1000UL; // 10 minutes

// IP cleanup timing control
unsigned long lastCleanupTime = 0;
const unsigned long CLEANUP_INTERVAL = 60000;
const unsigned long ONE_WEEK_MS = 604800000UL; // 7 days in milliseconds

String lastClock = "";
String lastClockUI = "";

const char index_html[] PROGMEM = "<!DOCTYPE html><html lang='en'><head>"
"<meta charset='UTF-8'>"
"<meta name='viewport' content='width=device-width, initial-scale=1'>"
"<title>Parola Matrix</title>"
"<link href='https://fonts.googleapis.com/icon?family=Material+Icons' "
"rel='stylesheet'>"
"<style>"
"*{box-sizing:border-box;}"
"body{background:#151922;color:#fff;font-family:sans-serif;margin:0;}"
".mainwrap{max-width:960px;margin:36px auto 24px auto;padding:0 32px 36px "
"32px;transition:max-width .2s,padding .2s;}"
".dashboard-row{display:flex;gap:40px;align-items:flex-start;justify-"
"content:space-between;}"
".info-section, .main-controls{width:100%;max-width:500px;}"
"@media(max-width:900px){.mainwrap{max-width:99vw;padding:0 4vw 24px "
"4vw;}.dashboard-row{flex-direction:column;gap:0;}.info-section,.main-"
"controls{max-width:unset;width:100%;}}"
".brandcard{background:#1a1d25;border-radius:13px;box-shadow:0 4px 20px "
"#10152063;margin-bottom:28px;padding:20px 20px 11px 25px;"
"display:flex;flex-direction:row;align-items:center;gap:10px;}"
".logoicon{width:44px;height:44px;border-radius:12px;background:#254bbb;"
"display:flex;align-items:center;justify-content:center;}"
".brandtxt{font-size:1.31em;font-weight:700;letter-spacing:.08em;color:#"
"fff;}"
".subtitle{color:#a3c4ff;font-size:.98em;font-weight:600;margin-top:4px;"
"margin-bottom:0;line-height:120%;}"
" #reset-button .material-icons {"
" color:#fff;"
" font-size:2.03em;"
" transition:color .12s;"
"}"
"#reset-button:hover .material-icons {"
" color:#ff6373;"
"}"
".logout-ico{margin-left:auto;border:none;outline:none;cursor:pointer;"
"background:none;padding:0;display:flex;align-items:center;}"
".logout-ico .material-icons{font-size:2.03em;color:#fff;transition:color "
".12s;}"
".logout-ico:hover .material-icons{color:#36c7fc;}"
".info-section{display:flex;flex-direction:column;gap:14px;margin-bottom:0;"
"}"
".info-card-wide{background:#1a202a;border-radius:16px;box-shadow:0 2px "
"18px #0f1b299c;"
"padding:21px 25px 17px "
"23px;position:relative;display:flex;flex-direction:column;justify-content:"
"center;align-items:flex-start;"
"min-height:80px;width:100%;box-sizing:"
"border-box;margin-bottom:0;}"
".icon-top-right{position:absolute;top:18px;right:22px;font-size:1.34em;"
"color:#fff;opacity:.92;}"
".info-title-modern{color:#b2d3ff;font-size:1.11em;font-weight:600;letter-"
"spacing:.04em;margin-bottom:3px;}"
".info-value-modern{font-size:2.25em;font-weight:700;letter-spacing:.06em;"
"color:#fff;line-height:1.07em;margin-bottom:8px;}"
".info-desc-modern{color:#7ad9ff;font-size:.98em;font-weight:500;margin-"
"top:0;margin-bottom:0;line-height:1.14em;opacity:.88;}"
".fx-gallery{"
" width:100%;"
" max-width:480px;"
" display:grid;"
" grid-template-columns:repeat(3,1fr);"
" grid-auto-rows:minmax(50px,auto);"
" gap:16px;"
" margin:0 auto 18px auto;"
" padding:16px 0;"
" box-sizing:border-box;"
" transition:max-width .2s,gap .2s;"
" justify-items:stretch;"
" align-items:stretch;"
" min-width:240px;"
"}"
"@media (min-width:570px){"
" .fx-gallery{grid-template-columns:repeat(4,1fr);}"
"}"
".fx-cell{"
" background:#1a2130;"
" border:2.4px solid #32415a;"
" border-radius:12px;"
" min-width:0;"
" min-height:50px;"
" padding-top:14px;"
" padding-bottom:10px;"
" text-align:center;"
" cursor:pointer;"
" transition:border 0.13s,box-shadow 0.12s,background 0.13s,transform "
"0.18s;"
" box-shadow:0 1.1px 8px #121b2c33;"
" display:flex;"
" flex-direction:column;"
" align-items:center;"
" justify-content:center;"
" aspect-ratio:1/1;"
" min-width:60px;"
"}"
".fx-label{"
" color:#fff;"
" font-size:0.78em;"
" line-height:1.3;"
" display: block;"
" font-weight:600;"
" margin-top:2px;"
" margin-bottom:4px;"
" letter-spacing:0.03em;"
" white-space:normal;"
" word-break:break-word;"
" text-align:center;"
" padding:0 4px;"
"}"
".card.message{"
" background:#1a202a;"
" border-radius:16px;"
" box-shadow:0 2px 18px #0f1b299c;"
" padding:21px 25px 17px 23px;"
" margin-bottom:19px;"
" align-items:center;"
" box-sizing:border-box;"
" margin-left:auto;"
" margin-right:auto;"
" min-width:0;"
" display:flex;"
" flex-direction:column;"
" justify-content:center;"
"}"
".msgsection{"
" width:292px;"
" max-width:94vw;"
" margin:0 auto;"
" display:flex;"
" flex-direction:column;"
" align-items:center;"
" padding:0;"
"}"
".wifi-section{"
" width:100%;"
" max-width:400px;"
" margin:0 auto 20px auto;"
" background:#1a202a;"
" border-radius:16px;"
" padding:21px 25px 17px 23px;"
" box-shadow:0 2px 18px #0f1b299c;"
"}"
".wifi-title{"
" color:#b2d3ff;"
" font-size:1.11em;"
" font-weight:600;"
" margin-bottom:15px;"
" display:flex;"
" align-items:center;"
" gap:8px;"
"}"
".wifi-input{"
" width:100%;"
" padding:12px 16px;"
" margin-bottom:12px;"
" background:#151922;"
" border:2px solid #32415a;"
" border-radius:8px;"
" color:#fff;"
" font-size:14px;"
" font-family:inherit;"
" outline:none;"
" transition:border-color 0.2s;"
"}"
".wifi-input:focus{"
" border-color:#36c7fc;"
"}"
".wifi-input::placeholder{"
" color:#7a8a9e;"
"}"
".wifi-btn{"
" width:100%;"
" padding:12px 16px;"
" background:#254bbb;"
" border:none;"
" border-radius:8px;"
" color:#fff;"
" font-size:14px;"
" font-weight:600;"
" cursor:pointer;"
" transition:background 0.2s;"
"}"
".wifi-btn:hover{"
" background:#1e3d96;"
"}"
".wifi-status{"
" margin-top:12px;"
" padding:8px 12px;"
" border-radius:6px;"
" font-size:13px;"
" font-weight:500;"
"}"
".wifi-status.ap{"
" background:#ff637333;"
" color:#ff9aa2;"
" border:1px solid #ff637355;"
"}"
".wifi-status.connected{"
" background:#4ade8033;"
" color:#86efac;"
" border:1px solid #4ade8055;"
"}"
".msg-label{"
" font-weight:700;"
" color:#c6fae9;"
" font-size:1.04em;"
" margin-bottom:10px;"
" display:block;"
" text-align:center;"
"}"
".msg-inp{"
" width:100%;"
" background:#232e39;"
" border:1.8px solid #3372e0;"
" border-radius:7px;"
" font-size:1.12em;"
" padding:13px 13px;"
" color:#fff;"
" transition:border 0.17s;"
" box-sizing:border-box;"
" margin-bottom:0;"
"}"
".msg-inp:focus{"
" border:2px solid #20d5e0;"
" background:#181f28;"
"}"
".pill-sliders{display:flex;flex-direction:column;gap:32px;margin-bottom:"
"22px;}"
".pill-group{width:100%;display:flex;flex-direction:column;align-items:"
"stretch;margin-left:auto;margin-right:auto;}"
".pill-label{color:#71bcfa;font-weight:700;font-size:1.08em;letter-spacing:"
".03em;margin-bottom:13px;margin-left:5px;}"
".pill-slider-wrap{width:100%;background:#202b38;border-radius:23px;box-"
"shadow:0 2.5px 14px #18429431;padding:3px 0 3px "
"0;display:flex;align-items:center;position:relative;min-height:46px;"
"cursor:pointer;transition:box-shadow .13s;}"
".pill-slider-bar{height:40px;width:100%;position:relative;border-radius:"
"22px;overflow:hidden;background:#29384c;transition:background 0.14s;}"
".pill-fill{position:absolute;top:0;left:0;bottom:0;width:0%;height:100%;"
"border-radius:22px;transition:width .28s "
"cubic-bezier(.68,-0.6,.32,1.6),background .18s;z-index:2;}"
".pill-value{position:absolute;left:50%;top:50%;transform:translate(-50%,-"
"50%);color:#fff;font-weight:700;font-size:1.14em;z-index:3;text-shadow:0 "
"1px 8px #000b;pointer-events:none;}"
".pill-icon{margin-left:17px;margin-right:19px;font-size:2em;color:#73dcfd;"
"z-index:3;opacity:0.93;}"
".firmware-card{"
" background:#1a202a;"
" border-radius:16px;"
" box-shadow:0 2px 18px #0f1b299c;"
" padding:36px 22px 30px 22px;"
" position:relative;"
" display:flex;"
" flex-direction:column;"
" align-items:center;"
" justify-content:center;"
" margin:0 auto 22px auto;"
" max-width:360px;"
" width:100%;"
" min-width:0;"
" box-sizing:border-box;"
" min-height:280px;"
"}"
"@media (min-width:900px){"
" .firmware-card{"
" max-width:500px;"
" padding-left:36px;"
" padding-right:36px;"
" }"
"}"
".fw-title-row{width:100%; display:flex;flex-direction:row;align-items:"
"center;margin-bottom:18px;}"
".fw-title-text{color:#b2d3ff;font-size:1.11em;font-weight:600;letter-"
"spacing:.04em;margin-right:auto;}"
".fw-icon{font-size:1.5em;color:#fff;opacity:.92;margin-left:auto;}"
".fw-btn-row{display:flex;width:100%;justify-content:center;align-items:"
"center;margin-bottom:16px;}"
"#otaBtn{border:none;background:#249dfa;padding:13px 25px 13px "
"25px;border-radius:8px;font-weight:700;font-size:1.08em;color:#fff;cursor:"
"pointer;box-shadow:0 2px 9px #3197f922;transition:background "
".12s;display:inline-flex;align-items:center;justify-content:center;gap:"
"8px;}"
"#otaBtn:hover{background:#39c3fc;}"
"#binFile{display:none;}"
".fw-uploadbox{width:100%;background:#232e39;border-radius:10px;box-shadow:"
"0 2px 14px #193d936a;padding:20px 16px 15px "
"16px;display:flex;flex-direction:column;align-items:center;justify-"
"content:center;}"
".fw-bar-bg{width:98%;background:#19233a;border-radius:6px;height:15px;box-"
"shadow:0 2px 9px "
"#28367333;position:relative;overflow:hidden;margin-bottom:10px;}"
".fw-bar-prg{background:#1ec2ed;transition:width "
".24s;min-width:8px;height:100%;border-radius:7px;width:0%;position:"
"absolute;top:0;left:0;}"
".fw-bar-text{color:#8ae4fa;font-size:1em;font-weight:700;position:"
"absolute;width:100%;top:49%;left:0;text-align:center;transform:translateY("
"-50%);pointer-events:none;}"
".fw-log{width:98%;color:#8ec7fc;font-size:.97em;font-family:monospace;"
"margin:4px auto 1px "
"auto;min-height:18px;text-align:center;word-break:break-all;opacity:.96;}"
"#login-modal{display:none;position:fixed;z-index:1000;left:0;top:0;width:"
"100vw;height:100vh;overflow:auto;background-color:rgba(0,0,0,0.6);padding-"
"top:0;align-items:center;justify-content:center;}"
"#login-modal.active{display:flex;}"
"#login-pop{background-color:#1a1d25;margin:auto;padding:32px 28px 28px "
"28px;border-radius:16px;box-shadow:0 8px 40px "
"rgba(0,0,0,0.3);width:340px;max-width:90vw;position:relative;display:flex;"
"flex-direction:column;align-items:center;animation:slideIn 0.3s ease-out;}"
"@keyframes "
"slideIn{from{transform:translateY(-30px);opacity:0;}to{transform:"
"translateY(0);opacity:1;}}"
"#login-icon{font-size:3.2em;color:#254bbb;margin-bottom:16px;}"
".login-main{font-size:1.5em;font-weight:700;color:#fff;margin-bottom:8px;"
"text-align:center;}"
".login-note{color:#a3c4ff;font-size:0.95em;margin-bottom:24px;text-align:"
"center;}"
".login-box-group{width:100%;display:flex;flex-direction:column;gap:16px;}"
".login-input{width:100%;padding:14px 16px;background:#232e39;border:2px "
"solid "
"#3372e0;border-radius:8px;color:#fff;font-size:1.1em;transition:border-"
"color 0.2s;box-sizing:border-box;}"
".login-input:focus{outline:none;border-color:#20d5e0;background:#181f28;}"
".login-input::placeholder{color:#7a8b9a;}"
".pw-container{position:relative;}"
"#pweye{position:absolute;right:12px;top:50%;transform:translateY(-50%);"
"color:#7a8b9a;cursor:pointer;font-size:1.2em;user-select:none;}"
"#btn-login{width:100%;padding:14px;background:#254bbb;color:#fff;border:"
"none;border-radius:8px;font-size:1.1em;font-weight:600;cursor:pointer;"
"transition:background 0.2s;margin-top:8px;}"
"#btn-login:hover{background:#1e3f99;}"
"#errbox{color:#ff6b6b;font-size:0.9em;margin-top:12px;text-align:center;"
"min-height:20px;}"
".toast{position:fixed;left:50%;bottom:26px;transform:translateX(-50%);"
"background:rgba(18,196,244,0.98);color:#01223d;padding:11px "
"22px;border-radius:21px;font-weight:700;font-size:1.1em;z-index:240;box-"
"shadow:0 2.3px 15px "
"#22b2e9ab;display:none;min-width:108px;text-align:center;}"
".card,.info-card-modern,.info-card-wide,.firmware-card{margin-left:auto;"
"margin-right:auto;box-sizing:border-box;}"
".footer-custom{width:100%;text-align:center;padding:22px 4px 12px "
"4px;margin:0 auto 0 "
"auto;letter-spacing:.02em;color:#a3c4ff;font-size:1.05em;font-weight:500;"
"background:transparent;}"
".footer-custom "
".heart-emoji{color:#f65a86;font-size:1.1em;vertical-align:-3px;}"
"</style></head><body>"
"<div id='login-modal'><form id='login-pop' autocomplete='off'>"
"<span id='login-icon' class='material-icons'>shield</span>"
"<span class='login-main'>Secure Access</span><div "
"class='login-note'>Enter credentials to control the matrix</div>"
"<div class='login-box-group'>"
"<input class='login-input' id='login-user' type='text' "
"placeholder='Username' autocomplete='username' required>"
"<div class='pw-container'><input class='login-input' id='login-pw' "
"type='password' placeholder='Password' autocomplete='current-password' "
"required>"
"<span id='pweye' class='material-icons'>visibility</span></div>"
"<button id='btn-login' type='submit'>Access Dashboard</button>"
"</div>"
"<div id='errbox'></div></form></div>"
"<div class='mainwrap'>"
"<div class='brandcard'>"
"<div class='logoicon'><span class='material-icons'>hub</span></div>"
"<div style='flex:1 1 0%;'><div class='brandtxt'>Parola Matrix</div><div "
"class='subtitle'>LED Matrix WiFi Display</div></div>"
"<button class='logout-ico' id='reset-button' title='Reset Settings'>"
"<span class='material-icons'>restart_alt</span>"
"</button>"
"<button class='logout-ico' id='logout-link' title='Logout'>"
"<span class='material-icons'>logout</span>"
"</button></div>"
"<div class='dashboard-row'>"
"<div class='info-section'>"
"<div class='info-card-wide'>"
"<span class='icon-top-right material-icons'>schedule</span>"
"<span class='info-title-modern'>Current Time</span>"
"<span class='info-value-modern' id='curtime'>--:--:--</span>"
"<span class='info-desc-modern' id='ssid'>&nbsp;</span>"
"</div>"
"<div class='info-card-wide'>"
"<span class='icon-top-right material-icons'>wifi</span>"
"<span class='info-title-modern'>Network</span>"
"<span class='info-value-modern' id='ip'>-</span>"
"<span class='info-desc-modern'>Device IP Address</span>"
"</div>"
"<div class='info-card-wide'>"
"<span class='icon-top-right material-icons'>timelapse</span>"
"<span class='info-title-modern'>Uptime</span>"
"<span class='info-value-modern' id='uptime'>--</span>"
"<span class='info-desc-modern'>Since last reboot</span>"
"</div>"
"<div class='card effectcd'>"
"<div class='fx-gallery' id='fxgallery'></div>"
"</div>"
"</div>"
"<div class='main-controls'>"
"<div class='wifi-section' id='wifiSection' style='display:none;'>"
"<div class='wifi-title'>"
"<i class='material-icons'>wifi</i>"
"WiFi Configuration"
"</div>"
"<input class='wifi-input' id='wifiSsid' type='text' placeholder='WiFi Network Name (SSID)' maxlength='63'>"
"<input class='wifi-input' id='wifiPassword' type='password' placeholder='WiFi Password' maxlength='63'>"
"<button class='wifi-btn' onclick='saveWifi()'>Save WiFi & Restart</button>"
"<div class='wifi-status' id='wifiStatus'>Checking status...</div>"
"</div>"
"<div class='card message'><div class='msgsection'>"
"<div class='msg-label'>Display Text</div>"
"<input class='msg-inp' id='msg' type='text' maxlength='63' "
"autocomplete='off' placeholder='Enter your message...'>"
"</div></div>"
"<div class='pill-sliders'>"
"<div class='pill-group' id='bright-group'>"
"<div class='pill-label'>Brightness</div>"
"<div class='pill-slider-wrap' id='bright-slider'>"
"<i class='material-icons pill-icon'>wb_sunny</i>"
"<div class='pill-slider-bar'>"
"<div class='pill-fill' id='bright-fill'></div>"
"<span class='pill-value' id='brightval'>5</span>"
"</div>"
"</div>"
"</div>"
"<div class='pill-group' id='speed-group'>"
"<div class='pill-label'>Speed</div>"
"<div class='pill-slider-wrap' id='speed-slider'>"
"<i class='material-icons pill-icon'>speed</i>"
"<div class='pill-slider-bar'>"
"<div class='pill-fill' id='speed-fill'></div>"
"<span class='pill-value' id='speedval'>5</span>"
"</div>"
"</div>"
"</div>"
"</div>"
"<div class='firmware-card'>"
"<div class='fw-title-row'>"
"<span class='fw-title-text'>Firmware Update</span>"
"<span class='fw-icon material-icons'>cloud_upload</span></div>"
"<form id='otaForm' enctype='multipart/form-data' "
"style='width:100%;margin:0;'>"
"<div class='fw-btn-row'>"
"<button type='button' id='otaBtn'>"
"<i class='material-icons' "
"style='vertical-align:middle;margin-right:0;'>cloud_upload</i>Choose File"
"</button>"
"</div>"
"<div class='fw-uploadbox'>"
"<div class='fw-bar-bg'><div class='fw-bar-prg' id='fwBarPrg'></div>"
"<div class='fw-bar-text' id='fwBarText'>0%</div></div>"
"<div class='fw-log' id='fwLog'>&nbsp;</div>"
"</div>"
"<input type='file' id='binFile' name='firmware' accept='.bin' />"
"</form>"
"</div>"
"</div>"
"</div>"
"</div>"
"<div class='footer-custom'>Made with <span "
"class=\"heart-emoji\">&#9829;&#65039;</span> by @wordwires</div>"
"<div class='toast' id='toast'></div>"
"<script>"
"const "
"effectIcons=['block','print','arrow_upward','arrow_downward','arrow_back',"
"'arrow_forward','animation','grid_on','blur_on','grain','view_week','"
"shuffle','waterfall_chart','keyboard_tab','deselect','table_chart','"
"height','align_vertical_bottom','open_in_full','keyboard_return','"
"fullscreen_exit','undo','north_east','north_west','south_east','south_"
"west','vertical_align_top','vertical_align_bottom'];"
"const effectNames=['No Effect','Print','Scroll Up','Scroll Down','Scroll "
"Left','Scroll "
"Right','Slice','Mesh','Fade','Dissolve','Blinds','Random','Wipe','Wipe "
"Cursor','Scan Horiz','Scan HorizX','Scan Vert','Scan "
"VertX','Opening','Opening Cursor','Closing','Closing Cursor','Scroll Up "
"Left','Scroll Up Right','Scroll Down Left','Scroll Down Right','Grow "
"Up','Grow Down'];"
"function isLoggedIn(){return localStorage.getItem('parola_token') !== "
"null;}"
"function "
"showLogin(){document.getElementById('login-modal').style.display='flex';"
"document.getElementById('errbox').textContent='';}"
"function "
"hideLogin(){document.getElementById('login-modal').style.display='none';}"
"function checkAuth(){if(!isLoggedIn()){showLogin();return false;}return "
"true;}"
"document.getElementById('login-pop').onsubmit=function(e){e."
"preventDefault();"
"var "
"u=document.getElementById('login-user').value.trim(),p=document."
"getElementById('login-pw').value;"
"fetch('/login',{method:'POST',headers:{'Content-Type':'application/"
"x-www-form-urlencoded'},"
"body:'username='+encodeURIComponent(u)+'&password='+encodeURIComponent(p)}"
")"
".then(r=>{if(!r.ok)throw new Error('Login failed');return r.json();})"
".then(data=>{localStorage.setItem('parola_token',data.token);hideLogin();"
"fetchStatus();})"
".catch(()=>{document.getElementById('errbox').textContent='Incorrect "
"username or password.';});"
"};"
"document.getElementById('reset-button').onclick=function(e){e.preventDefault();"
"if(!checkAuth()) return;"
"if(confirm('Reset all settings and clear EEPROM?')){"
" fetch('/resetAll',{"
" method:'POST',"
" headers:{"
" 'Authorization':'Bearer '+localStorage.getItem('parola_token')"
" }"
" }).then(res=>{"
" if(res.ok){"
" showToast('Reset done. Rebooting...');"
" setTimeout(()=>location.reload(),3000);"
" }else{"
" showToast('Reset failed');"
" }}"
" ).catch(()=>showToast('Request failed'));"
"}};"
"document.getElementById('logout-link').onclick=function(e){e."
"preventDefault();localStorage.removeItem('parola_token');showLogin();};"
"document.getElementById('pweye').onmousedown=document.getElementById('"
"pweye').ontouchstart=function(){var "
"pwin=document.getElementById('login-pw');pwin.type='text';setTimeout("
"function(){pwin.type='password';},2200);};"
"document.getElementById('pweye').onmouseup=document.getElementById('pweye'"
").onmouseleave=document.getElementById('pweye').ontouchend=function(){"
"document.getElementById('login-pw').type='password';};"
"if(!checkAuth()){showLogin();}else{hideLogin();}"
"function showToast(msg){"
" var t=document.getElementById('toast');"
" t.innerText=msg;"
" t.style.display='block';"
" setTimeout(function(){t.style.display='none';},1900);"
"}"
"var BRIGHT_MIN=1,BRIGHT_MAX=15,SPEED_MIN=1,SPEED_MAX=10;"
"function pillFillColor(type,value,min,max){"
" var pct=(value-min)/(max-min);"
" if(type==='bright'){"
" if(pct<0.34) return '#13e872';"
" if(pct<0.67) return '#fca638';"
" return '#fc3460';"
" }"
" if(type==='speed'){"
" if(pct<0.34) return '#13e872';"
" if(pct<0.67) return '#fca638';"
" return '#fc3460';"
" }"
" return '#1ec2ed';"
"}"
"function setPillSlider(id,value,min,max){"
" var fill=document.getElementById(id+'-fill'),"
" span=document.getElementById(id+'val'),"
" pct=(value-min)/(max-min);"
" fill.style.width=Math.round(pct*100)+'%';"
" fill.style.background=pillFillColor(id,value,min,max);"
" span.innerText=value;"
"}"
"function pillSliderHandler(id,min,max){"
" var wrap=document.getElementById(id+'-slider'),"
" fill=document.getElementById(id+'-fill'),"
" bar=fill.parentNode,"
" down=false;"
" function updateFromPos(x){"
" var rect=bar.getBoundingClientRect();"
" var rel=(x-rect.left)/rect.width;"
" rel=Math.max(0,Math.min(rel,1));"
" var value=Math.round(min+rel*(max-min));"
" setPillSlider(id,value,min,max);"
" if(checkAuth()){"
" fetch('/set'+id.charAt(0).toUpperCase()+id.slice(1),"
" {method:'POST',headers:{'Content-Type':'application/"
"x-www-form-urlencoded','Authorization':'Bearer "
"'+localStorage.getItem('parola_token')},"
" body:'value='+value});"
" showToast((id=='bright'?'Brightness: ':'Speed: ')+value);"
" }"
" }"
" wrap.addEventListener('mousedown', "
"function(e){down=true;updateFromPos(e.clientX);});"
" wrap.addEventListener('touchstart',function(e){down=true;updateFromPos(e."
"touches[0].clientX);});"
" document.addEventListener('mousemove',function(e){if(down)updateFromPos("
"e.clientX);});"
" document.addEventListener('touchmove',function(e){if(down)updateFromPos("
"e.touches[0].clientX);},{passive:false});"
" document.addEventListener('mouseup',function(){down=false;});"
" document.addEventListener('touchend',function(){down=false;});"
" wrap.addEventListener('click',function(e){updateFromPos(e.clientX);});"
"}"
"let "
"statusData={brightness:5,speed:5,effect:0,message:'',ip:'',curtime:'--:--:"
"--',ssid:'...'};"
"function fetchStatus(){"
" if(!checkAuth()){showLogin();return;}"
" fetch('/status',{headers:{'Authorization':'Bearer "
"'+localStorage.getItem('parola_token')}})"
" .then(function(r){return r.json();})"
" .then(function(d){"
" statusData=d;"
" setPillSlider('bright',d.brightness||5,BRIGHT_MIN,BRIGHT_MAX);"
" setPillSlider('speed',d.speed||5,SPEED_MIN,SPEED_MAX);"
" document.getElementById('msg').value=d.message;"
" Array.prototype.forEach.call(document.querySelectorAll('.fx-cell'),"
"function(el,ix){el.classList.toggle('fx-selected',ix===d.effect);});"
" document.getElementById('ip').innerText=d.ip;"
" document.getElementById('curtime').innerText=d.curtime;"
" document.getElementById('ssid').innerText=(d.ssid||'WiFi');"
" document.getElementById('uptime').innerText = d.uptime;"
" updateWifiStatus(d);"
" }).catch(function(){showLogin();});"
"}"
"function setFxGallery(){"
" var fx=document.getElementById('fxgallery');fx.innerHTML='';for(var "
"i=0;i<effectNames.length;++i){"
" var cd=document.createElement('div');cd.className='fx-cell';cd.id='fx'+i;"
" var ic=document.createElement('i');ic.className='material-icons "
"fx-icon';ic.innerText=effectIcons[i];"
" var "
"lb=document.createElement('span');lb.className='fx-label';lb.innerText="
"effectNames[i];"
" cd.appendChild(ic);cd.appendChild(lb);"
" (function(i){cd.addEventListener('click',function(){if(!checkAuth())"
"return;"
" fetch('/setEffect',{method:'POST',headers:{'Content-Type':'application/"
"x-www-form-urlencoded','Authorization':'Bearer "
"'+localStorage.getItem('parola_token')},body:'value='+i}).then(function(){"
"fetchStatus();showToast('"
"Effect: '+effectNames[i]);});});})(i);"
" fx.appendChild(cd);"
" }"
"}"
"document.addEventListener('DOMContentLoaded',function(){"
" var msg=document.getElementById('msg');"
" msg.addEventListener('input',function(){"
" if(!checkAuth())return;fetch('/"
"setMessage',{method:'POST',headers:{'Content-Type':'application/"
"x-www-form-urlencoded','Authorization':'Bearer "
"'+localStorage.getItem('parola_token')},body:'value='+encodeURIComponent("
"msg.value)});showToast('"
"Live updated!');});"
" setFxGallery();"
" if(checkAuth()){fetchStatus();setInterval(fetchStatus,4000);}"
" setPillSlider('bright',5,BRIGHT_MIN,BRIGHT_MAX);"
" setPillSlider('speed',5,SPEED_MIN,SPEED_MAX);"
" pillSliderHandler('bright',BRIGHT_MIN,BRIGHT_MAX);"
" pillSliderHandler('speed',SPEED_MIN,SPEED_MAX);"
"});"
"function updateWifiStatus(data){"
" var status = document.getElementById('wifiStatus');"
" var section = document.getElementById('wifiSection');"
" if(data.apMode){"
" status.textContent = 'Currently in AP Mode - Connect to configure WiFi';"
" status.className = 'wifi-status ap';"
" section.style.display = 'block';"
" }else{"
" status.textContent = 'Connected to: ' + (data.ssid || 'Unknown');"
" status.className = 'wifi-status connected';"
" section.style.display = 'block';"
" }"
"}"
"function saveWifi(){"
" if(!checkAuth())return;"
" var ssid = document.getElementById('wifiSsid').value.trim();"
" var password = document.getElementById('wifiPassword').value;"
" if(!ssid){"
" showToast('Please enter WiFi SSID');"
" return;"
" }"
" if(confirm('Save WiFi settings and restart device?')){"
" fetch('/setWifi',{"
" method:'POST',"
" headers:{'Content-Type':'application/x-www-form-urlencoded','Authorization':'Bearer '+localStorage.getItem('parola_token')},"
" body:'ssid='+encodeURIComponent(ssid)+'&password='+encodeURIComponent(password)"
" }).then(function(r){"
" if(r.ok){"
" showToast('WiFi settings saved! Device restarting...');"
" setTimeout(function(){window.location.reload();},3000);"
" }else{"
" showToast('Failed to save WiFi settings');"
" }"
" }).catch(function(){"
" showToast('Error saving WiFi settings');"
" });"
" }"
"}"
"document.getElementById('otaBtn').onclick = function(e) {"
" e.preventDefault();"
" if(!checkAuth())return;"
" document.getElementById('binFile').click();"
"};"
"document.getElementById('binFile').onchange = function() {"
" if (!checkAuth()) return;"
" var fileInput = document.getElementById('binFile');"
" var file = fileInput.files[0];"
" if (!file) return;"
" var xhr = new XMLHttpRequest();"
" var form = new FormData();"
" form.append('firmware', file);"
" xhr.open('POST', '/update', true);"
" xhr.setRequestHeader('Authorization', 'Bearer ' + "
"localStorage.getItem('parola_token'));"
" xhr.upload.onprogress = function(event) {"
" if (event.lengthComputable) {"
" var percent = Math.round((event.loaded / event.total) * 100);"
" document.getElementById('fwBarPrg').style.width = percent + '%';"
" document.getElementById('fwBarText').innerText = 'Uploading: ' + percent "
"+ '%';"
" document.getElementById('fwLog').innerText = 'Uploading...';"
" }"
" };"
" xhr.onload = function() {"
" if (xhr.status == 200) {"
" document.getElementById('fwBarPrg').style.width = '100%';"
" document.getElementById('fwBarText').innerText = 'Upload complete!';"
" document.getElementById('fwLog').innerText = 'Upload complete. "
"Rebooting...';"
" showToast('Firmware uploaded!');"
" } else {"
" document.getElementById('fwBarPrg').style.width = '0%';"
" document.getElementById('fwBarText').innerText = 'Failed!';"
" document.getElementById('fwLog').innerText = 'Update failed!';"
" showToast('Update failed!');"
" }"
" };"
" xhr.onerror = function() {"
" document.getElementById('fwBarPrg').style.width = '0%';"
" document.getElementById('fwBarText').innerText = 'Error!';"
" document.getElementById('fwLog').innerText = 'Upload failed (network "
"error)!';"
" showToast('Upload failed!');"
" };"
" document.getElementById('fwBarPrg').style.width = '0%';"
" document.getElementById('fwBarText').innerText = '0%';"
" document.getElementById('fwLog').innerText = 'Preparing upload...';"
" xhr.send(form);"
"};"
"</script></body></html>";

bool isAuthorized() {
  if (!server.hasHeader("Authorization"))
    return false;
  if (millis() - sessionTimestamp > SESSION_TIMEOUT)
    return false;
  String header = server.header("Authorization");
  bool ok = header == "Bearer " + sessionToken;
  if (ok)
    sessionTimestamp = millis();
  return ok;
}

String sanitizeMsg(String msg) {
  String o = "";
  for (uint8_t i = 0; i < msg.length() && o.length() < 63; i++) {
    char c = msg.charAt(i);
    if (c >= 32 && c <= 126 && c != '<' && c != '>' && c != '\"' &&
      c != '\'')
      o += c;
  }
  return o;
}
void cleanupOldIPs() {
  unsigned long now = millis();
  for (auto it = loginAttempts.begin(); it != loginAttempts.end();) {
    if (now - it -> second.lastSeen > ONE_WEEK_MS) {
      it = loginAttempts.erase(it);
    } else {
      ++it;
    }
  }
}
void updateClock() {
  if (timeClient.update()) {
    int hr = timeClient.getHours();
    int mn = timeClient.getMinutes();
    char ap = (hr < 12) ? 'A' : 'P';
    int dhr = hr % 12;
    if (dhr == 0)
      dhr = 12;
    char tbuf[10];
    snprintf(tbuf, sizeof(tbuf), "%d:%02d:%cM", dhr, mn, ap);
    char uiBuf[10];
    snprintf(uiBuf, sizeof(uiBuf), "%02d:%02d:%cM", dhr, mn, ap);
    lastClock = String(tbuf);
    lastClockUI = String(uiBuf);
  }
}
void forceShowDefaultMessage() {
  String msg = String(settings.message);
  msg.trim();
  myDisplay.displayClear();
  myDisplay.setIntensity(map(settings.brightness, 1, 15, 1, 15));
  myDisplay.setSpeed(map(settings.speed, 1, 10, 100, 10));
  const char * displayText =
    (msg.length() == 0) ? lastClock.c_str() : settings.message;
  textEffect_t eff =
    effects[settings.effect_idx < NUM_EFFECTS ? settings.effect_idx : 0];
  if (msg.length() == 0) {
    myDisplay.displayText(displayText, PA_CENTER, myDisplay.getSpeed(),
      1000, eff, eff);
  } else {
    myDisplay.displayText(displayText, PA_LEFT, myDisplay.getSpeed(), 0,
      eff, eff);
  }
}
void applySettings() {
  myDisplay.setIntensity(map(settings.brightness, 1, 15, 1, 15));
  myDisplay.setSpeed(map(settings.speed, 1, 10, 100, 10));
  forceShowDefaultMessage();
}
void saveSettingsToEEPROM() {
  EEPROM.write(EEPROM_ADDR_BRIGHTNESS, settings.brightness);
  EEPROM.write(EEPROM_ADDR_SPEED, settings.speed);
  EEPROM.write(EEPROM_ADDR_EFFECT, settings.effect_idx);
  for (uint8_t i = 0; i < sizeof(settings.message); i++) {
    EEPROM.write(EEPROM_ADDR_MESSAGE + i, settings.message[i]);
    if (settings.message[i] == '\0')
      break;
  }
  for (uint8_t i = 0; i < sizeof(settings.wifi_ssid); i++) {
    EEPROM.write(EEPROM_ADDR_WIFI_SSID + i, settings.wifi_ssid[i]);
    if (settings.wifi_ssid[i] == '\0')
      break;
  }
  for (uint8_t i = 0; i < sizeof(settings.wifi_password); i++) {
    EEPROM.write(EEPROM_ADDR_WIFI_PASS + i, settings.wifi_password[i]);
    if (settings.wifi_password[i] == '\0')
      break;
  }
  EEPROM.commit();
}
void loadSettingsFromEEPROM() {
  uint8_t b = EEPROM.read(EEPROM_ADDR_BRIGHTNESS);
  uint8_t s = EEPROM.read(EEPROM_ADDR_SPEED);
  uint8_t e = EEPROM.read(EEPROM_ADDR_EFFECT);
  if (b >= 1 && b <= 15)
    settings.brightness = b;
  if (s >= 1 && s <= 10)
    settings.speed = s;
  if (e < NUM_EFFECTS)
    settings.effect_idx = e;
  for (uint8_t i = 0; i < sizeof(settings.message); i++)
    settings.message[i] = EEPROM.read(EEPROM_ADDR_MESSAGE + i);
  settings.message[sizeof(settings.message) - 1] = '\0';
  
  // Load WiFi credentials
  for (uint8_t i = 0; i < sizeof(settings.wifi_ssid); i++)
    settings.wifi_ssid[i] = EEPROM.read(EEPROM_ADDR_WIFI_SSID + i);
  settings.wifi_ssid[sizeof(settings.wifi_ssid) - 1] = '\0';
  
  for (uint8_t i = 0; i < sizeof(settings.wifi_password); i++)
    settings.wifi_password[i] = EEPROM.read(EEPROM_ADDR_WIFI_PASS + i);
  settings.wifi_password[sizeof(settings.wifi_password) - 1] = '\0';
  
  // Initialize with defaults if empty
  if (strlen(settings.wifi_ssid) == 0) {
    strcpy(settings.wifi_ssid, "...");
  }
  if (strlen(settings.wifi_password) == 0) {
    strcpy(settings.wifi_password, "...");
  }
}
void setup() {
  Serial.begin(9600);
  delay(100);
  EEPROM.begin(EEPROM_SIZE);
  loadSettingsFromEEPROM();
  myDisplay.begin();

  // Check if WiFi credentials are set
  if (strlen(settings.wifi_ssid) > 0 && strcmp(settings.wifi_ssid, "...") != 0) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(settings.wifi_ssid, settings.wifi_password);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    #ifdef ESP8266
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
    #endif

    unsigned long startAttemptTime = millis();
    Serial.print("Connecting to WiFi: ");
    Serial.println(settings.wifi_ssid);

    while (WiFi.status() != WL_CONNECTED &&
      millis() - startAttemptTime < 20000) {
      delay(100);
      Serial.print(".");
      yield();
    }

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\nWiFi connection failed. Starting AP mode.");
      apMode = true;
    }
  } else {
    Serial.println("No WiFi credentials set. Starting AP mode.");
    apMode = true;
  }

  if (apMode) {
    WiFi.mode(WIFI_AP);
    WiFi.softAP("Parola", "parola123");
    Serial.print("AP Mode. IP: ");
    Serial.println(WiFi.softAPIP());
  }

  Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());
  ArduinoOTA.setHostname("Parola_");
  ArduinoOTA.begin();
  timeClient.begin();
  updateClock();

  server.on("/", HTTP_GET,
    []() {
      server.send_P(200, "text/html", index_html);
    });
  server.on("/status", HTTP_GET, []() {
    if (!isAuthorized()) {
      server.send(403, "text/plain", "Auth");
      return;
    }
    updateClock();
    String json = "{";
    json += "\"brightness\":" + String(settings.brightness) + ",";
    json += "\"speed\":" + String(settings.speed) + ",";
    json += "\"effect\":" + String(settings.effect_idx) + ",";
    json += "\"message\":\"" +
      String(sanitizeMsg(String(settings.message))) + "\",";
    json += "\"ip\":\"" + (apMode ? WiFi.softAPIP().toString() : WiFi.localIP().toString()) + "\",";
    json += "\"curtime\":\"" + lastClockUI + "\",";
    json += "\"ssid\":\"" + (apMode ? String("Parola") : String(WiFi.SSID())) + "\",";
    json += "\"apMode\":" + String(apMode ? "true" : "false") + ",";
    unsigned long sec = millis() / 1000;
    unsigned long hrs = sec / 3600;
    unsigned long min = (sec % 3600) / 60;
    unsigned long s = sec % 60;
    char uptime_str[24];
    snprintf(uptime_str, sizeof(uptime_str), "%02luh %02lum %02lus", hrs,
      min, s);
    json += "\"uptime\":\"";
    json += uptime_str;
    json += "\"";
    json += "}";
    server.send(200, "application/json", json);
  });

  server.on("/login", HTTP_POST, []() {
    String clientIP = server.client().remoteIP().toString();

    LoginAttempt & attempt = loginAttempts[clientIP];
    attempt.lastSeen = millis();
    if (millis() - attempt.lastAttemptTime < BLOCK_TIME &&
      attempt.attempts >= MAX_ATTEMPTS) {
      server.send(429, "text/plain", "Too many attempts. Try later.");
      return;
    }

    if (!server.hasArg("username") || !server.hasArg("password")) {
      server.send(400, "text/plain", "Missing credentials");
      return;
    }

    String username = server.arg("username");
    String pass = server.arg("password");

    if (username == adminUser && pass == adminPass) {
      sessionToken = String(random(0x0FFFFFFF), HEX);
      sessionTimestamp = millis();
      attempt.attempts = 0;
      attempt.lastSeen = millis();
      server.send(200, "application/json",
        "{\"token\": \"" + sessionToken + "\"}");
    } else {
      attempt.attempts++;
      attempt.lastAttemptTime = millis();
      server.send(401, "text/plain", "Invalid login");
    }
  });

  server.on("/resetAll", HTTP_POST, []() {
    if (!isAuthorized()) {
      server.send(403, "text/plain", "Auth");
      return;
    }

    for (int i = 0; i < EEPROM_SIZE; i++) {
      EEPROM.write(i, 0);
    }
    EEPROM.commit();

    server.send(200, "text/plain", "Reset OK");
    delay(500);
    ESP.restart(); // Optional: reboot after reset
  });

  server.on("/setBright", HTTP_POST, []() {
    if (!isAuthorized()) {
      server.send(403, "text/plain", "Auth");
      return;
    }
    if (server.hasArg("value")) {
      settings.brightness = constrain(server.arg("value").toInt(), 1, 15);
      applySettings();
      saveSettingsToEEPROM();
      server.send(200, "text/plain", "OK");
    } else
      server.send(400, "text/plain", "Missing value");
  });
  server.on("/setSpeed", HTTP_POST, []() {
    if (!isAuthorized()) {
      server.send(403, "text/plain", "Auth");
      return;
    }
    if (server.hasArg("value")) {
      settings.speed = constrain(server.arg("value").toInt(), 1, 10);
      applySettings();
      saveSettingsToEEPROM();
      server.send(200, "text/plain", "OK");
    } else
      server.send(400, "text/plain", "Missing value");
  });
  server.on("/setEffect", HTTP_POST, []() {
    if (!isAuthorized()) {
      server.send(403, "text/plain", "Auth");
      return;
    }
    if (server.hasArg("value")) {
      settings.effect_idx =
        constrain(server.arg("value").toInt(), 0, NUM_EFFECTS - 1);
      applySettings();
      saveSettingsToEEPROM();
      server.send(200, "text/plain", "OK");
    } else
      server.send(400, "text/plain", "Missing value");
  });
  server.on("/setWifi", HTTP_POST, []() {
    if (!isAuthorized()) {
      server.send(403, "text/plain", "Auth");
      return;
    }
    if (server.hasArg("ssid")) {
      String newSSID = server.arg("ssid");
      String newPassword = server.arg("password");
      
      if (newSSID.length() > 0 && newSSID.length() < 64) {
        newSSID.toCharArray(settings.wifi_ssid, 64);
        newPassword.toCharArray(settings.wifi_password, 64);
        saveSettingsToEEPROM();
        server.send(200, "text/plain", "OK");
        delay(1000);
        ESP.restart();
      } else {
        server.send(400, "text/plain", "Invalid SSID");
      }
    } else {
      server.send(400, "text/plain", "Missing SSID");
    }
  });
  server.on("/setMessage", HTTP_POST, []() {
    if (!isAuthorized()) {
      server.send(403, "text/plain", "Auth");
      return;
    }
    if (server.hasArg("value")) {
      String msg = sanitizeMsg(server.arg("value"));
      memset(settings.message, 0, sizeof(settings.message));
      msg.toCharArray(settings.message, sizeof(settings.message));
      myDisplay.setTextBuffer(settings.message);
      saveSettingsToEEPROM();
      server.send(200, "text/plain", "OK");
    } else
      server.send(400, "text/plain", "Missing value");
  });
  server.onNotFound([]() {
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "Not found, redirecting...");
  });
  httpUpdater.setup( & server, "/update", adminUser, adminPass);
  server.begin();
  applySettings();
}
unsigned long lastNtpSync = 0;
void loop() {
  server.handleClient();
  ArduinoOTA.handle();

  if (millis() - lastCleanupTime > CLEANUP_INTERVAL) {
    cleanupOldIPs();
    lastCleanupTime = millis();
  }
  unsigned long now = millis();
  if (now - lastWifiCheck > wifiRetryInterval) {
    lastWifiCheck = now;
    if (WiFi.status() != WL_CONNECTED) {
      if (wifiConnected) {
        Serial.println("WiFi disconnected. Turning off display.");
        wifiConnected = false;
        displayDisabled = true;
        myDisplay.displayClear();
        myDisplay.displaySuspend(true);
      }
      WiFi.disconnect();
      WiFi.begin(ssid, password);
    } else {
      if (!wifiConnected) {
        Serial.println("WiFi Reconnected.");
        wifiConnected = true;
        displayDisabled = false;
        myDisplay.displayClear();
        myDisplay.displaySuspend(false);
        applySettings();
        updateClock();
      }
    }
  }
  if (!displayDisabled) {
    if (now - lastNtpSync > 4000 || lastNtpSync == 0) {
      lastNtpSync = now;
      updateClock();
      String msg = String(settings.message);
      msg.trim();
      if (msg.length() == 0) {
        forceShowDefaultMessage();
      }
    }
    if (myDisplay.displayAnimate()) {
      myDisplay.displayReset();
    }
  }
}