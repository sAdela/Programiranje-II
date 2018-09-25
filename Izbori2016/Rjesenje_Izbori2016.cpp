//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

#include<iostream>
#include<iomanip>
#include<fstream>

using namespace std;

enum Opstina { Opstina1, Opstina2, Opstina3, Opstina4 };
const char * OpstineChar[] = { "Opstina1", "Opstina2", "Opstina3", "Opstina4" };
enum Kandidati { Kandidat1, Kandidat2, Kandidat3, Kandidat4 };
const char * KandidatiChar[] = { "Kandidat1", "Kandidat2", "Kandidat3", "Kandidat4" };

struct Datum {
	int _dan, _mjesec, _godina;
	void Unos(int d, int m, int g) { _dan = d; _mjesec = m; _godina = g; }
	void Ispis() { cout << _dan << "/" << _mjesec << "/" << _godina << endl; }
	int PretvoriUDane() {
		return _godina * 365 + _mjesec * 30 + _dan;
	}
};

const Datum datumIzbora = { 14, 7, 2016 };

struct Osoba {
	Datum _datumRodjenja;
	char * _imePrezime;
	char _JMBG[14];
	Opstina _Prebivaliste;
	void unos(Datum d, const char * ip, const char jmbg[], Opstina p) {
		_datumRodjenja = d;
		_imePrezime = new char[strlen(ip) + 1];
		strcpy(_imePrezime, ip);
		strcpy(_JMBG, jmbg);
		_Prebivaliste = p;
	}
	void Dealociraj() { delete[] _imePrezime; _imePrezime = nullptr; }
	void Info() {
		_datumRodjenja.Ispis();
		cout << _imePrezime << " " << _JMBG << " " <<
			OpstineChar[_Prebivaliste] << endl;
	}
};
struct Glas {
	Osoba * _glasac;
	Kandidati * _glasZa;
	void Unos(Osoba o, Kandidati k) {
		_glasac = new Osoba;
		_glasac->unos(o._datumRodjenja, o._imePrezime, o._JMBG, o._Prebivaliste);
		_glasZa = new Kandidati(k);
	}
	void Ispis() {
		_glasac->Info();
		cout << KandidatiChar[*_glasZa] << endl;
	}
	void Dealociraj() { _glasac->Dealociraj(); }
};

struct Glasovi {

	int _brojGlasova;
	Kandidati *kand;

	void Unos(int broj, Kandidati k) {
		kand = new Kandidati(k);
		_brojGlasova = broj;
	}

};
struct Izbori2016 {
	Opstina * _glasackoMjesto;
	Glas * _glasovi[1000];
	int _doSadaGlasalo;

	void Unos(Opstina opstina) {
		// 	1. Unos - na osnovu vrijednosti primljenih parametara, inicijalizovati vrijednosti atributa strukture.

		_glasackoMjesto = new Opstina(opstina); 
												/*for (int i = 0; i < 1000; i++)
												_glasovi[i] = nullptr;*/
		_doSadaGlasalo = 0;

	}

	bool DodajGlas(Glas g) {
		//	2. DodajGlas - funkcija treba da omoguci dodavanje informacija o novom glasu
		//  i tom prilikom onemoguciti: da ista osoba glasa vise puta, glasanje osobama mladjim od 18 godina
		//	(uzeti u obzir samo mjesec i godinu rodjenja), glasanje osobama iz drugih opstina.
		// U zavisnosti od uspjesnosti operacije funkcija vraca true ili false


		for (int i = 0; i < _doSadaGlasalo; i++)
			if (strcmp(_glasovi[i]->_glasac->_imePrezime, g._glasac->_imePrezime) == 0)
				return false;


		int godina = datumIzbora._godina - g._glasac->_datumRodjenja._godina;

		if (godina < 17)
			return false;

		if (godina == 17)
			if (datumIzbora._mjesec > g._glasac->_datumRodjenja._mjesec)
				return false;

		if (*_glasackoMjesto != g._glasac->_Prebivaliste)
			return false;

		if (_doSadaGlasalo == 999)
			return false;

		_glasovi[_doSadaGlasalo] = new Glas;
		_glasovi[_doSadaGlasalo]->Unos(*g._glasac, *g._glasZa);
		_doSadaGlasalo++;


		return true;

	}




