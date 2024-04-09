/********
Fichier: bibliotheque_images.c
Auteurs: Jean-Sebastien Giroux
Date: 28 octobre 2023
Description: Fichier de distribution pour GEN145.
********/

#include "bibliotheque_images.h"

//Fonctions pour les images en noir et blanc. 
int pgm_lire(char nom_fichier[], int matrice[MAX_HAUTEUR][MAX_LARGEUR], 
             int *p_lignes, int *p_colonnes, 
             int *p_maxval, struct MetaData *p_metadonnees)
{
	//Pour connaitre le type de pgm
	char pgm_type[3];
	FILE *filePointeur;
	
	//Ouvrir le fichier en mode lecture 
	filePointeur = fopen(nom_fichier, "r");
	
	//Validation du fonctionnement de fopen
	if(filePointeur == NULL){
		return ERREUR_FICHIER;
	}
	else {
		//Gerer les metadonnees
		fscanf(filePointeur, "#%1024[^;]", p_metadonnees->auteur);
		fscanf(filePointeur, ";%1024[^;]", p_metadonnees->dateCreation);
		fscanf(filePointeur, ";%1024[^\n]", p_metadonnees->lieuCreation);
		
		//Verification du P2 et P3
		fscanf(filePointeur, "%s", pgm_type);
		if(pgm_type[0] != 'P' || pgm_type[1] != '2' || pgm_type[2] !='\0'){
			fclose(filePointeur);
			return ERREUR_FORMAT;
		}
		
		//Nombre de ligne et de colonnes
		fscanf(filePointeur, "%d", p_colonnes);
		fscanf(filePointeur, "%d", p_lignes);
		//Error handling 
		if(*p_lignes > MAX_HAUTEUR || *p_colonnes > MAX_LARGEUR){
			fclose(filePointeur);
			return ERREUR_TAILLE;
		}
		
		//Valeur maximale
		fscanf(filePointeur, "%d", p_maxval);
		
		//Lire les informations de la matrice
		int temp = 0;
		for(int i=0; i<*p_colonnes; i++){
			for(int j=0; j<*p_lignes; j++){
				fscanf(filePointeur, "%d", &temp);
				matrice[i][j] = temp;
				if(matrice[i][j] > *p_maxval){
					return ERREUR_FORMAT;
			    }
			}
		}
    }
    //Fermeture du fichier
	fclose(filePointeur);
	
    return OK;
}

int pgm_ecrire(char nom_fichier[], int matrice[MAX_HAUTEUR][MAX_LARGEUR], 
               int lignes, int colonnes, 
               int maxval, struct MetaData metadonnees)
{
	FILE *filepointer;
	
	//Ouvrir le fichier en mode ecriture
	filepointer = fopen(nom_fichier, "w");
	if(filepointer == NULL){
		return ERREUR_FICHIER;
	} else{
		//Ecrire les metadatas dans le fichier 
		fprintf(filepointer, "#%s;", metadonnees.auteur);
		fprintf(filepointer, "%s;", metadonnees.dateCreation);
		fprintf(filepointer, "%s\n", metadonnees.lieuCreation);
		
		//Ecrire le format P2
		fprintf(filepointer, "P2\n");
		
		//Ecrire le nombre de lignes et de colonnes 
		fprintf(filepointer, "%d ", colonnes);
		fprintf(filepointer, "%d", lignes); 
		
		//Ecrire la valeur maximale 
		fprintf(filepointer, "\n");
		fprintf(filepointer, "%d", maxval);
		
		//Ecrire les informations de la matrice
		fprintf(filepointer, "\n");
		for(int i=0; i<colonnes; i++){
			for(int j=0; j<lignes; j++){
				fprintf(filepointer, "%d ", matrice[i][j]);
			}
			fprintf(filepointer, "\n");
		}
	}
	fclose(filepointer);

    return OK;
}

int pgm_copier(int matrice1[MAX_HAUTEUR][MAX_LARGEUR], int lignes1,
               int colonnes1, int matrice2[MAX_HAUTEUR][MAX_LARGEUR], 
               int *p_lignes2, int *p_colonnes2)
{
	for(int i=0; i<colonnes1; i++){
		for(int j=0; j<lignes1; j++){
			matrice2[i][j] = matrice1[i][j];
		}
	}
	*p_lignes2 = lignes1;
	*p_colonnes2 = colonnes1;
	return OK;
}

int pgm_creer_histogramme(int matrice[MAX_HAUTEUR][MAX_LARGEUR], 
                          int lignes, int colonnes, 
                          int histogramme[MAX_VALEUR+1])
{
	//Initialisation de histogramme
	for(int i=0; i<MAX_VALEUR+1; i++){
		histogramme[i] = 0;
	}
	//Realisation de histogramme
	for(int i=0; i<colonnes; i++){
		for(int j=0; j<lignes; j++){
			histogramme[matrice[i][j]] += 1;
		}
	}
	return OK;
}

