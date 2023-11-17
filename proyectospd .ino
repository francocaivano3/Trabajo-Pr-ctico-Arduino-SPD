#include <LiquidCrystal.h>

unsigned long delayMod = 2000;
int autom = 3;
String option = "";
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

unsigned long initialTime;
unsigned long elapsedTime;

int Menu = 0; // Inicializa valores para Menu
int estado = 0;
uint8_t Perfume[] ={B000001, B001010, B011111, B011010, B011001, B011000, B011100, B011100, B011100};

void setup() {

Serial.begin(9600);
 
lcd.begin(16,2);


pinMode(autom, OUTPUT);
  
lcd.createChar(7, Perfume);

lcd.clear();

lcd.setCursor(8, 1);
lcd.write(byte(7));

lcd.createChar(7, Perfume);
delay(1000);
lcd.clear();
lcd.setCursor(3, 0);
lcd.print ("Bienvenido!");
delay(2000);
  
lcd.begin(16, 2);
lcd.setCursor(0, 0);
  
lcd.print("15-11-2023"); 
}

int s=0;
int sec=0;
int hrs=0;
int minutes=0;
int initialHours = 00;
int initialMins = 2;
int initialSecs = 50;
 

int seconds()
{
s = initialHours*3600;
s = s+initialMins*60;
s = s+initialSecs;
s = s+(millis()/1000);
return s;
}

int hours()
{
hrs = seconds();
hrs = hrs/3600;
hrs = hrs%24;
return hrs;
}

int mins()
{
minutes = seconds();
minutes = minutes/60;
minutes = minutes%60;
return minutes;
}
 
int secs()
{
sec = seconds();
sec = sec%60;
return sec;
}

void loop()
{
digitalClockDisplay();
MenuPrincipal();
}

void printDigits(byte digits){
if(digits < 10)
lcd.print('0');
lcd.print(digits);
}
char sep()
{
s = millis()/1000;
if(s%2==0)
{
lcd.print(":");
}
else {
lcd.print(" ");
}
}
void digitalClockDisplay(){
lcd.setCursor(11,0);
printDigits(
hours());
sep();
printDigits(mins());
}



void MenuPrincipal() {
 int ValEstado = 0; // Inicializa valores para ValEstado
 int x = analogRead (0);
 lcd.setCursor(0,0); 
 if(Serial.available() != 0){
    option = Serial.readString();
    Serial.println("Bienvenido");
    Serial.println("Si quiere el modo automático ingrese autom");
   	Serial.println("Si quiere configurar el intervalo ingrese intervalo-perfume");
    if(option == "autom"){
      Serial.println("Usted a seleccionado el modo autom, si quiere salir escriba salir");
      automatico();
      option = "";
      Serial.println(">A salido del modo autom");
    }
    else if(option == "intervalo-perfume"){
      Serial.println("Usted a seleccionado el modo intervalo-perfume");
      Serial.println("Aparecera hora y minutos, primero debe ingresar la hora");
      Serial.println("Si quiere sumar una hora ingrese +1h, si quiere restar ingrese -1h");
      Serial.println("Para pasar a minutos tiene que escribir sig");
      Serial.println("Los minutos son iguales que la hora, +1m y -1m. Para finalizar escriba salir");
      modificarIntervalo();
      Serial.println(">Tiempo Confirmado");
      automatico();
    }
   
  }

 if (x < 80) { 
 } 
 else if (x < 200) { 
 ValEstado = 1;
 } 
 else if (x < 400){
 ValEstado = 2;
 }
 else if (x < 600){ 
 } 
 else if (x < 800){ 
 ValEstado = 3;
 }

 if (Menu < 0 || Menu > 3) {
 Menu = 0; 
 }

 if (ValEstado != estado) {
 if (ValEstado == 1) {
 Menu = Menu - 1; 
 displayMenu(Menu);
 } else if (ValEstado == 2) {
 Menu = Menu + 1; 
 displayMenu(Menu);
 } else if (ValEstado == 3) {
 selectMenu(Menu); 
 }
 estado = ValEstado;
 } 
 delay(5);
}