	Opstina Ispis() {

		//	3. Ispis - u zavisnosti od postignutog uspjeha funkcija treba ispisati 
		//	listu kandidata sortiranu opadajucim redoslijedom,
		//	a uz svakog kandidata je potrebno ispisati i osvojeni procenat glasova.
		//	Funkcija vraca opstinu u kojoj je pobjednicki kandidat ostvario najveci broj glasova,
		//	a ukoliko je pobjednicki kandidat ostvario podjednak broj glasova u vise opstina,
		//	funkcija vraca prvu koja se nalazi u nizu na koji pokazuje pokazivac izbornaMjesta

		int sumaGlasova[4] = { 0 };

		Opstina *opstina = nullptr;

		Glasovi * niz[4];

		for (int i = 0; i < _doSadaGlasalo; i++)

			sumaGlasova[*_glasovi[i]->_glasZa]++;



		for (int i = 0; i < 4; i++) {

			niz[i] = new Glasovi;
			niz[i]->Unos(sumaGlasova[i], Kandidati(i));
		}



		bool promjena = true;
		Glasovi *temp = new Glasovi;

		while (promjena) {
			promjena = false;
			for (int i = 0; i < 3; i++) {
				if (niz[i]->_brojGlasova < niz[i + 1]->_brojGlasova) {

					temp->Unos(niz[i]->_brojGlasova, *niz[i]->kand);
					niz[i]->Unos(niz[i + 1]->_brojGlasova, *niz[i + 1]->kand);
					niz[i + 1]->Unos(temp->_brojGlasova, *temp->kand);

					promjena = true;
				}
			}
		}
		cout << "Kandidat \t" << "Broj glasova \t" << "Procenat \t" << endl;
		for (int i = 0; i < 4; i++) {

			cout << i + 1 << ". " << KandidatiChar[*niz[i]->kand] << "\t\t" << niz[i]->_brojGlasova << setprecision(2) << "\t" << (float(niz[i]->_brojGlasova) / _doSadaGlasalo)*100 << "%" << endl;

		}

		return *_glasackoMjesto;
	}

	int BrojGlasacaPoRodjenju(Datum OD, Datum DO) {
	
		//	4. BrojGlasacaPoRodjenju - funkcija vraca broj glasaca koji su do tog momenta glasali,
		//a koji su rodjeni u parametrima definisanom periodu (parametri su tipa datum, period OD i DO)
		Datum temp;

		if (DO._godina < OD._godina) {
			temp.Unos(OD._dan, OD._mjesec, OD._godina);
			OD.Unos(DO._dan, DO._mjesec, DO._godina);
			DO.Unos(temp._dan, temp._mjesec, temp._godina);
		}

		int brojac = 0;
		
		for (int i = 0; i < _doSadaGlasalo; i++) {

			if (OD.PretvoriUDane() <= _glasovi[i]->_glasac->_datumRodjenja.PretvoriUDane()
				&& DO.PretvoriUDane() >= _glasovi[i]->_glasac->_datumRodjenja.PretvoriUDane())

				brojac++;
	
		}


		return brojac;
	}
	
	
};

int PretragaRekurzivno(Izbori2016 izbori, Kandidati kandidat, int brojGlasova = 0, int glasalo = 0) {
	
	if (glasalo == izbori._doSadaGlasalo)
		return brojGlasova;

	if (*izbori._glasovi[glasalo]->_glasZa == kandidat)
		return PretragaRekurzivno(izbori, kandidat, brojGlasova + 1, glasalo + 1);
	else
		return PretragaRekurzivno(izbori, kandidat, brojGlasova, glasalo + 1);


}

