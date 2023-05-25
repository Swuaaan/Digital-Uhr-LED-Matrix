#include <Arduino.h>

#include "Configuration.h"
#include "Digital_uhr.h"

void Display_Reset_and_Clear (void) {
  Matrix.displayReset();
  Matrix.displayClear();
}

void Untermenue_Hoch_short_Plus (byte menue_counter_value) {
  // Mit der Plus taste weiter im untermenue
  if (true == short_Plus_Press) {
    Anzeige_Counter = menue_counter_value; 
    Alarm_Counter = 0;
    short_Plus_Press = false;
    Display_Reset_and_Clear();
  }
}

void Untermenue_Runter_short_Minus (byte menue_counter_value) {
  // Mit der Minus taste zurueck im untermenue
  if (true == short_Minus_Press) {
    Anzeige_Counter = menue_counter_value; 
    Alarm_Counter = 0;
    short_Minus_Press = false;
    Display_Reset_and_Clear();
  }
}

void Untermenue_waehlen_short_Setup (byte menue_counter_value) {
  if (true == short_Setup_Press) {
    Anzeige_Counter = menue_counter_value; 
    short_Setup_Press = false;
    Alarm_Counter = 0;
    Display_Reset_and_Clear();
  } 
}

void short_Plus_set_counter (byte max_value, byte set_value) {
  if (true == short_Plus_Press) {
    Alarm_Counter++;
    if (true == short_Plus_Press && max_value == Alarm_Counter) Alarm_Counter = set_value;
    short_Plus_Press = false;
  }
}

void short_Minus_set_counter (byte min_value, byte set_value) {
  if (true == short_Minus_Press) {
    Alarm_Counter--;
    if (true == short_Minus_Press && min_value == Alarm_Counter) Alarm_Counter = set_value;
    short_Minus_Press = false;
  } 
}

