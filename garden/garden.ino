// #include <LiquidCrystal.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int RS = 12, EN = 11, D4 = 10, D5 = 9, D6 = 8, D7 = 7;
// LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
LiquidCrystal_I2C lcd(0x27, 16, 2);

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
bool soil_is_dry = false;

enum State { Monitoring, Cooldown, Irrigate };
enum State current_state;

long monitoring_start = 0;
long irrigation_start = 0;
long cooldown_start = 0;
// const int irrigation_limit_time = 5;


void setup() {
    Serial.begin(9600);

    pinMode(pin_hygrometer, INPUT);
    pinMode(pin_is_wet, OUTPUT);
    pinMode(pin_is_humid, OUTPUT);
    pinMode(pin_is_dry, OUTPUT);
    pinMode(pin_open_valve, OUTPUT);

    lcd.init();

    lcd.setCursor(0, 0);
    lcd.print("Monitor Jardim");

    Serial.println("starting lcd");
    Serial.println("starting umidity sensor");
    // current_state = Monitoring;
    startState(Monitoring, &monitoring_start);
}

void loop() {
    lcd.setBacklight(HIGH);
    lcd.clear();

    printStatus(current_state);

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

    soil_is_dry = false;
    if (humidity_percentage > max_umidity) {
        Serial.println("Soil is wet");
        digitalWrite(pin_is_wet, HIGH);
        digitalWrite(pin_is_humid, LOW);
        digitalWrite(pin_is_dry, LOW);
        digitalWrite(pin_open_valve, LOW);

        current_state = Monitoring;
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
        soil_is_dry = true;
        digitalWrite(pin_is_wet, LOW);
        digitalWrite(pin_is_humid, LOW);
        digitalWrite(pin_is_dry, HIGH);

        if (!valve_is_open) {
            digitalWrite(pin_open_valve, HIGH);
        }
    }

    if (current_state == Irrigate) {
        long time_irrigating = (millis() - irrigation_start) / 1000;
        if (time_irrigating > maximum_valve_open_seconds) {
            startState(Cooldown, &cooldown_start);
        }
    }
    else if (current_state == Cooldown) {
        long time_cooldown = (millis() - cooldown_start) / 1000;
        if (time_cooldown > 10) {
            startState(Monitoring, &monitoring_start);
        }
    } else if (current_state == Monitoring and soil_is_dry) {
        startState(Irrigate, &irrigation_start);
    }

    Serial.print("state=");
    Serial.println(current_state);

    delay(1000);
}

void printStatus(enum State state) {
    lcd.clear();
    int seconds_lapsed;

    lcd.setCursor(0, 0);
    if (state == Monitoring) {
        lcd.print("Monitor Jardim");
        seconds_lapsed = secondsLapsedSince(monitoring_start);
    }
    else if (state == Irrigate) {
        lcd.print("Irrigando");
        seconds_lapsed = secondsLapsedSince(irrigation_start);
        Serial.print("irrigation start: ");
        Serial.println(irrigation_start);
    }
    else if (state == Cooldown) {
        lcd.print("Cooldown");
        seconds_lapsed = secondsLapsedSince(cooldown_start);
    }

    lcd.setCursor(0, 1);
    lcd.print(seconds_lapsed);
    lcd.print("s");
    // char *seconds_str;
    // setTimeString(seconds_str, seconds_lapsed);
    // lcd.print(*seconds_str);


    lcd.setCursor(8, 1);
    lcd.print("um: ");
    lcd.print(humidity_percentage);
    lcd.print("%");
}

void startState(enum State new_state, long *start_time) {
    current_state = new_state;
    *start_time = millis();
}

long secondsLapsedSince(long start_time) {
    return (millis() - start_time) / 1000;
}

void setTimeString(char *str, long time_seconds){
    sprintf(str, "%d ss", time_seconds);

    // return "abc";
}