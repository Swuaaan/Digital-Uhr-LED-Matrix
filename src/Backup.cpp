// // ========================================================================================================== 
// // 	Authoren
// // ========================================================================================================== 

// // KIAN Tetzlaff
// // JOSI Brodersen
// // LARS Breiholz

// /**
//  * @file Digital LED Matrix Clock
//  * @version 0.9V
//  * @date 2022-12-15
//  * @copyright Copyright (c) 2023
//  */

// // ========================================================================================================== 
// // 	Include
// // ========================================================================================================== 

// #include <Arduino.h>

// #include <WiFi.h>
// #include <SPIFFS.h>
// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>
 
// #include <MD_Parola.h>
// #include <MD_MAX72xx.h>
// #include <SPI.h>

// #include <RTClib.h>

// #include <Temperature_LM75_Derived.h>

// #include <time.h>

// // ========================================================================================================== 
// // 	Setup
// // ========================================================================================================== 

// #define Serial_Baud 115200

// #define SDA 21  
// #define SCL 22

// #define Faktor_Millis_zu_Min 60000

// // ========================================================================================================== 
// // 	Degug
// // ========================================================================================================== 

// #define SERIAL_Monitor

// // Debug mode
// #define DEBUG

// // Wenn auskommentiert dann ist die Wifi Abfrage aktiv
// #define WIFI

// // I2C an und ausmachen
// #define I2C

// // ========================================================================================================== 
// // 	Accespoint / Wifi
// // ========================================================================================================== 

// // Gibt die Zeit an nach der der Verbindungsversuch zum internet abgebrochen wird.
// #define Wifi_cut_off_timer 3
// // Startet Unseren Webserver
// AsyncWebServer server(80);
// // Hier wird der Name des Acces Points Hinterlegt
// const char* ssid0 = "esp32";
// // Hier wird die ssid gespeichert
// String ssid;
// // Hie wird das Passwort gespeichert
// String password;
// // Dient als Zwischen speicher des Passwortes um eine eingabe zu erkennen
// String old_password;

// // ========================================================================================================== 
// // 	Matrix
// // ========================================================================================================== 

// // Gibt die Initale Helligkeit des Led Matrix an
// #define Init_Matrix_Helligkeit 2
// // Als Zwischen speicher fuer millis. 
// // Diese Nutzen wir damit wir delay nicht benutzen muessen und der µC da durch nicht untaetik beschaftigt wird
// unsigned long int prevMillis = 0;
// // Diese angabe ist für die Libary Max72xx damit diese weiß um Welchen Chip es geht
// #define HARDWARE_TYPE MD_MAX72XX::FC16_HW
// // Gibt die Anzahle der Segmente der LED Matrix.
// #define MAX_DEVICES 4

// // Gibt denn Clock pin der SPI verbindung zur LED Matrix an
// #define CLK_PIN   18
// // Gibt denn Daten pin der SPI verbindung zur LED Matrix an
// #define DATA_PIN  23
// // Gibt denn Chip Select pin der SPI verbindung zur LED Matrix an
// #define CS_PIN    5

// // Hier wird nun der Hardware type und die Anzahl von sigmenten zusammen gefügt und an die Libary uebergeben. Entsprechend wird nun Auch die Klasse "Matrix" für unsere Anzeige angelegt
// MD_Parola Matrix = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES); 
// // Der Globale buffer für die am Display dargestellten Zeichen
// char Buffer[25];

// // ========================================================================================================== 
// // 	Menue
// // ========================================================================================================== 
// // Hier wird abgespeichert in welchem bereich des Menues man sich befindet
// // Wenn true befindet man sich im Normalen Programm durchlauf mit Uhrzeit, Date und Temp
// bool Maincycle = true;
// // Speichert ob der Alarm Aktivist oder nicht
// bool Alarm_State = false;
// // Speichert zwischen ob man die Taster lang oder Kurz gedrueckt hat.
// // true = Langer Setup druck
// volatile bool long_Setup_Press = false;
// // true = kurzer Setup druck
// volatile bool short_Setup_Press = false;
// // true = kurzer Plus druck
// volatile bool short_Plus_Press = false;
// // true = kurzer Minus druck
// volatile bool short_Minus_Press = false;
// // Speichert den status des Bewegungssensor zwischen
// bool Bewegungssensor_aktiv = false;
// // Dient als zwischen speicher zum verändern der zeit 
// byte Alarm_Counter = 0;
// // Alarm_Counter uebergiebt seinen wert an den Stunden Speicher
// byte Counter_Stunden = 0;
// // Alarm_Counter uebergiebt seinen wert an den Minuten Speicher
// byte Counter_Minute = 0;

// // Hier werden die Millis beim Druecken eins tasters zwischengespeichert
// // volatile sorgt dafür das die Variable aus dem ram geladen wird und nicht aus dem flash
// volatile unsigned long int Button_Pressed_time;
// // Gibt die Zeit an die ein Taster gedrueckt sein muss um als Langer Druck erkannt zu werden
// #define long_Button_Presse 2000
// // Gibt die zeit an die ein Taster aktive sein muss um als tasten druck erkannt zu werden
// #define entprellen 20

// // ========================================================================================================== 
// // Hier wird das °C hinterlegt das Logo wird hier um 90° gedreht angezeigt
// // Byte 1 gibt die Laenge der Zeichens an
//   const byte degC[8] = {  0b00000111,
//                           0b00000010, 
//                           0b00000101, 
//                           0b00000010, 
//                           0b00111000, 
//                           0b01000100, 
//                           0b01000100,
//                           0b01000100  
// };

// // ========================================================================================================== 

// // Hier wird die Aktuelle position im Menue gespeichert
// byte Anzeige_Counter = 1;

// // ========================================================================================================== 
// // 	Temp. Sensor
// // ========================================================================================================== 
// // Hier Wird der Bibliothek der Sensor Type uebergebn und das dieser mit einer 12 Bit ausloesung arbeitet
// // Zudem wird die Klasse LM75_Sensor angelegt für dem temp Sensor
// Generic_LM75_12Bit LM75_Sensor;

// // ========================================================================================================== 
// // 	RTC > Real-Time-Clock
// // ========================================================================================================== 
// // Hier Wird der Bibliothek der Chip Type uebergebn und die Klasse rtc angelegt
// RTC_DS3231 rtc;
// // Dieses Offset dienst dazu das Jahr des RTC auf unser Aktuelle jahr anzuheben.
// // Weil NTP das jahr immer zwei stellig zurueckt gibt
// #define Offset_years 1900
// // Hier wird ein offset fuer die Monate hinterlegt da, dass RTC inter die monate von 0 - 11 zaehlt
// #define Offset_month 1

// // ========================================================================================================== 
// // 	NTP > Network Time Protocol
// // ========================================================================================================== 
// // Gibt die Adresse des Zeit servers and in diesem fall eine Sammlung von verschiedenen
// const char* ntpServer = "pool.ntp.org";
// // Hier wird der Zeit zohnen versatz angegeben. In diesem fall 3600 sek. für Berlin
// #define UCT_Offset_sec 3600
// // Hier kann man eine offset fuer den Sonnen Aufgang angeben. Da wir diese nicht nutzen ist der Wert 0
// #define Sonnenaufgang_Offset_sec 0

// // ========================================================================================================== 
// // 	Button
// // ========================================================================================================== 
// // Hier werden die Taster Hinterlegt
// const byte Taster_Pins[6] = {13, 12, 25, 33, 32, 17};

// // ========================================================================================================== 
// // 	Buzzer
// // ========================================================================================================== 
// // Pin fuer den Buzzer
// #define Buzzer_Pin 26

// // ========================================================================================================== 
// // 	Bewegungsmelder
// // ========================================================================================================== 
// // Pin fuer den Bewegungssensor
// #define Bewegungssensor_Pin 15
// // Gibt die zeit an die der Bewegungsmelder Aktive ist
// #define Bewegung_aktive_time 10000
// // Zwischen speicher fuer die millis seit start des Sensors
// unsigned long int PrevMillis_Bewegung = 0;
// // ========================================================================================================== 
// // 	Interval Poti
// // ========================================================================================================== 

