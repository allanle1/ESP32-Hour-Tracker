#ifndef Network_H_
#define Network_H_

#include <WiFi.h>
#include <Firebase_ESP_Client.h>

class Network
{
private:
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;

    void firebaseInit();
    friend void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info);
    friend void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
    friend void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
    friend void FirestoreTokenStatusCallback(TokenInfo info);

public:
    Network();
    void initWiFi();
    void firestoreDataUpdate(double temp);
};

#endif