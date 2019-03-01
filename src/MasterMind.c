
#include <stdio.h>
#include <stdlib.h>
#define NIVEAU_MAX 8
/*
int main(void) {


	int niveauDeJeu = 0;					// chiffre qui sera défini par le jouer permettant de déterminer le niveau de jeu
	int combinaison[NIVEAU_MAX];		// tableau contenant la combinaison à trouver
	int proposition[NIVEAU_MAX];		// tableau qui contient la proposition du joueur


	choix difficulté = taille de nos tableaux
	donc plus le chiffre est grand, plus il est difficile de trouver la bonne combinaison
}
*/
void presentationJeu(char* message[]){
	
	message="----------------------------------------\n----Bienvenue dans le jeu MasterMind----\n----------------------------------------\nVeuillez choisir votre niveau de difficulté entre 1 et 8 ? \n(1 très facile et 8 très difficile)";
}
/*
	//printf("----------------------------------------\n");
	//printf("----Bienvenue dans le jeu MasterMind----\n");
	//printf("----------------------------------------\n");

	do {
	//printf("Veuillez choisir votre niveau de difficulté entre 1 et 8 ? \n(1 très facile et 8 très difficile)\n");
	printf("Niveau : ");
	scanf("%d", &niveauDeJeu);
	} while (niveauDeJeu < 0 && niveauDeJeu > 8);
	printf("\Vous avez choisi le niveau %d, êtes-vous sur de vouloir continuer ?\n", niveauDeJeu);
	printf("\t- Si oui, tapez la lettre O et la combinaison sera générer aléatoirement?\n\t- Si non, taper la lettre N pour choisir un autre niveau\n");

	char decision;
	printf("Choix : ");
	scanf("%c", &decision);


	tirage aléatoire de la combinaison :
	on fait un rand sur chaque position du tableau pour
	tirer un chiffre de manière aléatoire (un même nombre peut apparaitre plusieurs fois)

	int i = 0;
			srand(time(NULL));
	for (i= 0 ; i < niveauDeJeu ; i++) {
		combinaison[i] = rand()%(niveauDeJeu-1+1) + 1;
	}

// c'est juste pour affiche le résultat du tirage au sort afin de vérifier le rés
int l = 0;
printf("La combinaison tirée aléatoirement est :\n");
printf("[ ");
for (l=0 ; l< niveauDeJeu ; l++) {
	printf("%d ",combinaison[l]);
}
printf("]");




	void proposerEtEvaluerUneCombinaison() {    // faire une fonction

		int nbBonnePlace = 0 ;
		int nbMauvaisePlace = 0 ;
		int inexistant = 0;

		// proposition du joueur : on lui demande de saisir une valeur "niveauDeJeu fois"

		printf("veuillez saisir votre proposition de combinaison avec les chiffres de 1 à %d",niveauDeJeu);
		int i = 0;
		while (i < niveauDeJeu) {
			printf("choix case N°%d : ", i);
			proposition[i] = scanf("%d", &i);
			i++;
		}



		// comparer la proposition à la combinaison
		// existe-t-il ? => si non inexistant++
		// est-il à la bonne place? OUI nbBonnePlace ++   NON nbMauvaisePlace ++

		int j = 0;
		int k;
		for(j=0 ; j < niveauDeJeu ; j++){
			k = 0;
			if (proposition[j] == combinaison[k]) {
				nbBonnePlace++;
			}
			else
			{
			for (k=0 ; k < niveauDeJeu ; k++){
				if (proposition[j] == combinaison[k]) {
					nbMauvaisePlace++;
				}
			}
		}

		if (nbBonnePlace == niveauDeJeu){
			printf("Bravo ! Vous avez trouvé la bonne combinaison !");
		}
		else
		{
			proposerEtEvaluerUneCombinaison(); // on recommance
		}
	}

	
}
*/