// // Pin fuer den Poti
// #define Poti_Pin 4

// // Hier wird der Bereich angegeben in dem der LDR sich bewegt
// #define Min_Potiwert 0
// #define Max_Potiwert 4095
// // Hier wird der Bereich der Matrix Helligkeit angegeben
// // Helligkeit des LDRs wird in den Matrix bereich gewandelt
// #define Min_Programm_interval 1000
// #define Max_Programm_interval 10000

// // ========================================================================================================== 
// // 	LDR > Light Dependent Resistor
// // ========================================================================================================== 
// // Pin fuer den LDR
// #define LDR_Pin 0

// // Hier wird der Bereich angegeben in dem der LDR sich bewegt
// #define Min_Helligkeitswert 1896
// #define Max_Helligkeitswert 4095
// // Hier wird der Bereich der Matrix Helligkeit angegeben
// // Helligkeit des LDRs wird in den Matrix bereich gewandelt
// #define Min_Matrix_Helligkeit 0
// #define Max_Matrix_Helligkeit 15

// // ========================================================================================================== 
// // 	Unterfunktionen
// // ========================================================================================================== 

// // ========================================================================================================== 
// // Funktion		:	setupWifi
// // ----------------------------------------------------------------------------------------------------------
// // Aufgabe		:	Erstellt ein Soft Accespoint und laede dann die html und css datei 
// //            : fü die Website zu Wifi abfrage  
// // Parameter	: void  
// // Return-Wert:	void
// // ========================================================================================================== 

// void setupWifi (void) {  
//   // Hier wird Sichergestellt das die Dateien (HTML und CSS) für das Abfragen der Netzwerk daten geladen werden können
//   if (!SPIFFS.begin(true)) {
//     //Debug mode
//     #ifdef DEBUG
//       // Gibt eine Fehlermeldung aus
//       Serial.println("An error has occurred while mounting SPIFFS");
//     #endif
//     //NOTE return vielleicht nicht nötig
//     return;
//   }
//   // Setzt den modus auf Acces Point so das ein Acces Point erstellt wird, zu dem man sich verbinden kann
//   WiFi.mode(WIFI_AP);
//   // Setzt denn Namen des Accespoints. Indiesem fall wird auch kein Passwort uebergebn, damit man sich einfach darauf verbinden kann
//   WiFi.softAP(ssid0);
//   // Wenn der debug Modus Aktive ist wird die IP Adresse im Serielen Monitor ausgegeben
//   #ifdef DEBUG
//     Serial.print("AP IP address: ");
//     Serial.println(WiFi.softAPIP());
//   #endif

//   // Hier wird auf das Starten des Servers Reagiert wenn ein / übermittelt wird 
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//     // Die index.html wird geladen und als text definiert. Entsprechend wird die Website ausgebaut
//     request->send(SPIFFS, "/index.html", "text/html");
//   });
//   // Hier wird auf das aufruffen der CSS datei durch den HTML codes
//   server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
//     // Die style.css wird geladen und als text definiert. Entsprechend wird die Website ausgebaut
//     request->send(SPIFFS, "/style.css", "text/css");
//   });
//   // Der Server Startet und wird Somit Freigegeben. 
//   server.begin();
// }

// // ========================================================================================================== 
// // Funktion		:	SoftAccesPoint_abfrage
// // ----------------------------------------------------------------------------------------------------------
// // Aufgabe		:	Extrahiert die ssid und Passwort von der localen Website
// // Parameter	:	void
// // Return-Wert:	void
// // ========================================================================================================== 

// void SoftAccesPoint_abfrage (void) {
  
//   // So bald man auf der Website auf Senden Druckt wird der Adresszeile ein / hinzugefuegt  
//   server.on("/", HTTP_POST, [](AsyncWebServerRequest *request){
//     // Hier wird die ssid von der Webiste als String an die Variable uebergeben
//     ssid = request->getParam("ssid", true)->value();
//     // Hier wird das passwort von der Webiste als String an die Variable uebergeben
//     password = request->getParam("password", true)->value();
//     // Debug mode
//     // Gibt ssid und Passwort and den Derielen monitor aus. Sofern Aktive
//     #ifdef DEBUG
//       Serial.println("SSID: " + ssid);
//       Serial.println("Password: " + password);
//     #endif

//     // Weiterleitung zur Hauptseite
//     request->redirect("/");
//   });
// }

// // ========================================================================================================== 
// // Funktion		:	LDR
// // ----------------------------------------------------------------------------------------------------------
// // Aufgabe		:	Erfast mit dem LDR die Helligkeit des Raumes und gibt diese in 12 Bits (0 - 4095) wieder aus. 
// //            : Dann werden diese auf 4 bits (0 - 16) reduziert.
// // Parameter	:	void  
// // Return-Wert:	byte 0 - 16 Helligkeit des Raumes
// // ========================================================================================================== 

// byte LDR (void) {
//   // Speichert den bit wert des LDRs
//   int Helligkeitswert = analogRead(LDR_Pin);
//   // Wandel den LDR Helligkeitsbereich in den Matrix Heligkeit und Speichert in der Variablen zwischen
//   Helligkeitswert = map(Helligkeitswert, Min_Helligkeitswert, Max_Helligkeitswert, Min_Matrix_Helligkeit , Max_Matrix_Helligkeit);
//   // Debug modus
//   #ifdef DEBUG
//     Serial.print("LDR Bit Wert: ");  
//     Serial.println(Helligkeitswert);
//   #endif
//   // Gibt den Helligkeitswert fuer die Matrix zurueck
//   return Helligkeitswert;
// }

// // ========================================================================================================== 
// // Funktion		:	Bewegungssensor_time
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		:	Wenn der Sensor aktive dann gibt dieser fuer eine bestimmte zeit ein true zurueck
// //            : anderen falls ein false
// // Parameter	:	void
// // Return-Wert:	bool Sensor Status
// // ========================================================================================================== 


// bool Bewegungssensor_time (void) {
//   // Legt den bool fuer den status speicher an
//   bool aktive = false;
//   // Wenn der Sensor ein true zurueck gibt, dann wird der Status true = aktive
//   if (true == digitalRead(Bewegungssensor_Pin)) {
//     // Speichert die Programm zeit der Aktivierung
//     PrevMillis_Bewegung = millis();
//     // Setzt aktive true
//     aktive = true;
//   } 

//   // Wenn der sensor aktive ist, die zeit noch nicht ueberschritten ist und der Sensor generel aktive ist, dann
//   // wird eine true zurueck gegeben
//   if (true == aktive || millis() - PrevMillis_Bewegung <= Bewegung_aktive_time || true == Bewegungssensor_aktiv) {
//     // Debugging
//     #ifdef DEBUG
//       Serial.print("Bewegunngssensor: ");  
//       Serial.println("true");
//     #endif
//     // Gibt true zrueck
//     return true;
//   }
  
//   // Wenn das nicht zutrift dann wir das display deaktiviert und ein false zurueck gegeben
//   else {
//     // loescht display
//     Matrix.displayClear();
//     // Debugging
//     #ifdef DEBUG
//       Serial.print("Bewegunngssensor: ");  
//       Serial.println("false");
//     #endif
//     // Gibt false zurueck
//     return false;  
//   }
// }

// // ========================================================================================================== 
// // Funktion		:	Program_interval_time
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		:	ermöglichst das einstellen des Programm intervales
// // Parameter	:	void
// // Return-Wert:	int = programm interval zeit
// // ========================================================================================================== 

