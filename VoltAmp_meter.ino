#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3c // initialize with the I2C addres 0x3C
//#define i2c_Address 0x3d //initialize with the I2C addres 0x3D

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int voltage_divider_measurement_pin = A1;
float arduino_voltage = 5.105; // volatege beetwen 5V and GND on the Arduino board

int analogPin = A0; // Current sensor output

void setup()
{
    pinMode(voltage_divider_measurement_pin, INPUT);
    display.begin(i2c_Address, true); // Address 0x3C default
    display.clearDisplay();           // Clear the OLED buffer
}

void loop()
{
    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.println("V =");

    display.setCursor(60, 0);
    display.println(get_voltage());

    display.setCursor(0, 32);
    display.println("A =");

    display.setCursor(60, 32);
    display.println(get_current());

    display.display();
    delay(100);
}

float get_voltage()
{
    int get_volatege_value = 0;
    float vout = 0.0;
    float measured_voltage = 0.0;
    float R1 = 30900.0; // measured resistance of R1
    float R2 = 9300.0;  // measured resistance of R2

    get_volatege_value = analogRead(voltage_divider_measurement_pin);
    vout = (get_volatege_value * arduino_voltage) / 1024.0;
    measured_voltage = vout / (R2 / (R1 + R2));
    return measured_voltage;
}

float get_current()
{
    const int averageValue = 510;
    long int sensorValue = 0; // variable to store the sensor value read

    float voltage = 0;
    float measured_current = 0;

    for (int i = 0; i < averageValue; i++)
    {
        sensorValue += analogRead(analogPin);

        // wait 2 milliseconds before the next loop
        delay(2);
    }

    sensorValue = sensorValue / averageValue;
    voltage = sensorValue * 5.093 / 1024.0;
    measured_current = (voltage - 2.54) / 0.100;
    return measured_current;
}