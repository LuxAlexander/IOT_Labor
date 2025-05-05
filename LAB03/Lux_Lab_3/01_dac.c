#include <wiringPi.h>
#include <mcp4802.h>
#include <mcp3004.h>
#include <stdio.h>
#include <math.h>

#define PIN_BASE        200     // Basis für virtuelle DAC-Pins
#define SPI_DAC_CHANNEL 1       // SPI-Kanal 1 (CE1) für DAC
#define SPI_ADC_CHANNEL 0       // SPI-Kanal 0 (CE0) für ADC
#define ADC_PIN_BASE    100     // Basis für virtuelle ADC-Pins
#define MAX_VALUE       254     // Maximalwert für 8-Bit-DAC
#define DAC_CHANNEL_A   (PIN_BASE + 0)  // DAC Kanal A (virtuell Pin 120)

float convertToVoltage(int value) {
    return (float)value / (pow(2,10) - 1) * 3.3;
}
void generate_sawtooth(){
    //1.5 V (Peak to Peak)
    int value = 0;
    while (1) {
        analogWrite(DAC_CHANNEL_A, value);  // Wert an DAC schreiben
        int adcValue = analogRead(ADC_PIN_BASE + 0); // ADC-Kanal 0 lesen
        printf("DAC: %.2f | ADC: %.2f\n", convertToVoltage(value), convertToVoltage(adcValue)); // Ausgabe

        delayMicroseconds(300); // 10 ms Pause für stabile Ausgabe und Messung

        value++;
        if (convertToVoltage(adcValue) >1.5) {
            value = 0;
        }
    }
}

void generate_sinewave() {
    while (1) {
        float t = (float)millis() / 1000.0; // Zeit in Sekunden
        const double PI  =3.141592653589793238463;
        float voltage = 0.7 * sin(2 * PI * 7 * t) + 1.1;
        int dac_value = (int)((voltage / 3.3) * MAX_VALUE);
        
        analogWrite(DAC_CHANNEL_A, dac_value);
        int adcValue = analogRead(ADC_PIN_BASE + 0);
        
        printf("DAC: %.2f | ADC: %.2f\n", voltage, convertToVoltage(adcValue));
        
        delay(1); // ca. 1 ms Delay → ergibt ca. 1000 Samples/s
    }
}


int main() {
    wiringPiSetup();

    // Initialisiere DAC und ADC
    mcp4802Setup(PIN_BASE, SPI_DAC_CHANNEL);
    mcp3004Setup(ADC_PIN_BASE, SPI_ADC_CHANNEL);

    generate_sinewave();

    return 0;
}
// Kompilieren mit: gcc -o 01_dac 01_dac.c -lwiringPi -lmcp4802 -lmcp3004