#include <LiquidCrystal.h>

const int RS = 12, EN = 11, D4 = 10, D5 = 9, D6 = 8, D7 = 7;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

const int pin_hygrometer = A0;
const int pin_is_wet = PIN6;
const int pin_is_dry = PIN5;
const int pin_is_humid = PIN4;
const int pin_open_valve = PIN3;

int umidity;
float humidity_input;
int humidity_percentage = 0;

const int min_humidity = 40;
const int max_umidity = 50;

const int maximum_valve_open_seconds = 5 * 1;  // 5 seconds, TODO: change to 5 minutes
bool valve_is_open = false;


void setup() {
    Serial.begin(9600);

    pinMode(pin_hygrometer, INPUT);
    pinMode(pin_is_wet, OUTPUT);
    pinMode(pin_is_humid, OUTPUT);
    pinMode(pin_is_dry, OUTPUT);
    pinMode(pin_open_valve, OUTPUT);

    lcd.begin(16, 2);

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Monitor Jardim");

    Serial.println("starting lcd");
    Serial.println("starting umidity sensor");
}

void loop() {
    umidity = analogRead(pin_hygrometer);
    Serial.print("umidity: ");
    Serial.println(umidity);
    humidity_input = float(1023 - umidity) / 1023;
    humidity_percentage = int(humidity_input * 100);

    Serial.print("humidity input: ");
    Serial.println(humidity_input);
    Serial.print("umidity percentage: ");
    Serial.println(humidity_percentage);
    Serial.print("min_humidity: ");
    Serial.println(min_humidity);

    // Serial.println(umidity);

    unsigned long tempo_decorrido = millis() / 1000;



    if (humidity_percentage > max_umidity) {
        Serial.println("Soil is wet");
        digitalWrite(pin_is_wet, HIGH);
        digitalWrite(pin_is_humid, LOW);
        digitalWrite(pin_is_dry, LOW);
        digitalWrite(pin_open_valve, LOW);
    }
    else if (humidity_percentage > min_humidity) {
        Serial.println("Soil is humidy");
        digitalWrite(pin_is_wet, LOW);
        digitalWrite(pin_is_humid, HIGH);
        digitalWrite(pin_is_dry, LOW);
        digitalWrite(pin_open_valve, LOW);
    }
    else {
        Serial.println("Soil is dry");
        digitalWrite(pin_is_wet, LOW);
        digitalWrite(pin_is_humid, LOW);
        digitalWrite(pin_is_dry, HIGH);

        if (!valve_is_open) {
            digitalWrite(pin_open_valve, HIGH);

        }
    }


    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Monitor Jardim");

    lcd.setCursor(0, 1);
    lcd.print(tempo_decorrido);
    lcd.print("s");

    lcd.setCursor(8, 1);
    lcd.print("        ");
    lcd.setCursor(8, 1);
    lcd.print("um: ");

    lcd.print(humidity_percentage);
    lcd.print("%");


    delay(1000);
}