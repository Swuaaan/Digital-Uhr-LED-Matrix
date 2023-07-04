#ifndef Bio_Gas_h
#define Bio_Gas_h

//#define DEBUG

#define WARNTONE

// LCD Pin-Definitionen
//Angabe der Pins (RS, EN, D4, D5, D6, D7) des angeschlossenen LCD
#define RS 12
#define EN 11
#define D4 5
#define D5 4
#define D6 3
#define D7 2

//Luefter
#define LED8_Pin 10

#define LED9_Pin 13

// Pin 15 == A1
#define Heizungs_Pin 15
// Pin 14 == A0
#define Gas_Sensor_In_Pin 14

#define Ample_Adresse 0b00111111

#define LM75_Adresse  0b01001101

//LED Ampel-Muster
//Ampel 1 (P0-P2)
#define aus   0b00000000
#define rot   0b00000100
#define gelb  0b00000010
#define gruen 0b00000001

#define Normal_Temp_min 32.0
#define Normal_Temp_max 35.0
#define Normal_Gas_min 30
#define Normal_Gas_max 100

#define Stoerung_Temp_Min 20.0
#define Stoerung_Temp_Max 42.0

#define Stoerung_Gas 20

#define Motor_Ein_Min 50
#define Motor_delay 5000

//WarnTonAusgang
#define Warnton_Pin 8

#define LCD_INTERVAL_TIME 250

class Bio_Gas {
  private:

  unsigned long int PrevMillis = 0;

  unsigned long int PrevMillis_Motor = 0;

  public:
  
  void Bio_Gas::DataWrite(byte data);

  void Bio_Gas::Init_LCD (void);

  void Bio_Gas::Init_IO (void);

  int Bio_Gas::Gas_Wert_erfassung (byte Pin);

  double Bio_Gas::Temp_erfassung (void);

  void Bio_Gas::Ample_Steuerung (double Temp, byte Gas_wert);

  void Bio_Gas::Motor_Steuerung (byte Gasvalue);

  void Bio_Gas::Heizung (double Temp);

  void Bio_Gas::Ausgabe (double Temp, byte Gasvalue);
};

#endif