/* 
 * File:   Selecao_Cap.h
 * Author: diego
 *
 * Created on 8 de Julho de 2017, 09:40
 */

#ifndef SELECAO_CAP_H
#define	SELECAO_CAP_H

#include <p18f4550.h>

//*** Definições para facilitar
#define Cap1    PORTDbits.RD0
#define Cap2    PORTDbits.RD1
#define Cap3    PORTDbits.RD2
#define Cap4    PORTDbits.RD3
#define Liga 1
#define Desliga 0

void Seleciona_Capacitor(unsigned int);    // função que faz o chaveamento do capacitor de acordo com fator de potencia
char Capacitor_Menos_Acionado();   // Retorna o capacitor menos acionado


#endif	/* SELECAO_CAP_H */

