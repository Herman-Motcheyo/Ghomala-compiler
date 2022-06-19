

%{
/*code qui sera rajouté en haut du fichier généré*/
#include<stdio.h>
%}
operateur \+|\*|\=|\;|\=\=|\!\=|\<|\>|\-|\/|\%|\+\=|\-\=|\/=|\%\=|\*\=
/*  definition des entiers*/
entier [0-9]+ 
parenthese \(|\)  
variable [a-zA-Z]

%%
{entier}     printf("entier");
{operateur}  printf("operateur");
{variable}   printf("variable");
.;
[\n];

%%
/* code ajouté en bas du parseur*/
int main(int argc, char **argv)
{
	printf("\n");
	yylex();
	printf("\n");
	return 0;
}
/*compilation
flex -o exo7.lex.yy.c exo7.lex
gcc -pedantic -Wall -O2 exo7.lex.yy.c -o exo7 -lfl
utilisation : echo "12+4+56" | ./exo7
*/


