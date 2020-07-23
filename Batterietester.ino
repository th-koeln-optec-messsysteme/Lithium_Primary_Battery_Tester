/* Arduino Tutorial: Learn how to use an LCD 16x2 screen
   More info: http://www.ardumotive.com/how-to-use-an-lcd-dislpay-en.html  */

//Include LCD library
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Wire.h>
#include <MCP492X.h>          //link: https://github.com/michd/Arduino-MCP492X/blob/master/README.md
#include <Adafruit_ADS1015.h>

LiquidCrystal lcd(12, 10, 5, 4, 3, 2);
Adafruit_ADS1115 ads;
#define PIN_SPI_CHIP_SELECT_DAC 2
MCP492X myDac(PIN_SPI_CHIP_SELECT_DAC);

double Spannung, Spannungsspeicher[20];
float Vergleichswerte[20][2];
float KapazitaetKurve[35][2] = { {1.649,3500},{1.522,3400},{1.520,3300},{1.516,3200},{1.511,3100},{1.506,3000},{1.502,3300},{1.498,2900},{1.495,2800},{1.492,2700},{1.489,2600},{1.484,2500},{1.480,2400},{1.478,2300},{1.476,2200},{1.474,2100},{1.472,2000},{1.470,1900},{1.467,1800},{1.464,1700},{1.460,1600},{1.455,1500},{1.445,1400},{1.430,1300},{1.421,1300},{1.412,1200},{1.401,1100},{1.390,1000},{1.379,900},{1.367,800},{1.353,700},{1.335,600},{1.310,500},{1.256,400},{0.127,0}};
float Kurve[ 5 ][ 2 ] = {{1780,3500},{ 1650,1600},{ 1590,1500},{ 1500,300},{ 1400,0}};
unsigned int Laufvariabel = 0;
short zaehler = 0;

// I/O und Variablen definieren

void setup() {
 Serial.begin(9600);       //9600 Baud
  lcd.begin(20,4);
  ads.setGain(GAIN_TWO);    //GAIN_TWO: 2.048V max
  ads.begin();
  myDac.begin();
  myDac.analogWrite(0);     //Ausgang des DA-Wandlers 168: 100mA    582: 350mA 

}

//Arithmetisches Mittel über Spannung

double SpannungMitteln(){
  
  double GemittetelteSpannung = 0;
  double Zwischenergebnis = 0;

   for(int j = 0; j < 20; j++)
   {    
   
    Zwischenergebnis  += Spannungsspeicher[j];
    }

    GemittetelteSpannung = Zwischenergebnis / 20;

    return GemittetelteSpannung;
}


//Ausgabe der Kapazität
//Vergleich Impulsantwort mit Tabelle
//Vergleich Spannung mit Tabelle KapaziteatKurve

double KapazitaetBerechnen(double GemittetelteSpannung) {
  double Kapazitaet;
  short i = 0;
      
    do{
       if(GemittetelteSpannung == KapazitaetKurve[ i][ 0]){
        
           Kapazitaet = KapazitaetKurve[ i][ 1 ];
       
       }else if((GemittetelteSpannung > KapazitaetKurve[ i+1][ 0 ]) && (GemittetelteSpannung < KapazitaetKurve[ i][ 0])){
        
                 Kapazitaet =  ((KapazitaetKurve[ i][ 1 ]-KapazitaetKurve[ i+1][ 1])/(KapazitaetKurve[ i][ 0]-KapazitaetKurve[ i+1][ 0]))*(GemittetelteSpannung-KapazitaetKurve[ i+1][ 0]) + KapazitaetKurve[ i+1][ 1];
            
              }else if(GemittetelteSpannung > KapazitaetKurve[ i][ 0 ] || GemittetelteSpannung == 0) {
                Kapazitaet = 3500;
                }
       i++;
     }while( GemittetelteSpannung < KapazitaetKurve[ i][ 0 ]);
       
  return Kapazitaet;
}

//Ausgabe kümmert sich nur um die Ausgabe am LCD

void Ausgabe() {
  int KapazitaetProzent;
  if(Laufvariabel%1000 == 0 && Laufvariabel != 0){
 //Ausgabe Kapazität
       KapazitaetProzent = ( KapazitaetBerechnen(SpannungMitteln()) / 3500 )* 100;
       lcd.setCursor(0, 1);
       lcd.print( KapazitaetProzent);
       lcd.setCursor(2, 1);
       lcd.print( " % ");

  //Ausgabe der Spannung
       lcd.setCursor(0, 0);
       lcd.print(SpannungMitteln());
       lcd.setCursor(7, 0);
       lcd.print(" mV   ");  
     Serial.println(SpannungMitteln());
  Laufvariabel = 0;
  while(1);
  }
}
//Spannung Überwachen

void loop() {
  
  //dauerhaftes lesen der Spannung
  float UBatterie = ads.readADC_SingleEnded(0)*0.0000625;
 
  if(Laufvariabel%50 == 0) {
      Spannungsspeicher[zaehler] = UBatterie;
      zaehler++;
      Serial.println(UBatterie);
  }
  
// Ausgabe nur in gewissen Abständen um Lesbarkeit zu gewährleisten
  if(Laufvariabel%1000 == 0 && Laufvariabel != 0){
      zaehler = 0;
      Ausgabe();
  } 
  Laufvariabel++; 
}
