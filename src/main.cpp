// ========================================================================================================== 
// 	Authoren
// ========================================================================================================== 

// KIAN Tetzlaff
// JOSI Brodersen
// LARS Breiholz

/**
 * @file Digital LED Matrix Clock
 * @version 0.9V
 * @date 2022-12-15
 * @copyright Copyright (c) 2023
 */

// ========================================================================================================== 
// 	Include
// ========================================================================================================== 

#include <Arduino.h>

#include "Configuration.h"

#include "LED_Matrix_Display.h"
#include "Digital_uhr.h"



// ========================================================================================================== 
// 	Maincode
// ========================================================================================================== 



void setup (void) {
  // Aufruffen der Init unterfunktionen
  Init();
  // Stellt wifi verbindung her und holt sich datum und uhrzeit
  #ifdef WIFI
    Aktuallisiere_Date_Time(ssid_pass_erhalten(Wifi_cut_off_timer), Wifi_cut_off_timer);
  #endif

  // Weißt den tastern ihre interrupts zu
  Taster_Attach_Interrupte();
}

void loop (void) {
  // Speichert die Aktuellen Programmzeit um im verlauf damit die Intervalzeit Abfragen zu koennen
  prevMillis = millis();
  // Diese ersetzt die delayfunktion do das der uc nicht unnoetig beschaeftigt wird
  while (millis() - prevMillis <= Program_interval_time()) {
    Menue_switch();
    // Dieser delay ist dafuer das das die Esp32 nicht abstuerst dadurch das, das programm sonst zu schnell und zu oft durchlaeuft
    delay(100);
  }

  // Hochzaehlen des Anzeige_Counter wenn Maincycle und der Alarm aus ist
  if (true == Maincycle && false == Alarm_Ausloesen()) Anzeige_Counter++;

  // wenn der Alarm_Ausloesen ein true zurueck gibt, dann wird der Alarm solange gelooped bis der Alarm deaktiviert wird
  else if (true == Alarm_Ausloesen()) {
    while (false == short_Setup_Press) {
      Alarm_State = false;
      Serial.println("ALARM");
      LED_Matrix.Display_Center_Text("ALARM", true);
      delay(50);
    }
    // rueck setzen des tasters
    short_Setup_Press = false;
  }
  // Degugging
  #ifdef DEBUG
    Serial.print("Anzeige1: ");
    Serial.println(Anzeige_Counter);
  #endif
}
