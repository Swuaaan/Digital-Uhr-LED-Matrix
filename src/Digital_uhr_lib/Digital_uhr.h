#ifndef Digital_uhr_h
#define Digital_uhr_h

// ========================================================================================================== 
// 	Unterfunktionen
// ========================================================================================================== 

// ========================================================================================================== 
// Funktion		:	setupWifi
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Erstellt ein Soft Accespoint und laede dann die html und css datei 
//            : fü die Website zu Wifi abfrage  
// Parameter	: void  
// Return-Wert:	void
// ========================================================================================================== 

void setupWifi (void);

// ========================================================================================================== 
// Funktion		:	SoftAccesPoint_abfrage
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Extrahiert die ssid und Passwort von der localen Website
// Parameter	:	void
// Return-Wert:	void
// ========================================================================================================== 

void SoftAccesPoint_abfrage (void);

// ========================================================================================================== 
// Funktion		:	Program_interval_time
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	ermöglichst das einstellen des Programm intervales
// Parameter	:	void
// Return-Wert:	int = programm interval zeit
// ========================================================================================================== 

int Program_interval_time (void);

// ========================================================================================================== 
// Funktion		:	ssid_pass_erhalten
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Kombiniert setupWifi und SoftAccesPoint_abfrage. Loopt solange bis die breaktime erreicht 
//            : oder Netzwerk zugangsdaten erhalten wurde
// Parameter	:	byte breaktime = nach der zeit wir der Verbindungs versuch abgebrochen
// Return-Wert:	bool status = ist Wifi Verbunden
// ========================================================================================================== 

bool ssid_pass_erhalten (byte breaktimer);

// ========================================================================================================== 
// Funktion		:	Aktuallisiere_Date_Time
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Verbindung zu dem internet mit den Empfangenen Netzwerk daten
// Parameter	:	bool Verbunden = ob Netzwerk daten uebergeben wurden
//            : byte breaktime = nach der zeit wir der Verbindungs versuch abgebrochen
// Return-Wert:	void
// ========================================================================================================== 

void Aktuallisiere_Date_Time (bool Verbunden, byte breaktimer);

// ========================================================================================================== 
// Funktion		:	Alarm_Ausloesen
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Vergleich die Alarm zeit mit der Aktuellen uhr zeit wenn diese gleich sind wird ein true 
//            : zurück gegeben
// Parameter	:	void
// Return-Wert:	bool wenn Alarm = uhrzeit ist == true
// ========================================================================================================== 

bool Alarm_Ausloesen (void);

// ========================================================================================================== 
// 	ISR
// ========================================================================================================== 

// ========================================================================================================== 
// Funktion		:	ISR_Clock_FAL
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Bei einem Kurzen druck wird die Mainschleife aufgerufen und bei einem Langen druck gelangt
//            : man ins Untermenue der Uhrzeit
// ========================================================================================================== 

void IRAM_ATTR ISR_Clock_FAL (void);

// ========================================================================================================== 
// Funktion		:	ISR_Clock_RIS
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		: Wenn der taster gedruckt wird dann wird die Aktuelleprogramm zeit in millis abgespeichert	
//            : und der zu gewiesene Interrupt geaendert sowie auf die Fallende flanke reagiert
// ========================================================================================================== 

void IRAM_ATTR ISR_Clock_RIS (void);

// ========================================================================================================== 
// Funktion		:	ISR_Date_FAL
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Bei einem Kurzen druck wird die Mainschleife aufgerufen und bei einem Langen druck gelangt
//            : man ins Untermenue des Datums
// ========================================================================================================== 

void IRAM_ATTR ISR_Date_FAL (void);

// ========================================================================================================== 
// Funktion		:	ISR_Date_RIS
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		: Wenn der taster gedruckt wird dann wird die Aktuelleprogramm zeit in millis abgespeichert	
//            : und der zu gewiesene Interrupt geaendert sowie auf die Fallende flanke reagiert
// ========================================================================================================== 

void IRAM_ATTR ISR_Date_RIS (void);

// ========================================================================================================== 
// Funktion		:	ISR_Temp_FAL
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Bei einem Kurzen druck wird die Mainschleife aufgerufen und bei einem Langen druck gelangt
//            : man ins Untermenue der Temperatur
// ========================================================================================================== 

void IRAM_ATTR ISR_Temp_FAL (void);

