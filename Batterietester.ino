//Batterietester

//Bibliotheken
#include <Adafruit_ADS1015.h>
#include <MCP492X.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 10, 5, 4, 3, 2);
Adafruit_ADS1115 ads;
#define PIN_SPI_CHIP_SELECT_DAC 2
MCP492X myDac(PIN_SPI_CHIP_SELECT_DAC);

float Kurve[] = { 1.649,1.522,1.520,1.516,1.511,1.506,1.502,1.498,1.495,1.492,1.489,1.484,1.480,1.478,1.476,1.474,1.472,1.470,1.467,1.464,1.460,1.455,1.445,1.430,1.421,1.412,1.401,1.390,1.379,1.367,1.353,1.335,1.310,1.256,0.127};

float Impulsspannung = 0;


void setup() {
  Serial.begin(9600);       //9600 Baud
  lcd.begin(16,2);
  lcd.clear();
  ads.setGain(GAIN_TWO);    //GAIN_TWO: 2.048V max
  ads.begin();
  myDac.begin();
  myDac.analogWrite(0);     //Ausgang des DA-Wandlers 168: 100mA    582: 350mA
  lcd.print("KapauitaetMesser: ");   
  impulsMessen();
}


void impulsMessen(){

  float einzelMessung = 0;
  myDac.analogWrite(168);             //100mA Last
  Impulsspannung = 0;
  delay(2000);
  
  for(int i=0; i<20; i++){
    einzelMessung = ads.readADC_SingleEnded(0)*0.0000625;
    Impulsspannung += einzelMessung;
    delay(5);
  }
  Impulsspannung = Impulsspannung/20;
  delay(10);
  lcd.setCursor(1,2);
  lcd.print("Spannung: ");
  lcd.print(Impulsspannung);
  myDac.analogWrite(0);

 
}



void loop() {
  // put your main code here, to run repeatedly:

}