// //NOTE maybe byte
// int Program_interval_time (void) {
//   // Speichert den bit wert des Potis
//   int Poti_value = analogRead(Poti_Pin);
//   // Wandel den Poti wert 0 - 4095 in die Programm interval zeit
//   Poti_value = map(Poti_value, Min_Potiwert, Max_Potiwert, Min_Programm_interval , Max_Programm_interval);
//   // Debugging
//   #ifdef DEBUG
//     Serial.print("Zyklus ");  
//     Serial.println(Poti_value);
//   #endif
//   // Gibt den Interval wert fuer das Programm zurueck
//   return Poti_value;
// }

// // ========================================================================================================== 
// // Funktion		:	ssid_pass_erhalten
// // ----------------------------------------------------------------------------------------------------------
// // Aufgabe		:	Kombiniert setupWifi und SoftAccesPoint_abfrage. Loopt solange bis die breaktime erreicht 
// //            : oder Netzwerk zugangsdaten erhalten wurde
// // Parameter	:	byte breaktime = nach der zeit wir der Verbindungs versuch abgebrochen
// // Return-Wert:	bool status = ist Wifi Verbunden
// // ========================================================================================================== 

// bool ssid_pass_erhalten (byte breaktimer) {
//   // Speichert den Status der Verbindung
//   bool status = false;
//   // Gibt die Delay zeit an
//   #define delay_zeit 1000
//   // Ruft das Wifi Setup auf
//   setupWifi();

//   //FIXME WIFI ersetzen
//   // Setzt die Helligkeit der Matrix
//   Matrix.setIntensity(Init_Matrix_Helligkeit);
//   // Setzt den angezeigten texst mittig
//   Matrix.setTextAlignment(PA_CENTER);
//   // Gibt den text in den "" auf dem Display aus
//   Matrix.print("Wifi?");
//   // Solange Passwort gleich old_Passwort ist und Millis nicht Groesser als die Voreingestellte zeit ist, wird die While schleife widerholt
//   while ((password == old_password) && (millis() <= (breaktimer * Faktor_Millis_zu_Min))) {
//     // Setzt old_Passwort gleich passwort.
//     // Somit kann erkannt werden wenn ob ein Passwort eingegeben wurde und somit die While schleife unterbrochen werden
//     old_password = password;
//     // Aufruffen der unterfunktion sodas ssid und passwort abgefragt werden kann
//     SoftAccesPoint_abfrage();
//     // Wartet bis die Eingestellte seit abgelaufen ist.
//     delay(delay_zeit);
//     // Debug modus
//     #ifdef DEBUG
//       // Gibt an das keine Daten erhalten wurden und die aktuelle system zeit in millis
//       Serial.println("No ssid or Passwort");
//       Serial.print("Sys Millis: ");
//       Serial.println(millis());
//     #endif
//   }
//   // Deaktiviert den Accespoint
//   WiFi.softAPdisconnect(true);
//   // Schaltet Wifi aus, da es sonst zu Problemen mit dem Analog Digital Wandlern kommen kann
//   WiFi.disconnect(true);
//   // Wenn millis kleiner als die Angegebene zeit, dann wird der Verbindungsstatus auf true gesetzt anderen falls bleibt er false
//   if (millis() <= (breaktimer * Faktor_Millis_zu_Min)) status = true;
//   // Gibt den Status zurueck
//   return status;
// }

// // ========================================================================================================== 
// // Funktion		:	Aktuallisiere_Date_Time
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		:	Verbindung zu dem internet mit den Empfangenen Netzwerk daten.
// // Parameter	:	bool Verbunden = ob Netzwerk daten uebergeben wurden. 
// //            : byte breaktime = nach der zeit wir der Verbindungs versuch abgebrochen
// // Return-Wert:	void
// // ========================================================================================================== 

// void Aktuallisiere_Date_Time (bool Verbunden, byte breaktimer) {
//   // Wenn eine Verbindung zu einem Netzwerk erfolgt ist wird der nachfolgende Programm code ausgefuert
//   if (Verbunden) {
//     //FIXME woher und const entfernen?
//     // Mit den uebergebenen werten wird eine neue Wifi verbindung zu dem Netzwerk aufgebaut. Hier bei ist zubeachten das ssid und passwort als String gespeichert sind, 
//     // dafuer haben wir davor den type zu const char*geaendert und mit .c_str der funktion übermittel das es sich um einen String handelt
//     // FIXME wofuer sternchen
//     WiFi.begin(((const char*)ssid.c_str()), ((const char*)password.c_str()));
//     // Debug modus
//     // Gibt ssid und Passwort am serielen Monitor aus
//     #ifdef DEBUG
//       Serial.println((const char*)ssid.c_str());
//       Serial.println((const char*)password.c_str());
//     #endif
//     // Waehren die verbundung aufgabut wird, wird in dieser schleife gewartet
//     while (WiFi.status() != WL_CONNECTED) {
//       // Gibt die delay zeit an
//       #define delay_timer 500
//       // delay damit der druchlauf der schleife gebremmst wird
//       delay(delay_timer);
//       // Debug modus
//       #ifdef DEBUG  
//         Serial.print(".");
//       #endif
//       // wenn keine Verbindung nach der voreingestellten zeit hergestellt werden konnte. Wird zu der ENDE marke gesprungen
//       if ((millis() > (breaktimer * Faktor_Millis_zu_Min))) goto ENDE;
//     }
//     // Debug modus
//     #ifdef DEBUG
//       // Gibt eine bestaetigung der Verbindung aus
//       Serial.println(" CONNECTED");     
//     #endif 

//     // Hier wird nun die zeitzone, Sonneaufgangs offset und der NTP server an die RTC bibliothek ubergeben
//     configTime(UCT_Offset_sec, Sonnenaufgang_Offset_sec, ntpServer);
//     //FIXME was?
//     // Hold sich von NTP Server die Aktuelle uhrzeit und datum
//     struct tm timeinfo;
//     // Wenn keine daten erhatlen werden koennen dann wird eine Fehler meldung ausgegeben
//     if (!getLocalTime(&timeinfo)) {
//       // Debug modus
//       // Gibt eine Fehler meldung aus
//       #ifdef DEBUG
//         Serial.println("Failed to obtain time");
//       #endif
//       // Springe zu der ENDE marke
//       goto ENDE;
//     }
//     // Hier werden nun die erhaltene daten an das RTC uebergeben.
//     rtc.adjust(DateTime(
//       (timeinfo.tm_year + Offset_years), 
//       (timeinfo.tm_mon + Offset_month), 
//       timeinfo.tm_mday, 
//       timeinfo.tm_hour, 
//       timeinfo.tm_min, 
//       timeinfo.tm_sec
//     ));
//   }
//   // ENDE marke als Sprung position
//   ENDE:
//   // Loest die Wifi verbindung damit keine Fehler mit dem ADCs des 2 Kanales auftreten
//   WiFi.disconnect(true);
// }

// // ========================================================================================================== 
// // Funktion		:	Alarm_Ausloesen
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		:	Vergleich die Alarm zeit mit der Aktuellen uhr zeit wenn diese gleich sind wird ein true 
// //            : zurück gegeben
// // Parameter	:	void
// // Return-Wert:	bool wenn Alarm = uhrzeit ist == true
// // ========================================================================================================== 

// bool Alarm_Ausloesen (void) {
//   // speichert die minuten als vergleichswert
//   byte m = rtc.now().minute();
//   // speichert die Stunden als vergleichswert
//   byte h = rtc.now().hour();

//   // vergleicht stunden mit stunden und minuten mit minuten, sowie ob der alarm aktive ist
//   if (h == Counter_Stunden && m == Counter_Minute && true == Alarm_State) {
//     // Gibt true zurueck
//     return true;
//   }
//   else {
//     // Gibt false zurueck
//     return false;
//   }
// }

// // ========================================================================================================== 
// // 	ISR
// // ========================================================================================================== 

// // Damit die Interrupts moeglichst ohne grosse verzoegerungen ausgefuehrt werden koennen habe ich IRAM_ATTR dem Funktions kopf hinzugefuegt.
// // Dieses Attribut sorgt dafuer das code fuer die Interrupts im Ram gespeichert werden, da Ram deutlich schneller ist als der Flash speicher.

