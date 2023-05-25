#include <Arduino.h>

#include "Configuration.h"
#include "Digital_uhr.h"

// ========================================================================================================== 
// Funktion		:	Init_Button
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Initalisiert die Taster
// Parameter	:	void
// Return-Wert:	void
// ========================================================================================================== 

void Init_Button (void) {
  // Diese schleife zaehlt von 0 bis 5 
  for(byte i = 0; i < 6; i++) {
    // Der wert i wird an die Array abfrage uebergeben, 
    //so das nach und nach jeder Taster als input gesetzt wird.
    pinMode(Taster_Pins[i], INPUT_PULLDOWN);
  }
}

// ========================================================================================================== 
// Funktion		:	Taster_Attach_Interrupte
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Weißt den tastern eine Interrupt funktion zu
// Parameter	:	void  
// Return-Wert:	void
// ========================================================================================================== 

void Taster_Attach_Interrupte (void) {
  // mit dieser funktion wird jedem der taster eine interrupt angehaengt,
  // sowie wie das dazu gehoerige unterprogramm 
  attachInterrupt(Taster_Pins[0], ISR_Clock_RIS, RISING);
  attachInterrupt(Taster_Pins[1], ISR_Date_RIS, RISING);
  attachInterrupt(Taster_Pins[2], ISR_Temp_RIS, RISING);
  attachInterrupt(Taster_Pins[3], ISR_Setup_RIS, RISING);
  attachInterrupt(Taster_Pins[4], ISR_Plus_RIS, RISING);
  attachInterrupt(Taster_Pins[5], ISR_Minus_RIS, RISING);
}

// ========================================================================================================== 
// Funktion		:	Init_Buzzer
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Legt den Buzzer Pin als output fest
// Parameter	:	void 
// Return-Wert:	void
// ========================================================================================================== 

void Init_Buzzer (void) {
  // Setzt den Buzzer_Pin als Output
  pinMode(Buzzer_Pin, OUTPUT);
}

// ========================================================================================================== 
// Funktion		:	Init_Bewegungssensor
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Legt den Bewegungssensor Pin als input fest
// Parameter	:	void    
// Return-Wert:	void
// ========================================================================================================== 

void Init_Bewegungssensor (void) {
  // Setzt den Bewegungssensor_Pin als input
  pinMode(Bewegungssensor_Pin, INPUT);
}

// ========================================================================================================== 
// Funktion		:	Init_Interval_Poti
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Legt den Poti Pin als input fest
// Parameter	:	void
// Return-Wert:	void
// ========================================================================================================== 

void Init_Interval_Poti (void) {
  // Setzt den Poti_Pin als input
  pinMode(Poti_Pin, INPUT);
}

// ========================================================================================================== 
// Funktion		:	Init_RTC  
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Stellt sicher das, dass RTC verbunden ist und komunizieren kann. 
//            : Sollt es ein Stromausfallgegeben haben und die battery ist lehr,
//            : dann wird eine neue zeit gesetzt
// Parameter	:	void
// Return-Wert:	void
// ========================================================================================================== 

void Init_RTC (void) {
  // Wenn das RTC nich erkannt bzw. eine verbindungs aufbeaubt derden kann, 
  // dann wird ein fehler loop ausgefuert
  if (!rtc.begin()) {
    // Debugging
    #ifdef DEBUG
      Serial.println("RTC nicht verbunden oder erkannt");
    #endif
    // Wartet in dieser loop bis das Problem geloest ist
    while (true) delay(10);
  }

  // Wenn die battery leer und keine netzteil angeschlossen ist,
  // dann wird das RTC auf die eingestellte zeit gesetzt
  if (rtc.lostPower()) {
    // Debugging
    #ifdef DEBUG
      Serial.println("Battery leer RTC wird auf 2002, 9, 12, 0, 0, 0 gesetzt");
    #endif
    
    // Setzt das RTC auf die eingestellte datum und uhrzeit
    rtc.adjust(DateTime(2002, 9, 12, 0, 0, 0));
  }
}

// ========================================================================================================== 
// Funktion		:	Init_LED_Matrix
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Startet die Komunikation mit der LED Matrix 
//            : und legt ein zeichen für °C fest
// Parameter	:	void
// Return-Wert:	void
// ========================================================================================================== 

void Init_LED_Matrix (void) {
  // Starten der Matrix sowie anlegen der klasse Matrix
  Matrix.begin();
  // Einbinden eines custom erstelltem zeichen
  Matrix.addChar('$', degC);
}

// ========================================================================================================== 
// Funktion		:	Init
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Fast alle Init Unterprogramme zusammen
// Parameter	:	void
// Return-Wert:	void
// ========================================================================================================== 

void Init (void) {
  // Aktivieren des Serielen Monitors
  #ifdef SERIAL_Monitor
    Serial.begin(Serial_Baud);
  #endif

  // Aktivieren der I2C verbindung
  #ifdef I2C
    Wire.begin(SDA, SCL);
  #endif
  // Aufruffen der einzelnen Init unterprogramme
  Init_RTC();
  Init_LED_Matrix();
  Init_Button();
  Init_Buzzer();
  Init_Bewegungssensor();
}