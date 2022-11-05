#include "Selecao_Cap.h"
#include "main.h"



//*** Seleciona os capacitores
void Seleciona_Capacitor(unsigned int FP)
{
    //*** Se timer menos que 400(10segundos), não permite acionar mais capacitores
    if(Timer_Capacitor<400)
       return;
       
    if(FP < 9200) // Se FP menor que 92%
    {
        //***Se todos já estiverem acionados retorna função não há nada pra fazer
        if((Cap1)&&(Cap2)&&(Cap3)&&(Cap4))
            return;

        char Qual_Acionar=0;
        Qual_Acionar = Capacitor_Menos_Acionado(); // Função retorna o capacitor e relé que foi menos acionado


       if((!Cap1) && (Qual_Acionar==1)) //Capacitor 1 desligado?
        {
            Cap1=Liga; //Liga Capacitor 1
            Cont_Cap1++;
            Timer_Capacitor=0;  // zera contador de tempo do capacitor, para que se aguarde no minimo 10segundos para acionar o proximo
            return;
        }
        if((!Cap2) && (Qual_Acionar==2)) //Capacitor 2 desligado?
        {
            Cap2=Liga; //Liga Capacitor 2
            Cont_Cap2++;
            Timer_Capacitor=0;  // zera contador de tempo do capacitor, para que se aguarde no minimo 10segundos para acionar o proximo
            return;
        }
        if((!Cap3) && (Qual_Acionar==3)) //Capacitor 3 desligado?
        {
            Cap3=Liga; //Liga Capacitor 3
            Cont_Cap3++;
            Timer_Capacitor=0;  // zera contador de tempo do capacitor, para que se aguarde no minimo 10segundos para acionar o proximo
            return;
        }
        if((!Cap4) && (Qual_Acionar==4)) //Capacitor 4 desligado?
        {
            Cap4=Liga; //Liga Capacitor 4
            Cont_Cap4++;
            Timer_Capacitor=0;  // zera contador de tempo do capacitor, para que se aguarde no minimo 10segundos para acionar o proximo
            return;
        }
    }//if
    else if (FP > 9300) //Se FP maior que 93%
    {
         if(Cap1) //Capacitor 1 ligado?
        {
            Cap1=Desliga; //Liga Capacitor 1
             Timer_Capacitor=0;
            return;
        }
        if(Cap2) //Capacitor 2 ligado?
        {
            Cap2=Desliga; //Liga Capacitor 2
             Timer_Capacitor=0;
            return;
        }
        if(Cap3) //Capacitor 3 ligado?
        {
            Cap3=Desliga; //Liga Capacitor 3
             Timer_Capacitor=0;
            return;
        }
        if(Cap4) //Capacitor 4 ligado?
        {
            Cap4=Desliga; //Liga Capacitor 4
             Timer_Capacitor=0;
            return;
        }
    }
return;
}// Fim seleciona capacitores







char Capacitor_Menos_Acionado()
{
 /*
  *  Função resposável por informar qual é o capacitor/relé que foi menos acionado desde que foi ligado
  *  Através desta informação é tomada uma decisão para acionar aquele que foi menos acionado
  *  prolongando a vida útil dos mesmos, uma vez que a quantidade de acionamento será igual para todos
  */
        char Qual_Acionar=1;
       
        if((Cont_Cap1 < Cont_Cap2) || (Cont_Cap1 < Cont_Cap3) || (Cont_Cap1 < Cont_Cap4))
        {
            Qual_Acionar = 1;
        }
        else if((Cont_Cap2 < Cont_Cap1)|| (Cont_Cap2 < Cont_Cap3)|| (Cont_Cap2 < Cont_Cap4))
        {
            Qual_Acionar = 2;
        }
        else if((Cont_Cap3 < Cont_Cap1)|| (Cont_Cap3 < Cont_Cap2)|| (Cont_Cap3 < Cont_Cap4))
        {
            Qual_Acionar = 3;
        }
        else if((Cont_Cap4 < Cont_Cap1)|| (Cont_Cap4 < Cont_Cap2)|| (Cont_Cap4 < Cont_Cap3))
        {
            Qual_Acionar = 4;
        }

        return(Qual_Acionar);
}
