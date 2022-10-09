// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#include "Internet.h"
#include "Config.h"
#include "Private.h"
#include "Format.h"
#include "lwip/apps/sntp.h"
#include "tcpip_adapter.h"

// Смещение часовго пояса в секундах от UTC
RTC_DATA_ATTR int32_t timeZoneOffset = 0;

Internet internet;

Internet::Internet():
  _connected(false),
  _RSSI(WIFI_RSSI_OUT),
  _reconnectLimit(WiFiReconnectLimit),
  _reconnectInterval(WiFiReconnectDelay) {
}

Internet::~Internet()
{

}

bool Internet::isConnected() {
  return _connected;
}

bool Internet::connect() {
  #ifdef SERIAL_DEBUG
  Serial.println("Connecting to: " + WiFi_SSID);
  _timer =  millis();
  #endif
  IPAddress dns(DNS_IP);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA); // switch off AP
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  // bool connected = false;
  _RSSI = WIFI_RSSI_OUT;
  for (int count = 0; count < _reconnectLimit; count++) {
    WiFi.begin(WiFi_SSID.c_str(), WiFi_Password.c_str());
    _connected  = (WiFi.waitForConnectResult() == WL_CONNECTED);
    if (_connected) {
      _RSSI = WiFi.RSSI();
      break;
    }
    WiFi.disconnect(false);
    #ifdef SERIAL_DEBUG
    Serial.print(".");
    #endif
    delay(_reconnectInterval);
  }
  #ifdef SERIAL_DEBUG
  if (_connected) {
    Serial.println("WiFi connected " + String(millis() - _timer) + "ms");
    Serial.println("WiFi RSSI: " + String(WiFi.RSSI()));
    Serial.println("Local  IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("WiFi connection *** FAILED ***");
  }
  #endif
  return _connected;
}

void Internet::disconnect() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  _connected = false;
  #ifdef SERIAL_DEBUG
  _timer =  millis() - _timer;
  Serial.println("WiFi switched Off " + String(_timer) + "ms");
  #endif
}

void Internet::synchronizeTime() {
  #ifdef SERIAL_DEBUG
  uint32_t time0 = millis();
  #endif
  // https://github.com/espressif/esp-idf/tree/master/examples/protocols/sntp
  // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/system_time.html
  tcpip_adapter_init();  // Should not hurt anything if already inited
  if(sntp_enabled()){
      sntp_stop();
  }
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, (char*)NtpServer1);
  sntp_setservername(1, (char*)NtpServer2);
  sntp_setservername(2, (char*)NtpServer3);
  sntp_init();
  #ifdef SERIAL_DEBUG
  time0 = millis() - time0;
  Serial.println("synchronizeTime " + String(time0) + "ms");
  #endif
}

int  Internet::RSSI() {
  return _RSSI;
}

void setTimeZoneOffset(long offset){
  if (timeZoneOffset  != offset) {
    timeZoneOffset = offset;
  }
  fmtTime.Offset = offset;
}

String getLocalTimeISOtz(){
  time_t now;
  time(&now);
  return fmtTime.isotz(uint32_t(now));
}

void printLocalTimeISOtz(){
  Serial.println("Local time: " + getLocalTimeISOtz());
}