int pgm_couleur_preponderante(int matrice[MAX_HAUTEUR][MAX_LARGEUR],
                              int lignes, int colonnes)
{
	//Initialisation de histogramme
	int histogramme[MAX_VALEUR+1];
	for(int i=0; i<MAX_VALEUR+1; i++){
		histogramme[i] = 0;
	}
	//Realisation de histogramme
	for(int i=0; i<colonnes; i++){
		for(int j=0; j<lignes; j++){
			histogramme[matrice[i][j]] += 1;
		}
	}
	//Trouver la couleur preponderante
	int max_value = 0;
	int position = 0;
	for(int i=0; i<MAX_VALEUR+1; i++){
		if(histogramme[i] > max_value){
			max_value = histogramme[i];
			position = i;
		}
	}
	return position;
}
                              
int pgm_eclaircir_noircir(int matrice[MAX_HAUTEUR][MAX_LARGEUR],
                          int lignes, int colonnes, int maxval, 
                          int valeur)
{
	// Verification de la valeur a reduire
	if(valeur == 0){
		return ERREUR;
	}
	for(int i=0; i<lignes; i++){
		for(int j=0; j<colonnes; j++){
			//Realisation de operation eclarir ou noircir
			matrice[i][j] += valeur;
			//Verification que les nouvelles valeurs ne soient pas plus petites que 0
			if(matrice[i][j]<0){
				matrice[i][j] = 0;
			}
			//Verification que les nouvelles valeurs ne soient pas plus grande que la valeur maximale
			if(matrice[i][j]>maxval){
				matrice[i][j] = maxval;
			}
			
		}
	}
	return OK;
}
                          
int pgm_creer_negatif(int matrice[MAX_HAUTEUR][MAX_LARGEUR],
                      int lignes, int colonnes, int maxval)
{
	for(int i=0; i<lignes; i++){
		for(int j=0; j<colonnes; j++){
			matrice[i][j] = maxval - matrice[i][j];
		}
	}
	return OK;
}
                      
int pgm_extraire(int matrice[MAX_HAUTEUR][MAX_LARGEUR], int lignes1, 
                 int colonnes1, int lignes2, int colonnes2, 
                 int *p_lignes, int *p_colonnes)
{
	//Calculer la largeur de image a extraire
	int largeur = 0;
	int hauteur = 0; 
	largeur = colonnes2 - colonnes1 + 1; 
	hauteur = lignes2 - lignes1 + 1;
	
	// Verification que image a extraire est plus petite que image originale
	if(lignes1 < 0 || colonnes1 < 0 || lignes1 > MAX_HAUTEUR || colonnes1 > MAX_LARGEUR || (*p_lignes-1) < lignes1 || (*p_colonnes-1) < colonnes1)
	{
		return ERREUR;
	}
	if(lignes2 < 0 || colonnes2 < 0 || lignes2 > MAX_HAUTEUR || colonnes2 > MAX_LARGEUR || (*p_lignes -1) < lignes2 || (*p_colonnes-1) < colonnes2)
	{
		return ERREUR;
	}
	
	// Changer la valeur de la matrice avec image extraire
	for(unsigned short i=0; i<hauteur; i++){
		for(unsigned short j=0; j<largeur; j++){
			matrice[i][j] = matrice[i+lignes1][j+colonnes1];
		}
	}
	// Assigner les dimensions de image a extraire
	*p_lignes = hauteur;
	*p_colonnes = largeur;

	return OK; 
}
                 
int pgm_sont_identiques(int matrice1[MAX_HAUTEUR][MAX_LARGEUR], 
                        int lignes1, int colonnes1, int matrice2[MAX_HAUTEUR][MAX_LARGEUR], 
                        int lignes2, int colonnes2)
{
	if(lignes1 != lignes2 || colonnes1 != colonnes2){
		return ERREUR_FORMAT;
	}
	for(int i=0; i<colonnes1; i++){
		for(int j=0; j<lignes1; j++){
			if(matrice1[i][j] != matrice2[i][j]){
				return DIFFERENTES;
			}
		}
	}
	return IDENTIQUES; 
}
                        
