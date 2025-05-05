#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <math.h>
#include <errno.h>
#include <mcp3004.h>  // Für ADC
#include <mcp4802.h>  // Für DAC

#define MAX_BUFFER_SIZE 10000

#define PIN_BASE        200     // Basis für virtuelle DAC-Pins
#define SPI_DAC_CHANNEL 1       // CE1 für MCP4802
#define SPI_ADC_CHANNEL 0       // CE0 für MCP3008
#define ADC_PIN_BASE    100     // Basis für virtuelle ADC-Pins
#define DAC_CHANNEL_A   (PIN_BASE + 0)

#define MAX_ADC_CHANNEL 7       // MCP3008 hat 8 Kanäle (0–7)

// Ringbuffer
float timeBuffer[MAX_BUFFER_SIZE];
float voltageBuffer[MAX_BUFFER_SIZE];

// Konfigurierbare Parameter
int bufferSize = 1000;
float triggerLevel = 1.0;
float sampleDelayMs = 1.0;
int useACCoupling = 0;
int adcChannel = 0;

float read_voltage() {
    int value = analogRead(ADC_PIN_BASE + adcChannel);
    return (float)value / (pow(2, 10) - 1) * 3.3;
}

void write_output_file(float triggerTime) {
    freopen("out.txt", "w", stdout);
    printf("%.2f\t%.2f\n\n\n", triggerTime, triggerLevel);

    float dc = 0.0;
    if (useACCoupling) {
        for (int i = 0; i < bufferSize; i++) dc += voltageBuffer[i];
        dc /= bufferSize;
    }

    for (int i = 0; i < bufferSize; i++) {
        float t = timeBuffer[i];
        float v = voltageBuffer[i];
        if (useACCoupling) v -= dc;
        printf("%.2f\t%.2f\n", t, v);
    }
}

void run_scope() {
    int half = bufferSize / 2;
    int idx = 0;
    int triggerDetected = 0;
    float triggerTime = 0;
    int belowCount = 0;

    while (1) {
        float t = (float)millis();
        float v = read_voltage();

        timeBuffer[idx] = t;
        voltageBuffer[idx] = v;

        if (!triggerDetected) {
            if (v < triggerLevel) belowCount++;
            else belowCount = 0;

            if (belowCount >= 5) {
                triggerDetected = 1;
                triggerTime = t;
                idx = (idx + 1) % bufferSize;
                continue;
            }
        } else {
            if (idx == (half + half - 1)) {
                write_output_file(triggerTime);
                break;
            }
        }

        idx = (idx + 1) % bufferSize;
        delay(0.7);
    }
}

int main(int argc, char *argv[]) {
    // Argumente verarbeiten
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--buffersize=", 13) == 0) {
            bufferSize = atoi(argv[i] + 13);
            if (bufferSize > MAX_BUFFER_SIZE) bufferSize = MAX_BUFFER_SIZE;
        } else if (strncmp(argv[i], "--samples=", 10) == 0) {
            float ksps = atof(argv[i] + 10);
            if (ksps > 0) sampleDelayMs = 1000.0 / (ksps * 1000.0);
        } else if (strncmp(argv[i], "--trigger-level=", 16) == 0) {
            triggerLevel = atof(argv[i] + 16);
        } else if (strncmp(argv[i], "--coupling=", 11) == 0) {
            if (strcmp(argv[i] , "AC") == 0) useACCoupling = 1;
            else if (strcmp(argv[i], "DC") == 0) useACCoupling = 0;
        } else if (strncmp(argv[i], "--channel=", 10) == 0) {
            //adcChannel = atoi(argv[i] + 10);
            if (adcChannel < 0 || adcChannel > MAX_ADC_CHANNEL) {
                fprintf(stderr, "Ungültiger ADC-Kanal: %d\n", adcChannel);
                return 1;
            }
        } else {
            fprintf(stderr, "Unbekannte Option: %s\n", argv[i]);
        }
    }

    // wiringPi initialisieren
    if (wiringPiSetup() == -1) {
        fprintf(stderr, "Fehler bei wiringPiSetup: %s\n", strerror(errno));
        return 1;
    }

    // SPI-Geräte initialisieren
    if (mcp3004Setup(ADC_PIN_BASE, SPI_ADC_CHANNEL) == -1) {
        fprintf(stderr, "Fehler beim Initialisieren des MCP3008 (ADC)\n");
        return 1;
    }

    if (mcp4802Setup(PIN_BASE, SPI_DAC_CHANNEL) == -1) {
        fprintf(stderr, "Fehler beim Initialisieren des MCP4802 (DAC)\n");
        return 1;
    }

    // Übersicht
    printf("Start mit Parametern:\n");
    printf("  Buffersize     : %d Samples\n", bufferSize);
    printf("  Sample-Rate    : %.2f kS/s (%.2f ms Delay)\n", 1000.0 / sampleDelayMs / 1000.0, sampleDelayMs);
    printf("  Trigger-Level  : %.2f V\n", triggerLevel);
    printf("  Kopplung       : %s\n", useACCoupling ? "AC" : "DC");
    printf("  ADC-Kanal      : %d\n", adcChannel);

    run_scope();
    return 0;
}