// // ========================================================================================================== 
// // Funktion		:	ISR_Clock_FAL
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		:	Bei einem Kurzen druck wird die Mainschleife aufgerufen und bei einem Langen druck gelangt
// //            : man ins Untermenue der Uhrzeit
// // ========================================================================================================== 

// void IRAM_ATTR ISR_Clock_FAL (void) {
//   // Muss deklariert werden da sonst ein wechsel von Fal zu Ris nicht mehr möglich ist
//   void IRAM_ATTR ISR_Clock_RIS (void);

//   // Wenn man den taster kuerzer als die zeit fuer einen langen tasten drueckt dann wird clock augerufen
//   if (millis() - Button_Pressed_time < long_Button_Presse) {
//     // Setzt den Anzeigen counter auf 1 (ruft clock auf) 
//     Anzeige_Counter = 1;
//     // Gibt an das man sich nicht im unter menue befindet
//     Maincycle = true;
//     // Debugging
//     #ifdef DEBUG
//       Serial.println("Clock");
//       Serial.println(Anzeige_Counter);
//     #endif
//     // Setzt prevMillis gleich millis 
//     prevMillis = millis();
//   }

//   // Wenn die differenz groesser als die soll zeit fuer einen langen tasten druck ist 
//   if (millis() - Button_Pressed_time >= long_Button_Presse) {
//     // Setzt den Anzeigen counter auf 10 (untermenue)
//     Anzeige_Counter = 10; 
//     // Debugging
//     #ifdef DEBUG
//       Serial.println("Clock long");
//       Serial.println(Anzeige_Counter);
//     #endif
//   } 
//   // aendert den zugewiesenen interrupt auf eine andere unterfunktion und reagiert auf die Steigene flanke
//   attachInterrupt(Taster_Pins[0], ISR_Clock_RIS, RISING); 
// }

// // ========================================================================================================== 
// // Funktion		:	ISR_Clock_RIS
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		: Wenn der taster gedruckt wird dann wird die Aktuelleprogramm zeit in millis abgespeichert	
// //            : und der zu gewiesene Interrupt geaendert sowie auf die Fallende flanke reagiert
// // ========================================================================================================== 

// void IRAM_ATTR ISR_Clock_RIS (void) {
//   // speichert die zeit als der taster gedrueckt wurde
//   Button_Pressed_time = millis();

//   // Debugging
//   // zum testen ob das entprellen klappt
//   #ifdef DEBUG
//     Serial.print("milli: "); 
//     Serial.println(millis()); 
//     Serial.print("button =: ");
//     Serial.println(Button_Pressed_time);
//     Serial.print("diff: "); 
//     Serial.println(millis() - Button_Pressed_time); 
//   #endif

//   // Sprungmarke
//   CLOCK:
//   // Wenn der taster laenger als die Entprell zeit gedrueckt wurde, dann wird die zet gespeichert und
//   // der interrupt auf eine fallende flanke geaendert
//   if (millis() - Button_Pressed_time >= entprellen) {

//   // Speichert die aktuelle Programm zeit ab
//   Button_Pressed_time = millis();
//   // aendert den zugewiesenen interrupt auf eine andere unterfunktion und reagiert auf die fallende flanke
//   attachInterrupt(Taster_Pins[0], ISR_Clock_FAL, FALLING);
//   // Serial.println("Anzeige1: ");  
//   }

//   // Wenn die zeit noch nicht erreicht ist, dann wird zurueck an die Sprungmalre gesprungen
//   else {
//     // Springt an die Sprungmarke
//     goto CLOCK;
//   }
// }

// // ========================================================================================================== 
// // Funktion		:	ISR_Date_FAL
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		:	Bei einem Kurzen druck wird die Mainschleife aufgerufen und bei einem Langen druck gelangt
// //            : man ins Untermenue des Datums
// // ========================================================================================================== 

// void IRAM_ATTR ISR_Date_FAL (void) {
//   // Muss deklariert werden da sonst ein wechsel von Fal zu Ris nicht mehr möglich ist
//   void IRAM_ATTR ISR_Date_RIS (void);
//   // Zum entprellen der Taster muss ein Kurzer druck 20ms gedrueckt werden
//   if (millis() - Button_Pressed_time < long_Button_Presse)  {
//     // Setzt den Anzeigen counter auf 2
//     Anzeige_Counter = 2;
//     // Setzt prevMillis gleich millis 
//     prevMillis = millis();
//   }
//   // Wenn die differenz groesser als die soll zeit fuer einen langen tasten druck ist  
//   if (millis() - Button_Pressed_time >= long_Button_Presse) {
//     // Wenn die differenz kleiner als die soll zeit fuer einen langen tasten druck ist
//   } 
//   // aendert den zugewiesenen interrupt auf eine andere unterfunktion und reagiert auf die Steigene flanke
//   attachInterrupt(Taster_Pins[1], ISR_Date_RIS, RISING); 
// }

// // ========================================================================================================== 
// // Funktion		:	ISR_Date_RIS
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		: Wenn der taster gedruckt wird dann wird die Aktuelleprogramm zeit in millis abgespeichert	
// //            : und der zu gewiesene Interrupt geaendert sowie auf die Fallende flanke reagiert
// // ========================================================================================================== 

// void IRAM_ATTR ISR_Date_RIS (void) {
//   // speichert die zeit als der taster gedrueckt wurde
//   Button_Pressed_time = millis();
//   // Sprungmarke
//   DATE:
//   // Wenn der taster laenger als die Entprell zeit gedrueckt wurde, dann wird die zet gespeichert und
//   // der interrupt auf eine fallende flanke geaendert
//   if (millis() - Button_Pressed_time >= entprellen) {
//   // Speichert die aktuelle Programm zeit ab
//   Button_Pressed_time = millis();
//   // aendert den zugewiesenen interrupt auf eine andere unterfunktion und reagiert auf die fallende flanke
//   attachInterrupt(Taster_Pins[1], ISR_Date_FAL, FALLING);
//   }

//   // Wenn die zeit noch nicht erreicht ist, dann wird zurueck an die Sprungmalre gesprungen
//   else {
//     // Springt an die Sprungmarke
//     goto DATE;
//   }
// }

// // ========================================================================================================== 
// // Funktion		:	ISR_Temp_FAL
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		:	Bei einem Kurzen druck wird die Mainschleife aufgerufen und bei einem Langen druck gelangt
// //            : man ins Untermenue der Temperatur
// // ========================================================================================================== 

// void IRAM_ATTR ISR_Temp_FAL (void) {
//   void IRAM_ATTR ISR_Temp_RIS (void);
//   // Zum entprellen der Taster muss ein Kurzer druck 20ms gedrueckt werden
//   if (millis() - Button_Pressed_time < long_Button_Presse) {
//     // Setzt den Anzeigen counter auf 4
//     Anzeige_Counter = 4;
//     // Setzt prevMillis gleich millis 
//     prevMillis = millis();
//   }
//   // Wenn die differenz groesser als die soll zeit fuer einen langen tasten druck ist 
//   if (millis() - Button_Pressed_time >= long_Button_Presse) {
//     // Wenn die differenz kleiner als die soll zeit fuer einen langen tasten druck ist
//   } 
//   // aendert den zugewiesenen interrupt auf eine andere unterfunktion und reagiert auf die Steigene flanke
//   attachInterrupt(Taster_Pins[2], ISR_Temp_RIS, RISING);
// }

// // ========================================================================================================== 
// // Funktion		:	ISR_Temp_RIS
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		: Wenn der taster gedruckt wird dann wird die Aktuelleprogramm zeit in millis abgespeichert	
// //            : und der zu gewiesene Interrupt geaendert sowie auf die Fallende flanke reagiert
// // ========================================================================================================== 

