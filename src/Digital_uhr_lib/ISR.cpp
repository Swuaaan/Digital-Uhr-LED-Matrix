#include <Arduino.h>

#include "Configuration.h"
#include "Digital_uhr.h"

// Damit die Interrupts moeglichst ohne grosse verzoegerungen ausgefuehrt werden koennen habe ich IRAM_ATTR dem Funktions kopf hinzugefuegt.
// Dieses Attribut sorgt dafuer das code fuer die Interrupts im Ram gespeichert werden, da Ram deutlich schneller ist als der Flash speicher.

// ========================================================================================================== 
// Funktion		:	ISR_Clock_FAL
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Bei einem Kurzen druck wird die Mainschleife aufgerufen und bei einem Langen druck gelangt
//            : man ins Untermenue der Uhrzeit
// ========================================================================================================== 

void IRAM_ATTR ISR_Clock_FAL (void) {
  // Muss deklariert werden da sonst ein wechsel von Fal zu Ris nicht mehr möglich ist
  void IRAM_ATTR ISR_Clock_RIS (void);

  // Wenn man den taster kuerzer als die zeit fuer einen langen tasten drueckt dann wird clock augerufen
  if (millis() - Button_Pressed_time < long_Button_Presse) {
    // Setzt den Anzeigen counter auf 1 (ruft clock auf) 
    Anzeige_Counter = 1;
    // Gibt an das man sich nicht im unter menue befindet
    Maincycle = true;
    // Debugging
    #ifdef DEBUG
      Serial.println("Clock");
      Serial.println(Anzeige_Counter);
    #endif
    // Setzt prevMillis gleich millis 
    prevMillis = millis();
  }

  // Wenn die differenz groesser als die soll zeit fuer einen langen tasten druck ist 
  if (millis() - Button_Pressed_time >= long_Button_Presse) {
    // Setzt den Anzeigen counter auf 10 (untermenue)
    Anzeige_Counter = 10; 
    // Debugging
    #ifdef DEBUG
      Serial.println("Clock long");
      Serial.println(Anzeige_Counter);
    #endif
  } 
  // aendert den zugewiesenen interrupt auf eine andere unterfunktion und reagiert auf die Steigene flanke
  attachInterrupt(Taster_Pins[0], ISR_Clock_RIS, RISING); 
}

// ========================================================================================================== 
// Funktion		:	ISR_Clock_RIS
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		: Wenn der taster gedruckt wird dann wird die Aktuelleprogramm zeit in millis abgespeichert	
//            : und der zu gewiesene Interrupt geaendert sowie auf die Fallende flanke reagiert
// ========================================================================================================== 

void IRAM_ATTR ISR_Clock_RIS (void) {
  // speichert die zeit als der taster gedrueckt wurde
  Button_Pressed_time = millis();

  // Debugging
  // zum testen ob das entprellen klappt
  #ifdef DEBUG
    Serial.print("milli: "); 
    Serial.println(millis()); 
    Serial.print("button =: ");
    Serial.println(Button_Pressed_time);
    Serial.print("diff: "); 
    Serial.println(millis() - Button_Pressed_time); 
  #endif

  // Sprungmarke
  CLOCK:
  // Wenn der taster laenger als die Entprell zeit gedrueckt wurde, dann wird die zet gespeichert und
  // der interrupt auf eine fallende flanke geaendert
  if (millis() - Button_Pressed_time >= entprellen) {

  // Speichert die aktuelle Programm zeit ab
  Button_Pressed_time = millis();
  // aendert den zugewiesenen interrupt auf eine andere unterfunktion und reagiert auf die fallende flanke
  attachInterrupt(Taster_Pins[0], ISR_Clock_FAL, FALLING);
  // Serial.println("Anzeige1: ");  
  }

  // Wenn die zeit noch nicht erreicht ist, dann wird zurueck an die Sprungmalre gesprungen
  else {
    // Springt an die Sprungmarke
    goto CLOCK;
  }
}

// ========================================================================================================== 
// Funktion		:	ISR_Date_FAL
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Bei einem Kurzen druck wird die Mainschleife aufgerufen und bei einem Langen druck gelangt
//            : man ins Untermenue des Datums
// ========================================================================================================== 

