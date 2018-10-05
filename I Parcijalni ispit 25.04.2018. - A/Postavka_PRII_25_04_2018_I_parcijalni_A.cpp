#include <iostream>
#include<tuple>
const char * crt = "\n-------------------------------------\n";

using namespace std;

struct Putnik {
	char * _imePrezime;
	//pretpostavka je da ce se uvijek unijeti validna oznaka sjedista u formatu XXA gdje je XX dvocifreni broj, a A predstavlja veliko slovo. Jednocifrene oznake ce imati prefiks 0
	char _ozankaSjedista[4]; //npr. 02A, 12B, 14B, 28C -> broj predstavlja oznaku reda, a slovo oznaku kolone
};

struct Let {
	char * _relacija;//Mostar -> Sarajevo
	Putnik * _putnici;
	int _trenutnoPutnika;
	int _brojRedovaUAvionu;
	int _brojKolonaUAvionu;//broj kolona mora biti paran >=4 i <=10
};

void Dealociraj(Let & let) {
	delete[] let._relacija; let._relacija = nullptr;
	for (size_t i = 0; i < let._trenutnoPutnika; i++) {
		delete[] let._putnici[i]._imePrezime;
		let._putnici[i]._imePrezime = nullptr;
	}
	delete[] let._putnici;
	let._putnici = nullptr;
}

int main() {
	int oznakaR = 0, oznakaK = 0;
	Let mostar_sarajevo;
	//inicijalizuje vrijednosti atributa objekta mostar_sarajevo
	Unos(mostar_sarajevo, "Mostar -> Sarajevo", 10, 4);//relacija, broj_redova, broj_kolona

													   //GetPozicijuUReduIKoloni - na osnovu oznake sjedista vraca pair red i kolona tj. poziciju u matrici
	tie(oznakaR, oznakaK) = GetPozicijuUReduIKoloni("15B");
	cout << crt << "GetPozicijuUReduIKoloni(15B) ->" << oznakaR << "/" << oznakaK << endl;//ispisuje 14/1 
	tie(oznakaR, oznakaK) = GetPozicijuUReduIKoloni("01B");
	cout << "GetPozicijuUReduIKoloni(01B) ->" << oznakaR << "/" << oznakaK << crt;//ispisuje 0/1
																				  /*
																				  GetNovogPutnika - vraca objekat (ne pokazivac) tipa Putnik koji je inicijalizovan vrijednsotima proslijedjenih parametara
																				  DodajPutnika - podatke o novom putniku dodaje u listu (niz) putnika na proslijedjenom letu. Onemoguciti dodavanje putnika sa istim imenom i prezimenom,
																				  nepostojecom lokacijom sjedista ili u slucaju da su sva mjesta popunjena
																				  */
	if (DodajPutnika(mostar_sarajevo, GetNovogPutnika("01A", "Denis Music")))
		cout << crt << "Putnik uspjesno dodan!" << crt;
	if (DodajPutnika(mostar_sarajevo, GetNovogPutnika("07C", "Zanin Vejzovic")))
		cout << crt << "Putnik uspjesno dodan!" << crt;
	if (DodajPutnika(mostar_sarajevo, GetNovogPutnika("10D", "Adel Handzic")))
		cout << crt << "Putnik uspjesno dodan!" << crt;
	//PrikaziRasporedSjedenja - na osnovu oznake sjedista prikazuje raspored sjedenja u avionu za let koji je proslijedjen kao parametar
	PrikaziRasporedSjedenja(mostar_sarajevo);

	//rekBrojacKaraktera - rekurzivna funkcija koja vraca maksimalan broj karaktera u imenu i prezimenu putnika na odredjenom letu
	cout << crt << "Maksimalna broj karaktera u imenu i prezimenu putnika je -> " << rekBrojacKaraktera(mostar_sarajevo,
		mostar_sarajevo._trenutnoPutnika, 0) << crt; //broj 0 je pocetno stanje prilikom brojanja max karaktera

													 /*
													 DODATNO:
													 1. prilikom ispisa, sirinu kolone prilagoditi maksimalnom broju karaktera u imenu i prezimenu
													 2. automatski generisati oznaku sjedista na osnovu narednog slobodnog mjesta na letu (povratnom vrijednoscu sugerisati na slucaj u kome su sva mjesta na letu vec zauzeta). ovu funkciju nije potrebno integrisati u postojece rjesenje, vec se moze i zasebno testirati
													 */

	Dealociraj(mostar_sarajevo);
	system("pause>0");
	return 0;
}