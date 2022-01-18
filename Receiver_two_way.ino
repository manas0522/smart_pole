/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
// Receiver
#include <esp_now.h>
#include <WiFi.h>
int l;
uint8_t broadcastAddress[] = {0x9C, 0x9C, 0x1F, 0xE3, 0xD8, 0x08};// transmitter mac address

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    char a[32];
    int b;
} struct_message;

// Create a struct_message called myData
struct_message trdata;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&trdata, incomingData, sizeof(trdata));
 /* Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.a);
  Serial.print("Int: ");
  Serial.println(myData.b);
  Serial.print("Float: ");
  Serial.println(myData.c);
  Serial.print("Bool: ");
  Serial.println(myData.d);
  Serial.println();*/
  strcpy(trdata.a, "MY NAME IS SMART POLE");
  l=trdata.b;
  l=l+1;
  trdata.b=l ;
 esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &trdata, sizeof(trdata)); 
}

 void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

   if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
   }


  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

   


   esp_now_register_send_cb(OnDataSent);
}
   
void loop() {

}