void Menue_switch (void) {
    // Menue switch
    switch (Anzeige_Counter) {
        // Anzeigen der Uhrzeit und reaktivieren des Alarmes nach dem ausloesen
        case 1:
        LED_Matrix.LED_Matrix_Clock();
        if (rtc.now().minute() > Counter_Minute) {
            Alarm_State = true;
        }
        break;
        // Untermenue Alarm
        case 10:
            // Speichern das, dass programm in untermenue ist
            Maincycle = false;
            // Abfrage um den Alarm an und aus zuschalten
            if (true == long_Setup_Press) {
            Alarm_State = !Alarm_State;
            long_Setup_Press = false;
            Display_Reset_and_Clear();
            } 
            // Mit der Plus taste weiter im untermenue
            Untermenue_Hoch_short_Plus(20);

            // Mit der Minus taste zurueck im untermenue
            Untermenue_Runter_short_Minus(30);
            // aender der Anzeige wenn Alarm an oder aus ist
            if (false == Alarm_State) {
            // Wandeln fuer ausgabe
            sprintf(Buffer, "Alarm aus!");
            
            // Debugging
            #ifdef DEBUG
                Serial.println("Alarm aus");            
            #endif
            // Ausgegben an das anzeige unterprogramm
            LED_Matrix.Display_Scroll_Text(Buffer, true);        
            } 

            else {
            // Wandeln fuer ausgabe
            sprintf(Buffer, "Alarm on: %02d%c%02d", Counter_Stunden, ':', Counter_Minute);
            
            // Debugging
            #ifdef DEBUG
                Serial.println(Buffer);
            #endif
            // Ausgegben an das anzeige unterprogramm
            LED_Matrix.Display_Scroll_Text(Buffer, true);
            
            // Springen um die Weck stunde anzuzeigen
            Untermenue_waehlen_short_Setup(11); 
            }        
        break;
            // einstellen der Alarm Stunde
            case 11:
            // Anzeigen der Stunde
            LED_Matrix.Display_Center_Nummer (Alarm_Counter, true); 

            // Mit der Plus taste weiter im untermenue
            short_Plus_set_counter(25, 0);
            // Mit der Minus taste zurueck im untermenue                     
            short_Minus_set_counter(255, 24);

            // Uebergeben der ausgewaehlten stunde
            Counter_Stunden = Alarm_Counter;

            #ifdef DEBUG
                Serial.print("Alarm_Stunde: ");
                Serial.println(Counter_Stunden);
            #endif

            // Weiter zu Minuten
            Untermenue_waehlen_short_Setup(12); 

            break;
            // einstellen der Alarm Minuten
            case 12:
            // Anzeigen der Minuten
            LED_Matrix.Display_Center_Nummer (Alarm_Counter, true); 

            // Mit der Plus taste weiter im untermenue
            short_Plus_set_counter(61, 0);
            // Mit der Minus taste zurueck im untermenue                     
            short_Minus_set_counter(255, 60);

            // Debugging
            #ifdef DEBUG
                Serial.print("Alarm_Minute: ");
                Serial.println(Counter_Minute);
            #endif
            
            // Uebergeben der ausgewaehlten minute
            Counter_Minute = Alarm_Counter;

            // mit setup zurueck zur Alarm wahl
            Untermenue_waehlen_short_Setup(10); 

            break;
        // unter menue Uhrzeit einstellen
        case 20:
            // Speichern das, dass programm in untermenue ist
            Maincycle = false;
            
            // Mit der Plus taste weiter im untermenue
            Untermenue_Hoch_short_Plus(30);

            // Mit der Minus taste zurueck im untermenue
            Untermenue_Runter_short_Minus(20);        

            // Wandeln fuer ausgabe
            sprintf(Buffer, "Time: %02d%c%02d", rtc.now().hour(), ':', rtc.now().minute());
            
            // Debugging
            #ifdef DEBUG
            Serial.println(Buffer);
            #endif

            // Ausgegben an das anzeige unterprogramm
            LED_Matrix.Display_Scroll_Text(Buffer, true);

            // Springen um die Time stunde anzuzeigen
            Untermenue_waehlen_short_Setup(21);  
                
        break;

            // einstellen der Stunde
            case 21:
            // Anzeigen der Stunde
            LED_Matrix.Display_Center_Nummer (Alarm_Counter, true); 
            
            // Mit der Plus taste weiter im untermenue
            short_Plus_set_counter(25, 0);
            // Mit der Minus taste zurueck im untermenue                     
            short_Minus_set_counter(255, 24);

            // Debugging
            #ifdef DEBUG
                Serial.print("Time_Stunde: ");
                Serial.println(Alarm_Counter);
            #endif

            // Uebergeben der Stunde an das RTC
            if (true == short_Setup_Press) {
                Anzeige_Counter = 22; 
                short_Setup_Press = false;
                rtc.adjust(DateTime(rtc.now().year(), 
                                    rtc.now().month(), 
                                    rtc.now().day(), 
                                    Alarm_Counter, 
                                    rtc.now().minute(), 
                                    rtc.now().second()
                ));
                // zurueck setzen der counter variable
                Alarm_Counter = 0;
            } 
            break;

            // einstellen der Minute
            case 22:
            // Anzeigen der Minute
            LED_Matrix.Display_Center_Nummer (Alarm_Counter, true); 

            // Mit der Plus taste weiter im untermenue
            short_Plus_set_counter(61, 0);
            // Mit der Minus taste zurueck im untermenue                     
            short_Minus_set_counter(255, 60);

            // Debugging
            #ifdef DEBUG
                Serial.print("Time_Minuten: ");
                Serial.println(Alarm_Counter);
            #endif

            // Uebergeben der Minuten an das RTC
            // Setzen von sekunden auf null
            if (true == short_Setup_Press) {
                Anzeige_Counter = 20; 
                short_Setup_Press = false;
                rtc.adjust(DateTime(rtc.now().year(), 
                                    rtc.now().month(), 
                                    rtc.now().day(), 
                                    rtc.now().hour(), 
                                    Alarm_Counter, 
                                    0
                ));
                // zurueck setzen der counter variable
                Alarm_Counter = 0;
            } 
            break;

        // Untermenue fuer den Bewegungssensor
        case 30:
            
            // Anzeigen wenn Bewegungssen. aus ist
            if (true == Bewegungssensor_aktiv) {
            sprintf(Buffer, "Bewegungssensor aus");
            
            // Debugging
            #ifdef DEBUG
                Serial.println(Buffer);
            #endif

            // Ausgegben an das anzeige unterprogramm
            LED_Matrix.Display_Scroll_Text(Buffer, true);
            }

            // Anzeigen wenn Bewegungssen. an ist
            else {
            sprintf(Buffer, "Bewegungssensor an");
            
            // Debugging
            #ifdef DEBUG
                Serial.println(Buffer);
            #endif

            // Ausgegben an das anzeige unterprogramm
            LED_Matrix.Display_Scroll_Text(Buffer, true);
            }
            
            // Abfrage um den Alarm an und aus zuschalten
            if (true == long_Setup_Press) {
            Bewegungssensor_aktiv = !Bewegungssensor_aktiv;
            long_Setup_Press = false;
            Display_Reset_and_Clear();
            } 

            // Mit der Plus taste weiter im untermenue
            Untermenue_Hoch_short_Plus(10);

            // Mit der Minus taste zurueck im untermenue
            Untermenue_Runter_short_Minus(20);  

        break;

        case 40:
            // Weiterleifen an den anfang des menues
            Anzeige_Counter = 10;
            Display_Reset_and_Clear();
        break;

        case 2:
        // Anzeigen des Datums
        LED_Matrix.LED_Matrix_date();
        break;

        case 3:
        // Anzeigen des Jahres
        LED_Matrix.LED_Matrix_year();
        break;

        case 4:
        // Anzeigen der Temperatur
        LED_Matrix.LED_Matrix_temp(LM75_Sensor.readTemperatureC());
        break;

        default:
        // wenn das menue durcgelaufen ist oder ein fehler auftrit und maincycle == true dann Anzeige_Counter = 1
        if (Maincycle == true) {
            Anzeige_Counter = 1;
        } 
        // wenn maincycle == false dann springt dieser hier in unterprogramm
        else {
            Anzeige_Counter = 10;
        }
        
        break;
    }

}

