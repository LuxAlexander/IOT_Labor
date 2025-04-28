#include <wiringPi.h>
#include <stdio.h>
#include <math.h>
#include <mcp3004.h>

#define MCP3004_BASE 100
#define SPI_CHAN 0

float convertToVoltage(int value) {
    return (float)value / (pow(2,10) - 1) * 3.3;
}

double calcTemperatureNTC(float Uv) {
    if (Uv <= 0.001 || Uv >= 3.299) return 1e9; // protect against division by near zero denominator
    return 10000.0 * Uv / (3.3 - Uv);
}


double sthart(double r) {
    double a = 1.057071e-3;
    double b = 2.454913e-4;
    double c = 4.521886e-8;

    return 1 / (a + b * log(r) + c * pow(log(r), 3));
}

int main() {
    wiringPiSetup();
    mcp3004Setup(MCP3004_BASE, SPI_CHAN);
    
    int value;
    int time;
    FILE *fptr = fopen("ad_data_1_3.txt", "a+");
    if (fptr == NULL) {
        perror("Failed to open file");
        return 1;
    }

    for (int i = 0; i < 600; i++) {
        value = analogRead(MCP3004_BASE + 4); // reading from channel 4
        time = millis(); 
        double uc=convertToVoltage(value);
        double r = calcTemperatureNTC(uc);
        printf("Resistance R: %.2lf Ohms\n", r);
        double tempK = sthart(r);
        printf("Temperature: %.2lf K\n", tempK);
        double tempC = tempK - 273.15;
        printf("Temperature: %.2lf °C\n", tempC);
        //time u_{o} u_{ntc} r_{ntc} t_{ntc}
        fprintf(fptr, "%d %.2lf %f %.2lf %.2lf\n", time,3.3,uc,r, tempC);
        fflush(fptr);
        delay(100); // wait 0.1 sec
    }

    fclose(fptr);
    return 0;
}
