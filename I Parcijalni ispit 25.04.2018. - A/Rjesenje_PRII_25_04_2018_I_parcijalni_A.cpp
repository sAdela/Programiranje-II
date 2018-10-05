#include <iostream>
#include<iomanip>
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

void Unos(Let &let, const char* nazivLeta, int broj_redova, int broj_kolona) {

	if (broj_kolona % 2 != 0 || broj_kolona < 4 || broj_kolona > 10)
		return;

	let._brojRedovaUAvionu = broj_redova;
	let._brojKolonaUAvionu = broj_kolona;

	int vel = strlen(nazivLeta) + 1;
	let._relacija = new char[vel];
	strcpy_s(let._relacija, vel, nazivLeta);

	let._putnici = nullptr;
	let._trenutnoPutnika = 0;

}

pair <int, int> GetPozicijuUReduIKoloni(const char* oznaka) {
	char oznakaPrva[3] = { ' ' };
	int prvi, drugi;

	strncpy_s(oznakaPrva, oznaka, 2);
	prvi = atoi(oznakaPrva) - 1;
	drugi = int(oznaka[2]) - 65;

	return make_pair(prvi, drugi);
}

Putnik GetNovogPutnika(const char* oznaka, const char* imePrezime) {
	/*
	GetNovogPutnika - vraca objekat (ne pokazivac) tipa Putnik koji je inicijalizovan vrijednsotima proslijedjenih parametara
	*/

	Putnik novi;
	int vel = strlen(imePrezime) + 1;

	novi._imePrezime = new char[vel];
	strcpy_s(novi._imePrezime, vel, imePrezime);
	strcpy_s(novi._ozankaSjedista, oznaka);

	return novi;
}

bool DodajPutnika(Let &let, Putnik putnik) {
	//	DodajPutnika - podatke o novom putniku dodaje u listu (niz) putnika na proslijedjenom letu. 
	//	Onemoguciti dodavanje putnika sa istim imenom i prezimenom,
	//	nepostojecom lokacijom sjedista ili u slucaju da su sva mjesta popunjena
	int prvi, drugi;
	for (int i = 0; i < let._trenutnoPutnika; i++)
		if (strcmp(let._putnici[i]._imePrezime, putnik._imePrezime) == 0)
			return false;

	if (let._brojKolonaUAvionu * let._brojRedovaUAvionu <= let._trenutnoPutnika)
		return false;

	tie(prvi, drugi) = GetPozicijuUReduIKoloni(putnik._ozankaSjedista);

	if (prvi >= let._brojRedovaUAvionu)
		return false;
	if (drugi >= let._brojKolonaUAvionu)
		return false;

	int vel;
	Putnik *novi = new Putnik[let._trenutnoPutnika + 1];
	for (int i = 0; i < let._trenutnoPutnika; i++) {
		vel = strlen(let._putnici[i]._imePrezime) + 1;

		novi[i]._imePrezime = new char[vel];
		strcpy_s(novi[i]._imePrezime, vel, let._putnici[i]._imePrezime);
		strcpy_s(novi[i]._ozankaSjedista, let._putnici[i]._ozankaSjedista);

		delete[] let._putnici[i]._imePrezime;
	}

	delete[] let._putnici;

	vel = strlen(putnik._imePrezime) + 1;
	novi[let._trenutnoPutnika]._imePrezime = new char[vel];

	strcpy_s(novi[let._trenutnoPutnika]._imePrezime, vel, putnik._imePrezime);
	strcpy_s(novi[let._trenutnoPutnika]._ozankaSjedista, putnik._ozankaSjedista);

	let._putnici = novi;
	let._trenutnoPutnika++;

	return true;
}


int rekBrojacKaraktera(Let let, int trenutnoPutnika, int maxKaraktera, int max=0, int putnik=0) {
	
	if (putnik == trenutnoPutnika)
		return maxKaraktera;

	if (strlen(let._putnici[putnik]._imePrezime) > max)
		return rekBrojacKaraktera(let, trenutnoPutnika, strlen(let._putnici[putnik]._imePrezime), strlen(let._putnici[putnik]._imePrezime), putnik + 1);
	else
		return rekBrojacKaraktera(let, trenutnoPutnika, maxKaraktera, max, putnik + 1);

}


