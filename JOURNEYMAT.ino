#include <VirtualWire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 7, 8, 9, 10);
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

void defaultniHlaska() {
   lcd.clear();
   lcd.print("** JourneyMat **");
   lcd.setCursor(0,1);
   lcd.print("Jak se citis?");
}

void setupRadio() {
  vw_set_tx_pin(13);
  vw_set_rx_pin(5); 

  // nastavení typu bezdrátové komunikace
  vw_set_ptt_inverted(true);
  // nastavení rychlosti přenosu v bitech za sekundu
  vw_setup(1000);
}

void sendRadio(char* zprava) {
  Serial.println(zprava);
  // odeslání textu v proměnné zprava
  vw_send((uint8_t *)zprava, strlen(zprava));
  // vyčkání na odeslání celé zprávy
  vw_wait_tx();
}

void setup() {
  lcd.begin(16,2);
  defaultniHlaska();

  setupRadio();
  
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
    
    Serial.print("Prepnuto na ");
    Serial.println(vybranyNufik == NUFINKA ? "Nufinku" : "Nufika");
    delay(1000);
    defaultniHlaska();
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
   
   if (hodnoceni1 || hodnoceni2 || hodnoceni3) {
     int hodnoceni = hodnoceni1 ? 1 : (hodnoceni2 ? 2 : (hodnoceni3 ? 3 : 0));

      char hodnoticiZprava[16];
      char hodnoticiZPrava2[16];
      strcpy(hodnoticiZprava, vybranyNufik == NUFINKA ? "Nufinka:" : "Nufik: ");
      strcpy(hodnoticiZPrava2, hodnoceni == 1 ? "Super" : (hodnoceni == 2 ? "Neutral" : (hodnoceni == 3 ? "Hruza" : "-")));
      strcat(hodnoticiZprava, hodnoticiZPrava2);     

     Serial.println(hodnoticiZprava);
     
     lcd.clear();
     lcd.print(vybranyNufik == NUFINKA ? "Nufinka:" : "Nufik: ");
     lcd.setCursor(0,1);
     lcd.print(hodnoceni == 1 ? "Super" : (hodnoceni == 2 ? "Neutral" : (hodnoceni == 3 ? "Hruza" : "-")));
     sendRadio(hodnoticiZprava);
     
     delay(1000);
     defaultniHlaska();
   }
}
