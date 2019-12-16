////////////////////////////////////////////////////////////////
//devoir3.cpp
//Cr�� par: Sara S�guin
//Date: 24 octobre 2017
//Modifi� par:Sara S�guin
//Date de modification: 19 novembre 2019
//Description:
//CE FICHIER N'EST PAS EXHAUSTIF, IL DONNE SIMPLEMENT DES EXEMPLES ET UNE CERTAINE STRUCTURE.
////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <bitset>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <chrono>
using namespace std;

#define PAGE_t 256 //Taille d'une page (256 bytes)


////////////////////////////////////////////////////////////////
//Cette fonction cr�� un masque afin de lire les bits n�cessaires. NE PAS MODIFIER ET UTILISER TEL QUEL DANS LE MAIN
//Cr�� par: Sara S�guin
//Modifi� par:
//Description:
////////////////////////////////////////////////////////////////
unsigned createMask(unsigned a, unsigned b)
{

	unsigned r = 0;
	for (unsigned i = a; i <= b; i++)
	{
		r |= 1 << i;
	}
	return r;
}

/**
 * @brief  Lit le fichier contenant les 1000 adresses logiques et les places dans le vecteur d'adresses logiques
 * @param  fileName: nom du fichier contenant les adresses logiques
 * @param  &adresseLogique: le vecteur dans lequel on charge les adresses
 * @retval None
 */
void LireFichierAdresses(string fileName, vector<int> &adresseLogique)
{
	ifstream fichierContenantLesAdresses;
	fichierContenantLesAdresses.open(fileName);
	string ligne;

	if (fichierContenantLesAdresses.is_open())
	{
		for (int x = 0; x < 1000; x++)
		{
			fichierContenantLesAdresses >> ligne;
			adresseLogique.push_back(stoi(ligne));
		}
		fichierContenantLesAdresses.close();


	} else
	{
		cout << "fichier pas ouvert" << endl;
	}

	cout << "fonction finie";
}

/**
 * @brief  Fait une rotation à droite des bits d'un nombre entier 
 * @param  n: L'entier sur lequel on fait la rotation
 * @param  d: Le nombre de rotations a faire
 * @retval L'entier boucler sur lui-même
 */
int rightRotate(int n, unsigned int d) 
{ 
    return (n >> d)|(n << (32 - d)); 
} 

/**
 * @brief  Fait une rotation à gauche des bits d'un entier
 * @param  n: L'entier sur lequel on fait la rotation
 * @param  d: Le nombre de rotation à faire
 * @retval L'entier boucler sur lui-même
 */
int leftRotate(int n, unsigned int d) 
{ 
    return (n << d)|(n >> (32 - d)); 
} 

/**
 * @brief  Recherche une page dans le TLB
 * @param  TLB[16][2]: Le TLB
 * @param  pageRecherchee: La page que l'on cherche
 * @retval retourne l'adresse du frame si trouvé ou -1 si non présent
 */
int RechercheTLB(int TLB[16][2], int pageRecherchee)
{
	for (int i = 0; i < 16; i++)
	{
		if(TLB[i][0] == pageRecherchee){
			return TLB[i][1];
		}
	}
	return -1;
}

/**
 * @brief  Gère l'ajout et le retrait de valeur dans le TLB
 * @param  TLB[16][2]: Le TLB
 * @param  page: La page à chargée
 * @param  numFrame: Le numéro du frame dans lequel la page est chargé
 * @retval None
 */
void GestionTLB(int TLB[16][2], int page, int numFrame)
{
	int frame = RechercheTLB(TLB, page);

	if(frame == -1) {
		//on avance tous les element de 1 case
		for (int i = 0; i < 15; i++)
		{
			TLB[i][0] = TLB[i+1][0];
			TLB[i][1] = TLB[i+1][1];
		}
		TLB[15][0] = page;
		TLB[15][1] = numFrame;
	} else //si l'element est deja dans le tlb
	{
		int positionElement;
		for (int i = 0; i < 16; i++)
		{
			if(TLB[i][0] == page) positionElement = i;
		}
		for (int i = positionElement; i < 15; i++)
		{
			TLB[i][0] = TLB[i+1][0];
			TLB[i][1] = TLB[i+1][1];
		}
		TLB[15][0] = page;
		TLB[15][1] = numFrame;
	}
}


