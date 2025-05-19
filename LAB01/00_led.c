#include <wiringPi.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int red; // = HIGH or LOW
    int green; // = HIGH or LOW
    int blue; // = HIGH or LOW
    char name[9];
} Color;

int main ()
{
    wiringPiSetup ();
    const int led_red = 0; // WiringPi pin 1 equals pin 18 on the GPIO header
    const int led_blue=2;
    const int led_green=3;
    Color array[8]={};
    /*RED = 17
    GREEN = 27
    BLUE = 22 */
    
    array[0].red=1;
    array[0].green=1;
    array[0].blue=1;
    strcpy(array[0].name,"white");

    array[1].red=0;
    array[1].green=0;
    array[1].blue=0;
    strcpy(array[1].name,"off");

    array[2].red=1;
    array[2].green=0;
    array[2].blue=0;
    strcpy(array[2].name,"red");

    array[3].red=0;
    array[3].green=1;
    array[3].blue=0;
    strcpy(array[3].name,"green");

    array[4].red=0;
    array[4].green=0;
    array[4].blue=1;
    strcpy(array[4].name,"blue");

    array[5].red=1;
    array[5].green=1;
    array[5].green=0;
    strcpy(array[5].name,"gelb");

    array[6].red=0;
    array[6].green=1;
    array[6].blue=1;
    strcpy(array[6].name,"cyan");

    array[7].red=1;
    array[7].green=0;
    array[7].blue=1;
    strcpy(array[7].name,"pink");


    pinMode (led_red, OUTPUT); // Set pin as output
    pinMode(led_blue,OUTPUT);
    pinMode(led_green,OUTPUT);

    for(int i=0;i<8;i++){
        printf("Color: %s\n",array[i].name);
        if(array[i].red==1){
            digitalWrite (led_red, HIGH); // Set pin high (output voltage 3.3V)
        }
        if(array[i].blue==1){
            digitalWrite (led_blue, HIGH); // Set pin high (output voltage 3.3V)
        }
        if(array[i].green==1){
            digitalWrite(led_green,HIGH);
        }
        
        delay(2000); // Wait 500 ms
        digitalWrite (led_blue, LOW); // Set pin low (output voltage 0.0V)
        digitalWrite (led_green, LOW); // Set pin low (output voltage 0.0V)
        digitalWrite (led_red, LOW); // Set pin low (output voltage 0.0V)
        delay(1); // Wait 500 ms

    }

    pinMode (led_red, INPUT); // Set pin as input
    pinMode(led_blue,INPUT);
    pinMode(led_green,INPUT);

    return 0;
}