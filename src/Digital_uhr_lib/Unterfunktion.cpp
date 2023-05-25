#include <Arduino.h>

#include "Configuration.h"
#include "Digital_uhr.h"

// ========================================================================================================== 
// Funktion		:	setupWifi
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Erstellt ein Soft Accespoint und laede dann die html und css datei 
//            : fü die Website zu Wifi abfrage  
// Parameter	: void  
// Return-Wert:	void
// ========================================================================================================== 

void setupWifi (void) {  
  // Hier wird Sichergestellt das die Dateien (HTML und CSS) für das Abfragen der Netzwerk daten geladen werden können
  if (!SPIFFS.begin(true)) {
    //Debug mode
    #ifdef DEBUG
      // Gibt eine Fehlermeldung aus
      Serial.println("An error has occurred while mounting SPIFFS");
    #endif
    //NOTE return vielleicht nicht nötig
    return;
  }
  // Setzt den modus auf Acces Point so das ein Acces Point erstellt wird, zu dem man sich verbinden kann
  WiFi.mode(WIFI_AP);
  // Setzt denn Namen des Accespoints. Indiesem fall wird auch kein Passwort uebergebn, damit man sich einfach darauf verbinden kann
  WiFi.softAP(ssid0);
  // Wenn der debug Modus Aktive ist wird die IP Adresse im Serielen Monitor ausgegeben
  #ifdef DEBUG
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
  #endif

  // Hier wird auf das Starten des Servers Reagiert wenn ein / übermittelt wird 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    // Die index.html wird geladen und als text definiert. Entsprechend wird die Website ausgebaut
    request->send(SPIFFS, "/index.html", "text/html");
  });
  // Hier wird auf das aufruffen der CSS datei durch den HTML codes
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    // Die style.css wird geladen und als text definiert. Entsprechend wird die Website ausgebaut
    request->send(SPIFFS, "/style.css", "text/css");
  });
  // Der Server Startet und wird Somit Freigegeben. 
  server.begin();
}

// ========================================================================================================== 
// Funktion		:	SoftAccesPoint_abfrage
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Extrahiert die ssid und Passwort von der localen Website
// Parameter	:	void
// Return-Wert:	void
// ========================================================================================================== 

void SoftAccesPoint_abfrage (void) {
  
  // So bald man auf der Website auf Senden Druckt wird der Adresszeile ein / hinzugefuegt  
  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request){
    // Hier wird die ssid von der Webiste als String an die Variable uebergeben
    ssid = request->getParam("ssid", true)->value();
    // Hier wird das passwort von der Webiste als String an die Variable uebergeben
    password = request->getParam("password", true)->value();
    // Debug mode
    // Gibt ssid und Passwort and den Derielen monitor aus. Sofern Aktive
    #ifdef DEBUG
      Serial.println("SSID: " + ssid);
      Serial.println("Password: " + password);
    #endif

    // Weiterleitung zur Hauptseite
    request->redirect("/");
  });
}

// ========================================================================================================== 
// Funktion		:	Program_interval_time
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	ermöglichst das einstellen des Programm intervales
// Parameter	:	void
// Return-Wert:	int = programm interval zeit
// ========================================================================================================== 

int Program_interval_time (void) {
  // Speichert den bit wert des Potis
  int Poti_value = analogRead(Poti_Pin);
  // Wandel den Poti wert 0 - 4095 in die Programm interval zeit
  Poti_value = map(Poti_value, Min_Potiwert, Max_Potiwert, Min_Programm_interval , Max_Programm_interval);
  // Debugging
  #ifdef DEBUG
    Serial.print("Zyklus ");  
    Serial.println(Poti_value);
  #endif
  // Gibt den Interval wert fuer das Programm zurueck
  return Poti_value;
}

// ========================================================================================================== 
// Funktion		:	ssid_pass_erhalten
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Kombiniert setupWifi und SoftAccesPoint_abfrage. Loopt solange bis die breaktime erreicht 
//            : oder Netzwerk zugangsdaten erhalten wurde
// Parameter	:	byte breaktime = nach der zeit wir der Verbindungs versuch abgebrochen
// Return-Wert:	bool status = ist Wifi Verbunden
// ========================================================================================================== 

bool ssid_pass_erhalten (byte breaktimer) {
  // Speichert den Status der Verbindung
  bool status = false;
  // Gibt die Delay zeit an
  #define delay_zeit 1000
  // Ruft das Wifi Setup auf
  setupWifi();

  //FIXME WIFI ersetzen
  // Setzt die Helligkeit der Matrix
  Matrix.setIntensity(Init_Matrix_Helligkeit);
  // Setzt den angezeigten texst mittig
  Matrix.setTextAlignment(PA_CENTER);
  // Gibt den text in den "" auf dem Display aus
  Matrix.print("Wifi?");
  // Solange Passwort gleich old_Passwort ist und Millis nicht Groesser als die Voreingestellte zeit ist, wird die While schleife widerholt
  while ((password == old_password) && (millis() <= (breaktimer * Faktor_Millis_zu_Min))) {
    // Setzt old_Passwort gleich passwort.
    // Somit kann erkannt werden wenn ob ein Passwort eingegeben wurde und somit die While schleife unterbrochen werden
    old_password = password;
    // Aufruffen der unterfunktion sodas ssid und passwort abgefragt werden kann
    SoftAccesPoint_abfrage();
    // Wartet bis die Eingestellte seit abgelaufen ist.
    delay(delay_zeit);
    // Debug modus
    #ifdef DEBUG
      // Gibt an das keine Daten erhalten wurden und die aktuelle system zeit in millis
      Serial.println("No ssid or Passwort");
      Serial.print("Sys Millis: ");
      Serial.println(millis());
    #endif
  }
  // Deaktiviert den Accespoint
  WiFi.softAPdisconnect(true);
  // Schaltet Wifi aus, da es sonst zu Problemen mit dem Analog Digital Wandlern kommen kann
  WiFi.disconnect(true);
  // Wenn millis kleiner als die Angegebene zeit, dann wird der Verbindungsstatus auf true gesetzt anderen falls bleibt er false
  if (millis() <= (breaktimer * Faktor_Millis_zu_Min)) status = true;
  // Gibt den Status zurueck
  return status;
}