// ========================================================================================================== 
// Funktion		:	ISR_Temp_RIS
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		: Wenn der taster gedruckt wird dann wird die Aktuelleprogramm zeit in millis abgespeichert	
//            : und der zu gewiesene Interrupt geaendert sowie auf die Fallende flanke reagiert
// ========================================================================================================== 

void IRAM_ATTR ISR_Temp_RIS (void);

// ========================================================================================================== 
// Funktion		:	ISR_Setup_FAL
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Erkennt ob die Setup Taste kurz oder Lang gedrueckt wurden
//            : 
// ========================================================================================================== 

void IRAM_ATTR ISR_Setup_FAL (void);

// ========================================================================================================== 
// Funktion		:	ISR_Setup_RIS
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		: Wenn der taster gedruckt wird dann wird die Aktuelleprogramm zeit in millis abgespeichert	
//            : und der zu gewiesene Interrupt geaendert sowie auf die Fallende flanke reagiert
// ========================================================================================================== 

void IRAM_ATTR ISR_Setup_RIS (void);

// ========================================================================================================== 
// Funktion		:	ISR_Plus_RIS
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Das hoch und runterzaehlen von werten bzw. im menue auf und ab
//
// ========================================================================================================== 

void IRAM_ATTR ISR_Plus_RIS (void);

// ========================================================================================================== 
// Funktion		:	ISR_Minus_RIS
// ---------------------------------------------------------------------------------------------------------- 
// Aufgabe		:	Das hoch und runterzaehlen von werten bzw. im menue auf und ab
//
// ========================================================================================================== 

void IRAM_ATTR ISR_Minus_RIS (void);

// ========================================================================================================== 
// 	Init
// ========================================================================================================== 

// ========================================================================================================== 
// Funktion		:	Init_Button
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Initalisiert die Taster
// Parameter	:	void
// Return-Wert:	void
// ========================================================================================================== 

void Init_Button (void);

// ========================================================================================================== 
// Funktion		:	Taster_Attach_Interrupte
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Weißt den tastern eine Interrupt funktion zu
// Parameter	:	void  
// Return-Wert:	void
// ========================================================================================================== 

void Taster_Attach_Interrupte (void);

// ========================================================================================================== 
// Funktion		:	Init_Buzzer
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Legt den Buzzer Pin als output fest
// Parameter	:	void 
// Return-Wert:	void
// ========================================================================================================== 

void Init_Buzzer (void);

// ========================================================================================================== 
// Funktion		:	Init_Bewegungssensor
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Legt den Bewegungssensor Pin als input fest
// Parameter	:	void    
// Return-Wert:	void
// ========================================================================================================== 

void Init_Bewegungssensor (void);

// ========================================================================================================== 
// Funktion		:	Init_Interval_Poti
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Legt den Poti Pin als input fest
// Parameter	:	void
// Return-Wert:	void
// ========================================================================================================== 

void Init_Interval_Poti (void);

// ========================================================================================================== 
// Funktion		:	Init_RTC  
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Stellt sicher das, dass RTC verbunden ist und komunizieren kann. 
//            : Sollt es ein Stromausfallgegeben haben und die battery ist lehr,
//            : dann wird eine neue zeit gesetzt
// Parameter	:	void
// Return-Wert:	void
// ========================================================================================================== 

void Init_RTC (void);

// ========================================================================================================== 
// Funktion		:	Init_LED_Matrix
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Startet die Komunikation mit der LED Matrix 
//            : und legt ein zeichen für °C fest
// Parameter	:	void
// Return-Wert:	void
// ========================================================================================================== 

void Init_LED_Matrix (void);

// ========================================================================================================== 
// Funktion		:	Init
// ----------------------------------------------------------------------------------------------------------
// Aufgabe		:	Fast alle Init Unterprogramme zusammen
// Parameter	:	void
// Return-Wert:	void
// ========================================================================================================== 

void Init (void);

// ========================================================================================================== 
// 	Menue
// ========================================================================================================== 

void Display_Reset_and_Clear (void);

void Untermenue_Hoch_short_Plus (byte menue_counter_value);

void Untermenue_Runter_short_Minus (byte menue_counter_value);

void Untermenue_waehlen_short_Setup (byte menue_counter_value);

void short_Plus_set_counter (byte max_value, byte set_value);

void short_Minus_set_counter (byte min_value, byte set_value);

void Menue_switch (void);

#endif