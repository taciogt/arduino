#include <LiquidCrystal.h>

const int RS = 12, EN = 11, D4 = 10, D5 = 9, D6 = 8, D7 = 7;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

const int pinHygrometer = A0;

int umidity;
float umidity_percentage; 

void setup() {
    Serial.begin(9600);

    pinMode(pinHygrometer, INPUT);

    lcd.begin(16, 2);

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Kit Avancado RC");

    Serial.println("starting lcd");
    Serial.println("starting umidity sensor");
}

void loop() {
    umidity = analogRead(pinHygrometer);
    Serial.print("umidity: ");
    Serial.println(umidity);
    umidity_percentage = float(1023-umidity)/1023;
    Serial.print("umidity percentage: ");
    Serial.println(umidity_percentage);

    // Serial.println(umidity);

    unsigned long tempo_decorrido = millis() / 1000;

    lcd.setCursor(0, 1);
    lcd.print(tempo_decorrido);
    lcd.print("s");

    lcd.setCursor(8, 1);
    lcd.print("        ");
    lcd.setCursor(8, 1);
    lcd.print("um: ");

    lcd.print(int(umidity_percentage*100));
    lcd.print("%");


    delay(1000);
}