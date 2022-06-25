/*
 ============================================================================
 Name        : exo4.c, created on: Dec 23, 2012
 Author      : Messi Nguélé Thomas
 Version     : 1.0
 Copyright   : Welcome, your are allowed to copy this code and reuse it as you want!
 Description : This program build a tinny lexical analysis for arithmetic expression which print lexemes come from the file file_expression_name. Ansi-style
 Usage        : ./exo3-2 file_expression_name
 ============================================================================
 */

#include <stdio.h>

#define etat_initial 0
#define etat_nombre 1
#define etat_operateur 2
#define etat_final 3
#define etat_reel 4

typedef int Etat;

void afficher(float valeur, Etat q_courant){
	if(q_courant == etat_nombre)
		printf("<int:%d>",(int)valeur);
	if(q_courant == etat_operateur)
		printf("<oper:%c>",(char)valeur);
	if(q_courant == etat_reel)
		printf("<reel:%f>",valeur);
}

int analyse_lexicale(FILE* fdw)
{
	float nbOc = 0, i = 0, j = 0, state = -1, nbr=0,nbr1;
	int init = 0;
	Etat q = etat_initial, q_precedent = etat_initial;


	char car,tmp; 
	int rg=1; 
	float dec=0;
	printf("\n");

	while(!feof(fdw)){
		fread(&car,1,1,fdw);

		switch(car)
		{
		   	case '0':
		   	case '1':
		   	case '2':
		   	case '3':
		   	case '4':
		   	case '5':
		   	case '6':
		   	case '7':
		   	case '8':
		   	case '9':
					  if(q==etat_reel){
						  
						  rg*=10;
						  dec=dec*10+(car-'0');
						  
						}else{
							q = etat_nombre;
							nbr = nbr*10 + (car-'0');
							
					  }

	                  break;
			case '.':

					if(q==etat_nombre){
						q = etat_reel;

						
						dec=0;
						rg=1;
					
					}

					
				    break;

		   	case '+':
		   	case '*':
			case '-':
			case '/':
			

					if(q==etat_nombre){
						afficher(nbr,q);
						nbr = 0;
					}
					if (q==etat_reel){
						dec=(float)dec/rg;
						nbr+=dec;
						afficher(nbr,q);
						
						nbr=0;
						dec=0;
					}

					q = etat_operateur;
					afficher(car,q);
				    break;
		   	case '\n':
			   			//afficher(nbr,q);
		   				break;

			default:
				printf("\n%c : Parse Error, unkown char\n",car);
				return 1;
		}
	}
	fclose(fdw);

	if(q==etat_nombre || q==etat_reel){
		

		dec=(float)dec/rg;
		
		nbr=nbr+dec;
		afficher(nbr,q);}
	printf("\n\n");
	return 0;
}

int main(int argc, char** argv)
{
FILE* fdw = fopen(argv[1],"r");
int val_anal = analyse_lexicale(fdw);

return 0;
}