void IRAM_ATTR ISR_Date_FAL (void) {
  // Muss deklariert werden da sonst ein wechsel von Fal zu Ris nicht mehr möglich ist
  void IRAM_ATTR ISR_Date_RIS (void);
  // Zum entprellen der Taster muss ein Kurzer druck 20ms gedrueckt werden
  if (millis() - Button_Pressed_time < long_Button_Presse)  {
    // Setzt den Anzeigen counter auf 2
    Anzeige_Counter = 2;
    // Setzt prevMillis gleich millis 
    prevMillis = millis();
  }
  // Wenn die differenz groesser als die soll zeit fuer einen langen tasten druck ist  
  if (millis() - Button_Pressed_time >= long_Button_Presse) {
    // Wenn die differenz kleiner als die soll zeit fuer einen langen tasten druck ist
  } 
  // aendert den zugewiesenen interrupt auf eine andere unterfunktion und reagiert auf die Steigene flanke
  attachInterrupt(Taster_Pins[1], ISR_Date_RIS, RISING); 
}

// ========================================================================================================== 
// Funktion		:	ISR_Date_RIS
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		: Wenn der taster gedruckt wird dann wird die Aktuelleprogramm zeit in millis abgespeichert	
//            : und der zu gewiesene Interrupt geaendert sowie auf die Fallende flanke reagiert
// ========================================================================================================== 

void IRAM_ATTR ISR_Date_RIS (void) {
  // speichert die zeit als der taster gedrueckt wurde
  Button_Pressed_time = millis();
  // Sprungmarke
  DATE:
  // Wenn der taster laenger als die Entprell zeit gedrueckt wurde, dann wird die zet gespeichert und
  // der interrupt auf eine fallende flanke geaendert
  if (millis() - Button_Pressed_time >= entprellen) {
  // Speichert die aktuelle Programm zeit ab
  Button_Pressed_time = millis();
  // aendert den zugewiesenen interrupt auf eine andere unterfunktion und reagiert auf die fallende flanke
  attachInterrupt(Taster_Pins[1], ISR_Date_FAL, FALLING);
  }

  // Wenn die zeit noch nicht erreicht ist, dann wird zurueck an die Sprungmalre gesprungen
  else {
    // Springt an die Sprungmarke
    goto DATE;
  }
}

// ========================================================================================================== 
// Funktion		:	ISR_Temp_FAL
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Bei einem Kurzen druck wird die Mainschleife aufgerufen und bei einem Langen druck gelangt
//            : man ins Untermenue der Temperatur
// ========================================================================================================== 

void IRAM_ATTR ISR_Temp_FAL (void) {
  void IRAM_ATTR ISR_Temp_RIS (void);
  // Zum entprellen der Taster muss ein Kurzer druck 20ms gedrueckt werden
  if (millis() - Button_Pressed_time < long_Button_Presse) {
    // Setzt den Anzeigen counter auf 4
    Anzeige_Counter = 4;
    // Setzt prevMillis gleich millis 
    prevMillis = millis();
  }
  // Wenn die differenz groesser als die soll zeit fuer einen langen tasten druck ist 
  if (millis() - Button_Pressed_time >= long_Button_Presse) {
    // Wenn die differenz kleiner als die soll zeit fuer einen langen tasten druck ist
  } 
  // aendert den zugewiesenen interrupt auf eine andere unterfunktion und reagiert auf die Steigene flanke
  attachInterrupt(Taster_Pins[2], ISR_Temp_RIS, RISING);
}

// ========================================================================================================== 
// Funktion		:	ISR_Temp_RIS
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		: Wenn der taster gedruckt wird dann wird die Aktuelleprogramm zeit in millis abgespeichert	
//            : und der zu gewiesene Interrupt geaendert sowie auf die Fallende flanke reagiert
// ========================================================================================================== 

void IRAM_ATTR ISR_Temp_RIS (void) {
  // speichert die zeit als der taster gedrueckt wurde
  Button_Pressed_time = millis();
  // Sprungmarke
  TEMP:
  // Wenn der taster laenger als die Entprell zeit gedrueckt wurde, dann wird die zet gespeichert und
  // der interrupt auf eine fallende flanke geaendert
  if (millis() - Button_Pressed_time >= entprellen) {
  // Speichert die aktuelle Programm zeit ab
  Button_Pressed_time = millis();
  // aendert den zugewiesenen interrupt auf eine andere unterfunktion und reagiert auf die fallende flanke
  attachInterrupt(Taster_Pins[2], ISR_Temp_FAL, FALLING);
  }

  // Wenn die zeit noch nicht erreicht ist, dann wird zurueck an die Sprungmalre gesprungen
  else {
    // Springt an die Sprungmarke
    goto TEMP;
  }
}

// ========================================================================================================== 
// Funktion		:	ISR_Setup_FAL
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Erkennt ob die Setup Taste kurz oder Lang gedrueckt wurden
//            : 
// ========================================================================================================== 

