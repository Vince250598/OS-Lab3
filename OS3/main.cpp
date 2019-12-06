////////////////////////////////////////////////////////////////
//devoir3.cpp
//Créé par: Sara Séguin
//Date: 24 octobre 2017
//Modifié par:Sara Séguin
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
using namespace std;

#define PAGE_t 256 //Taille d'une page (256 bytes)

////////////////////////////////////////////////////////////////
//Cette fonction retourne la valeur du byte signé
//Créé par: 
//Date: 
//Modifié par:
//Description:
////////////////////////////////////////////////////////////////
/*int fct_SignedByte(int page, int offset,std::string nomduFichier)
{
	//Ouvrir le fichier binaire
	std::ifstream fichierBinaire;
	fichierBinaire.open(nomduFichier);

	unsigned int LENGTH = 1; //Le byte signé a une longueur de 1 byte


	nomduFichier.std::seekg(...); //Trouver l'endroit correspondant au byte signé dans le fichier
	nomduFichier.std::read(...); //Lire cet emplacement

	//Fermer le fichier
	fichierBinaire.close();

	//Retourner la valeur du byte signé

}
*/
////////////////////////////////////////////////////////////////
//Cette fonction créé un masque afin de lire les bits nécessaires. NE PAS MODIFIER ET UTILISER TEL QUEL DANS LE MAIN
//Créé par: Sara Séguin
//Modifié par:
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


void LireFichierAdresses(string fileName, int adressesPhysiques[1000])
{
	
	ifstream fichierContenantLesAdresses ( fileName);
	//fichierContenantLesAdresses.open(nomDuFichierContenantLesAdresses, ios::in || ios::beg);

	//if (fichierContenantLesAdresses.is_open())
	//{
		for (int x = 0; x < 1000; x++)
		{
			fichierContenantLesAdresses >> adressesPhysiques[x];
		}
		fichierContenantLesAdresses.close();


	//}

	//else
	//	cout << "fichier pas ouvert" << endl;

	cout << "fonction finie";
}

int ExtractOffset(int a)
{
	int b = a % 256;
	return b;

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


	//Initialisation et déclarations
	int memPhysique[256] = { 0 }; //Mémoire physique
	int adressePhysique[1000] = { 0 }; //Adresses Physiques
	int tablePage[256][2] = { 0 }; //Table de page
	std::vector<int>adresseLogique; //Adresses Logiques

	//Lire le fichier d'adresses à traduire
	LireFichierAdresses("addresses.txt", adressePhysique);


	//Traduire l'adresse physique en adresse logique
	//1. Traduire l'entier en bits

	//Stocker les nombres binaires dans un vecteur

	std::vector<int>bits_offset, bits_page; //Un vecteur pour les bits de page et un autre pour les bits d'offset

	//Crééer un masque pour lire juste les bits 0 à 7 (offset)
	unsigned r = 0;
	r = createMask(0, 7);

	//Créer un masque pour lire juste les bits 8 à 15 (page)
	unsigned r2 = 0;
	r2 = createMask(8, 15);

	//Boucler sur les 1000 adresses
	for (int i = 0; i < adresseLogique.size(); i++)
	{
		bits_offset.at(i) = ExtractOffset(adressePhysique[i]);
		int a = adressePhysique[i] - 256;
		bits_page.at(i) = ExtractOffset(a);
		//Traduire en bits, EXEMPLE
		unsigned A = adresseLogique[i];
		//EXEMPLE DE SYNTAXE POUR UTILISER LE MASQUE	
		std::bitset<16>  result = r & A;
		std::bitset<16>  result2 = r2 & A;


		//TODO: ETC... 

		//Vecteurs de page et d'offset

		bits_page.push_back((result, nullptr, 2));
		bits_offset.push_back((result2, nullptr, 2));
	}




	//Table de pages
	//Une adresse à la fois, vérifier si elle est dans la table de page

	for (int i = 0; i < bits_page.size(); i++)
	{

		if (tablePage[bits_page[i]][1] != 1)
		{
			std::cout << "Page non-chargée dans la table" << std::endl;
			//Charger la page



		}
	}



	//Calcul de l'adresse physique
	for (int i = 0; i < bits_page.size(); i++)
	{
		//Construire en bits et traduire en décimal

		//Obtenir la valeur du byte signé
		//TODO: fix this line
		//... = fct_SignedByte(bits_page[i], bits_offset[i]);

	}



	//Ecrire le fichier de sortie



	return 0;

}
