#include "stm8s.h"
#include "milis.h"
#include "stm8_hd44780.h"
#include "spse_stm8.h"
#include "delay.h"
#include <stdio.h>
#define _ISOC99_SOURCE
#define _GNU_SOURCE

void ADC_init(void){
// na pinech/vstupech ADC_IN2 (PB2) a ADC_IN3 (PB3) vypneme vstupní buffer
ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL2,DISABLE);
// nastavíme clock pro ADC (16MHz / 4 = 4MHz)
ADC2_PrescalerConfig(ADC2_PRESSEL_FCPU_D4);
// volíme zarovnání výsledku (typicky vpravo, jen vyjmečně je výhodné vlevo)
ADC2_AlignConfig(ADC2_ALIGN_RIGHT);
// nasatvíme multiplexer na některý ze vstupních kanálů
ADC2_Select_Channel(ADC2_CHANNEL_2);
// rozběhneme AD převodník
ADC2_Cmd(ENABLE);
// počkáme než se AD převodník rozběhne (~7us)
ADC2_Startup_Wait();
}

void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz
    init_milis();
    lcd_init();
    ADC_init();
}


int main(void)
{
    init();
    uint32_t timeA = 0;
    uint16_t adc_value;
    char text[32];
    int16_t voltage1;
    int16_t teplota1;
    int16_t teplota2;

    while (1) {
        if (milis()-timeA>100){
            timeA=milis();
            adc_value = ADC_get(ADC2_CHANNEL_2); // do adc_value ulož výsledek převodu vstupu ADC_IN2 (PB2)
            voltage1 = ((uint32_t)adc_value*5000 + 512)/1024;
            teplota1=voltage1/10;
            teplota2=voltage1%10;

        }
        lcd_gotoxy(0, 0);
        sprintf(text,"Teplota = %2u %1u C",teplota1,teplota2);
        lcd_puts(text);
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
