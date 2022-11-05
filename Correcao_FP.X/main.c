/*********************************************************************
*Nome do arquivo:	main.c
*Data:	    		27 de fevereiro de 2017
*Versao:                1.0
*MPLAB X IDE:		v2.25 
*Compilador:            XC8 v1.32
*Autor:			Diego de Lima Simão
*********************************************************************/
#include <p18f4550.h>		
#include <timers.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <portb.h>
#include "Cfg_Bit_Set.h"
#include "Calculos.h"
#include "Formata_Str.h"
#include "main.h"
#include "Selecao_Cap.h"
#include "Meu_LCD.h"
#include "ExecutaMaquinaEstados.h"


//*** Programa Pincipal
void main (void)
{
        //*** Inicia Variáveis
        Cont_Cap1 = Cont_Cap2 = Cont_Cap3 = Cont_Cap4 = 0 ;
        Timer_Capacitor = 0;
        Lista_Estados=0;
       

        //*** Funções de inicialização
	Inicia_Registradores();      // Inicia registradores
        Config_Timer0();            // Configura Timer0      
        lcd_inicia();               // Inicia LCD


        //*** Loop infinito  Principal
	while (1)
	{
            if(!BOTAO_SELECT) Lista_Estados=10;  //Começa executar um auto teste no lcd e nos led
            Estouro_Timer0();               // Verifica estouro do Timer 0
            Indicador_Main();               // Troca estado do led indicador principal
            ExecutaMaquinaEstados();        // Executa maquina de estados
	} // fecha while
}// fecha main

void Inicia_Registradores()
{
   //*** desabilita USB
    //UCONbits.USBEN = 0;
    //UCFGbits.UTRDIS=1;

    //*** Inicia registradores
      ADCON1 = 0x0F;                       //configura PORTB com I/O digitais
      TRISA = 0b00000001;                  //Define RA0 Como entrada
      TRISB = 0x04;                        //RB2 como entrada, demais Port B como saída
      TRISC=0;
      TRISC=TRISD=TRISE=0;                 //PORTB,C,D,E saida     
      PORTA=PORTB=PORTC=PORTD=PORTE=0x00;     // Limpa todos os PORTs
}


void Habilita_Interrupcoes()
{
        INTCONbits.GIE=1;      //  bit de interrupção global- 1 permite a execução das interrupções
        INTCONbits.PEIE = 1;    // Habilita interrupção dos periféricos
}

void Desabilita_Interrupcoes()
{
        INTCONbits.GIE=0;      //  bit de interrupção global- 1 permite a execução das interrupções
        INTCONbits.PEIE = 0;    // Habilita interrupção dos periféricos
}

//*** Função incrementa variaveis base de tempo a cada 25ms
void Estouro_Timer0()
{
    //*** Verifica estouro do Timer 0, deverá ocorrer a cada 25ms
    if(INTCONbits.TMR0IF)
    {
        Timer_T1++;       // Auxiliar para base de tempo 1
        Timer_T2++;       // Auxiliar para base de tempo 2
        Timer_T3++;       // Auxiliar para base de tempo 3
        Timer_Capacitor++;       // Auxiliar para base de tempo 4
        WriteTimer0(0x9E58);    //Valor inicial do Timer 0
        INTCONbits.TMR0IF = 0; //Limpa flag de estouro do timer
    }
}

//*** Função troca estado do led indicador do funcionamento do loop principal
void Indicador_Main()
{
    //***Indicador visual de que o loop esta ativo
    if(Timer_T2)
    {
        LED_IND_LCD = 0;    //Apaga indicador do LCD para que o mesmo pisque somente quando realmente estiver escrevendo no LCD
        LED_IND_MAIN = ~LED_IND_MAIN; //Troca estado do led, indicador que o loop esta funcionando
        Timer_T2 = 0;
    }

}