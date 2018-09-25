﻿//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum Opstina { Opstina1, Opstina2, Opstina3, Opstina4 };
char * OpstineChar[] = { "Opstina1", "Opstina2", "Opstina3", "Opstina4" };
enum Kandidati { Kandidat1, Kandidat2, Kandidat3, Kandidat4 };
char * KandidatiChar[] = { "Kandidat1", "Kandidat2", "Kandidat3", "Kandidat4" };

struct Datum {
	int _dan, _mjesec, _godina;
	void Unos(int d, int m, int g) { _dan = d; _mjesec = m; _godina = g; }
	void Ispis() { cout << _dan << "/" << _mjesec << "/" << _godina << endl; }
};
struct Osoba {
	Datum _datumRodjenja;
	char * _imePrezime;
	char _JMBG[14];
	Opstina _Prebivaliste;
	void unos(Datum d, char * ip, char jmbg[], Opstina p) {
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
		_glasac = o;
		_glasac->unos(o._datumRodjenja, o._imePrezime, o._JMBG, o._Prebivaliste);
		_glasZa = new Kandidati(k);
	}
	void Ispis() {
		_glasac->Info();
		cout << KandidatiChar[*_glasZa] << endl;
	}
	void Dealociraj() { _glasac->Dealociraj(); }
};
struct Izbori2016 {
	Opstina * _glasackoMjesto;
	Glas * _glasovi[1000];
	int _doSadaGlasalo;

	/*
	1. Unos - na osnovu vrijednosti primljenih parametara, inicijalizovati vrijednosti atributa strukture.
	2. DodajGlas - funkcija treba da omoguci dodavanje informacija o novom glasu i tom prilikom onemoguciti: da ista osoba glasa vise puta, glasanje osobama mladjim od 18 godina (uzeti u obzir samo mjesec i godinu rodjenja), glasanje osobama iz drugih opstina. U zavisnosti od uspjesnosti operacije funkcija vraca true ili false
	3. Ispis - u zavisnosti od postignutog uspjeha funkcija treba ispisati listu kandidata sortiranu opadajucim redoslijedom, a uz svakog kandidata je potrebno ispisati i osvojeni procenat glasova.
	Funkcija vraca opstinu u kojoj je pobjednicki kandidat ostvario najveci broj glasova, a ukoliko je pobjednicki kandidat ostvario podjednak broj glasova u vise opstina, funkcija vraca prvu koja se nalazi u nizu na koji pokazuje pokazivac izbornaMjesta
	4. BrojGlasacaPoRodjenju - funkcija vraca broj glasaca koji su do tog momenta glasali, a koji su rodjeni u parametrima definisanom periodu (parametri su tipa datum, period OD i DO)
	*/
};
/*
1. UpisiUBinarniFajl - objekat koji je primljen kao parametar upisuje u binarni fajl. Naziv fajla treba biti identican nazivu opstine u kojoj se odrzavaju izbori
2. UcitajIzBinarnogFajla - u objekat koji je primljen kao parametar ucitava sadrzaj binarnog fajla (naziv fajla je takodjer proslijedjen kao parametar)
3. PretragaRekurzivno - rekurzivna funkcija koja treba da vrati broj glasova koje je na izborima ostvario odredjeni kandidat (kandidat se proslijedjuje kao parametar)
*/

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
	datumIzbora.Unos(14, 7, 2016);
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
	cout << "Glasaca u starost od 18 - 30 godina->" << izbori.BrojGlasacaPoRodjenju(OD, DO) << endl;
	cout << "Broj glasova za Kandidat1->" << PretragaRekurzivno(/*potrebni parametri*/) << endl;
	UpisiUBinarniFajl(izbori);
	UcitajIzBinarnogFajla(izbori_FromFile, "Opstina1");
	izbori_FromFile.Ispis();
}