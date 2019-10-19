#include <VirtualWire.h>
#include <LiquidCrystal.h>
#include "ESP8266WiFi.h"

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

void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
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

void wifi() {
  Serial.println("Zahajeni skenovani..");
  // načtení WiFi sítí v okolí a uložení jejich počtu do proměnné
  int n = WiFi.scanNetworks();
  // v případě nulového počtu sítí vypíšeme informaci
  // po sériové lince
  if (n == 0) {
    Serial.println("Zadne viditelne WiFi site v okoli.");
  }
  // pokud byly nalezeny WiFi sítě v okolí,
  // vypíšeme jejich počet a další informace
  else  {
    Serial.print(n);
    Serial.println(" WiFi siti v okoli. Seznam:");
    // výpis všech WiFi sítí v okolí,
    // vypíšeme název, sílu signálu a způsob zabezpečení
    for (int i = 0; i < n; ++i)
    {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  // ukončení výpisu
  Serial.println("");
  // pauza po dobu pěti vteřin před novým skenováním
  delay(5000);
}

void setup() {
  lcd.begin(16,2);
  defaultniHlaska();

  // setupRadio();
  setupWifi();
  
  pinMode(tlacitkoNufici, INPUT_PULLUP);
  pinMode(tlacitko1, INPUT_PULLUP);
  pinMode(tlacitko2, INPUT_PULLUP);
  pinMode(tlacitko3, INPUT_PULLUP);

//  pinMode(diodaNufik, OUTPUT);
//  pinMode(diodaNufinka, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  wifi();
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
     //sendRadio(hodnoticiZprava);
     
     delay(1000);
     defaultniHlaska();
   }
}
