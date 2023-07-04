#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Temperature_LM75_Derived.h>

#include "Bio_Gas.h"

//Generic_LM75 temperature1;

Generic_LM75_11Bit temperature1(LM75_Adresse);

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

// unsigned long int PrevMillis = 0;

// unsigned long int PrevMillis_Motor = 0;

//Daten auf PCF8574A schreiben
void Bio_Gas::DataWrite(byte data) {
  data = ~data;                                 //Daten einmal negieren, da LED mit invertierter Logik eingeschaltet werden  
  Wire.beginTransmission(Ample_Adresse);        //Öffnen der Verbindung
  Wire.write(data);                             //Schreiben des Bytes. Jede Null stellt ein Bit an den Pins des PCF8574 dar (Auch möglich in Hex Schreibweise)
  Wire.endTransmission();                       //Schließen der Verbindung
  delay(50);
}

void Bio_Gas::Init_LCD (void) {
  lcd.setCursor(4, 0);                          // LCD zu Beginn mit Prüfungsname bespielen und Ausgabe auf serieller Schnittstelle
  lcd.print("Pruefung ");                       // zur Überprüfung beim flashen und 5 Sekunden warten 
  lcd.setCursor(2, 1);
  lcd.print("Winter 19/20");
  lcd.setCursor(2, 2);
  lcd.print("Ueberarbeitet");
  lcd.setCursor(4, 3);
  lcd.print("Stollen");
  
  delay(1000);
  lcd.clear();
}

void Bio_Gas::Init_IO (void) {
  pinMode(Gas_Sensor_In_Pin, INPUT);
  pinMode(LED8_Pin, OUTPUT);
  pinMode(LED9_Pin, OUTPUT);
  pinMode(Warnton_Pin, OUTPUT);
  pinMode(Heizungs_Pin, OUTPUT);
  DataWrite(aus);
}

int Bio_Gas::Gas_Wert_erfassung (byte Pin) {
  int Gas_Value = map(analogRead(Pin), 0, 1023, 0, 100);
  return Gas_Value;
}

double Bio_Gas::Temp_erfassung (void) {
  double temp = temperature1.readTemperatureC();
  return temp;
}

void Bio_Gas::Ample_Steuerung (double Temp, byte Gas_wert) {

  // Rot
  if ((Temp > Stoerung_Temp_Max) || (Temp < Stoerung_Temp_Min) || (Gas_wert < Stoerung_Gas)) { 
    DataWrite(rot);
    #ifdef WARNTONE
      digitalWrite(Warnton_Pin, HIGH);
    #endif
  }

  // Grün
  else if (((Temp >= Normal_Temp_min) && (Temp <= Normal_Temp_max)) && (Gas_wert >= Normal_Gas_min)) {
    DataWrite(gruen);
    #ifdef WARNTONE
      digitalWrite(Warnton_Pin, LOW);
    #endif
  } 
  
  // Gelb
  
  else if (Temp > (Normal_Temp_max + 1) || Temp > (Normal_Temp_min + 1) || Gas_wert < Normal_Gas_min) {
    DataWrite(gelb);
    #ifdef WARNTONE
      digitalWrite(Warnton_Pin, LOW);
    #endif
  } 

  
}

void Bio_Gas::Motor_Steuerung (byte Gasvalue) {
  if (Gasvalue >= Motor_Ein_Min && (millis() - PrevMillis_Motor) >= Motor_delay) {
    digitalWrite(LED9_Pin, HIGH);
  }

  else if (Gasvalue >= Motor_Ein_Min) {
    if (!digitalRead(LED8_Pin)) PrevMillis_Motor = millis();
    digitalWrite(LED8_Pin, HIGH);
  }

  else if (Gasvalue < Motor_Ein_Min) {
    if (digitalRead(LED9_Pin)) PrevMillis_Motor = millis();
    digitalWrite(LED9_Pin, LOW);

    if (millis() - PrevMillis_Motor >= Motor_delay) digitalWrite(LED8_Pin, LOW);
  }
}

void Bio_Gas::Heizung (double Temp) {
  if (Temp >= Normal_Temp_max) digitalWrite(Heizungs_Pin, LOW);
  
  else if (Temp <= Normal_Temp_min) digitalWrite(Heizungs_Pin, HIGH);  
}

void Bio_Gas::Ausgabe (double Temp, byte Gasvalue) {
  char Buffer[16];

  #define GAS

  #define TEMP

  // Gibt die laenge der der zahl an mit von und nach komma stelle
  #define Zahllaenge 5
  // gibt die nachkomma stellen an
  #define Nachkommastelle 3
  
  // Wandelt den float wert in eine String wert der angezeigt werden kann
  dtostrf(Temp, Zahllaenge, Nachkommastelle, Buffer);

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Temp: ");
  lcd.print(Buffer);
  lcd.write(0b11011111);
  lcd.print("C");

  #ifdef TEMP
    #ifdef DEBUG
      Serial.print(Buffer);
      Serial.println("°C");
    #endif
  #endif

  lcd.setCursor(1, 1);
  lcd.print("Gas-Wert: ");
  lcd.print(Gasvalue);
  lcd.print("%");

  #ifdef GAS
    #ifdef DEBUG
      Serial.print(Gasvalue);
      Serial.println("%");
    #endif
  #endif
}