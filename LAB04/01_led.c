#include <stdio.h>
#include <wiringPi.h>
#include <mcp3004.h>
#include <mcp4802.h>

#define ADC_BASE 100
#define ADC_CH 0
#define DAC_BASE 200
#define DAC_CH 1
#define R_SHUNT 120       // Messwiderstand in Ohm
#define V_REF 3.3

float adcToVolt(int adc) {
    return (adc / 1023.0) * V_REF;
}

int main(void) {
    wiringPiSetup();
    mcp3004Setup(ADC_BASE, ADC_CH);
    mcp4802Setup(DAC_BASE, DAC_CH);

    FILE *fp = fopen("Gruen82Ohm.txt", "w");
    if (fp == NULL) {
        perror("Kannst nicht aufmachen weil fehlende Kompetenz");
        return 1;
    }

    fprintf(fp, "u_p u_o u_r i_mA adc_uo adc_ur dac_val r\n");
    printf("u_p u_o u_r i_mA adc_uo adc_ur dac_val r\n");

    for (int dacVal = 0; dacVal <= 255; dacVal++) {
        analogWrite(DAC_BASE, dacVal);
        delay(20);

        int adcUo = analogRead(ADC_BASE);       // z. B. CH0
        int adcUr = analogRead(ADC_BASE + 1);   // z. B. CH1

        float Uo = adcToVolt(adcUo);
        float Ur = adcToVolt(adcUr);
        float Up = Uo - Ur;
        float I_mA = (Ur / R_SHUNT) * 1000.0;

        fprintf(fp, "%.6f %.6f %.6f %.6f %d %d %d %d\n",
                Up, Uo, Ur, I_mA, adcUo, adcUr, dacVal, R_SHUNT);

        printf("%.6f %.6f %.6f %.6f %d %d %d %d\n",
               Up, Uo, Ur, I_mA, adcUo, adcUr, dacVal, R_SHUNT);
    }

    analogWrite(DAC_BASE, 0);
    fclose(fp);

    return 0;
}