#include <HTTPClient.h>
#include <Update.h>
#include <Firebase_ESP_Client.h>                            
#include "addons/TokenHelper.h"                            
#include "../lib/OTA.h"                                     


FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool taskCompleted = false;

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

void OTAUpdate()
{
    swversion = (swversion.substring((swversion.indexOf(".")), (swversion.lastIndexOf("\\")) + 1))+" "+__DATE__+" "+__TIME__;   
    Serial.print("SW version: ");
    Serial.println(swversion);
    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.token_status_callback = tokenStatusCallback; 
    Firebase.begin(&config, &auth);

    /* Assign download buffer size in byte */
    // Data to be downloaded will read as multiple chunks with this size, to compromise between speed and memory used for buffering.
    // The memory from external SRAM/PSRAM will not use in the TCP client internal rx buffer.
    config.fcs.download_buffer_size = 2048;

    Firebase.reconnectWiFi(true);

    // Firebase.ready() should be called repeatedly to handle authentication tasks.
    
    if (Firebase.ready() && !taskCompleted)
    {
        taskCompleted = true;

        // If you want to get download url to use with your own OTA update process using core update library,
        // see Metadata.ino example

        Serial.println("\nChecking for new firmware update available...\n");

        // In ESP8266, this function will allocate 16k+ memory for internal SSL client.
        if (!Firebase.Storage.downloadOTA(
            &fbdo, STORAGE_BUCKET_ID                      /* Firebase Storage bucket id */, 
            FIRMWARE_PATH                                 /* path of firmware file stored in the bucket */, 
            fcsDownloadCallback                           /* callback function */
            )){
            Serial.println(fbdo.errorReason());
            } else {
                                                          // Delete the file after update
            Serial.printf("Delete file... %s\n",Firebase.Storage.deleteFile(&fbdo, STORAGE_BUCKET_ID,FIRMWARE_PATH) ? "ok" : fbdo.errorReason().c_str());

            Serial.println("Restarting...\n\n");
            delay(2000);
            ESP.restart();  
            }
    }
}
