
#include "Bio_Gas.h"
#include <Arduino.h>

Bio_Gas Bio();

void setup() {
  Serial.begin(9600);
  Wire.begin();                                             //Initialisieren der I2C Verbindung. MUSS im Setup aufgerufen werden.
  lcd.begin(16, 4);  
  Bio.Init_LCD();
  Bio.Init_IO();
}

void loop() {
  Bio.Heizung(temperature1.readTemperatureC());

  while (millis() - PrevMillis >= LCD_INTERVAL_TIME) {
    PrevMillis = millis();
    Bio.Ausgabe(temperature1.readTemperatureC(), Bio.Gas_Wert_erfassung(Gas_Sensor_In_Pin));
  }

  Bio.Motor_Steuerung(Gas_Wert_erfassung(Gas_Sensor_In_Pin));
  
  Bio.Ample_Steuerung(temperature1.readTemperatureC(), Bio.Gas_Wert_erfassung(Gas_Sensor_In_Pin));

  delay(50);
}