void PrikaziRasporedSjedenja(Let let) {
	int red, kolona;
	bool flag = false;
	for (int i = 0; i < let._brojRedovaUAvionu; i++) {
		for (int j = 0; j < let._brojKolonaUAvionu; j++)
			cout << "+--------------";
		cout << "+" << endl << endl;
		for (int j = 0; j < let._brojKolonaUAvionu; j++) {
			cout << "|" << setw(rekBrojacKaraktera(let, let._trenutnoPutnika, 0));
			char *imePrezime = nullptr;
			for (int z = 0; z < let._trenutnoPutnika; z++) {
				tie(red, kolona) = GetPozicijuUReduIKoloni(let._putnici[z]._ozankaSjedista);

				if (red == i && kolona == j) {
					imePrezime = let._putnici[z]._imePrezime;
					break;
				}
			}
			if (imePrezime != nullptr)
				cout << imePrezime;
			else
				cout << " ";
		}
		cout << "|" << endl;
	}
	for (int j = 0; j < let._brojKolonaUAvionu; j++)
		cout << "+-------------";
	cout << "+" << endl << endl;
}

pair <int, int> automatski(Let let) {
	int prvi = -1, drugi = -1;
	pair<int, int> par;
	int flag = false;

	for (int i = 0; i < let._brojRedovaUAvionu; i++) {
		for (int j = 0; j < let._brojKolonaUAvionu; j++) {
			flag = false;
			for (int z = 0; z < let._trenutnoPutnika; z++) {

				tie(prvi, drugi) = GetPozicijuUReduIKoloni(let._putnici[z]._ozankaSjedista);
				if (prvi == i && drugi == j) {
					flag = true;
					break;
				}

			}
			if (flag == false) {
				return par = make_pair(i + 1, j);
			}
		}
	}

		cout << "Sva mjesta su puna";
		return par = make_pair(-1, -1);
	}

int main() {
	int oznakaR = 0, oznakaK = 0;
	int automatski1 = 0, automatski2 = 0;
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
																				  //	 DodajPutnika - podatke o novom putniku dodaje u listu (niz) putnika na proslijedjenom letu. Onemoguciti dodavanje putnika sa istim imenom i prezimenom,
																				  //	 nepostojecom lokacijom sjedista ili u slucaju da su sva mjesta popunjena
																				  //	 */
	if (DodajPutnika(mostar_sarajevo, GetNovogPutnika("01A", "Denis Music")))
		cout << crt << "Putnik uspjesno dodan!" << crt;
	if (DodajPutnika(mostar_sarajevo, GetNovogPutnika("01B", "Deanis Music")))
		cout << crt << "Putnik uspjesno dodan!" << crt;
	if (DodajPutnika(mostar_sarajevo, GetNovogPutnika("07C", "Zanin Vejzovic")))
		cout << crt << "Putnik uspjesno dodan!" << crt;
	if (DodajPutnika(mostar_sarajevo, GetNovogPutnika("10D", "Adel Handzic")))
		cout << crt << "Putnik uspjesno dodan!" << crt;

	////PrikaziRasporedSjedenja - na osnovu oznake sjedista prikazuje raspored sjedenja u avionu za let koji je proslijedjen kao parametar
	PrikaziRasporedSjedenja(mostar_sarajevo);

	////rekBrojacKaraktera - rekurzivna funkcija koja vraca maksimalan broj karaktera u imenu i prezimenu putnika na odredjenom letu
	cout << crt << "Maksimalna broj karaktera u imenu i prezimenu putnika je -> " << rekBrojacKaraktera(mostar_sarajevo,
		mostar_sarajevo._trenutnoPutnika, 0) << crt; //broj 0 je pocetno stanje prilikom brojanja max karaktera

	/*											 
	//	 DODATNO:
	//	 1. prilikom ispisa, sirinu kolone prilagoditi maksimalnom broju karaktera u imenu i prezimenu
	//IMPLEMENTIRANO U FUNKCIJI PrikaziRasporedSjedenja
	//	 2. automatski generisati oznaku sjedista na osnovu narednog slobodnog mjesta na letu (povratnom vrijednoscu sugerisati na slucaj u kome su sva mjesta na letu vec zauzeta). ovu funkciju nije potrebno integrisati u postojece rjesenje, vec se moze i zasebno testirati
	//FUNKCIJA AUTOMATSKI 
	*/ 
	tie(automatski1, automatski2) = automatski(mostar_sarajevo);
	cout << automatski1;
	char neki = ' ';
	neki = char(automatski2 + 65);
	cout << neki;

	Dealociraj(mostar_sarajevo);
	system("pause>0");
	return 0;
}