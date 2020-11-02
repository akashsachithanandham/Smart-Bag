/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the ID/UID, type and any data blocks it can read. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * If your reader supports it, this sketch/program will read all the PICCs presented (that is: multiple tag reading).
 * So if you stack two or more PICCs on top of each other and present them to the reader, it will first output all
 * details of the first and then the next PICC. Note that this may take some time as all data blocks are dumped, so
 * keep the PICCs at reading distance until complete.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define RST_PIN         D1        // Configurable, see typical pin layout above
#define SS_PIN          D2        // Configurable, see typical pin layout above

//firebase and Wifi
#define FIREBASE_HOST "smartbag-64bd7.firebaseio.com"
#define FIREBASE_AUTH "174ofBhkN6h2k1jwTH4t9pTxSa6loQ1wu4g7soUe"
#define WIFI_SSID "Babu"
#define WIFI_PASSWORD "9994650116"

int a = 0;
String userid = "hxYVGsxhhrYg41pFqIic1IaFKE93";
int PinButton = D8;  


MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
	Serial.begin(9600);// Initialize serial communications with the PC
  pinMode(PinButton, INPUT);  
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}

void loop() {
  
  //
  //
  // ALERT BUTTON
  //
  //
  
  int stateButton = digitalRead(PinButton);  
    if (stateButton == 1)  
    {  
        Serial.println("Send Message");
        delay(1000);
        Firebase.setInt( "/users/"+ userid + "/emergency", 1 ); 
    }  

  //
  //
  // GPS LOCATION
  //
  //

  if (WiFi.status() == WL_CONNECTED) 
  {  
    
    HTTPClient http; //Object of class HTTPClient
    http.begin("http://api.ipstack.com/check?access_key=99720de31663f6d6f885d98f1c53b875");
    int httpCode = http.GET();
    Serial.println(httpCode);
    Serial.println();
    if (httpCode > 0) 
    { Serial.println("SUCCESS");
      const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
 
      
      float latitude = root["latitude"]; 
      float longitude= root["longitude"];   

      Serial.print("lat:");
      Serial.println(latitude);
      Serial.print("long");
      Serial.println(longitude);
      Firebase.setFloat( "/users/"+ userid + "/location/recent/lat", latitude);
      Firebase.setFloat( "/users/"+ userid + "/location/recent/long", longitude);
     
    }
    http.end(); //Close connection
  }

  //
  //
  // RFID part
  //
  //

    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  String RFID= "";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     RFID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
     RFID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  

  RFID.toUpperCase();
  String path = "/users/" + userid + "/items/" + RFID;
  Serial.println("UID tag :" + RFID);

  Serial.println("path:" + path +"ok");
 
  FirebaseObject object  = Firebase.get(path);
   

  int state = object.getInt("status"); 
  state = 1- state;
  Firebase.setInt( path +"/status", state); 
 

}

  
