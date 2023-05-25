#include <Arduino.h>

#include "LED_Matrix_Display.h"
#include "Configuration.h"

// ========================================================================================================== 
// Funktion		:	LDR
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Erfast mit dem LDR die Helligkeit des Raumes und gibt diese in 12 Bits (0 - 4095) wieder aus. 
//            : Dann werden diese auf 4 bits (0 - 16) reduziert.
// Parameter	:	void  
// Return-Wert:	byte 0 - 16 Helligkeit des Raumes
// ========================================================================================================== 

byte LED_Matrix::LDR (void) {
  // Speichert den bit wert des LDRs
  int Helligkeitswert = analogRead(LDR_Pin);
  // Wandel den LDR Helligkeitsbereich in den Matrix Heligkeit und Speichert in der Variablen zwischen
  Helligkeitswert = map(Helligkeitswert, Min_Helligkeitswert, Max_Helligkeitswert, Min_Matrix_Helligkeit , Max_Matrix_Helligkeit);
  // Debug modus
  #ifdef DEBUG
    Serial.print("LDR Bit Wert: ");  
    Serial.println(Helligkeitswert);
  #endif
  // Gibt den Helligkeitswert fuer die Matrix zurueck
  return Helligkeitswert;
}

// ========================================================================================================== 
// Funktion		:	Bewegungssensor_time
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Wenn der Sensor aktive dann gibt dieser fuer eine bestimmte zeit ein true zurueck
//            : anderen falls ein false
// Parameter	:	void
// Return-Wert:	bool Sensor Status
// ========================================================================================================== 

bool LED_Matrix::Bewegungssensor_time (void) {
  // Legt den bool fuer den status speicher an
  bool aktive = false;
  // Wenn der Sensor ein true zurueck gibt, dann wird der Status true = aktive
  if (true == digitalRead(Bewegungssensor_Pin)) {
    // Speichert die Programm zeit der Aktivierung
    PrevMillis_Bewegung = millis();
    // Setzt aktive true
    aktive = true;
  } 

  // Wenn der sensor aktive ist, die zeit noch nicht ueberschritten ist und der Sensor generel aktive ist, dann
  // wird eine true zurueck gegeben
  if (true == aktive || millis() - PrevMillis_Bewegung <= Bewegung_aktive_time || true == Bewegungssensor_aktiv) {
    // Debugging
    #ifdef DEBUG
      Serial.print("Bewegunngssensor: ");  
      Serial.println("true");
    #endif
    // Gibt true zrueck
    return true;
  }
  
  // Wenn das nicht zutrift dann wir das display deaktiviert und ein false zurueck gegeben
  else {
    // loescht display
    Matrix.displayClear();
    // Debugging
    #ifdef DEBUG
      Serial.print("Bewegunngssensor: ");  
      Serial.println("false");
    #endif
    // Gibt false zurueck
    return false;  
  }
}

// ========================================================================================================== 
// 	Matrix
// ========================================================================================================== 

// ========================================================================================================== 
// Funktion		:	Display_Center_Text
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Das anzeigen von text mittig der anzeige
// Parameter	:	char anzeigen[8] und bool aktive
// Return-Wert:	void
// ========================================================================================================== 


void LED_Matrix::Display_Center_Text (char anzeigen[8], bool aktive) {
  // wenn aktive == true dann wird der text in anzeige mittig auf dem dem Display angezeigt
  if (true == aktive) {
    // Setzt die helligkeit der Matrix
    Matrix.setIntensity(LDR());
    Matrix.setTextAlignment(PA_CENTER);
    Matrix.print(anzeigen);  
  }
}

// ========================================================================================================== 
// Funktion		:	Display_Center_Nummer
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Das anzeigen von nummern mittig der anzeige
// Parameter	:	int anzeigen und bool aktive
// Return-Wert:	void
// ========================================================================================================== 


void LED_Matrix::Display_Center_Nummer (int anzeigen, bool aktive) {
  // wenn aktive == true dann wird die nummer in anzeige mittig auf dem dem Display angezeigt
  if (true == aktive) {
    // Setzt die helligkeit der Matrix
    Matrix.setIntensity(LDR());
    Matrix.setTextAlignment(PA_CENTER);
    Matrix.print(anzeigen);  
  }
}

// ========================================================================================================== 
// Funktion		:	Display_Scroll_Text
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	SOrgt dafuer das der Text in anzeige ueber die Matrix laueft
// Parameter	:	char anzeigen[25] und bool aktive
// Return-Wert:	void
// ========================================================================================================== 