// void IRAM_ATTR ISR_Temp_RIS (void) {
//   // speichert die zeit als der taster gedrueckt wurde
//   Button_Pressed_time = millis();
//   // Sprungmarke
//   TEMP:
//   // Wenn der taster laenger als die Entprell zeit gedrueckt wurde, dann wird die zet gespeichert und
//   // der interrupt auf eine fallende flanke geaendert
//   if (millis() - Button_Pressed_time >= entprellen) {
//   // Speichert die aktuelle Programm zeit ab
//   Button_Pressed_time = millis();
//   // aendert den zugewiesenen interrupt auf eine andere unterfunktion und reagiert auf die fallende flanke
//   attachInterrupt(Taster_Pins[2], ISR_Temp_FAL, FALLING);
//   }

//   // Wenn die zeit noch nicht erreicht ist, dann wird zurueck an die Sprungmalre gesprungen
//   else {
//     // Springt an die Sprungmarke
//     goto TEMP;
//   }
// }

// // ========================================================================================================== 
// // Funktion		:	ISR_Setup_FAL
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		:	Erkennt ob die Setup Taste kurz oder Lang gedrueckt wurden
// //            : 
// // ========================================================================================================== 

// void IRAM_ATTR ISR_Setup_FAL (void) {
//   void IRAM_ATTR ISR_Setup_RIS (void);
//   // Zum entprellen der Taster muss ein Kurzer druck 20ms gedrueckt werden
//   if (millis() - Button_Pressed_time < long_Button_Presse) {
//     short_Setup_Press = true;
//     long_Setup_Press = false;
//     #ifdef DEBUG
//       Serial.println("Setup short");
//     #endif
//   } 
//   // Wenn die differenz groesser als die soll zeit fuer einen langen tasten druck ist dann langer Tatsen druck
//   if (millis() - Button_Pressed_time >= long_Button_Presse) {
//     long_Setup_Press = true;
//     short_Setup_Press = false;
//     #ifdef DEBUG
//       Serial.println("Setup long");
//     #endif
//     // Wenn die differenz kleiner als die soll zeit fuer einen langen tasten druck ist dann Kurzer tasten druck
//   }  
//   // aendert den zugewiesenen interrupt auf eine andere unterfunktion und reagiert auf die Steigene flanke
//   attachInterrupt(Taster_Pins[3], ISR_Setup_RIS, RISING);
// }

// // ========================================================================================================== 
// // Funktion		:	ISR_Setup_RIS
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		: Wenn der taster gedruckt wird dann wird die Aktuelleprogramm zeit in millis abgespeichert	
// //            : und der zu gewiesene Interrupt geaendert sowie auf die Fallende flanke reagiert
// // ========================================================================================================== 

// void IRAM_ATTR ISR_Setup_RIS (void) {
//   // speichert die zeit als der taster gedrueckt wurde
//   Button_Pressed_time = millis();
//   // Sprungmarke
//   SETUP:
//   // Wenn der taster laenger als die Entprell zeit gedrueckt wurde, dann wird die zet gespeichert und
//   // der interrupt auf eine fallende flanke geaendert
//   if (millis() - Button_Pressed_time >= entprellen) {
//   // Speichert die aktuelle Programm zeit ab
//   Button_Pressed_time = millis();
//   // aendert den zugewiesenen interrupt auf eine andere unterfunktion und reagiert auf die fallende flanke
//   attachInterrupt(Taster_Pins[3], ISR_Setup_FAL, FALLING);
//   }

//   // Wenn die zeit noch nicht erreicht ist, dann wird zurueck an die Sprungmalre gesprungen
//   else {
//     // Springt an die Sprungmarke
//     goto SETUP;
//   }
// }

// // ========================================================================================================== 
// // Funktion		:	ISR_Plus_RIS
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		:	Das hoch und runterzaehlen von werten bzw. im menue auf und ab
// //
// // ========================================================================================================== 

// void IRAM_ATTR ISR_Plus_RIS (void) {
//   // speichert die zeit als der taster gedrueckt wurde
//   Button_Pressed_time = millis();
//   // Sprungmarke
//   PLUS:
//   // Wenn der taster laenger als die Entprell zeit gedrueckt wurde, dann wird short_Plus_Press true und 
//   // entsprechend short_Minus_Press false zum verrigeln
//   if (millis() - Button_Pressed_time >= entprellen) {
//     // Speichert wenn plus gedrueckt wurde
//     short_Plus_Press = true;
//     // Setzt minus zurueck zum gegenseitigen verrigeln
//     short_Minus_Press = false;
//     // Debugging
//     #ifdef DEBUG
//       Serial.println("Plus short");
//     #endif
//   }

//   // Wenn die zeit noch nicht erreicht ist, dann wird zurueck an die Sprungmalre gesprungen
//   else {
//     // Springt an die Sprungmarke
//     goto PLUS;
//   }
// }

// // ========================================================================================================== 
// // Funktion		:	ISR_Minus_RIS
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		:	Das hoch und runterzaehlen von werten bzw. im menue auf und ab
// //
// // ========================================================================================================== 

// void IRAM_ATTR ISR_Minus_RIS (void) {
//   // speichert die zeit als der taster gedrueckt wurde
//   Button_Pressed_time = millis();
//   // Sprungmarke
//   MINUS:
//   // Wenn der taster laenger als die Entprell zeit gedrueckt wurde, dann wird short_Plus_Press false und 
//   // entsprechend short_Minus_Press true zum verrigeln
//   if (millis() - Button_Pressed_time >= entprellen) {
//     // Speichert wenn minus gedrueckt wurde
//     short_Minus_Press = true;
//     // Setzt plus zurueck zum gegenseitigen verrigeln
//     short_Plus_Press = false;
//     // Debugging
//     #ifdef DEBUG
//       Serial.println("Minus short");
//     #endif
//   }
  
//   // Wenn die zeit noch nicht erreicht ist, dann wird zurueck an die Sprungmalre gesprungen
//   else {
//     // Springt an die Sprungmarke
//     goto MINUS;
//   }
// }


// // ========================================================================================================== 
// // 	Init
// // ========================================================================================================== 

// // ========================================================================================================== 
// // Funktion		:	Init_Button
// // ----------------------------------------------------------------------------------------------------------
// // Aufgabe		:	Initalisiert die Taster
// // Parameter	:	void
// // Return-Wert:	void
// // ========================================================================================================== 

// void Init_Button (void) {
//   // Diese schleife zaehlt von 0 bis 5 
//   for(byte i = 0; i < 6; i++) {
//     // Der wert i wird an die Array abfrage uebergeben, 
//     //so das nach und nach jeder Taster als input gesetzt wird.
//     pinMode(Taster_Pins[i], INPUT_PULLDOWN);
//   }
// }

// // ========================================================================================================== 
// // Funktion		:	Taster_Attach_Interrupte
// // ----------------------------------------------------------------------------------------------------------
// // Aufgabe		:	Weißt den tastern eine Interrupt funktion zu
// // Parameter	:	void  
// // Return-Wert:	void
// // ========================================================================================================== 

// void Taster_Attach_Interrupte (void) {
//   // mit dieser funktion wird jedem der taster eine interrupt angehaengt,
//   // sowie wie das dazu gehoerige unterprogramm 
//   attachInterrupt(Taster_Pins[0], ISR_Clock_RIS, RISING);
//   attachInterrupt(Taster_Pins[1], ISR_Date_RIS, RISING);
//   attachInterrupt(Taster_Pins[2], ISR_Temp_RIS, RISING);
//   attachInterrupt(Taster_Pins[3], ISR_Setup_RIS, RISING);
//   attachInterrupt(Taster_Pins[4], ISR_Plus_RIS, RISING);
//   attachInterrupt(Taster_Pins[5], ISR_Minus_RIS, RISING);
// }

// // ========================================================================================================== 
// // Funktion		:	Init_Buzzer
// // ----------------------------------------------------------------------------------------------------------
// // Aufgabe		:	Legt den Buzzer Pin als output fest
// // Parameter	:	void 
// // Return-Wert:	void
// // ========================================================================================================== 