void displayMenu(int x) {
 switch (x) {
 case 1:
 clearPrintTitle();
 lcd.print ("-> Automatico <-"); 
 break;
 case 2:
 clearPrintTitle();
 lcd.print ("--> Interval <--"); 
 break;
}
}

void clearPrintTitle() {
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Perfume");
 lcd.setCursor(8,0);
 lcd.write(byte(7));
 lcd.setCursor(0,1); 
}

void selectMenu(int x) {
 switch (x) {
 case 1:
 clearPrintTitle();
 digitalClockDisplay();
 lcd.setCursor(0, 1);
 lcd.print ("Selec. Autom"); 
 delay(2000);
 lcd.setCursor(0, 1);
 lcd.print ("Confirmado Autom");
 delayMod=5000; //DEFAULT 1 minuto
 delay(1000);
 automatico();  
 delay(500);
 lcd.setCursor(0, 1);
 lcd.print ("  Salir Autom   ");
 delay(2000);
 break;
   
 case 2:
 clearPrintTitle();
 digitalClockDisplay();
 lcd.print ("Selec. Interval"); 
 delay(2000);
 modificarIntervalo();
 automatico(); //con valores modificados
   
 break;

}
  }

void automatico(){
  unsigned long tiempoAhora;
  int valEstado=analogRead(0);
  
  lcd.setCursor(0, 1);
  lcd.print ("Inicializando...");
  do
    {
  digitalClockDisplay();
  tiempoAhora = millis();

    
 while (millis() < tiempoAhora + delayMod && valEstado==1023 && option != "salir"){
    valEstado=analogRead(0);
    option = Serial.readString();
  }
  if (millis() >= tiempoAhora+delayMod){
    digitalWrite(autom, HIGH);
  lcd.setCursor(0, 1);
  lcd.print ("Lanzando perfume");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print ("-_-_-_-_-_-_-_-_");
  }

  if (millis() >= tiempoAhora){
    digitalWrite(autom, LOW);
  }
  
  } while (valEstado==1023 && option != "salir"); // valor ningun boton = 1023

}

void modificarIntervalo(){ 
 int botones;
 lcd.setCursor(0, 1);
 
  lcd.print (">Ingrese horas");
do 
{
  botones = analogRead(0);
  option = Serial.readString();
    if(botones >= 99 && botones <= 127 || option == "+1h")
    {
      delayMod = delayMod + 3600000;
      lcd.setCursor(0, 1);
      lcd.print(" + 1 hora       ");
      delay(1500);
    }
    if(botones >= 255 && botones <= 280 || option == "-1h")
    {
      if(delayMod < 3600000)
      {
        delayMod = delayMod;
      }
      else
      { 
        delayMod = delayMod - 3600000;
        lcd.setCursor(0, 1);
        lcd.print(" - 1 Hora       ");
        delay(1500);
      }
    }
 } while (botones<640 || botones>670 || option == "sig");
   
  lcd.setCursor(0, 1);
  lcd.print (">Ingrese min  ");
  delay(2000);
do
{
  botones = analogRead(0);
  	if(botones >=0 && botones <= 60 || option == "+1m")
    { 
      delayMod = delayMod + 60000;
      lcd.setCursor(0, 1);
      lcd.print(" + 1 Min         ");
      delay(1500);
    }
    if(botones >=399 && botones <= 435 || option == "-1m") 
    {
      if(delayMod < 60000)
      {
        delayMod = delayMod;
      }
      else 
      {
        delayMod = delayMod - 60000;
        lcd.setCursor(0, 1);
        lcd.print(" - 1 Min         ");
        delay(1500);
      }
     }
  }while (botones<640 || botones>670 || option == "salir");

  lcd.setCursor(0, 1);
  lcd.print (">Tiempo Confirm.");
  delay(2000);

 }
