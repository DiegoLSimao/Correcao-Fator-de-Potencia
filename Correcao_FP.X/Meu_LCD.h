/* 
 * File:   Meu_LCD.h
 * Author: Diego de Lima Sim�o
 * Versao:                0.1
 * MPLAB X IDE:		  v2.25 
 * Compilador:            XC8 v1.32
 * Created on 4 de Mar�o de 2017, 12:25
 * Descri��o:  Usado para controlar o LCD 16x2 comunicando a 4 bits
 */
#ifndef Meu_LCD_H
#define	Meu_LCD_H

//******************************************************************************
//**********************************************************prot�tipo de fun��es
//******************************************************************************

void lcd_inicia(void);
void lcd_limpa_tela(void);
void lcd_port(unsigned char);
void lcd_comando(unsigned char);
void lcd_posicao_cursor(char, char);
void lcd_escreve_caractere(char);
void lcd_escreve_string(char *str);
void Meu_Caractere(void);   //*** Caracteres personalizados
void Pulso_EN(void);


#endif