// void Init_Buzzer (void) {
//   // Setzt den Buzzer_Pin als Output
//   pinMode(Buzzer_Pin, OUTPUT);
// }

// // ========================================================================================================== 
// // Funktion		:	Init_Bewegungssensor
// // ----------------------------------------------------------------------------------------------------------
// // Aufgabe		:	Legt den Bewegungssensor Pin als input fest
// // Parameter	:	void    
// // Return-Wert:	void
// // ========================================================================================================== 

// void Init_Bewegungssensor (void) {
//   // Setzt den Bewegungssensor_Pin als input
//   pinMode(Bewegungssensor_Pin, INPUT);
// }

// // ========================================================================================================== 
// // Funktion		:	Init_Interval_Poti
// // ----------------------------------------------------------------------------------------------------------
// // Aufgabe		:	Legt den Poti Pin als input fest
// // Parameter	:	void
// // Return-Wert:	void
// // ========================================================================================================== 

// void Init_Interval_Poti (void) {
//   // Setzt den Poti_Pin als input
//   pinMode(Poti_Pin, INPUT);
// }

// // ========================================================================================================== 
// // Funktion		:	Init_RTC  
// // ----------------------------------------------------------------------------------------------------------
// // Aufgabe		:	Stellt sicher das, dass RTC verbunden ist und komunizieren kann. 
// //            : Sollt es ein Stromausfallgegeben haben und die battery ist lehr,
// //            : dann wird eine neue zeit gesetzt
// // Parameter	:	void
// // Return-Wert:	void
// // ========================================================================================================== 

// void Init_RTC (void) {
//   // Wenn das RTC nich erkannt bzw. eine verbindungs aufbeaubt derden kann, 
//   // dann wird ein fehler loop ausgefuert
//   if (!rtc.begin()) {
//     // Debugging
//     #ifdef DEBUG
//       Serial.println("RTC nicht verbunden oder erkannt");
//     #endif
//     // Wartet in dieser loop bis das Problem geloest ist
//     while (true) delay(10);
//   }

//   // Wenn die battery leer und keine netzteil angeschlossen ist,
//   // dann wird das RTC auf die eingestellte zeit gesetzt
//   if (rtc.lostPower()) {
//     // Debugging
//     #ifdef DEBUG
//       Serial.println("Battery leer RTC wird auf 2002, 9, 12, 0, 0, 0 gesetzt");
//     #endif
    
//     // Setzt das RTC auf die eingestellte datum und uhrzeit
//     rtc.adjust(DateTime(2002, 9, 12, 0, 0, 0));
//   }
// }

// // ========================================================================================================== 
// // Funktion		:	Init_LED_Matrix
// // ----------------------------------------------------------------------------------------------------------
// // Aufgabe		:	Startet die Komunikation mit der LED Matrix 
// //            : und legt ein zeichen für °C fest
// // Parameter	:	void
// // Return-Wert:	void
// // ========================================================================================================== 

// void Init_LED_Matrix (void) {
//   // Starten der Matrix sowie anlegen der klasse Matrix
//   Matrix.begin();
//   // Einbinden eines custom erstelltem zeichen
//   Matrix.addChar('$', degC);
// }

// // ========================================================================================================== 
// // Funktion		:	Init
// // ----------------------------------------------------------------------------------------------------------
// // Aufgabe		:	Fast alle Init Unterprogramme zusammen
// // Parameter	:	void
// // Return-Wert:	void
// // ========================================================================================================== 

// void Init (void) {
//   // Aktivieren des Serielen Monitors
//   #ifdef SERIAL_Monitor
//     Serial.begin(Serial_Baud);
//   #endif

//   // Aktivieren der I2C verbindung
//   #ifdef I2C
//     Wire.begin(SDA, SCL);
//   #endif
//   // Aufruffen der einzelnen Init unterprogramme
//   Init_RTC();
//   Init_LED_Matrix();
//   Init_Button();
//   Init_Buzzer();
//   Init_Bewegungssensor();
// }

// // ========================================================================================================== 
// // 	Matrix
// // ========================================================================================================== 

// // ========================================================================================================== 
// // Funktion		:	Display_Center_Text
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		:	Das anzeigen von text mittig der anzeige
// // Parameter	:	char anzeigen[8] und bool aktive
// // Return-Wert:	void
// // ========================================================================================================== 


// void Display_Center_Text (char anzeigen[8], bool aktive) {
//   // wenn aktive == true dann wird der text in anzeige mittig auf dem dem Display angezeigt
//   if (true == aktive) {
//     // Setzt die helligkeit der Matrix
//     Matrix.setIntensity(LDR());
//     Matrix.setTextAlignment(PA_CENTER);
//     Matrix.print(anzeigen);  
//   }
// }

// // ========================================================================================================== 
// // Funktion		:	Display_Center_Nummer
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		:	Das anzeigen von nummern mittig der anzeige
// // Parameter	:	int anzeigen und bool aktive
// // Return-Wert:	void
// // ========================================================================================================== 


// void Display_Center_Nummer (int anzeigen, bool aktive) {
//   // wenn aktive == true dann wird die nummer in anzeige mittig auf dem dem Display angezeigt
//   if (true == aktive) {
//     // Setzt die helligkeit der Matrix
//     Matrix.setIntensity(LDR());
//     Matrix.setTextAlignment(PA_CENTER);
//     Matrix.print(anzeigen);  
//   }
// }

// // ========================================================================================================== 
// // Funktion		:	Display_Scroll_Text
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		:	SOrgt dafuer das der Text in anzeige ueber die Matrix laueft
// // Parameter	:	char anzeigen[25] und bool aktive
// // Return-Wert:	void
// // ========================================================================================================== 


// void Display_Scroll_Text (char anzeigen[25], bool aktive) {
//   // Gibt die Geschwindigkeit an in der, der Text durchlaeuft
//   #define display_speed 50
//   // Gibt die Pause zeit zwischen den durch laeufen an
//   #define display_pause 0
//   // wenn aktive == true dann wird der inhalt von anzeige ueber den laufen
//   if (true == aktive) {
//     // Warte immer auf das ende der letzen animation
//     if (Matrix.displayAnimate()) {
//       // Loescht das display
//       Matrix.displayClear();
//       // Hier wird alles zusammen gefuert und definiert welcher effekt angewendet werden soll
//       Matrix.displayText(anzeigen, PA_CENTER, display_speed, display_pause, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
//     }
//   }
// }

// // ========================================================================================================== 
// // Funktion		:	LED_Matrix_Clock
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		:	Das Anzeigen der Uhr zeit mittig auf der Matrix
// // Parameter	:	void
// // Return-Wert:	void
// // ========================================================================================================== 

// void LED_Matrix_Clock (void) {
//   // Speichert die Aktuelle minute zwischen
//   byte m = rtc.now().minute();
//   // Speichert die Aktuelle Stunde zwischen
//   byte h = rtc.now().hour();

//   // Fuegt die beiden Variablen mit einem : in der mitte zusammen
//   // %02d steht dafuer das es eine 2 stellige zahl ist und wenn diese zahl einstellig ist 
//   // wird eine fuerende 0 beigefuegt
//   // %c gibt ein einzelnes zeichen an
//   sprintf(Buffer, "%02d%c%02d", h, ':', m);

//   // Debugging
//   #ifdef DEBUG
//     Serial.print("Uhrzeit: ");
//     Serial.println(Buffer);
//     Serial.print("sek: ");
//     Serial.println(rtc.now().second());
//   #endif

//   // Aufruffen des Anzeige unterprgrammes
//   Display_Center_Text (Buffer, Bewegungssensor_time()); 
// }

// // ========================================================================================================== 
// // Funktion		:	LED_Matrix_date
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		:	Das Anzeigen des tag und monat mittig auf der Matrix
// // Parameter	:	void
// // Return-Wert:	void
// // ========================================================================================================== 

