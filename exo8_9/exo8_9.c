/*
 * exo8_9.c
 *
 *  Created on: Jan 1, 2013
 *      Author: messi
 * In this exercice, we do left-right evaluation with and without operator priority
 * using respectively functions exp_post_fixe_avec_prio and exp_post_fixe_sans_prio
 * usage: ./exo8_9 ../../tp1/exo4/test.txt
 */

#include <stdio.h>
#include <stdlib.h>
#include "utilitaire.h"

#define etat_initial 0
#define etat_entier_positif 1
#define etat_entier_negatif 2
#define etat_reel_positif 3
#define etat_reel_negatif 4
#define etat_operateur 5
#define etat_paranthese_ouvrante 6
#define etat_paranthese_fermante 7
#define etat_puit 8
#define etat_signe_positif 9
#define etat_signe_negatif 10

typedef int Etat;
char *yytext;
Noeud *exp_arith;

void afficher(float valeur, Etat q_courant)
{
	if (q_courant == etat_entier_positif || q_courant == etat_entier_negatif)
		printf("<int:%d>", (int)valeur);
	if (q_courant == etat_reel_positif || q_courant == etat_reel_negatif)
		printf("<reel:%f>", valeur);
	if (q_courant == etat_paranthese_ouvrante || q_courant == etat_paranthese_fermante)
		printf("<paranthese :%c>", (int)valeur);
	if (q_courant == etat_operateur)
		printf("<operateur :%c>", (int)valeur);
	insert_queue(&exp_arith, valeur);
}

int est_operateur(int car)
{
	int bool = 0;
	if (car == '+' || car == '*' || car == '(' || car == ')' || car == '/' || car == '%' || car == '-')
		bool = 1;
	return bool;
}

void exp_post_fixe_sans_prio(Noeud *exp, File *sortie)
{
	Pile p;
	Noeud *l = exp;
	int val;
	init_file_vide(sortie);
	init_pile_vide(&p);

	while (exp != 0)
	{
		if (!est_operateur(exp->info))
		{
			enfiler(sortie, exp->info);
		}
		else
		{
			if (!pile_est_vide(p))
			{
				val = depiler(&p);
				enfiler(sortie, val);
			}

			empiler(&p, exp->info);
		}
		exp = exp->suivant;
	}
	if (!pile_est_vide(p))
	{
		val = depiler(&p);
		enfiler(sortie, val);
	}
}

int priorite_operateur(int operateur)
{
	int priorite = -1;

	switch (operateur)
	{
	case '-':
	case '+':
		priorite = 0;
		break;
	case '/':
	case '*':
		priorite = 1;
		break;
	case '^':
		priorite = 2;
		break;
	}

	return priorite;
}
/*
 * Shunting-yard algorithm
 */
void exp_post_fixe_avec_prio(Noeud *exp, File *sortie)
{
	Pile p;
	Noeud *l = exp;
	int val;
	init_file_vide(sortie);
	init_pile_vide(&p);

	while (exp != 0)
	{
		if (!est_operateur(exp->info))
		{
			enfiler(sortie, exp->info);
			// printf("%d\n ", exp->info);
		}
		else if (exp->info == ')')
		{
			while (!pile_est_vide(p) && tete_de_pile(p) != '(')
			{
				val = depiler(&p);
				enfiler(sortie, val);
			}
			depiler(&p);
		}else if (exp->info == '('){
			empiler(&p, exp->info);
			// printf("%c\n", val);
		}else{
			printf("%c\n", exp->info);
			while (!pile_est_vide(p) && tete_de_pile(p) != '(' && (priorite_operateur(tete_de_pile(p)) > priorite_operateur(exp->info)))
			{
				val = depiler(&p);
				// printf("%c\n", val);
				enfiler(sortie, val);
			}
			empiler(&p, exp->info);
		}
		exp = exp->suivant;

		
	}
	while (!pile_est_vide(p))
	{
		val = depiler(&p);
		enfiler(sortie, val);
	}
}

