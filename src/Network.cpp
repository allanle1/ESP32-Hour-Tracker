#include "Network.h"
#include "addons/TokenHelper.h"

#define WIFI_SSID "ASUS"
#define WIFI_PASSWORD "iamnotfat"

#define API_KEY "AIzaSyAiZeTRLXlOSmTDHRi4tRLH5IX4gnF5yO0"
#define FIREBASE_PROJECT_ID "dashboardv2-358a4"
#define USER_EMAIL "test@gmail.com"
#define USER_PASSWORD "Qweasd112"

static Network *instance = NULL;

Network::Network()
{
    instance = this;
}

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("Connected to AP successfully!");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    instance->firebaseInit();
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("Disconnected from WiFi access point");
    Serial.print("WiFi lost connection. Reason: ");
    Serial.println(info.wifi_sta_disconnected.reason);
    Serial.println("Trying to Reconnect");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void FirestoreTokenStatusCallback(TokenInfo info)
{
    Serial.printf("Token Info: type = %s, status = %s\n", getTokenType(info), getTokenStatus(info));
}

void Network::initWiFi()
{
    WiFi.disconnect();
    WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void Network::firebaseInit()
{
    config.api_key = API_KEY;

    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    config.token_status_callback = FirestoreTokenStatusCallback;

    Firebase.begin(&config, &auth);
}

void Network::firestoreDataUpdate(double temp)
{
    if (WiFi.status() == WL_CONNECTED && Firebase.ready())
    {
        String documentPath = "/sites/HyuEaiMLaCgTA8Jagl77/machines/KJCALUh441zFpq2Nvu6m";

        FirebaseJson content;

        content.set("fields/temperature/doubleValue", String(temp).c_str());

        if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(), "temperature"))
        {
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            return;
        }
        else
        {
            Serial.println(fbdo.errorReason());
        }

        if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw()))
        {
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            return;
        }
        else
        {
            Serial.println(fbdo.errorReason());
        }
    }
}