int pgm_pivoter90(int matrice[MAX_HAUTEUR][MAX_LARGEUR], int *p_lignes, 
                  int *p_colonnes, int sens)
{
	if(sens == 1){
		//Tourner la matrice dans le sens horaire
		int matrice_temp[MAX_LARGEUR][MAX_HAUTEUR];
		for(int i=0; i <*p_colonnes; i++){
			for(int j=0; j<*p_lignes; j++){
				matrice_temp[i][j] = matrice[*p_lignes-j-1][i];
			}
		}
		//Ecrire la matrice pivote dans la matrice
		for(int i=0; i <*p_colonnes; i++){
			for(int j=0; j<*p_lignes; j++){
				matrice[i][j] = matrice_temp[i][j];
			}
		}
		//Assigner les valeurs 
		int temp = *p_colonnes;
		*p_colonnes = *p_lignes;
		*p_lignes = temp;
		return OK; 
		
	} else if(sens == 0){
		//Tourner la matrice dans le sens horaire 
		int matrice_temp[MAX_LARGEUR][MAX_HAUTEUR];
		for(int k=0;k<3;k++){
			for(int i=0; i<*p_colonnes; i++){
				for(int j=0; j<*p_lignes; j++){
					matrice_temp[i][j] = matrice[*p_lignes-j-1][i];
				}
			}
			//Ecrire la matrice pivote dans la matrice
			for(int i=0; i<*p_colonnes; i++){
				for(int j=0; j<*p_lignes; j++){
					matrice[i][j]=matrice_temp[i][j];
				}
			}
			//Assigner les valeurs 
			int temp = *p_colonnes;
			*p_colonnes = *p_lignes;
			*p_lignes = temp;
			}
		return OK;	
	} else {
		return ERREUR;
	}
	
	return OK; 
}
                              
//Fonctions pour les images en couleurs. 
int ppm_lire(char nom_fichier[], struct RGB matrice[MAX_HAUTEUR][MAX_LARGEUR], 
             int *p_lignes, int *p_colonnes, int *p_maxval, 
             struct MetaData *p_metadonnees)
{
	// Pour connaitre le type de pgm
	char pgm_type[3];
	FILE *filePointer;
	
	//Lire image en couleur 
	filePointer = fopen(nom_fichier, "r");
	
	//Verifier si le fichier est vide
	if(filePointer == NULL){
		return ERREUR_FICHIER;
	}else{
		//Lire les metadonnees
		fscanf(filePointer, "#%1024[^;]", p_metadonnees->auteur);
		fscanf(filePointer, ";%1024[^;]", p_metadonnees->dateCreation);
		fscanf(filePointer, ";%1024[^\n]", p_metadonnees->lieuCreation);
		
		//Lire le type image
		fscanf(filePointer, "%s", pgm_type);
		// Verification des erreurs
		if(pgm_type[0] != 'P' || pgm_type[1] != '3' || pgm_type[2] !='\0'){
			return ERREUR_FORMAT;
		}
		
		//Lire les dimentions de image
		fscanf(filePointer, "%d", p_colonnes);
		fscanf(filePointer, "%d", p_lignes);
		//Verification des erreurs
		if(*p_lignes > MAX_HAUTEUR || *p_colonnes > MAX_LARGEUR){
			fclose(filePointer);
			return ERREUR_TAILLE;
		}
		
		//Lire la valeur maximale des pixels
		fscanf(filePointer, "%d", p_maxval);
		
		//Lire les donnees 
		for(int i=0; i<*p_lignes; i++){
			for(int j=0; j<*p_colonnes; j++){
				fscanf(filePointer, "%d", &matrice[i][j].valeurR);
				fscanf(filePointer, "%d", &matrice[i][j].valeurG);
				fscanf(filePointer, "%d", &matrice[i][j].valeurB);
			}
		}
	}
		
	//Fermer le fichier
	fclose(filePointer);
		
	return OK;
}

int ppm_ecrire(char nom_fichier[], struct RGB matrice[MAX_HAUTEUR][MAX_LARGEUR], 
               int lignes, int colonnes, int maxval, 
               struct MetaData metadonnees)
{
	FILE *filePointer;
	
	// Ouvrir le fichier en mode ecriture
	filePointer = fopen(nom_fichier, "w");
	
	// Verifier si le fichier est vide
	if(filePointer == NULL){
		return ERREUR_FICHIER;
	}
	
	//Ecrire les metadonnees 
	fprintf(filePointer, "#%s;", metadonnees.auteur);
	fprintf(filePointer, "%s;", metadonnees.dateCreation);
	fprintf(filePointer, "%s\n", metadonnees.lieuCreation);
	
	char type[3] = "P3";
	//Ecriture du type de fichier
	fprintf(filePointer, "%s", type);
	fprintf(filePointer, "\n");
	
	//Ecriture des dimentions de image
	fprintf(filePointer, "%d ", colonnes); 
	fprintf(filePointer, "%d", lignes);
	fprintf(filePointer, "\n");
	
