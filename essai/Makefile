parser:
	flex -o f.lex.yy.c lexical.lex

ccode:f.lex.yy.c
	gcc -pedantic -Wall -O2 f.lex.yy.c -o parse -lfl

run:
	./parse