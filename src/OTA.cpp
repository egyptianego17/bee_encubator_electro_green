#include <HTTPClient.h>
#include <Update.h>
#include <ArduinoJson.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "../lib/OTA.h"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool taskCompleted = false;

/* URL for the endpoint */
const char* url = "https://mere-gerta-xavis.koyeb.app/latest-stable-firmware";

String swversion = "__FILE__";

unsigned long previousMillis;

void fcsDownloadCallback(FCS_DownloadStatusInfo info)
{
    if (info.status == fb_esp_fcs_download_status_init)
    {
        Serial.printf("New update found\n");
        Serial.printf("Downloading firmware %s (%d bytes)\n", info.remoteFileName.c_str(), info.fileSize);
    }
    else if (info.status == fb_esp_fcs_download_status_download)
    {
        Serial.printf("Downloaded %d%s\n", (int)info.progress, "%");
    }
    else if (info.status == fb_esp_fcs_download_status_complete)
    {
        Serial.println("Donwload firmware completed.");
        Serial.println();
    }
    else if (info.status == fb_esp_fcs_download_status_error)
    {
        Serial.printf("New firmware update not available or download failed, %s\n", info.errorMsg.c_str());
    }
}

String getFirmwareVersion() {
    HTTPClient http;
    http.begin(url);

    int httpCode = http.GET();
    if (httpCode > 0) {
        Serial.printf("HTTP Response code: %d\n", httpCode);
        String response = http.getString();
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, response);

        if (error) {
            Serial.print("Failed to parse JSON: ");
            Serial.println(error.c_str());
            http.end();
            return "";
        } else {
            const char* firmwareVersion = doc["firmwareVersion"];
            http.end();
            return String(firmwareVersion);
        }
    } else {
        Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
        http.end();
        return "";
    }
}

void OTAUpdate(String version)
{
    swversion = (swversion.substring((swversion.indexOf(".")), (swversion.lastIndexOf("\\")) + 1))+" "+__DATE__+" "+__TIME__;   
    Serial.print("SW version: ");
    Serial.println(swversion);
    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
    String firmwarePath = FIRMWARE_PATH + version + "/firmware.bin";
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.token_status_callback = tokenStatusCallback; 
    Firebase.begin(&config, &auth);

    config.fcs.download_buffer_size = 2048;

    Firebase.reconnectWiFi(true);

    if (Firebase.ready() && !taskCompleted)
    {
        taskCompleted = true;
        Serial.println("\nChecking for new firmware update available...\n");

        if (!Firebase.Storage.downloadOTA(
            &fbdo, STORAGE_BUCKET_ID,
            firmwarePath,
            fcsDownloadCallback))
        {
            Serial.println(fbdo.errorReason());
        }
        else
        {
            Serial.printf("Delete file... %s\n", Firebase.Storage.deleteFile(&fbdo, STORAGE_BUCKET_ID, firmwarePath) ? "ok" : fbdo.errorReason().c_str());

            Serial.println("Restarting...\n\n");
            delay(2000);
            ESP.restart();
        }
    }
}

