/*********************************************************************
*Nome do arquivo:	ExecutaMaquinaEstados.c
*Data:	    		25 de novembro de 2017
*Versao:                1.0
*MPLAB X IDE:		v2.25
*Compilador:            XC8 v1.32
*Autor:			Diego de Lima Simão
*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <delays.h>
#include "Calculos.h"
#include "Formata_Str.h"
#include "main.h"
#include "Selecao_Cap.h"
#include "Meu_LCD.h"


void ExecutaMaquinaEstados(void)
{
    //*** Lista de estados do switch case
        enum 
        {            
            Uniso,                  //00
            Projeto,                //01
            Alunos,                 //02
            Aguarde,                //03
            Leitura_Do_FP,          //04
            Fator_De_Potencia,      //05
            Angulo_Tempo,           //06
            Aciona_Capacitor,       //07
            Travou_Em_Low_RA0,      //08
            Travou_Em_High_RA0,     //09
            TesteLCD,               //10
            TesteLED_Principal,
            TesteLED_LCD,
            Teste_Cap1,
            Teste_Cap2,
            Teste_Cap3,
            Teste_Cap4,
            Erro,
        };

 //*** Lista de execução dos estados
    switch(Lista_Estados)
    {
        char StrLocal[20]={0};
        char i=0, j=0;

        case Uniso:
             lcd_limpa_tela();
             lcd_posicao_cursor(0,1);
             lcd_escreve_string("Eng. Eletrica");
             lcd_posicao_cursor(1,2);
             lcd_escreve_string("UNISO - 2017");
             Delay10KTCYx(200);//1s
             Lista_Estados=Projeto;
        break;

        case Projeto:
            lcd_limpa_tela();
            lcd_posicao_cursor(0,0);
            sprintf(StrLocal,"FATOR D POTENCIA");
            lcd_escreve_string(StrLocal);
            lcd_posicao_cursor(1,0);
            sprintf(StrLocal,"BANCO CAPACITOR");
            lcd_escreve_string(StrLocal);
            Delay10KTCYx(200);//1s          
            Lista_Estados =Alunos;
        break;

        case Alunos:
            lcd_limpa_tela();            
            lcd_posicao_cursor(0,0);
            sprintf(StrLocal,"Diego Simao");
            lcd_escreve_string(StrLocal);
            lcd_posicao_cursor(1,0);
            sprintf(StrLocal,"Elnatan Minuci");
            lcd_escreve_string(StrLocal);
            Delay10KTCYx(200);//1s
            //Lista_Estados = Erro;
            Lista_Estados = Aguarde;
        break;

        case Aguarde:
            lcd_limpa_tela();
            lcd_posicao_cursor(0,3);
            sprintf(StrLocal,"AGUARDE...");
            lcd_escreve_string(StrLocal);
           Lista_Estados = Leitura_Do_FP;
        break;

        case Leitura_Do_FP:
            //*** Esta condição garante que o display seja atualizado somente se houver anteração de valor
            if(ValorGeral !=(calcula_fat_pot()))
           {
               ValorGeral = calcula_fat_pot();
                Lista_Estados = Fator_De_Potencia;
                break;
            }
            Lista_Estados = Aciona_Capacitor;
        break;


        case Fator_De_Potencia:
                Formata_Str(ValorGeral);              // Formata String do valor numérico a ser exibido no LCD
                lcd_posicao_cursor(0,0);                 // Posiciona cursor para linha 0, coluna 0
                sprintf(StrLocal,"FPt: %s",Str_Gen);     // Escreve Fator de potencia já formatado no vetor StrLocal
                lcd_escreve_string(StrLocal);
                //Delay10KTCYx(20);//1s
            Lista_Estados = Angulo_Tempo; // manda para o proximo estado
        break;

        case Angulo_Tempo:
                lcd_posicao_cursor(1,0);             // Posiciona Cursor para linha 1, coluna 0
                sprintf(StrLocal,"%.2f%c - %i%cs%c%c%c%c%c%c",Angulo,0xDF,Tempous,0xE4,0x20,0x20,0x20,0x20,0x20,0x20); //0xDF é o simbolo de grau / 0xE4 Micro / 0x20 caracter vazio em ascii
                lcd_escreve_string(StrLocal);
                Delay10KTCYx(20);//1s

            Lista_Estados = Aciona_Capacitor; // manda para o proximo estado
        break;

        case Aciona_Capacitor:
             Seleciona_Capacitor(ValorGeral); // Seleciona o capacitor para
             Lista_Estados = Leitura_Do_FP;  // manda para o proximo estado
        break;


        case Travou_Em_Low_RA0:
            lcd_limpa_tela();
            lcd_posicao_cursor(0,0);
            lcd_escreve_string("RA0 Travou low");
            lcd_posicao_cursor(1,0);
            lcd_escreve_string("Reset reinic");
            Lista_Estados = Erro;
       break;

       case Travou_Em_High_RA0:
            lcd_limpa_tela();
            lcd_posicao_cursor(0,0);
            lcd_escreve_string("RA0 Travou High");
            lcd_posicao_cursor(1,0);
            lcd_escreve_string("Reset reinic");
           // Lista_Estados = Erro;
       break;

        case TesteLCD:
            //Faz um auto teste do LCD Acendendo todos os segmentos
            for(i=0; i<2; i++)
            {
                for(j=0; j<16; j++)
                {
                    lcd_posicao_cursor(i,j);
                    lcd_escreve_caractere(0xFF);
                    Delay10KTCYx(10);//100ms
                    Delay10KTCYx(10);//100ms
                }
            }
            Lista_Estados = TesteLED_Principal;
        break;

        case TesteLED_Principal:
            // Testa os Leds
            PORTBbits.RB0 = 0;
            PORTBbits.RB1 = 0;
            lcd_limpa_tela();
            lcd_posicao_cursor(0,0);
            lcd_escreve_string("LED PRINCIPAL");
            PORTBbits.RB0 = 1;
            Delay10KTCYx(200);//1s
            Lista_Estados = TesteLED_LCD;
        break;

        case TesteLED_LCD:
            //***Led LCD
            lcd_limpa_tela();
            lcd_posicao_cursor(0,0);
            lcd_escreve_string("LED LCD");
            PORTBbits.RB0 = 0;
            PORTBbits.RB1 = 1;
            Delay10KTCYx(200);//1s
            Lista_Estados = Teste_Cap1;
        break;

        case Teste_Cap1:
            //***capacitor 1
            lcd_limpa_tela();
            lcd_posicao_cursor(0,0);
            lcd_escreve_string("Capacitor 1");
            PORTBbits.RB1 = 0;
            PORTDbits.RD0 = 1;

            for (i=0;i<5;i++)
            {
                Delay10KTCYx(200);//1s
            }
            Lista_Estados = Teste_Cap2;
        break;


        case Teste_Cap2:
            //***capacitor 2
            lcd_limpa_tela();
            lcd_posicao_cursor(0,0);
            lcd_escreve_string("Capacitor 2");
            PORTDbits.RD0 = 0;
            PORTDbits.RD1 = 1;
            for (i=0;i<5;i++)
            {
                Delay10KTCYx(200);//1s
            }
            Lista_Estados = Teste_Cap3;
        break;

        case Teste_Cap3:
            //***capacitor 3
            lcd_limpa_tela();
            lcd_posicao_cursor(0,0);
            lcd_escreve_string("Capacitor 3");
            PORTDbits.RD1 = 0;
            PORTDbits.RD2 = 1;
            for (i=0;i<5;i++)
            {
                Delay10KTCYx(200);//1s
            }
            Lista_Estados = Teste_Cap4;
        break;


        case Teste_Cap4:
            //***capacitor 4
            lcd_limpa_tela();
            lcd_posicao_cursor(0,0);
            lcd_escreve_string("Capacitor 4");
            PORTDbits.RD2 = 0;
            PORTDbits.RD3 = 1;
            for (i=0;i<5;i++)
            {
                Delay10KTCYx(200);//1s
            }  
            PORTDbits.RD3 = 0;
            Lista_Estados = Uniso;  // manda para o proximo estado
        break;


        default:    // Estrará aqui somente se não entrar em nenhum dos estados anteriores
            lcd_limpa_tela();
            lcd_posicao_cursor(0,0);             // Posiciona Cursor para linha 0, coluna 0
            sprintf(StrLocal,"Erro.Estado");
            lcd_escreve_string(StrLocal);
            lcd_posicao_cursor(1,0);
            sprintf(StrLocal,"Indefinido");
            lcd_escreve_string(StrLocal);
            //Lista_Estados = Erro;
       // break;

        case Erro:
        break;// Fica travado neste estado até que seja resetado

    }// Fecha switch case
    return;
}// Fecha função