// void LED_Matrix_date (void) {
//   // Speichert den tag zwischen
//   byte day = rtc.now().day();
//   // Speichert den monat zwischen
//   byte month = rtc.now().month();

//   // Fuegt die beiden Variablen mit einem . in der mitte zusammen
//   sprintf(Buffer, "%02d%c%02d", day, '.', month);

//   // Debugging
//   #ifdef DEBUG
//     Serial.println("Datum: ");
//     Serial.println(Buffer);
//   #endif

//   // Aufruffen des Anzeige unterprgrammes
//   Display_Center_Text (Buffer, Bewegungssensor_time());  
// }

// // ========================================================================================================== 
// // Funktion		:	LED_Matrix_year
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		:	Das Anzeigen des Jahres mittig auf der Matrix
// // Parameter	:	void
// // Return-Wert: void	
// // ========================================================================================================== 

// void LED_Matrix_year (void) {
//   // Zwischen speichern des Jahres
//   int y = rtc.now().year();

//   // Gibt an das es sich um eine 4 stellige zahl handel und jede leer stelle mit 0 gefuehlt wird
//   sprintf(Buffer, "%04d", y);

//   // Debugging
//   #ifdef DEBUG
//     Serial.println("Jahr: ");
//     Serial.println(Buffer);
//   #endif

//   // Aufruffen des Anzeige unterprgrammes
//   Display_Center_Text (Buffer, Bewegungssensor_time()); 
// }

// // ========================================================================================================== 
// // Funktion		:	LED_Matrix_temp
// // ---------------------------------------------------------------------------------------------------------- 
// // Aufgabe		:	Das Anzeigen der temperratur mittig auf der Matrix
// // Parameter	:	float temp = tmperatur wert in °C
// // Return-Wert:	void
// // ========================================================================================================== 

// void LED_Matrix_temp (float temp) {
//   // Gibt die laenge der der zahl an mit von und nach komma stelle
//   #define Zahllaenge 3
//   // gibt die nachkomma stellen an
//   #define Nachkommastelle 1
  
//   // Wandelt den float wert in eine String wert der angezeigt werden kann
//   dtostrf(temp, Zahllaenge, Nachkommastelle, Buffer);
//   // fuegt dem gewandelten float wert ein custom °C bei
//   strcat(Buffer, "$");

//   // Debugging 
//   #ifdef DEBUG
//     Serial.println("Temperatur: ");
//     Serial.println(temp);
//   #endif

//   // Aufruffen des Anzeige unterprgrammes
//   Display_Center_Text (Buffer, Bewegungssensor_time()); 
// }

// // ========================================================================================================== 
// // 	Maincode
// // ========================================================================================================== 

// void setup (void) {
//   // Aufruffen der Init unterfunktionen
//   Init();
//   // Stellt wifi verbindung her und holt sich datum und uhrzeit
//   #ifdef WIFI
//     Aktuallisiere_Date_Time(ssid_pass_erhalten(Wifi_cut_off_timer), Wifi_cut_off_timer);
//   #endif

//   // Weißt den tastern ihre interrupts zu
//   Taster_Attach_Interrupte();
// }

// void loop (void) {
//   // Speichert die Aktuellen Programmzeit um im verlauf damit die Intervalzeit Abfragen zu koennen
//   prevMillis = millis();
//   // Diese ersetzt die delayfunktion do das der uc nicht unnoetig beschaeftigt wird
//   while (millis() - prevMillis <= Program_interval_time()) {
//     // Menue switch
//     switch (Anzeige_Counter) {
//       // Anzeigen der Uhrzeit und reaktivieren des Alarmes nach dem ausloesen
//       case 1:
//         LED_Matrix_Clock();
//         if (rtc.now().minute() > Counter_Minute) {
//           Alarm_State = true;
//         }
//       break;
//         // Untermenue Alarm
//         case 10:
//           // Speichern das, dass programm in untermenue ist
//           Maincycle = false;
//           // Abfrage um den Alarm an und aus zuschalten
//           if (true == long_Setup_Press) {
//             Alarm_State = !Alarm_State;
//             long_Setup_Press = false;
//             Matrix.displayReset();
//             Matrix.displayClear();
//           } 
//           // Mit der Plus taste weiter im untermenue
//           if (true == short_Plus_Press) {
//             Anzeige_Counter = 20; 
//             Alarm_Counter = 0;
//             short_Plus_Press = false;
//             Matrix.displayReset();
//             Matrix.displayClear();
//           }
//           // Mit der Minus taste zurueck im untermenue
//           if (true == short_Minus_Press) {
//             Anzeige_Counter = 30; 
//             Alarm_Counter = 0;
//             short_Minus_Press = false;
//             Matrix.displayReset();
//             Matrix.displayClear();
//           }
//           // aender der Anzeige wenn Alarm an oder aus ist
//           if (false == Alarm_State) {
//             // Wandeln fuer ausgabe
//             sprintf(Buffer, "Alarm aus!");
            
//             // Debugging
//             #ifdef DEBUG
//               Serial.println("Alarm aus");            
//             #endif
//             // Ausgegben an das anzeige unterprogramm
//             Display_Scroll_Text(Buffer, true);        
//           } 

//           else {
//             // Wandeln fuer ausgabe
//             sprintf(Buffer, "Alarm on: %02d%c%02d", Counter_Stunden, ':', Counter_Minute);
            
//             // Debugging
//             #ifdef DEBUG
//               Serial.println(Buffer);
//             #endif
//             // Ausgegben an das anzeige unterprogramm
//             Display_Scroll_Text(Buffer, true);
            
//             // Springen um die Weck stunde anzuzeigen
//             if (true == short_Setup_Press) {
//               Anzeige_Counter = 11; 
//               short_Setup_Press = false;
//               Alarm_Counter = 0;
//               Matrix.displayReset();
//               Matrix.displayClear();
//             } 
//           }        
//         break;
//           // einstellen der Alarm Stunde
//           case 11:
//             // Anzeigen der Stunde
//             Display_Center_Nummer (Alarm_Counter, true); 

//             // Mit der Plus taste weiter im untermenue
//             if (true == short_Plus_Press) {
//               Alarm_Counter++;
//               if (true == short_Plus_Press && 25 == Alarm_Counter) Alarm_Counter = 0;
//             short_Plus_Press = false;
//             }

//             // Mit der Minus taste zurueck im untermenue
//             if (true == short_Minus_Press) {
//               Alarm_Counter--;
//               if (true == short_Minus_Press && 255 == Alarm_Counter) Alarm_Counter = 24;
//             short_Minus_Press = false;
//             }                      

//             // Uebergeben der ausgewaehlten stunde
//             Counter_Stunden = Alarm_Counter;

//             #ifdef DEBUG
//               Serial.print("Alarm_Stunde: ");
//               Serial.println(Counter_Stunden);
//             #endif

//             // Weiter zu Minuten
//             if (true == short_Setup_Press) {
//               Anzeige_Counter = 12; 
//               short_Setup_Press = false;
//               Alarm_Counter = 0;
//             } 

//           break;
//           // einstellen der Alarm Minuten
//           case 12:
//             // Anzeigen der Minuten
//             Display_Center_Nummer (Alarm_Counter, true); 

//             // Mit der Plus taste weiter im untermenue
//             if (true == short_Plus_Press) {
//               Alarm_Counter++;
//               if (true == short_Plus_Press && 61 == Alarm_Counter) Alarm_Counter = 0;
//             short_Plus_Press = false;
//             }

//             // Mit der Minus taste zurueck im untermenue
//             if (true == short_Minus_Press) {
//               Alarm_Counter--;
//               if (true == short_Minus_Press && 255 == Alarm_Counter) Alarm_Counter = 60;
//             short_Minus_Press = false;
//             }

//             // Debugging
//             #ifdef DEBUG
//               Serial.print("Alarm_Minute: ");
//               Serial.println(Counter_Minute);
//             #endif
            
//             // Uebergeben der ausgewaehlten minute
//             Counter_Minute = Alarm_Counter;