// ========================================================================================================== 
// Funktion		:	Aktuallisiere_Date_Time
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Verbindung zu dem internet mit den Empfangenen Netzwerk daten.
// Parameter	:	bool Verbunden = ob Netzwerk daten uebergeben wurden. 
//            : byte breaktime = nach der zeit wir der Verbindungs versuch abgebrochen
// Return-Wert:	void
// ========================================================================================================== 

void Aktuallisiere_Date_Time (bool Verbunden, byte breaktimer) {
  // Wenn eine Verbindung zu einem Netzwerk erfolgt ist wird der nachfolgende Programm code ausgefuert
  if (Verbunden) {
    //FIXME woher und const entfernen?
    // Mit den uebergebenen werten wird eine neue Wifi verbindung zu dem Netzwerk aufgebaut. Hier bei ist zubeachten das ssid und passwort als String gespeichert sind, 
    // dafuer haben wir davor den type zu const char*geaendert und mit .c_str der funktion übermittel das es sich um einen String handelt
    // FIXME wofuer sternchen
    WiFi.begin(((const char*)ssid.c_str()), ((const char*)password.c_str()));
    // Debug modus
    // Gibt ssid und Passwort am serielen Monitor aus
    #ifdef DEBUG
      Serial.println((const char*)ssid.c_str());
      Serial.println((const char*)password.c_str());
    #endif
    // Waehren die verbundung aufgabut wird, wird in dieser schleife gewartet
    while (WiFi.status() != WL_CONNECTED) {
      // Gibt die delay zeit an
      #define delay_timer 500
      // delay damit der druchlauf der schleife gebremmst wird
      delay(delay_timer);
      // Debug modus
      #ifdef DEBUG  
        Serial.print(".");
      #endif
      // wenn keine Verbindung nach der voreingestellten zeit hergestellt werden konnte. Wird zu der ENDE marke gesprungen
      if ((millis() > (breaktimer * Faktor_Millis_zu_Min))) goto ENDE;
    }
    // Debug modus
    #ifdef DEBUG
      // Gibt eine bestaetigung der Verbindung aus
      Serial.println(" CONNECTED");     
    #endif 

    // Hier wird nun die zeitzone, Sonneaufgangs offset und der NTP server an die RTC bibliothek ubergeben
    configTime(UCT_Offset_sec, Sonnenaufgang_Offset_sec, ntpServer);
    //FIXME was?
    // Hold sich von NTP Server die Aktuelle uhrzeit und datum
    struct tm timeinfo;
    // Wenn keine daten erhatlen werden koennen dann wird eine Fehler meldung ausgegeben
    if (!getLocalTime(&timeinfo)) {
      // Debug modus
      // Gibt eine Fehler meldung aus
      #ifdef DEBUG
        Serial.println("Failed to obtain time");
      #endif
      // Springe zu der ENDE marke
      goto ENDE;
    }
    // Hier werden nun die erhaltene daten an das RTC uebergeben.
    rtc.adjust(DateTime(
      (timeinfo.tm_year + Offset_years), 
      (timeinfo.tm_mon + Offset_month), 
      timeinfo.tm_mday, 
      timeinfo.tm_hour, 
      timeinfo.tm_min, 
      timeinfo.tm_sec
    ));
  }
  // ENDE marke als Sprung position
  ENDE:
  // Loest die Wifi verbindung damit keine Fehler mit dem ADCs des 2 Kanales auftreten
  WiFi.disconnect(true);
}

// ========================================================================================================== 
// Funktion		:	Alarm_Ausloesen
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Vergleich die Alarm zeit mit der Aktuellen uhr zeit wenn diese gleich sind wird ein true 
//            : zurück gegeben
// Parameter	:	void
// Return-Wert:	bool wenn Alarm = uhrzeit ist == true
// ========================================================================================================== 

bool Alarm_Ausloesen (void) {
  // speichert die minuten als vergleichswert
  byte m = rtc.now().minute();
  // speichert die Stunden als vergleichswert
  byte h = rtc.now().hour();

  // vergleicht stunden mit stunden und minuten mit minuten, sowie ob der alarm aktive ist
  if (h == Counter_Stunden && m == Counter_Minute && true == Alarm_State) {
    // Gibt true zurueck
    return true;
  }
  else {
    // Gibt false zurueck
    return false;
  }
}