int calcul(int operateur, int operande_1, int operande_2)
{
	switch (operateur)
	{
	case '+':
		return operande_1 + operande_2;
	case '*':
		return operande_1 * operande_2;
	case '/':
		return operande_1 / operande_2;
	case '-':
		return operande_2 - operande_1;
	case '%':
		return operande_1 % operande_2;
	}
}

int evaluation_gauche_droite(File exp_post_fixe)
{
	Pile p;
	int val, operateur, operande_1, operande_2, resultat;
	while (!file_est_vide(exp_post_fixe))
	{
		val = defiler(&exp_post_fixe);
		if (!est_operateur(val))
		{
			empiler(&p, val);
		}
		else
		{
			operande_1 = depiler(&p);
			operande_2 = depiler(&p);
			operateur = val;
			resultat = calcul(operateur, operande_1, operande_2);
			empiler(&p, resultat);
		}
	}
	resultat = depiler(&p);
	return resultat;
}

int analyse_lexicale(FILE *fdw)
{
	int nbOc = 0, i = 0, j = 0, state = -1,nbr1;
	int init = 0;
	Etat q = etat_initial;

	char car, tmp;
	int rg = 1;
	float dec = 0;
	float nbr = 0;
	printf("\n");
	while (!feof(fdw))
	{
		fread(&car, 1, 1, fdw);
		switch (car)
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
			switch (q)
			{
			case etat_operateur:
			case etat_initial:
			case etat_signe_positif:
				q = etat_entier_positif;
				nbr = nbr * 10 + (car - '0');
				break;
			case etat_signe_negatif:
				q = etat_entier_negatif;
				nbr = nbr * 10 + (car - '0');
				break;
			case etat_reel_positif:
			case etat_reel_negatif:
				rg *= 10;
				dec = dec * 10 + (car - '0');
				break;
			case etat_entier_positif:
			case etat_entier_negatif:
				nbr = nbr * 10 + (car - '0');
				break;
			case etat_paranthese_ouvrante:
				q = etat_entier_positif;
				nbr = nbr * 10 + (car - '0');
				break;
			default:
				q = etat_puit;
				// printf("error sdsds\n");
				// exit(EXIT_FAILURE);
				break;
			}
			break;
		case '+':
			switch (q)
			{
			case etat_reel_positif:

				dec = (float)dec/rg;
				nbr += dec;
				afficher(nbr, q);
				q = etat_operateur;
				nbr = 0;
				afficher(car, q);
				break;
			case etat_entier_positif:
				afficher(nbr, q);
				q = etat_operateur;
				nbr = 0;
				afficher(car, q);
				break;
			case etat_reel_negatif:
				dec = (float)dec/rg;
				nbr += dec;
				afficher(-nbr, q);
				q = etat_operateur;
				nbr = 0;
				afficher(car, q);
				break;
			case etat_entier_negatif:
				nbr = -nbr;
				afficher(nbr, q);
				q = etat_operateur;
				nbr = 0;
				afficher(car, q);
				break;
			case etat_paranthese_fermante:
				q = etat_operateur;
				afficher(car, q);
				break;
			case etat_initial:
			case etat_paranthese_ouvrante:
			case etat_operateur:
				afficher(car, q);
				q = etat_signe_positif;
				break;
			default:
				q = etat_puit;
				// printf("error\n");
				// exit(EXIT_FAILURE);
				break;
			}
			break;
		case '-':
			switch (q)
			{
			case etat_reel_positif:
				dec = (float)dec/rg;
				nbr += dec;
				afficher(nbr, q);
				q = etat_operateur;
				nbr = 0;
				afficher(car, q);
			case etat_entier_positif:
				afficher(nbr, q);
				q = etat_operateur;
				nbr = 0;
				afficher(car, q);
				break;
			case etat_reel_negatif:
				dec = (float)dec/rg;
				nbr += dec;
				afficher(-nbr, q);
				q = etat_operateur;
				afficher(car, q);
				nbr = 0;
				break;
			case etat_entier_negatif:
				afficher(-nbr, q);
				q = etat_operateur;
				nbr = 0;
				afficher(car, q);
				break;
			case etat_paranthese_fermante:
				q = etat_operateur;
				afficher(car, q);
				break;
			case etat_initial:
			case etat_paranthese_ouvrante:
			case etat_operateur:
				q = etat_entier_negatif;
				break;
			default:
				q = etat_puit;
				// printf("error\n");
				// exit(EXIT_FAILURE);
				break;
			}
			break;
		case '*':
		case '%':
		case '/':
			switch (q)
			{
			case etat_reel_positif:
				dec = (float)dec/rg;
				nbr += dec;
				afficher(nbr, q);
				q = etat_operateur;
				afficher(car, q);
				nbr = 0;
			case etat_entier_positif:
				afficher(nbr, q);
				q = etat_operateur;
				afficher(car, q);
				nbr = 0;
				break;
			case etat_reel_negatif:
				dec = (float)dec/rg;
				nbr += dec;
				afficher(-nbr, q);
				q = etat_operateur;
				afficher(car, q);
				nbr = 0;
				break;
			case etat_entier_negatif:
				afficher(-nbr, q);
				q = etat_operateur;
				afficher(car, q);
				nbr = 0;
				break;
			default:
				q = etat_puit;
				// printf("error\n");
				// exit(EXIT_FAILURE);
				break;
			}
			break;
		case '(':
			switch (q)
			{
			case etat_initial:
			case etat_operateur:
				q = etat_paranthese_ouvrante;
				afficher(car, q);
				break;
			default:
				q = etat_puit;
				// printf("error\n");
				// exit(EXIT_FAILURE);
				break;
			}
			break;
		case ')':
			switch (q)
			{
			case etat_reel_positif:
				dec = (float)dec/rg;
				nbr += dec;
				afficher(nbr, q);
				q = etat_paranthese_fermante;
				afficher(car, q);
				nbr = 0;
				break;
			case etat_entier_positif:
				afficher(nbr, q);
				q = etat_paranthese_fermante;
				afficher(car, q);
				nbr = 0;
				break;
			case etat_reel_negatif:
				dec = (float)dec/rg;
				nbr += dec;
				afficher(-nbr, q);
				q = etat_paranthese_fermante;
				afficher(car, q);
				nbr = 0;
				break;
			case etat_entier_negatif:
				nbr = -nbr;
				afficher(nbr, q);
				q = etat_paranthese_fermante;
				afficher(car, q);
				nbr = 0;
				break;
			default:
				q = etat_puit;
				// printf("error\n");
				// exit(EXIT_FAILURE);
				break;
			}
			break;
		case '.':
			switch (q)
			{
			case etat_entier_positif:
				q = etat_reel_positif;
				dec = 0;
				rg = 1;
				break;
			case etat_entier_negatif:
				q = etat_reel_negatif;
				dec = 0;
				rg = 1;
				break;
			default:
				q = etat_puit;
				// printf("error\n");
				// exit(EXIT_FAILURE);
				break;
			}
			break;
		case '\n':
			break;
		default:
			printf("\n%c : Parse Error, unkown char\n", car);
			return 1;
		}
	}
	fclose(fdw);

	if (q == etat_entier_positif)
		afficher(nbr, q);
	else if (q == etat_reel_positif)
	{
		dec = (float)dec/rg;
		nbr += dec;
		afficher(nbr, q);
	}
	else if (q == etat_entier_negatif)
		afficher(-nbr, q);
	else if(q == etat_reel_negatif)
	{
		dec = (float)dec/rg;
		nbr += dec;
		afficher(-nbr, q);
	}
	printf("\n\n");
	return 0;
}

int main(int argc, char **argv)
{

	int resultat1, resultat2;
	FILE *fdw = fopen(argv[1], "r");
	File sortie;

	int val_anal = analyse_lexicale(fdw);
	// exp_post_fixe_sans_prio(exp_arith, &sortie);
	// resultat1 = evaluation_gauche_droite(sortie);
	
	exp_post_fixe_avec_prio(exp_arith, &sortie);
	resultat2 = evaluation_gauche_droite(sortie);

	// printf("Avec priorité: %d\t Sans priorité:%d\n", resultat2, resultat1);
	printf("Avec priorité: %d\n", resultat2);
	/*if(val_anal == 0)*/

	return 0;
}