//             // mit setup zurueck zur Alarm wahl
//             if (true == short_Setup_Press) {
//               Anzeige_Counter = 10; 
//               short_Setup_Press = false;
//               Alarm_Counter = 0;
//             }

//           break;
//         // unter menue Uhrzeit einstellen
//         case 20:
//           // Speichern das, dass programm in untermenue ist
//           Maincycle = false;
          
//           // Mit der Plus taste weiter im untermenue
//           if (true == short_Plus_Press) {
//             Anzeige_Counter = 30; 
//             Alarm_Counter = 0;
//             short_Plus_Press = false;
//             Matrix.displayReset();
//             Matrix.displayClear();
//           }
//           // Mit der Minus taste zurueck im untermenue
//           if (true == short_Minus_Press) {
//             Anzeige_Counter = 20; 
//             Alarm_Counter = 0;
//             short_Minus_Press = false;
//             Matrix.displayReset();
//             Matrix.displayClear();
//           }         

//           // Wandeln fuer ausgabe
//           sprintf(Buffer, "Time: %02d%c%02d", rtc.now().hour(), ':', rtc.now().minute());
          
//           // Debugging
//           #ifdef DEBUG
//             Serial.println(Buffer);
//           #endif

//           // Ausgegben an das anzeige unterprogramm
//           Display_Scroll_Text(Buffer, true);

//           // Springen um die Time stunde anzuzeigen
//           if (true == short_Setup_Press) {
//             Anzeige_Counter = 21; 
//             short_Setup_Press = false;
//             Alarm_Counter = 0;
//           } 
                
//         break;

//           // einstellen der Stunde
//           case 21:
//             // Anzeigen der Stunde
//             Display_Center_Nummer (Alarm_Counter, true); 
            
//             // Mit der Plus taste weiter im untermenue
//             if (true == short_Plus_Press) {
//               Alarm_Counter++;
//               if (true == short_Plus_Press && 25 == Alarm_Counter) Alarm_Counter = 0;
//               short_Plus_Press = false;
//             }
//             // Mit der Minus taste zurueck im untermenue
//             if (true == short_Minus_Press) {
//               Alarm_Counter--;
//               if (true == short_Minus_Press && 255 == Alarm_Counter) Alarm_Counter = 24;
//             short_Minus_Press = false;
//             }
            
//             // Debugging
//             #ifdef DEBUG
//               Serial.print("Time_Stunde: ");
//               Serial.println(Alarm_Counter);
//             #endif

//             // Uebergeben der Stunde an das RTC
//             if (true == short_Setup_Press) {
//               Anzeige_Counter = 22; 
//               short_Setup_Press = false;
//               rtc.adjust(DateTime(rtc.now().year(), 
//                                   rtc.now().month(), 
//                                   rtc.now().day(), 
//                                   Alarm_Counter, 
//                                   rtc.now().minute(), 
//                                   rtc.now().second()
//               ));
//               // zurueck setzen der counter variable
//               Alarm_Counter = 0;
//             } 
//           break;

//           // einstellen der Minute
//           case 22:
//             // Anzeigen der Minute
//             Display_Center_Nummer (Alarm_Counter, true); 
//             // Mit der Plus taste weiter im untermenue
//             if (true == short_Plus_Press) {
//               Alarm_Counter++;
//               if (true == short_Plus_Press && 61 == Alarm_Counter) Alarm_Counter = 0;
//             short_Plus_Press = false;
//             }
//             // Mit der Minus taste zurueck im untermenue
//             if (true == short_Minus_Press) {
//               Alarm_Counter--;
//               if (true == short_Minus_Press && 255 == Alarm_Counter) Alarm_Counter = 60;
//             short_Minus_Press = false;
//             }

//             // Debugging
//             #ifdef DEBUG
//               Serial.print("Time_Minuten: ");
//               Serial.println(Alarm_Counter);
//             #endif

//             // Uebergeben der Minuten an das RTC
//             // Setzen von sekunden auf null
//             if (true == short_Setup_Press) {
//               Anzeige_Counter = 20; 
//               short_Setup_Press = false;
//               rtc.adjust(DateTime(rtc.now().year(), 
//                                   rtc.now().month(), 
//                                   rtc.now().day(), 
//                                   rtc.now().hour(), 
//                                   Alarm_Counter, 
//                                   0
//               ));
//               // zurueck setzen der counter variable
//               Alarm_Counter = 0;
//             } 
//           break;

//         // Untermenue fuer den Bewegungssensor
//         case 30:
          
//           // Anzeigen wenn Bewegungssen. aus ist
//           if (true == Bewegungssensor_aktiv) {
//             sprintf(Buffer, "Bewegungssensor aus");
            
//             // Debugging
//             #ifdef DEBUG
//               Serial.println(Buffer);
//             #endif

//             // Ausgegben an das anzeige unterprogramm
//             Display_Scroll_Text(Buffer, true);
//           }

//           // Anzeigen wenn Bewegungssen. an ist
//           else {
//             sprintf(Buffer, "Bewegungssensor an");
            
//             // Debugging
//             #ifdef DEBUG
//               Serial.println(Buffer);
//             #endif

//             // Ausgegben an das anzeige unterprogramm
//             Display_Scroll_Text(Buffer, true);
//           }
          
//           // Abfrage um den Alarm an und aus zuschalten
//           if (true == long_Setup_Press) {
//             Bewegungssensor_aktiv = !Bewegungssensor_aktiv;
//             long_Setup_Press = false;
//             Matrix.displayReset();
//             Matrix.displayClear();
//           } 

//           // Mit der Plus taste weiter im untermenue
//           if (true == short_Plus_Press) {
//             Anzeige_Counter = 10; 
//             short_Plus_Press = false;
//             Matrix.displayReset();
//             Matrix.displayClear();
//           }

//           // Mit der Minus taste zurueck im untermenue
//           if (true == short_Minus_Press) {
//             Anzeige_Counter = 20; 
//             short_Minus_Press = false;
//             Matrix.displayReset();
//             Matrix.displayClear();
//           }

//         break;

//         case 40:
//           // Weiterleifen an den anfang des menues
//           Anzeige_Counter = 10;
//           Matrix.displayReset();
//           Matrix.displayClear();
//         break;

//       case 2:
//         // Anzeigen des Datums
//         LED_Matrix_date();
//       break;

//       case 3:
//         // Anzeigen des Jahres
//         LED_Matrix_year();
//       break;

//       case 4:
//         // Anzeigen der Temperatur
//         LED_Matrix_temp(LM75_Sensor.readTemperatureC());
//       break;

//       default:
//         // wenn das menue durcgelaufen ist oder ein fehler auftrit und maincycle == true dann Anzeige_Counter = 1
//         if (Maincycle == true) {
//           Anzeige_Counter = 1;
//         } 
//         // wenn maincycle == false dann springt dieser hier in unterprogramm
//         else {
//           Anzeige_Counter = 10;
//         }
        
//       break;
//     }
//     // Dieser delay ist dafuer das das die Esp32 nicht abstuerst dadurch das, das programm sonst zu schnell und zu oft durchlaeuft
//     delay(100);
//   }

//   // Hochzaehlen des Anzeige_Counter wenn Maincycle und der Alarm aus ist
//   if (true == Maincycle && false == Alarm_Ausloesen()) Anzeige_Counter++;

//   // wenn der Alarm_Ausloesen ein true zurueck gibt, dann wird der Alarm solange gelooped bis der Alarm deaktiviert wird
//   else if (true == Alarm_Ausloesen()) {
//     while (false == short_Setup_Press) {
//       Alarm_State = false;
//       Serial.println("ALARM");
//       Display_Center_Text("ALARM", true);
//       delay(50);
//     }
//     // rueck setzen des tasters
//     short_Setup_Press = false;
//   }
//   // Degugging
//   #ifdef DEBUG
//     Serial.print("Anzeige1: ");
//     Serial.println(Anzeige_Counter);
//   #endif
// }
