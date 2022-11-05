/* 
 * File:   main.h
 * Author: diego
 *
 * Created on 8 de Julho de 2017, 09:41
 */

#ifndef MAIN_H
#define	MAIN_H

#define LED_IND_MAIN    PORTBbits.RB0
#define LED_IND_LCD     PORTBbits.RB1
#define BOTAO_SELECT    PORTBbits.RB2

void Inicia_Registradores();
void Habilita_Interrupcoes();
void Desabilita_Interrupcoes();
void Estouro_Timer0(void);
void Indicador_Main(void);

char Str_Gen[20]={0},Flag;
int Cont_Cap1,Cont_Cap2,Cont_Cap3,Cont_Cap4; // Contadores de acionameto dos capacitores
unsigned  Lista_Estados=0; // Controle do estado
float Angulo=0;
unsigned  Tempous=0, ValorGeral=0;
unsigned  Timer_T1=0,       // Variáveis auxiliares para base de tempo
          Timer_T2=0,
          Timer_T3=0;
unsigned  int  Timer_Capacitor = 0; // Inicio em 500 para que o capacitor possa ser acionado direto na primeira vez, sem ter que esperar 10segundosF

#endif	/* MAIN_H */

