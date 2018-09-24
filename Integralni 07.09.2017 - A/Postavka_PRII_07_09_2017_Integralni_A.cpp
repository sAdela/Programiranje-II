﻿#include <iostream>
using namespace std;

/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEĆE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ĆETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

/*
u nastavku je prikazan primjer ispitnog zapisnika (izvještaj o održanom ispitu) sa detaljima/stavkama

Datum ispita: 07.09.2017
Predmet: Programiranje II
IZVJESTAJ O ODRZANOM ISPITU
Br indeksa Ime prezime Komisijsko*** Ocjena
16001	A. Joldic	DA	5
15251	D. Music	NE	7 Ponisteno
16014	E. Babovic	DA	8
UKUPNO POLOZILO:	2
PROSJECNA OCJENA:	6.67
*** komisijski ispit: ako student pristupa četvrti put i dalje (računajući i poništene rezultate)
*/

char * crt = "\n-------------------------------------------\n";
struct Datum {
	int _dan;
	int _mjesec;
	int _godina;
	void Unos(int d, int m, int g) {
		_dan = d;
		_mjesec = m;
		_godina = g;
	}
	void Ispis() {
		cout << _dan << "." << _mjesec << "." << _godina << " ";
	}
};
struct Student {
	char _imePrezime[20];
	int _brojIndeksa;
	int _polozioECTS;

	void Unos(char* naziv, float cijena) {
		strcpy_s(_imePrezime, naziv);
		_brojIndeksa = cijena;
		_polozioECTS = 0;
	}
	void DodajECTS(float ects) {
		_polozioECTS += ects;
	}
	void UmanjiECTS(float ects) {
		_polozioECTS += ects;
	}
	void Ispis() {
		cout << _imePrezime << ", Indeks: " << _brojIndeksa << ", ECTS: " << _polozioECTS << " " << endl;
	}
};

struct ZapisnikStavka {
	int _ocjena;
	Student* _student; //ZapisnikStavka će pokazivati na objekat tipa student koji se nalazi u instanci tipa Fakultet, pa nije potrebno vršiti nove alokacije
	bool _ponisteno;
	bool _komisijskoPolaganje;

	void Unos(Student* proizvod, int ocjena, bool komisijskoPolaganje) {
		_ocjena = ocjena;
		_student = proizvod;
		_komisijskoPolaganje = komisijskoPolaganje;
		_ponisteno = false;
	}

	void Ispis() {
		cout << _student->_imePrezime << "\t" << _ocjena << "\t" << _student->_brojIndeksa << "\t";
		if (_ponisteno)
			cout << " Ponisteno";
		cout << endl;
	}
};
struct IspitniZapisnik {
	Datum _datum;
	ZapisnikStavka ** _stavke;//svaki put se dodaje po jedna stavka
	int _brojStavki;
	int _ects;
	char _predmet[20];
	void Unos(Datum datum, char* predmet, int ects) {
		_datum = datum;
		_brojStavki = 0;
		_stavke = nullptr;
		_ects = ects;
		strcpy_s(_predmet, predmet);
	}
	/*Prosjecnu ocjenu izračunati koristeći lambda funkciju koja se poziva koristeći objekat
	tipa auto. u prosjek računati i poništene rezultate (stavke)*/
	float GetProsjecnaOcjena() {
		return 0;
	}
};
const int Max = 100;

/*unutar objekta tipa Fakultet ce se cuvati podaci o svim studentima i ispitnim zapisnicima, a dodaju se pomocu globalnih funkcija*/
struct Fakultet {
	//umjesto brojaca koristiti default.ne vrijednosti
	Student* Studenti[Max] = { NULL };
	IspitniZapisnik* Zapisnici[Max] = { NULL };

	void Dealociraj() {
		for (size_t i = 0; i < Max; i++) {
			delete Studenti[i]; Studenti[i] = NULL;
		}
		for (size_t i = 0; i < Max; i++) {
			delete Zapisnici[i]; Zapisnici[i] = NULL;
		}
	}
};
void main() {
	Fakultet fakultet;

	/*dodaje podatke o novom studentu u fakultet */
	//fakultet, imePrezime, broj indeksa
	dodaj_student(fakultet, "A. Joldic", 16001);
	dodaj_student(fakultet, "D. Music", 15251);
	dodaj_student(fakultet, "E. Babovic	", 16014);
	dodaj_student(fakultet, "E. Junuz", 16015);
	dodaj_student(fakultet, "J. Azemovic", 16014);//ne dodaje studenta jer je broj indeksa zauzet
												  /*kraira novi zapisnik*/
												  //fakultet, datum, predmet, ects krediti
	kreiraj_zapisnik(fakultet, 6, 9, 2017, "Programiranje 2", 7);

	/*dodaje stavku na posljednji kreiran zapisnik, a ects studenta uveća ako je student položio ispit. */

	if (dodaj_stavku_na_zapisnik(fakultet, 16001, 5))
		cout << "Stavka (rezultat) uspjesno dodata!" << endl;

	/*ako je student vec dodat u zapisnik, onda se vrsi samo modifikacija njegove ocjene*/
	if (!dodaj_stavku_na_zapisnik(fakultet, 16001, 6))
		cout << "Student vec dodat na zapisnik. Ocjena je ispravljena!" << endl;

	dodaj_stavku_na_zapisnik(fakultet, 15251, 5);
	dodaj_stavku_na_zapisnik(fakultet, 16015, 6);
	dodaj_stavku_na_zapisnik(fakultet, 16014, 8);

	kreiraj_zapisnik(fakultet, 7, 9, 2017, "Računarske mreze", 5);
	//naredne stavke se dodaju na novi zapisnik
	dodaj_stavku_na_zapisnik(fakultet, 16015, 5);
	dodaj_stavku_na_zapisnik(fakultet, 16014, 10);


	/*u posljednjem zapisniku stavku za studenta 16001 ponistiti, a studentu umanjiti ects bodove.
	funkcija vraca false u slucaju da se student ne nalazi na posljednjem zapisniku*/

	if (ponisti_stavku_rezultat(fakultet, 16001))
		cout << "Stavka (rezultat) uspjesno ponisten!" << endl;

	if (!ponisti_stavku_rezultat(fakultet, 19001))
		cout << "Student nije pronadjen!" << endl;

	/* funkcija printaj prikazuje posljednji ispitni zapisnik u formatu prikazanom na pocetku zadatka.
	* pored prikazivanja, identican sadrzaj ispitnog zapisnika je potrebno je pasiti u tekstualni fajl.
	* naziv fajla treba biti Predmet_datum.txt, npr. ProgramiranjeII_2017-09-01.txt. zapisnik se nece
	prikazati niti sacuvati u fajl ukoliko ne posjeduje niti jednu stavku ili su sve stavke ponistene*/
	if (printaj(fakultet))
		cout << "Ispitni zapisnik uspjesno prikazan i spasen u fajl!" << endl;
	fakultet.Dealociraj();
}