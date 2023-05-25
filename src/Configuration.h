#ifndef Configuration_h
#define Configuration_h

#include <WiFi.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Matrix
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#include "LED_Matrix_Display.h"

#include <RTClib.h>

#include <Temperature_LM75_Derived.h>

#include <time.h>

// ========================================================================================================== 
// 	Setup
// ========================================================================================================== 

#define Serial_Baud 115200

#define SDA 21  
#define SCL 22

#define Faktor_Millis_zu_Min 60000

// ========================================================================================================== 
// 	Degug
// ========================================================================================================== 

#define SERIAL_Monitor

// Debug mode
#define DEBUG

// Wenn auskommentiert dann ist die Wifi Abfrage aktiv
#define WIFI

// I2C an und ausmachen
#define I2C

// ========================================================================================================== 
// 	Accespoint / Wifi
// ========================================================================================================== 

// Gibt die Zeit an nach der der Verbindungsversuch zum internet abgebrochen wird.
#define Wifi_cut_off_timer 3

// Startet Unseren Webserver
AsyncWebServer server(80);

// Hier wird der Name des Acces Points Hinterlegt
const char* ssid0 = "esp32";

// Hier wird die ssid gespeichert
String ssid;

// Hie wird das Passwort gespeichert
String password;

// Dient als Zwischen speicher des Passwortes um eine eingabe zu erkennen
String old_password;

// ========================================================================================================== 
// 	Matrix
// ========================================================================================================== 

// Gibt die Initale Helligkeit des Led Matrix an
#define Init_Matrix_Helligkeit 2

// Als Zwischen speicher fuer millis. 
// Diese Nutzen wir damit wir delay nicht benutzen muessen und der µC da durch nicht untaetik beschaftigt wird
unsigned long int prevMillis = 0;

// Diese angabe ist für die Libary Max72xx damit diese weiß um Welchen Chip es geht
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

// Gibt die Anzahle der Segmente der LED Matrix.
#define MAX_DEVICES 4

// Gibt denn Clock pin der SPI verbindung zur LED Matrix an
#define CLK_PIN   18

// Gibt denn Daten pin der SPI verbindung zur LED Matrix an
#define DATA_PIN  23

// Gibt denn Chip Select pin der SPI verbindung zur LED Matrix an
#define CS_PIN    5

// Hier wird nun der Hardware type und die Anzahl von sigmenten zusammen gefügt und an die Libary uebergeben. Entsprechend wird nun Auch die Klasse "Matrix" für unsere Anzeige angelegt
MD_Parola Matrix = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES); 

// Hier wird die klasse fuer die anzeige angelegt
LED_Matrix LED_Matrix;

// Der Globale buffer für die am Display dargestellten Zeichen
char Buffer[25];

// ========================================================================================================== 
// 	Menue
// ========================================================================================================== 

// Hier wird abgespeichert in welchem bereich des Menues man sich befindet
// Wenn true befindet man sich im Normalen Programm durchlauf mit Uhrzeit, Date und Temp
bool Maincycle = true;

// Speichert ob der Alarm Aktivist oder nicht
bool Alarm_State = false;

// Speichert zwischen ob man die Taster lang oder Kurz gedrueckt hat.
// true = Langer Setup druck
volatile bool long_Setup_Press = false;

// true = kurzer Setup druck
volatile bool short_Setup_Press = false;

// true = kurzer Plus druck
volatile bool short_Plus_Press = false;

// true = kurzer Minus druck
volatile bool short_Minus_Press = false;

// Speichert den status des Bewegungssensor zwischen
bool Bewegungssensor_aktiv = false;

// Dient als zwischen speicher zum verändern der zeit 
byte Alarm_Counter = 0;

// Alarm_Counter uebergiebt seinen wert an den Stunden Speicher
byte Counter_Stunden = 0;

// Alarm_Counter uebergiebt seinen wert an den Minuten Speicher
byte Counter_Minute = 0;

// Hier werden die Millis beim Druecken eins tasters zwischengespeichert
// volatile sorgt dafür das die Variable aus dem ram geladen wird und nicht aus dem flash
volatile unsigned long int Button_Pressed_time;

