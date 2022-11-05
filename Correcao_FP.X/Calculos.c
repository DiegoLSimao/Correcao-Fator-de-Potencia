#include "Calculos.h"
#include "main.h"
#include <delays.h>
#include <math.h>
#include <timers.h>


//*** Calcula o fator de potencia
unsigned int calcula_fat_pot(void)
{
    int FP,i;
    unsigned int atraso_ms, MediaLocal,aux=0,Contador=0;
    float fi_rad,FP_Arredonda;
    unsigned long int Total;

/*********************************************************************
CONFIGURA��O DO TIMER1 E DA ENTRADA DO SINAL SINCRO NO PORT A
*********************************************************************/
   // TRISAbits.RA0 = 1;
    OpenTimer1(TIMER_INT_OFF    & //Disable TIMER Interrupt
               T1_16BIT_RW      & //Enables register read/write of Timer1 in one 16-bit operation
               T1_SOURCE_INT    & //Internal instruction cycle clock (CLKO) acts as source of clock
               T1_PS_1_2       & //1:2 prescale value
               T1_OSC1EN_OFF   ); //Timer 1 oscilator enable off

/*********************************************************************
AQUISI��O DO SINAL DE SINCRONISMO
*********************************************************************/
        
        // Faz duas vezes para garantir correta inicializa��o do timer, por n�o se saber em que n�vel estar� qdo a fun��o for chamada
         Contador=0;
        for (i=0;i<1000;i++) // Faz 100 vezes para garantir uma bia m�dia de valores e evitar picos na leitura (ficar� preso por aproximados 1,67s)
         {
            //*** Verifica estouro do Timer 0, dever� ocorrer a cada 25ms
                if(INTCONbits.TMR0IF)
                {
                    Timer_T1++;       // Auxiliar para base de tempo 1
                    Timer_T2++;       // Auxiliar para base de tempo 2
                    Timer_T3++;       // Auxiliar para base de tempo 3
                    Timer_Capacitor++;       // Auxiliar para base de tempo 4
                    WriteTimer0(0x9E58);    //Valor inicial do Timer 0
                    INTCONbits.TMR0IF = 0; //Limpa flag de estouro do timer
                }

           LED_IND_LCD = ~LED_IND_LCD;   //Troca estado do led sempre qeu passar por aqui
           
            while(!SINAL) // Fica preso aqui enquanto RA0 em nivel l�gico baixo
            {
                continue;
            }

            WriteTimer1(0); // zera valor do timer 1
            aux=0;
            while(SINAL) // Fica preso aqui enquanto RA0 em nivel l�gico alto
            {  
                aux = ReadTimer1(); // Armazena em result o tempo em que RA0 esteve em n�vel l�gico alto
            }

            if (aux > 1000) // Conta somente se o tempo for maior que 1ms
            {
                Total += aux;
                Contador++;
            }
        }
	WriteTimer1 (0); // zera valor do timer 1
	CloseTimer1();  // fecha timer 1

    MediaLocal = Total/Contador; // faz a m�dia dos valores com tempo maior que 1ms


    //*** A constante 8333 � o tempo de dura��o de um semiciclo da senoide da rede em 60Hz, ou seja, 8333us
    atraso_ms = 8333 - MediaLocal; // Armazena o complemento de result
      
    if (atraso_ms ==8333) atraso_ms=0;
//******************************************************************************
//**************************************************C�LCULO DO FATOR DE POT�NCIA
//******************************************************************************
/*
Exemplo para um angulo de 45�, igual ao material do Jos� Luiz,
***Regra de tr�s
    2*pi     --- (1/60)(s)
    fi_rad   ---  atraso_ms
***Isolando fi_rad
     fi_rad = (2*pi*atraso_ms)/(1/60)(s)
     fi_rad = 120*pi*atraso_ms
     fi_rad = 377*atraso_ms
 * atraso_ms vir� como inteiro, ou seja, se for 2,080ms vir� 2080, ent�o para
 * igualar as unidades de tempo, transformamos para segundo devemos dividir
 * por 1 milh�o ou 1E6. logo podemos escrever a equa��o da seguinte forma
 * fi_rad = (0.377 * atraso_ms)/1000
 * atraso_ms=2083
 * fi_rad = (0.377 * 2083)/1000
 * fi_rad = (0.377 * 2083)/1000
 * fi_rad = 0.78529

*/
    Tempous = atraso_ms; // Armazena em uma vari�vel global para mostrar no display
    fi_rad = (atraso_ms*0.377)/1000.0; // Calcula o valor em radianos do angulo medido
    Angulo = (fi_rad*180.0)/PI;   // Converte em graus o angulo de defasagem
    FP_Arredonda = round((1000.0*cos(fi_rad))* 10.0); // Tira o cosseno do angulo e arredonda, faz as devidas manipula��es para a maneira que ser� exibido no lcd
    FP = (unsigned int)(FP_Arredonda); // Converte para inteiro

    return(FP);
}// fecha fat_pot