void IRAM_ATTR ISR_Setup_FAL (void) {
  void IRAM_ATTR ISR_Setup_RIS (void);
  // Zum entprellen der Taster muss ein Kurzer druck 20ms gedrueckt werden
  if (millis() - Button_Pressed_time < long_Button_Presse) {
    short_Setup_Press = true;
    long_Setup_Press = false;
    #ifdef DEBUG
      Serial.println("Setup short");
    #endif
  } 
  // Wenn die differenz groesser als die soll zeit fuer einen langen tasten druck ist dann langer Tatsen druck
  if (millis() - Button_Pressed_time >= long_Button_Presse) {
    long_Setup_Press = true;
    short_Setup_Press = false;
    #ifdef DEBUG
      Serial.println("Setup long");
    #endif
    // Wenn die differenz kleiner als die soll zeit fuer einen langen tasten druck ist dann Kurzer tasten druck
  }  
  // aendert den zugewiesenen interrupt auf eine andere unterfunktion und reagiert auf die Steigene flanke
  attachInterrupt(Taster_Pins[3], ISR_Setup_RIS, RISING);
}

// ========================================================================================================== 
// Funktion		:	ISR_Setup_RIS
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		: Wenn der taster gedruckt wird dann wird die Aktuelleprogramm zeit in millis abgespeichert	
//            : und der zu gewiesene Interrupt geaendert sowie auf die Fallende flanke reagiert
// ========================================================================================================== 

void IRAM_ATTR ISR_Setup_RIS (void) {
  // speichert die zeit als der taster gedrueckt wurde
  Button_Pressed_time = millis();
  // Sprungmarke
  SETUP:
  // Wenn der taster laenger als die Entprell zeit gedrueckt wurde, dann wird die zet gespeichert und
  // der interrupt auf eine fallende flanke geaendert
  if (millis() - Button_Pressed_time >= entprellen) {
  // Speichert die aktuelle Programm zeit ab
  Button_Pressed_time = millis();
  // aendert den zugewiesenen interrupt auf eine andere unterfunktion und reagiert auf die fallende flanke
  attachInterrupt(Taster_Pins[3], ISR_Setup_FAL, FALLING);
  }

  // Wenn die zeit noch nicht erreicht ist, dann wird zurueck an die Sprungmalre gesprungen
  else {
    // Springt an die Sprungmarke
    goto SETUP;
  }
}

// ========================================================================================================== 
// Funktion		:	ISR_Plus_RIS
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Das hoch und runterzaehlen von werten bzw. im menue auf und ab
//
// ========================================================================================================== 

void IRAM_ATTR ISR_Plus_RIS (void) {
  // speichert die zeit als der taster gedrueckt wurde
  Button_Pressed_time = millis();
  // Sprungmarke
  PLUS:
  // Wenn der taster laenger als die Entprell zeit gedrueckt wurde, dann wird short_Plus_Press true und 
  // entsprechend short_Minus_Press false zum verrigeln
  if (millis() - Button_Pressed_time >= entprellen) {
    // Speichert wenn plus gedrueckt wurde
    short_Plus_Press = true;
    // Setzt minus zurueck zum gegenseitigen verrigeln
    short_Minus_Press = false;
    // Debugging
    #ifdef DEBUG
      Serial.println("Plus short");
    #endif
  }

  // Wenn die zeit noch nicht erreicht ist, dann wird zurueck an die Sprungmalre gesprungen
  else {
    // Springt an die Sprungmarke
    goto PLUS;
  }
}

// ========================================================================================================== 
// Funktion		:	ISR_Minus_RIS
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Das hoch und runterzaehlen von werten bzw. im menue auf und ab
//
// ========================================================================================================== 

void IRAM_ATTR ISR_Minus_RIS (void) {
  // speichert die zeit als der taster gedrueckt wurde
  Button_Pressed_time = millis();
  // Sprungmarke
  MINUS:
  // Wenn der taster laenger als die Entprell zeit gedrueckt wurde, dann wird short_Plus_Press false und 
  // entsprechend short_Minus_Press true zum verrigeln
  if (millis() - Button_Pressed_time >= entprellen) {
    // Speichert wenn minus gedrueckt wurde
    short_Minus_Press = true;
    // Setzt plus zurueck zum gegenseitigen verrigeln
    short_Plus_Press = false;
    // Debugging
    #ifdef DEBUG
      Serial.println("Minus short");
    #endif
  }
  
  // Wenn die zeit noch nicht erreicht ist, dann wird zurueck an die Sprungmalre gesprungen
  else {
    // Springt an die Sprungmarke
    goto MINUS;
  }
}