#include "mbed.h"
#include "C12832.h"
 
C12832 lcd(p5, p7, p6, p8, p11); 
DigitalOut led1(LED1);
DigitalOut led2(LED2);

Serial uart(p9, p10);

int main() {
    lcd.cls();
    lcd.locate(0,3);
    lcd.printf("mbed application board!");
    while (1){
        int c = 0;
        c = uart.getc();
        wait(0.002);
        if(c == 1){
            lcd.cls();
            lcd.locate(0,15);
            lcd.printf("%d", c);
            led1 = !led1;
        }
        if(c == 2){
            lcd.cls();
            lcd.locate(0,15);
            lcd.printf("%d", c);
            led2 = !led2;
        }
    }
}