void Config_Timer0() //*** Fun��o respos�vel por garantir uma base de tempo
{
       // INTCONbits.TMR0IE = 1;  // Habilita interrup��o do Timer 0
        INTCONbits.TMR0IF = 0;  // Limpa flag de interrup��o
        //INTCON2bits.TMR0IP = 1; // Alta prioridade para interrup��o do timer 0
/*
        // Bits do registrador T0CON
         T0CONbits.TMR0ON = 1;  //*Liga o timer
         T0CONbits.T08BIT = 1;  //*Configura como temporizador de 8bit
         T0CONbits.T0CS = 0;    //*Clock interno CLK0
         T0CONbits.T0SE = 0;    //*Borda de subida
         T0CONbits.PSA = 0;     //*habilita prescaller
         T0CONbits.T0PS2=0;     //bit 2 Prescaller
         T0CONbits.T0PS1=1;     //bit 1 Prescaller
         T0CONbits.T0PS2=0;     //bit 0 Prescaller
         //prescaller 1:256
        //T0CON = 0b11000001;
*/
         OpenTimer0(TIMER_INT_OFF & T0_16BIT & T0_SOURCE_INT & T0_EDGE_RISE &  T0_PS_1_2 );
         WriteTimer0(0x9E58); //40536 Valor inicial do Timer
         //TMR0H = 0x9E;
         //TMR0L = 0x58;


/*Calculo da Base de tempo
 * O clock do oscilador � de 8MHz e uma instru��o em nivel de maquina � executada a cada 4 ciclos
 * ent�o o processamento interno ocorre a cada 2Mhz, o inverso da frequencia � o tempo 1/2MHz = 500ns,
 * logo, o tcy(tempo de ciclo de maquina) � de 500ns. Como o prescaller configurado � de 1 : 2
 * ent�o o timer ser� incrementado a cada (2 x 500ns) 1us.
 * O  estouro do registrador ocorrer� da transi��o de (0xFFFF -> 0x0000), ou seja,
 * como o temporizador � de 16 bits, um estouro ocorrer� a cada (1us x (65536-40536) = 25ms.
 */
}
/*
unsigned int Calcula_Mediana()
{
    char i,j;
    unsigned int temp, Mediana;

    //*** Ondena numeros em ordem crescente
    for(i=0;i<100;i++)
    {
        for(j=0;j<100;j++)
        {
            if(Valores[j] > Valores[j+1])
            {
                temp = Valores[j];
                Valores[j] =Valores[j+1];
                Valores[j+1]=temp;
            }
        }
    }




    //Mediana = (Valores[49] + Valores[50])/2;
   // Indice=0;

   // Flag=1;  // Permite atualizar valor na tela

    if(Flag==1)
     return(Mediana);
    else
        return(0);
}
 */