/*
2. UcitajIzBinarnogFajla - u objekat koji je primljen kao parametar ucitava sadrzaj binarnog fajla (naziv fajla je takodjer proslijedjen kao parametar)
3. PretragaRekurzivno - rekurzivna funkcija koja treba da vrati broj glasova koje je na izborima ostvario odredjeni kandidat (kandidat se proslijedjuje kao parametar)
*/

void UpisiUBinarniFajl(Izbori2016 &izbori) {
//	1. UpisiUBinarniFajl - objekat koji je primljen kao parametar upisuje u binarni fajl.
//	Naziv fajla treba biti identican nazivu opstine u kojoj se odrzavaju izbori
	int vel = strlen(OpstineChar[*izbori._glasackoMjesto]) + 5;
	char *imeFajla = new char [vel];
	strcpy_s(imeFajla, vel, OpstineChar[*izbori._glasackoMjesto]);
	strcat_s(imeFajla, vel, ".bin");

	ofstream upis(imeFajla, ios::binary);
	if (upis.good())
		upis.write((char*)&izbori, sizeof(Izbori2016));
	upis.close();
}

void UcitajIzBinarnogFajla(Izbori2016 &izbori_FromFile, const char* opstina) {
	int vel = strlen(opstina) + 5;
	char *imeFajla = new char[vel];
	strcpy_s(imeFajla, vel, opstina);
	strcat_s(imeFajla, vel, ".bin");

	ifstream ispis(imeFajla, ios::binary);
	if (ispis.good())
		ispis.read((char*)&izbori_FromFile, sizeof(Izbori2016));
	ispis.close();

}

void main() {
	/*
	1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NE�E BODOVATI
	2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
	3. KREIRAJTE .DOC FAJL SA VA�IM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJE�ENJA VA�IH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
	4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORI�TENJE HELP-A
	5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI �ETE KOPIRATI VA�A RJE�ENJA)
	6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URA�ENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
	*/

	Datum datumGlasac1, datumGlasac2, datumGlasac3, datumGlasac4, datumIzbora;
	datumGlasac1.Unos(18, 5, 1990);
	datumGlasac2.Unos(5, 3, 1982);
	datumGlasac3.Unos(12, 8, 1958);
	datumGlasac4.Unos(22, 6, 2000);

	Osoba a, b, c, d, e;
	a.unos(datumGlasac1, "Denis Music", "1111111111111", Opstina1);
	b.unos(datumGlasac2, "Jasmin Azemovic", "1111111111112", Opstina1);
	c.unos(datumGlasac3, "Indira Hamulic", "1111111111113", Opstina1);
	d.unos(datumGlasac4, "Adel Handzic", "1111111111114", Opstina2);
	e.unos(datumGlasac4, "Zanin Vejzovic", "1111111111115", Opstina1);


	Glas g1, g2, g3, g4, g5;
	g1.Unos(a, Kandidat1);
	g2.Unos(b, Kandidat1);
	g3.Unos(c, Kandidat2);
	g4.Unos(d, Kandidat1);
	g5.Unos(e, Kandidat2);

	Izbori2016 izbori, izbori_FromFile;
	izbori.Unos(Opstina1);

	izbori.DodajGlas(g1);
	izbori.DodajGlas(g2);
	izbori.DodajGlas(g3);
	izbori.DodajGlas(g4);
	izbori.DodajGlas(g5);
	izbori.Ispis();

	Datum OD, DO;
	OD.Unos(17, 7, 1998);
	DO.Unos(17, 7, 1986);
	cout << "Glasaca u starosti od 18 - 30 godina -> " << izbori.BrojGlasacaPoRodjenju(OD, DO) << endl;
	cout << "Broj glasova za Kandidat1 -> " << PretragaRekurzivno(izbori, Kandidat1) << endl;
	UpisiUBinarniFajl(izbori);
	UcitajIzBinarnogFajla(izbori_FromFile, "Opstina1");
	izbori_FromFile.Ispis();
	system("Pause>0");
}
