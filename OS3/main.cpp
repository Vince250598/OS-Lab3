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
//Cette fonction retourne la valeur du byte sign�
//Cr�� par: 
//Date: 
//Modifi� par:
//Description:
////////////////////////////////////////////////////////////////
/*int fct_SignedByte(int page, int offset,std::string nomduFichier)
{
	//Ouvrir le fichier binaire
	std::ifstream fichierBinaire;
	fichierBinaire.open(nomduFichier);

	unsigned int LENGTH = 1; //Le byte sign� a une longueur de 1 byte


	nomduFichier.std::seekg(...); //Trouver l'endroit correspondant au byte sign� dans le fichier
	nomduFichier.std::read(...); //Lire cet emplacement

	//Fermer le fichier
	fichierBinaire.close();

	//Retourner la valeur du byte sign�

}
*/
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

int rightRotate(int n, unsigned int d) 
{ 
    return (n >> d)|(n << (32 - d)); 
} 

int leftRotate(int n, unsigned int d) 
{ 
    return (n << d)|(n >> (32 - d)); 
} 

void LireFichierBin(ifstream &fichierBin, char * &buffer, int positionDebut, int positionFin)
{
	string ligne;
	for (int i = 0; !fichierBin.eof(); i++)
	{
		getline(fichierBin, ligne);
		if(i < positionDebut || i > positionFin + 1) continue;

		
	}
}

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
	int memPhysique[256][256] = { 0 }; //M�moire physique
	int adressePhysique[1000] = { 0 }; //Adresses Physiques(on utilise juste pour fichier output)
	int tablePage[256][2] = { 0 }; //Table de page, 1 espace pour l'adresse et l'autre pour le dirty bit
	vector<int>adresseLogique; //Adresses Logiques a traduire, prises a partir du fichier addresses.txt
	int TLB[16][2];
	vector<int> ordreTLB;

	//Lire le fichier d'adresses � traduire
	LireFichierAdresses("OS3/addresses.txt", adresseLogique);


	//Traduire l'adresse physique en adresse logique
	//1. Traduire l'entier en bits

	//Stocker les nombres binaires dans un vecteur

	std::vector<int>bits_offset, bits_page; //Un vecteur pour les bits de page et un autre pour les bits d'offset

	//Cr��er un masque pour lire juste les bits 0 � 7 (offset)
	unsigned r = 0;
	r = createMask(0, 7);

	//Cr�er un masque pour lire juste les bits 8 � 15 (page)
	unsigned r2 = 0;
	r2 = createMask(8, 15);

	//Boucler sur les 1000 adresses
	for (int i = 0; i < adresseLogique.size(); i++)
	{
		bits_offset.push_back(adresseLogique.at(i) & r);
		unsigned a = adresseLogique.at(i) & r2;
		a = rightRotate(a, 8);
		bits_page.push_back(a);
	}



	ifstream fichierSimulDisqueBinaire("OS3/simuleDisque.bin", ios::binary | ios::in);
	if(!fichierSimulDisqueBinaire.is_open()){
		cout << "ddsadsdad";
	}

	//Table de pages
	//Une adresse � la fois, v�rifier si elle est dans la table de page

	char * buffer = new char [256];

	int frame;
	int numFrame;

	for (int i = 0; i < bits_page.size(); i++)
	{
		frame = RechercheTLB(TLB, bits_page[i]);
		if(frame != -1)
		{
			adressePhysique[i] = leftRotate(frame, 8);
			adressePhysique[i] += bits_offset.at(i);
			TLBHits++;
			numFrame = frame;
		}
		else if (tablePage[bits_page[i]][1] != 1)
		{
			pageFaults++;
			int position = bits_page.at(i) * 256;
			fichierSimulDisqueBinaire.seekg(position);
			fichierSimulDisqueBinaire.read(buffer, 256);

			/*for (int i = 0; i < 256; i++)
			{
				cout << buffer[i]<< endl;
			}*/
			//on cherche le premier frame libre dans la memoire
			for (int i = 0; i < 256; i++)
			{
				if(memPhysique[i] == 0) 
				{
					numFrame = i;
					break;
				}
			}
			
			for (int i = 0; i < 256; i++)
			{
				memPhysique[numFrame][i] = buffer[i];
			}
			


			//Je met le bit a 1 pour dire que la page est chargee
			adressePhysique[i] = leftRotate(numFrame, 8);
			adressePhysique[i] += bits_offset.at(i);
			tablePage[bits_page[i]][0] = numFrame;
			tablePage[bits_page[i]][1] = 1;
		} else
		{
			adressePhysique[i] = leftRotate(tablePage[bits_page[i]][0], 8);
			adressePhysique[i] += bits_offset.at(i);
		}
		

		//TODO: ajouter function de gestion du tlb
		GestionTLB(TLB, bits_page[i], tablePage[bits_page[i]][0]);
		octetsSignes[i] = numFrame*PAGE_t + bits_offset.at(i);
		valeurOctetsSignes[i] = memPhysique[numFrame][bits_offset[i]];
		
	}



	//Calcul de l'adresse physique
	for (int i = 0; i < bits_page.size(); i++)
	{
		//Construire en bits et traduire en d�cimal

		//Obtenir la valeur du byte sign�
		//TODO: fix this line
		//... = fct_SignedByte(bits_page[i], bits_offset[i]);

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
		output << octetsSignes[i] << "\t" << memPhysique[octetsSignes[i]] << endl;
	}
	

	return 0;

}
