#include "Keypad.h"
#include "config.h"
#include <LiquidCrystal.h> // importation de la librairie

AdafruitIO_Feed *analog = io.feed("mot-de-passe");
int LED_R=7;
int LED_V=10;
//variables keypad
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
const byte BORNE = 11;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// For Arduino Microcontroller
//byte rowPins[ROWS] = {9, 8, 7, 6}; 
//byte colPins[COLS] = {5, 4, 3, 2}; 

// For ESP8266 Microcontroller
//byte rowPins[ROWS] = {D1, D2, D3, D4}; 
//byte colPins[COLS] = {D5, D6, D7, D8}; 

// For ESP32 Microcontroller
byte colPins[COLS] = {12, 14, 18, 17};
byte rowPins[ROWS] = {35, 36, 5, 6}; 


String code;
String code_crypt;
int bon_code = 0;

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//variables liquid
const int RS=3; //declaration constante de broche
const int E=1; //declaration constante de broche
 
const int D4=38; //declaration constante de broche
const int D5=33; //declaration constante de broche
const int D6=9; //declaration constante de broche
const int D7=8; //declaration constante de broche 

LiquidCrystal lcd(RS,E,D4,D5,D6,D7);

int tab[6] = {RS,E,D4,D5,D6,D7};


void setup() {
  
  Serial.begin(115200);

  io.connect(); 

  analog->onMessage(handleMessage);

  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  analog->get();
  
  for(int k=0;k<6;k++) {
    pinMode(tab[k],OUTPUT);//pins 8-14 en sortie
  }

  lcd.begin(16,2); //on initialise la communication avec 16 colonnes et deux lignes
  lcd.clear(); // on efface l'écran
  lcd.setCursor(0, 0);
  lcd.print("   BIENVENUE");
  delay(2000);
  lcd.clear();
  lcd.print("ENTREZ CODE:");
  lcd.setCursor(0,1);
}

void validerCode(){
  if(String(bon_code) == code){
//LED VERT
    pinMode(LED_V,OUTPUT);
    digitalWrite(LED_V, HIGH);
    delay(3000);
    digitalWrite(LED_V, LOW);
    delay(1000);
//TONALITER DU SON
    pinMode(BORNE, OUTPUT);
    tone(BORNE, 800,1000);
    Serial.println("Entrez connasse!!");
    delay(500);
    lcd.clear();
    delay(500);
    lcd.setCursor(0,0);
    delay(500);
    lcd.print("BON CODE");
    delay(2000);
    lcd.setCursor(0,1);
  }else{
//LED ROUGE
    pinMode(LED_R,OUTPUT);
    digitalWrite(LED_R, HIGH);
    delay(3000);
    digitalWrite(LED_R, LOW);
    delay(1000);
//TONALITER DU SON ERREUR
    pinMode(BORNE, OUTPUT);
    tone(BORNE, 800,500);
    Serial.println("Mauvais code !!");
    delay(500);
    lcd.clear();
    delay(500);
    lcd.setCursor(0,0);
    lcd.print("MAUVAIS CODE");
    delay(2000);
    lcd.setCursor(0,1);
  }
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ENTREZ CODE:");
  lcd.setCursor(0,1);
  
  code = String();
  code_crypt = String();
}

void supprimerCode(char lettre){
  code_crypt = String();
  if(lettre == 'C'){
    String code_replace;
    int i;
    for(i = 0;i < code.length() - 1;i++){
      code_replace = code_replace + code[i];
      code_crypt = code_crypt + '*';
    }
    code = code_replace;
  }else{
    code = String();
  }
  
}


void loop() {
  io.run();
  char key = keypad.getKey();
  if (key){
    if(key == 'A'){
      validerCode();
    }
    else if(key == 'B'){
      
    }else if(key == 'C'){
      supprimerCode('C');
    }else if(key == 'D'){
      supprimerCode('D');
    }else if(key == '*' || key == '#'){
      
    }else{
      if(code.length() < 4){
        code = code + key;
        code_crypt = code_crypt + '*';
      }
    }
    
    Serial.println(code_crypt);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ENTREZ CODE:");
    lcd.setCursor(0,1);
 //lcd.print(code_crypt);
    lcd.print(code);
  }
}

void handleMessage(AdafruitIO_Data *data) {

  bon_code = data->toInt();

  Serial.println(bon_code);
  
}
