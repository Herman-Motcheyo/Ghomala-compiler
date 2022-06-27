%{
#include<stdio.h>
#include "simple.h"

%}

entier [0-9]+
operateur \+|\*|\=|\;|\=\=|\!\=|\<|\>|\-|\/|\%|\+\=|\-\=|\/=|\%\=|\*\=
parenthese \(|\)
variable [a-zA-Z]
chaine \"[a-zA-Z]+\"
%%

{entier} { yylval=atoi(yytext);return INTEGER;}
{operateur} {return *yytext;}
{parenthese} {return *yytext;}
{variable}  {yylval=*yytext; return VARIABLE;}
{chaine}  {yylval=*yytext; return CHAINE;}
[\n] ;
[ ] ;
[\t] ;

lachieeu {return PRINT;}
aPe {return SI;}
gue_e {return ALORS;}
tacPo_o {return SINON;}
aami {return FSI;}
Ke_e {return READ;}
neTo_o {return WHILE;}
rhe {return DO;}
te {return DONE;}

. { printf("erreur lexicale sur =>  %s \n",yytext); }

%%
