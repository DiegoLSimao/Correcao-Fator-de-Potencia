
/*
 * File:   Meu_LCD.c
 * Author: Diego de Lima Simão
 * Versao:                0.1
 * MPLAB X IDE:		  v2.25
 * Compilador:            XC8 v1.32
 * Created on 4 de Março de 2017, 12:25
 * Descrição:  Usado para controlar o LCD 16x2 comunicando a 4 bits no modo somente saida
 */

#include <stdio.h>
#include <stdlib.h>
#include <p18f4550.h>
#include "Meu_LCD.h"
#include "main.h"

//*** mapeamento do hardware
#define RS_PIN		LATCbits.LATC0 //Pino RB5 --> Register Select do lcd
#define E_PIN		LATCbits.LATC1 //Pino RB4 --> Enable do lcd
#define D4              PORTDbits.RD4
#define D5              PORTDbits.RD5
#define D6              PORTDbits.RD6
#define D7              PORTDbits.RD7 // Pino RC3 Não implemntado no PIC18F4550


//******************************************************************************
//***************************************************Desenvolvimento das funções
//******************************************************************************
//*** Preparação do port
void lcd_port(unsigned char  dado)
{
    
    D4=D5=D6=D7=0;
    if(dado&0x01)  D4 = 1;
    if(dado&0x02)  D5 = 1;
    if(dado&0x04)  D6 = 1;
    if(dado&0x08)  D7 = 1;
    return;
}
//***Envio de comandos
void lcd_comando(unsigned char cmd)
{
    RS_PIN = 0 ;    //D0 a D7 tratado como comando do lcd
    lcd_port(cmd);  // coloca comando no port
    Pulso_EN();
    return;
}

//*** Limpa atela do lcd
void lcd_limpa_tela(void)
{
    //*** envia comando 0x01
    lcd_comando(0x00);
    lcd_comando(0x01);
    Delay10KTCYx(10);//50ms
    return;
}

//*** Inicia lcd
void lcd_inicia(void)
{
    E_PIN=0;
    RS_PIN = 0 ;


    Delay1KTCYx(50); //(25ms) recomendação fabricante, estabilizar tensão, pg 46 datasheet

    //*** Fabricante 3 X 0x03
    lcd_comando(0x03);
    Delay1KTCYx(10); //pg 46, Esperar mais que 4.1ms
    lcd_comando(0x03);
    Delay10TCYx(35);  //pg 46, Esperar mais que 100us
    lcd_comando(0x03);
    Delay10TCYx(10);  //pg 46, Esperar mais que 40us

    //*** Comandos
    lcd_comando(0x02); //cursor home 2
    Delay10TCYx(8);  // 40us

    lcd_comando(0x02); // seleção de função 0x28
    lcd_comando(0x08); // Modo duas linhas à 4 bits
    Delay10TCYx(8);  // 40us,

    lcd_comando(0x00); // Comando 0x0C
    lcd_comando(0x0C); // Display ON
    Delay10TCYx(8);  // 40us

    lcd_comando(0x00); // COmando 0x06
    lcd_comando(0x06); // Modo entrada de caractere, incremento ON
    Delay10TCYx(8);  // 40us
    return;
}

//*** Escreve caractere
void lcd_escreve_caractere(char chr)
{
    char nibble_sup , nibble_inf;

    //*** Prepara nibbles
    nibble_sup = chr&0xF0;
    nibble_inf = chr&0x0F;

    //*** Escreve nibble superior
    lcd_port(nibble_sup>>4);
    RS_PIN = 1; //D0 a D7 tratado como dados
    Pulso_EN();

    //*** Escreve nibble inferior    
    lcd_port(nibble_inf);
    RS_PIN = 1; //D0 a D7 tratado como dados
    Pulso_EN();

    return;  
}

//*** Escreve string
void lcd_escreve_string(char *str)
{
    int i;
    for(i=0;str[i]!='\0';i++)
        lcd_escreve_caractere(str[i]);

    return;
}


//*** Deslocamento para direita
void lcd_desloca_direita(void)
{
    lcd_comando(0x01);
    lcd_comando(0x0C);
    return;
}

//*** Deslocamento para esquerda
void lcd_desloca_esquerda(void)
{
    lcd_comando(0x01);
    lcd_comando(0x08);
    return;
}

//*** Define posição do cursor
void lcd_posicao_cursor(char L, char C)
{
    unsigned char nibble_sup , nibble_inf,aux;

    if(!L)  // Se linha 0
    {
            aux = 0x80 + C;
            nibble_sup = aux>>4;
            nibble_inf = aux & 0x0F;
            lcd_comando(nibble_sup);
            lcd_comando(nibble_inf);
    }
    else // se linha 1
    {
            aux = 0xC0 + C;
            nibble_sup = aux>>4;
            nibble_inf = aux & 0x0F;
            lcd_comando(nibble_sup);
            lcd_comando(nibble_inf);
    }
    return;
}

//*** Caracteres personalizados
void Meu_Caractere(void)
{
    const unsigned short MeuChar[] = {
        0x0E,0x10,0x12,0x15,0x15,0x15,0x0E,0x04,
        0x0E,0x10,0x12,0x15,0x15,0x15,0x0E,0x04,
        0x0E,0x10,0x12,0x15,0x15,0x15,0x0E,0x04,
        0x0E,0x10,0x12,0x15,0x15,0x15,0x0E,0x04,
        0x0E,0x10,0x12,0x15,0x15,0x15,0x0E,0x04,
        0x0E,0x10,0x12,0x15,0x15,0x15,0x0E,0x04,
        0x0E,0x10,0x12,0x15,0x15,0x15,0x0E,0x04,
        0x0E,0x10,0x12,0x15,0x15,0x15,0x0E,0x04};

    char i;
    lcd_comando(0x04);  //configura endereço CGRAM
    lcd_comando(0x00);  //configura endereço CGRAM

    for(i=0; i<63;i++)
    {
        lcd_escreve_caractere(MeuChar[i]);
    }
    //lcd_comando(0x00);
    lcd_comando(0x02);

}

void Pulso_EN() // Da um pulso no bit do Enable do LCD
{
    Delay1TCYx(2); // delay 1us
    E_PIN = 1;
    Delay1TCYx(2); // delay 1us
    E_PIN = 0;
}
