#include <MCP492X.h>
#define PIN_SPI_CHIP_SELECT_DAC 2 // Or any pin you'd like to use

MCP492X myDac(PIN_SPI_CHIP_SELECT_DAC);



int int_intervall_impuls = 1;         //intervall in ms
int int_impuls_dauer = 500;           //länge impuls in ms
int int_strom_impuls = 500;
int int_strom_entladen = 350;

unsigned long entladezeit = 1799999; //1799999
unsigned long entladenStart = 0;


int analogPin = A0;     //Zur Messung der Batteriespannung
int analogStromMessung = A1;
boolean batteriezustandGeprintet = false;
 
int array_impuls[500];    //hier werden die 500 Messungen während des Impulses von 500mA gespeichert
long array_summen[20];    //hier wird die Summe der 500 Messungen für jeden der 20 Impulse gespeichert

void setup() {
  myDac.begin();
  Serial.begin(9600);
  Serial.print("Impulsdauer: ");
  Serial.println(500);
  myDac.analogWrite(0);
  delay(5000);
  
  start();
}

void start() {
  for(int i = 0; i < 20; i++){
    array_summen[i] = impuls();
   // Serial.println(array_summen[i]);
    //analogWrite strom_entladen: 350mA
    myDac.analogWrite(296);
    delay(1000);
    entladenStart = millis();
    while(millis()<(entladenStart + entladezeit)){
      //delay(1800000);                  //@350mA(5% = 30min): 1000ms * 60s * 30min
      if(((analogRead(analogStromMessung)/1023.0)*5.0)< 0.25){
        if(batteriezustandGeprintet==false){
          Serial.println("Batterie kann keine 350mA mehr liefern");
          batteriezustandGeprintet = true;
        }
      }
    }

    //5 min zum Abkühlen und erholen der Batterie
    myDac.analogWrite(0);
    delay(300000);         //(5min)  1000 * 60 * 5
  }
}

long impuls() {
  int i = 0;
  long summe = 0;
  unsigned long startzeit = millis();
  unsigned long intervall = startzeit;
  unsigned long dauer = startzeit + int_impuls_dauer;

  //analogWrite strom_impuls
  myDac.analogWrite(86);    //100mA
 

for(int i = 0; i < 500; i++){
  array_impuls[i] = analogRead(analogPin);
  summe += array_impuls[i];
 // Serial.println(summe);
  delay(1);
}
Serial.println(summe);
return summe;
}






void loop(){
}
