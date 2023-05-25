#ifndef LED_Matrix_Display_h
#define LED_Matrix_Display_h

class LED_Matrix {
    private:

    public:
        // ========================================================================================================== 
        // Funktion		:	LDR
        // ----------------------------------------------------------------------------------------------------------
        // Aufgabe		:	Erfast mit dem LDR die Helligkeit des Raumes und gibt diese in 12 Bits (0 - 4095) wieder aus. 
        //            : Dann werden diese auf 4 bits (0 - 16) reduziert.
        // Parameter	:	void  
        // Return-Wert:	byte 0 - 16 Helligkeit des Raumes
        // ========================================================================================================== 
        
        byte LDR (void);

        // ========================================================================================================== 
        // Funktion		:	Bewegungssensor_time
        // ---------------------------------------------------------------------------------------------------------- 
        // Aufgabe		:	Wenn der Sensor aktive dann gibt dieser fuer eine bestimmte zeit ein true zurueck
        //            : anderen falls ein false
        // Parameter	:	void
        // Return-Wert:	bool Sensor Status
        // ========================================================================================================== 

        bool Bewegungssensor_time (void);

        // ========================================================================================================== 
        // Funktion		:	Display_Center_Text
        // ---------------------------------------------------------------------------------------------------------- 
        // Aufgabe		:	Das anzeigen von text mittig der anzeige
        // Parameter	:	char anzeigen[8] und bool aktive
        // Return-Wert:	void
        // ========================================================================================================== 

        void Display_Center_Text (char anzeigen[8], bool aktive);

        // ========================================================================================================== 
        // Funktion		:	Display_Center_Nummer
        // ---------------------------------------------------------------------------------------------------------- 
        // Aufgabe		:	Das anzeigen von nummern mittig der anzeige
        // Parameter	:	int anzeigen und bool aktive
        // Return-Wert:	void
        // ========================================================================================================== 

        void Display_Center_Nummer (int anzeigen, bool aktive);

        // ========================================================================================================== 
        // Funktion		:	Display_Scroll_Text
        // ---------------------------------------------------------------------------------------------------------- 
        // Aufgabe		:	SOrgt dafuer das der Text in anzeige ueber die Matrix laueft
        // Parameter	:	char anzeigen[25] und bool aktive
        // Return-Wert:	void
        // ========================================================================================================== 
        
        void Display_Scroll_Text (char anzeigen[25], bool aktive);

        // ========================================================================================================== 
        // Funktion		:	LED_Matrix_Clock
        // ---------------------------------------------------------------------------------------------------------- 
        // Aufgabe		:	Das Anzeigen der Uhr zeit mittig auf der Matrix
        // Parameter	:	void
        // Return-Wert:	void
        // ========================================================================================================== 

        void LED_Matrix_Clock (void);

        // ========================================================================================================== 
        // Funktion		:	LED_Matrix_date
        // ---------------------------------------------------------------------------------------------------------- 
        // Aufgabe		:	Das Anzeigen des tag und monat mittig auf der Matrix
        // Parameter	:	void
        // Return-Wert:	void
        // ========================================================================================================== 

        void LED_Matrix_date (void);

        // ========================================================================================================== 
        // Funktion		:	LED_Matrix_year
        // ---------------------------------------------------------------------------------------------------------- 
        // Aufgabe		:	Das Anzeigen des Jahres mittig auf der Matrix
        // Parameter	:	void
        // Return-Wert: void	
        // ========================================================================================================== 

        void LED_Matrix_year (void);

        // ========================================================================================================== 
        // Funktion		:	LED_Matrix_temp
        // ---------------------------------------------------------------------------------------------------------- 
        // Aufgabe		:	Das Anzeigen der temperratur mittig auf der Matrix
        // Parameter	:	float temp = tmperatur wert in °C
        // Return-Wert:	void
        // ========================================================================================================== 

        void LED_Matrix_temp (float temp);
};

#endif