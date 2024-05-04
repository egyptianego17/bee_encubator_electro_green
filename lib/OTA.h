#ifndef _OTA_H
#define _OTA_H

#define API_KEY "AIzaSyBGA0TfErioLK9PeYl-zobjmUFJ8Xspl-0"                      // Firebase: Define the API Key 
#define USER_EMAIL "abdulrahman.omar17h@gmail.com"                             // Firebase: Define the user Email 
#define USER_PASSWORD "xavi1234"                                               // Firebase: Define password 
#define STORAGE_BUCKET_ID "bee-encubator-ota-v1.appspot.com"                   // Firebase: Define the Firebase storage bucket ID e.g bucket-name.appspot.com 
#define FIRMWARE_PATH "firmware/"                                           // Firebase: Define the firmware path on Firebase 

void OTAUpdate(String version);
String getFirmwareVersion();

#endif