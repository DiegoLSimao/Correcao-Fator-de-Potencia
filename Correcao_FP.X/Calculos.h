/* 
 * File:   Calculos.h
 * Author: diego
 *
 * Created on 8 de Julho de 2017, 09:33
 */

#ifndef CALCULOS_H
#define	CALCULOS_H

#define PI 3.1415926
#define SINAL PORTAbits.RA0

unsigned int calcula_fat_pot(void);        // Rotina de calculo do fator de potencia
unsigned int Valor_Medio(void);     /* Função faz a media de 10 para melhorar a precisão do valor exibido*/
void Config_Timer0();   // Configuração do Timer 0 para bases de tempo
unsigned int Calcula_Mediana(void); // Função calcula mediana dos valores lidos

#endif	/* CALCULOS_H */