//////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
// PROGRAMME PRINCIPAL
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
int main()
{
	auto start = chrono::system_clock::now();
	int TLBHits = 0;
	int pageFaults = 0;
	int octetsSignes[1000];
	int valeurOctetsSignes[1000];

	//Initialisation et d�clarations
	//On met la mémoire physique à 257 pour pouvoir savoir si elle est occupé ou non
	//8 bit ne peuvent pas représenter 257, sert pour trouver un frame libre
	int memPhysique[256][PAGE_t];
	for (int i = 0; i < PAGE_t; i++)
	{
		for (int j = 0; j < PAGE_t; j++)
		{
			memPhysique[i][j] = 257;
		}
	}
	
	int adressePhysique[1000] = { 0 }; //Adresses Physiques(on utilise juste pour fichier output)
	int tablePage[256][2] = { 0 }; //Table de page, 1 espace pour l'adresse et l'autre pour le dirty bit
	vector<int>adresseLogique; //Adresses Logiques a traduire, prises a partir du fichier addresses.txt
	int TLB[16][2];	//TLB, 1 case pour la page et 1 case pour le frame

	//Lire le fichier d'adresses � traduire
	LireFichierAdresses("OS3/addresses.txt", adresseLogique);


	std::vector<int>bits_offset, bits_page; //Un vecteur pour les bits de page et un autre pour les bits d'offset

	//Cr��er un masque pour lire juste les bits 0 � 7 (offset)
	unsigned r = 0;
	r = createMask(0, 7);

	//Cr�er un masque pour lire juste les bits 8 � 15 (page)
	unsigned r2 = 0;
	r2 = createMask(8, 15);

	//Boucler sur les 1000 adresses logiques
	//Pour chaque adresse on met la première partie dans le vecteur d'offset et la deuxième dans 
	//le vecteur de pages en utilisant les masques et la rotation de bits
	for (int i = 0; i < adresseLogique.size(); i++)
	{
		bits_offset.push_back(adresseLogique.at(i) & r);
		unsigned a = adresseLogique.at(i) & r2;
		a = rightRotate(a, 8);
		bits_page.push_back(a);
	}



	ifstream fichierSimulDisqueBinaire("OS3/simuleDisque.bin", ios::binary | ios::in);

	char * buffer = new char [256];

	int frame;
	int numFrame;

	for (int i = 0; i < bits_page.size(); i++)
	{
		frame = RechercheTLB(TLB, bits_page[i]);
		//On regarde si la page se trouve dans le TLB
		if(frame != -1)
		{
			//si oui on ne fait qu'ajouter au tableau adressePhysique pour le fichier output et TLBhits++
			adressePhysique[i] = leftRotate(frame, 8);
			adressePhysique[i] += bits_offset.at(i);
			TLBHits++;
		}
		//sinon, on regarde si la page est chargé avec le bit chargé de la table de page
		else if (tablePage[bits_page[i]][1] != 1)
		{
			pageFaults++;
			//On lit 256 octets du fichier binaire à la bonne position et on les place dans le buffer
			int position = bits_page.at(i) * PAGE_t;
			fichierSimulDisqueBinaire.seekg(position);
			fichierSimulDisqueBinaire.read(buffer, PAGE_t);

			//on cherche le premier frame libre dans la memoire
			for (int i = 0; i < 256; i++)
			{
				if(memPhysique[i][0] == 257) 
				{
					numFrame = i;
					break;
				}
			}
			
			//On place chaque élément du buffer dans le bon frame en mémoire principal
			for (int i = 0; i < 256; i++)
			{
				memPhysique[numFrame][i] = buffer[i];
			}
			
			//on met à jour la table de page
			tablePage[bits_page[i]][0] = numFrame;
			tablePage[bits_page[i]][1] = 1;

			//On met à jour pour l'output.txt
			adressePhysique[i] = leftRotate(numFrame, 8);
			adressePhysique[i] += bits_offset.at(i);
		} else
		{
			//si déjà chargé, on met à jour pour le fichier output.txt
			adressePhysique[i] = leftRotate(tablePage[bits_page[i]][0], 8);
			adressePhysique[i] += bits_offset.at(i);
		}
		
		//On met à jour les entrées du TLB
		GestionTLB(TLB, bits_page[i], tablePage[bits_page[i]][0]);

		//Pour le fichier output.txt
		octetsSignes[i] = numFrame*PAGE_t + bits_offset.at(i);
		valeurOctetsSignes[i] = memPhysique[numFrame][bits_offset[i]];
	}

	ofstream output("output.txt");

	double pourcentagePageFault = (pageFaults/(double)adresseLogique.size())*100;

	double pourcentageTLBHits = (TLBHits/(double)adresseLogique.size())*100;

	auto end = chrono::system_clock::now();
	chrono::duration<double> tempsExecution = end-start;

	cout << "\nRatio page fault: " << pourcentagePageFault << "%\n";
	cout << "Temps execution: " << tempsExecution.count() << " secondes\n";

	output << "Temps execution: " << tempsExecution.count() << " secondes\n";
	output << "Ratio page fault: " << pourcentagePageFault << "%\n";
	output << "Succes TLB: " << pourcentageTLBHits << "%\n";

	for (int i = 0; i < 1000; i++)
	{
		output << adresseLogique.at(i) << "\t" << adressePhysique[i] << "\t";
		output << octetsSignes[i] << "\t" << valeurOctetsSignes[i] << endl;
	}

	return 0;

}