	//Ecriture de la valeur maximale
	fprintf(filePointer, "%d", maxval);
	fprintf(filePointer, "\n");
	
	//Ecriture des valeurs de image
	for(int i=0; i<lignes; i++){
		for(int j=0; j<colonnes; j++){
			fprintf(filePointer, "%d ", matrice[i][j].valeurR);
			fprintf(filePointer, "%d ", matrice[i][j].valeurG);
			fprintf(filePointer, "%d ", matrice[i][j].valeurB);
		}
		fputs("\n", filePointer);
	}
	fclose(filePointer);
	
	return OK; 
}

int ppm_copier(struct RGB matrice1[MAX_HAUTEUR][MAX_LARGEUR], int lignes1, 
               int colonnes1, struct RGB matrice2[MAX_HAUTEUR][MAX_LARGEUR], 
               int *p_lignes2, int *p_colonnes2)
{
	for(int i=0; i<lignes1; i++){
		for(int j=0; j<colonnes1; j++){
			matrice2[i][j].valeurR = matrice1[i][j].valeurR;
			matrice2[i][j].valeurG = matrice1[i][j].valeurG;
			matrice2[i][j].valeurB = matrice1[i][j].valeurB;
		}
	}
	*p_lignes2 = lignes1;
	*p_colonnes2 = colonnes1;
	return OK; 
}

int ppm_sont_identiques(struct RGB matrice1[MAX_HAUTEUR][MAX_LARGEUR], 
                        int lignes1, int colonnes1, struct RGB matrice2[MAX_HAUTEUR][MAX_LARGEUR], 
                        int lignes2, int colonnes2)
{
	for(int i=0; i<lignes1; i++){
		for(int j=0; j<colonnes1; j++){
			if(matrice1[i][j].valeurR != matrice2[i][j].valeurR){
				return DIFFERENTES;
			}
			if(matrice1[i][j].valeurG != matrice2[i][j].valeurG){
				return DIFFERENTES;
			}
			if(matrice1[i][j].valeurB != matrice2[i][j].valeurB){
				return DIFFERENTES;
			}
		}
	}
	return IDENTIQUES; 
}

int ppm_pivoter90(struct RGB matrice[MAX_HAUTEUR][MAX_LARGEUR], 
                  int *p_lignes, int *p_colonnes, int sens)
{
	if(sens == 1){
		//Tourner la matrice dans le sens horaire
		struct RGB matrice_temp[MAX_LARGEUR][MAX_HAUTEUR];
		for(int i=0; i <*p_colonnes; i++){
			for(int j=0; j<*p_lignes; j++){
				matrice_temp[i][j].valeurR = matrice[*p_lignes-j-1][i].valeurR;
				matrice_temp[i][j].valeurG = matrice[*p_lignes-j-1][i].valeurG;
				matrice_temp[i][j].valeurB = matrice[*p_lignes-j-1][i].valeurB;
			}
		}
		//Ecrire la matrice pivote dans la matrice
		for(int i=0; i <*p_colonnes; i++){
			for(int j=0; j<*p_lignes; j++){
				matrice[i][j].valeurR = matrice_temp[i][j].valeurR;
				matrice[i][j].valeurG = matrice_temp[i][j].valeurG;
				matrice[i][j].valeurB = matrice_temp[i][j].valeurB;
			}
		}
		//Assigner les valeurs 
		int temp = *p_colonnes;
		*p_colonnes = *p_lignes;
		*p_lignes = temp;
		return OK; 
		
	} else if(sens == 0){
		//Tourner la matrice dans le sens horaire 
		struct RGB matrice_temp[MAX_LARGEUR][MAX_HAUTEUR];
		for(int k=0;k<3;k++){
			for(int i=0; i<*p_colonnes; i++){
				for(int j=0; j<*p_lignes; j++){
					matrice_temp[i][j].valeurR = matrice[*p_lignes-j-1][i].valeurR;
					matrice_temp[i][j].valeurG = matrice[*p_lignes-j-1][i].valeurG;
					matrice_temp[i][j].valeurB = matrice[*p_lignes-j-1][i].valeurB;
				}
			}
			//Ecrire la matrice pivote dans la matrice
			for(int i=0; i<*p_colonnes; i++){
				for(int j=0; j<*p_lignes; j++){
					matrice[i][j].valeurR = matrice_temp[i][j].valeurR;
					matrice[i][j].valeurG = matrice_temp[i][j].valeurG;
					matrice[i][j].valeurB = matrice_temp[i][j].valeurB;
				}
			}
			//Assigner les valeurs 
			int temp = *p_colonnes;
			*p_colonnes = *p_lignes;
			*p_lignes = temp;
			}
		return OK;	
	} else {
		return ERREUR;
	}
	
	return OK; 
}