void LED_Matrix::Display_Scroll_Text (char anzeigen[25], bool aktive) {
  // Gibt die Geschwindigkeit an in der, der Text durchlaeuft
  #define display_speed 50
  // Gibt die Pause zeit zwischen den durch laeufen an
  #define display_pause 0
  // wenn aktive == true dann wird der inhalt von anzeige ueber den laufen
  if (true == aktive) {
    // Warte immer auf das ende der letzen animation
    if (Matrix.displayAnimate()) {
      // Loescht das display
      Matrix.displayClear();
      // Hier wird alles zusammen gefuert und definiert welcher effekt angewendet werden soll
      Matrix.displayText(anzeigen, PA_CENTER, display_speed, display_pause, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    }
  }
}

// ========================================================================================================== 
// Funktion		:	LED_Matrix_Clock
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Das Anzeigen der Uhr zeit mittig auf der Matrix
// Parameter	:	void
// Return-Wert:	void
// ========================================================================================================== 

void LED_Matrix::LED_Matrix_Clock (void) {
  // Speichert die Aktuelle minute zwischen
  byte m = rtc.now().minute();
  // Speichert die Aktuelle Stunde zwischen
  byte h = rtc.now().hour();

  // Fuegt die beiden Variablen mit einem : in der mitte zusammen
  // %02d steht dafuer das es eine 2 stellige zahl ist und wenn diese zahl einstellig ist 
  // wird eine fuerende 0 beigefuegt
  // %c gibt ein einzelnes zeichen an
  sprintf(Buffer, "%02d%c%02d", h, ':', m);

  // Debugging
  #ifdef DEBUG
    Serial.print("Uhrzeit: ");
    Serial.println(Buffer);
    Serial.print("sek: ");
    Serial.println(rtc.now().second());
  #endif

  // Aufruffen des Anzeige unterprgrammes
  Display_Center_Text (Buffer, Bewegungssensor_time()); 
}

// ========================================================================================================== 
// Funktion		:	LED_Matrix_date
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Das Anzeigen des tag und monat mittig auf der Matrix
// Parameter	:	void
// Return-Wert:	void
// ========================================================================================================== 

void LED_Matrix::LED_Matrix_date (void) {
  // Speichert den tag zwischen
  byte day = rtc.now().day();
  // Speichert den monat zwischen
  byte month = rtc.now().month();

  // Fuegt die beiden Variablen mit einem . in der mitte zusammen
  sprintf(Buffer, "%02d%c%02d", day, '.', month);

  // Debugging
  #ifdef DEBUG
    Serial.println("Datum: ");
    Serial.println(Buffer);
  #endif

  // Aufruffen des Anzeige unterprgrammes
  Display_Center_Text (Buffer, Bewegungssensor_time());  
}

// ========================================================================================================== 
// Funktion		:	LED_Matrix_year
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Das Anzeigen des Jahres mittig auf der Matrix
// Parameter	:	void
// Return-Wert: void	
// ========================================================================================================== 

void LED_Matrix::LED_Matrix_year (void) {
  // Zwischen speichern des Jahres
  int y = rtc.now().year();

  // Gibt an das es sich um eine 4 stellige zahl handel und jede leer stelle mit 0 gefuehlt wird
  sprintf(Buffer, "%04d", y);

  // Debugging
  #ifdef DEBUG
    Serial.println("Jahr: ");
    Serial.println(Buffer);
  #endif

  // Aufruffen des Anzeige unterprgrammes
  Display_Center_Text (Buffer, Bewegungssensor_time()); 
}

// ========================================================================================================== 
// Funktion		:	LED_Matrix_temp
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Das Anzeigen der temperratur mittig auf der Matrix
// Parameter	:	float temp = tmperatur wert in °C
// Return-Wert:	void
// ========================================================================================================== 

void LED_Matrix::LED_Matrix_temp (float temp) {
  // Gibt die laenge der der zahl an mit von und nach komma stelle
  #define Zahllaenge 3
  // gibt die nachkomma stellen an
  #define Nachkommastelle 1
  
  // Wandelt den float wert in eine String wert der angezeigt werden kann
  dtostrf(temp, Zahllaenge, Nachkommastelle, Buffer);
  // fuegt dem gewandelten float wert ein custom °C bei
  strcat(Buffer, "$");

  // Debugging 
  #ifdef DEBUG
    Serial.println("Temperatur: ");
    Serial.println(temp);
  #endif

  // Aufruffen des Anzeige unterprgrammes
  Display_Center_Text (Buffer, Bewegungssensor_time()); 
}