// Gibt die Zeit an die ein Taster gedrueckt sein muss um als Langer Druck erkannt zu werden
#define long_Button_Presse 2000

// Gibt die zeit an die ein Taster aktive sein muss um als tasten druck erkannt zu werden
#define entprellen 20

// ========================================================================================================== 

// Hier wird das °C hinterlegt das Logo wird hier um 90° gedreht angezeigt
// Byte 1 gibt die Laenge der Zeichens an
  const byte degC[8] = {  0b00000111,
                          0b00000010, 
                          0b00000101, 
                          0b00000010, 
                          0b00111000, 
                          0b01000100, 
                          0b01000100,
                          0b01000100  
};

// ========================================================================================================== 

// Hier wird die Aktuelle position im Menue gespeichert
byte Anzeige_Counter = 1;

// ========================================================================================================== 
// 	Temp. Sensor
// ========================================================================================================== 

// Hier Wird der Bibliothek der Sensor Type uebergebn und das dieser mit einer 12 Bit ausloesung arbeitet
// Zudem wird die Klasse LM75_Sensor angelegt für dem temp Sensor
Generic_LM75_12Bit LM75_Sensor;

// ========================================================================================================== 
// 	RTC > Real-Time-Clock
// ========================================================================================================== 

// Hier Wird der Bibliothek der Chip Type uebergebn und die Klasse rtc angelegt
RTC_DS3231 rtc;

// Dieses Offset dienst dazu das Jahr des RTC auf unser Aktuelle jahr anzuheben.
// Weil NTP das jahr immer zwei stellig zurueckt gibt
#define Offset_years 1900

// Hier wird ein offset fuer die Monate hinterlegt da, dass RTC inter die monate von 0 - 11 zaehlt
#define Offset_month 1

// ========================================================================================================== 
// 	NTP > Network Time Protocol
// ========================================================================================================== 

// Gibt die Adresse des Zeit servers and in diesem fall eine Sammlung von verschiedenen
const char* ntpServer = "pool.ntp.org";

// Hier wird der Zeit zohnen versatz angegeben. In diesem fall 3600 sek. für Berlin
#define UCT_Offset_sec 3600

// Hier kann man eine offset fuer den Sonnen Aufgang angeben. Da wir diese nicht nutzen ist der Wert 0
#define Sonnenaufgang_Offset_sec 0

// ========================================================================================================== 
// 	Button
// ========================================================================================================== 
// Hier werden die Taster Hinterlegt
const byte Taster_Pins[6] = {13, 12, 25, 33, 32, 17};

// ========================================================================================================== 
// 	Buzzer
// ========================================================================================================== 
// Pin fuer den Buzzer
#define Buzzer_Pin 26

// ========================================================================================================== 
// 	Bewegungsmelder
// ========================================================================================================== 

// Pin fuer den Bewegungssensor
#define Bewegungssensor_Pin 15

// Gibt die zeit an die der Bewegungsmelder Aktive ist
#define Bewegung_aktive_time 10000

// Zwischen speicher fuer die millis seit start des Sensors
unsigned long int PrevMillis_Bewegung = 0;

// ========================================================================================================== 
// 	Interval Poti
// ========================================================================================================== 

// Pin fuer den Poti
#define Poti_Pin 4

// Hier wird der Bereich angegeben in dem der LDR sich bewegt
#define Min_Potiwert 0
#define Max_Potiwert 4095

// Hier wird der Bereich der Matrix Helligkeit angegeben
// Helligkeit des LDRs wird in den Matrix bereich gewandelt
#define Min_Programm_interval 1000
#define Max_Programm_interval 10000

// ========================================================================================================== 
// 	LDR > Light Dependent Resistor
// ========================================================================================================== 

// Pin fuer den LDR
#define LDR_Pin 0

// Hier wird der Bereich angegeben in dem der LDR sich bewegt
#define Min_Helligkeitswert 1896
#define Max_Helligkeitswert 4095

// Hier wird der Bereich der Matrix Helligkeit angegeben
// Helligkeit des LDRs wird in den Matrix bereich gewandelt
#define Min_Matrix_Helligkeit 0
#define Max_Matrix_Helligkeit 15

#endif
