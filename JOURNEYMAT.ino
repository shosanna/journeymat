#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 7,8,9,10);
int vybranyNufik = 0;

int tlacitkoNufici = 4;
int tlacitko1 = 6;
int tlacitko2 = 3;
int tlacitko3 = 2;
int toggleZapnut = 1;

int diodaNufik = A1;
int diodaNufinka = A2;

#define NUFIK 0
#define NUFINKA 1000
#define ANALOG_PRAH 1000

void defaultniHlaska() {
   lcd.clear();
   lcd.print("** JourneyMat **");
   lcd.setCursor(0,1);
   lcd.print("Jak se citis?");
}

void setup() {
  lcd.begin(16,2);
  defaultniHlaska();
  
  pinMode(tlacitkoNufici, INPUT_PULLUP);
  pinMode(tlacitko1, INPUT_PULLUP);
  pinMode(tlacitko2, INPUT_PULLUP);
  pinMode(tlacitko3, INPUT_PULLUP);

//  pinMode(diodaNufik, OUTPUT);
//  pinMode(diodaNufinka, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // PREPINANI NUFIKU
  int cudlik = digitalRead(tlacitkoNufici) == LOW;
  
  if (cudlik && toggleZapnut) {
    vybranyNufik = vybranyNufik ? 0 : 1000;
    lcd.clear();
    lcd.print("Prepnuto na:");
    lcd.setCursor(0,1);
    lcd.print(vybranyNufik == NUFINKA ? "Nufinka" : "Nufik ");
  }

  if (cudlik) {
    toggleZapnut = 0; 
  } else {
    toggleZapnut = 1;
  }

  // ZOBRAZOVANI NUFIKU
   //analogWrite(diodaNufinka, vybranyNufik ? 1000 : 0);
   //analogWrite(diodaNufik, vybranyNufik ? 0 : 1000);

   // HODNOCENI
   int hodnoceni1 = digitalRead(tlacitko1) == LOW;
   int hodnoceni2 = digitalRead(tlacitko2) == LOW;
   int hodnoceni3 = digitalRead(tlacitko3) == LOW;

   Serial.print("tlacitko1: ");
   Serial.println(hodnoceni1);

   Serial.print("tlacitko2: ");
   Serial.println(hodnoceni2);

   Serial.print("tlacitko3: ");
   Serial.println(hodnoceni3);
   
   if (hodnoceni1 || hodnoceni2 || hodnoceni3) {
     int hodnoceni = hodnoceni1 ? 1 : (hodnoceni2 ? 2 : (hodnoceni3 ? 3 : 0));
     Serial.print(vybranyNufik == NUFINKA ? "Nufinka:" : "Nufik: ");
     Serial.println(hodnoceni == 1 ? "Super" : (hodnoceni == 2 ? "Neutral" : (hodnoceni == 3 ? "Hruza" : "-")));
     
     lcd.clear();
     lcd.print(vybranyNufik == NUFINKA ? "Nufinka:" : "Nufik: ");
     lcd.setCursor(0,1);
     lcd.print(hodnoceni == 1 ? "Super" : (hodnoceni == 2 ? "Neutral" : (hodnoceni == 3 ? "Hruza" : "-")));
     delay(3000);
     lcd.clear();
     defaultniHlaska();
   }
}
