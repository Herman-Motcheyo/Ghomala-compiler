#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define etat_initial 0
#define etat_1 1
#define etat_final 2

void remove_char(char *str, int pos);

void remove_char(char *str, int pos)
{
    int x = 0;
    while (str[x] != '\0')
    {
        if (x >= pos)
        {
            str[x] = str[x + 1];
        }
        x++;
    }
}

int main(int agrc, char *argv[])
{
    int i = 0, etat = etat_initial, cmpt = 0;
    FILE *file = fopen(argv[2], "r");
    if (file == NULL)
    {
        printf("le fichier %s n'existe pas", argv[2]);
        exit(EXIT_FAILURE);
    }

    char *temp_ch = calloc(strlen(argv[1]), sizeof(char));
    strcpy(temp_ch, argv[1]);
    printf("%s\n", temp_ch);
    remove_char(temp_ch, 0);

    remove_char(temp_ch, strlen(temp_ch) - 1);
    remove_char(temp_ch, strlen(temp_ch) - 1);

    printf("%s\n", temp_ch);
    
    printf("%c %c %c\n",temp_ch[0], temp_ch[1], temp_ch[2]);
    char car;
    while (!feof(file))
    {
        fread(&car, 1, 1, file);
        if (car == temp_ch[0] || car == temp_ch[1] || car == temp_ch[2])
        {
            etat = etat_final;
        }
        else
        {
            if (etat == etat_final)
            {
                etat = etat_1;
                cmpt++;
            }
            else
            {
                etat = etat_1;
            }
        }
    }

    if (etat == etat_final)
        cmpt++;

    
    printf("la nombre d'occrence est %d\n", cmpt);
    free(temp_ch);
}