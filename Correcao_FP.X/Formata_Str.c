#include <stdlib.h>
#include <stdio.h>
#include "main.h"

void Formata_Str(unsigned int Valor_Medido)
{
        if(Valor_Medido >=10000)
        {
            Str_Gen[0] = (Valor_Medido/10000) + 0x30;
            Str_Gen[1] = (Valor_Medido/1000)%10 + 0x30;
            Str_Gen[2] = (Valor_Medido/100)%10 + 0x30;
            Str_Gen[3] = '.';
            Str_Gen[4] = (Valor_Medido/10)%10 + 0x30;
            Str_Gen[5] = Valor_Medido%10 + 0x30;
            Str_Gen[6] = '%';
            Str_Gen[7] = '\0';
        }
        else
        {
            Str_Gen[0] = (Valor_Medido/1000)%10 + 0x30;
            Str_Gen[1] = (Valor_Medido/100)%10 + 0x30;
            Str_Gen[2] = '.';
            Str_Gen[3] = (Valor_Medido/10)%10 + 0x30;
            Str_Gen[4] = Valor_Medido%10 + 0x30;
            Str_Gen[5] = '%';
            Str_Gen[6] = ' ';
            Str_Gen[7] = '\0';
        }
}
