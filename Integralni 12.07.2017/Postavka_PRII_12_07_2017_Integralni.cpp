﻿Izvršiti definiciju funkcija na način koji odgovara opisu(komentarima) datom neposredno uz pozive ili nazive funkcija.Možete dati komentar na bilo koju liniju code - a koju smatrate da bi trebalo unaprijediti ili da će eventualno uzrokovati grešku prilikom kompajliranja.Također, možete dodati dodatne funkcije koje će vam olakšati implementaciju programa.

#include <iostream>
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

char * crt = "\n-------------------------------\n";

struct Kandidat {
	char * _imePrezime;
	char _JMBG[14];
	void Unos(char * ip, char * jmbg) {
		_imePrezime = new char[strlen(ip) + 1];
		strcpy_s(_imePrezime, strlen(ip) + 1, ip);
		strcpy_s(_JMBG, jmbg);
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	void Ispis() {
		cout << _imePrezime << " " << _JMBG << " ";
	}
};

struct Pitanje {
	char * _tekstPitanja;
	char * _odgovori[10];//maksimalno 10 odgovora
	int _tacni[10];
	/*lokacije tacnih odgovora - svako pitanje moze imati jedan ili vise tacnih odgovora, a njihove lokacije se pohranjuju u niz _tacni*/
	int _bodova;
	/*ako pitanje ima vise tacnih odgovora, onda se broj osvojenih bodova (tokom izrade testa) odredjuje na osnovu broj tacnih odgovora (npr. ako pitanje nosi 5 boda i ima 2 tacna odgovora, a student oznaci 1 tacan odgovor onda ce osvojeni broj bodova na tom pitanju iznositi 2.5. drugim rijecima, vrijednost ovog atributa je bitna zbog odredjivanja uspjeha kandidata prilikom izrade ispita)*/

	void Unos(char * tekstPitanja, int bodova) {
		_tekstPitanja = new char[strlen(tekstPitanja) + 1];
		strcpy_s(_tekstPitanja, strlen(tekstPitanja) + 1, tekstPitanja);
		/*umjesto dodavanja brojaca iskoristite defaultne vrijednosti clanova niza kako biste manipulisali njihovim vrijednostima (dodavanje, uklanjanje)*/
		for (size_t i = 0; i < 10; i++) {
			_odgovori[i] = nullptr;
			_tacni[i] = -1;
		}
		_bodova = bodova;
	}
	void Dealociraj() {

		delete[] _tekstPitanja; _tekstPitanja = nullptr;
		for (size_t i = 0; i < 10; i++)
		{
			if (_odgovori[i] == nullptr)
				break;
			delete[] _odgovori[i]; _odgovori[i] = nullptr;
		}
	}
};
struct PrijemniIspit {
	//kandidati prijavljeni za prijemni ispit
	Kandidat * _prijavljeniKandidati[100];
	/*uspjeh ostvaren na prijemnom ispitu za svakog pojedinog kandidata(prijemnom ispitu moze pristupiti maksimalno 100 kandidata i za svakog od njih se cuva ostvareni uspjeh u okviru niza _uspjehKandidata (nakon sto urade ispit tj. nakon sto odgovore na pitanja)*/
	float * _uspjehKandidata[100];
	//prag prolaznosti na prijemnom ispitu npr. 60 procenata
	float _pragProlaznosti;
	float _kazneniBodovi;
	/*vrijednost kazenog boda (npr. 0.30) se mnozi sa svakim netacnim odgovorom na pitanje i oduzima od osvojenih bodova za to pitanje (npr. ako pitanje nosi 5 bodova i ima 2 tacna odgovora, a student odabere 1 tacan i 2 netacna odgovora, broj osvojenih bodova ce biti 1.9 (1 tacan = 2.5; 2 netacna * 0.30 kaznenih = 0.6; 2.5 - 0.6 = 1.9)*/

	void Unos(float pragProlaznosti, float kazneniBodovi) {
		_pragProlaznosti = pragProlaznosti;
		_kazneniBodovi = kazneniBodovi;
		/*umjesto dodavanja brojaca iskoristite defaultne vrijednosti clanova niza kako biste manipulisali njihovim vrijednostima (dodavanje, uklanjanje)*/
		for (size_t i = 0; i < 100; i++)
		{
			_prijavljeniKandidati[i] = nullptr;
			_uspjehKandidata[i] = nullptr;
		}
	}
};
int main() {

	Kandidat jasmin, adel;
	jasmin.Unos("Jasmin Azemovic", "1111111111111");
	adel.Unos("Adel Handzic", "2222222222222");

	int brojPitanja = 3;
	Pitanje * pitanja = new Pitanje[brojPitanja];
	//parametri: tekstPitanja, brojBodova
	pitanja[0].Unos("U kojem gradu se nalazi FIT?", 4);

	/*dodaje novi odgovor u listu ponudjenih odgovora. onemoguciti ponavljanje odgovora, te voditi racuna da vise odgovora moze biti tacno*/
	//parametri: tekstOdgovora,daLiJeTacanTajOdgovor
	if (pitanja[0].AddOdgovor("Mostar", true))
		cout << "Odgovor uspjesno dodan!" << crt;
	if (pitanja[0].AddOdgovor("Sarajevo", false))
		cout << "Odgovor uspjesno dodan!" << crt;
	if (!pitanja[0].AddOdgovor("Sarajevo", false))//Sarajevo je vec dodano
		cout << "Odgovor nije uspjesno dodan!" << crt;
	if (pitanja[0].AddOdgovor("Tuzla", false))
		cout << "Odgovor uspjesno dodan!" << crt;
	if (pitanja[0].AddOdgovor("Fojnica", false))
		cout << "Odgovor uspjesno dodan!" << crt;
	if (pitanja[0].AddOdgovor("Zenica", false))
		cout << "Odgovor uspjesno dodan!" << crt;
	/*
	na osnovu primljene lokacije uklanja podatke o ponudjenom odgovoru. prilikom uklanjanja ocuvati redoslijed dodavanja odgovora. onemoguciti uklanjanje svih tacnih odgovora tj. osigurati da pitanje u svakom momentu posjeduje najmanje jedan tacan odgovor
	*/
	if (pitanja[0].RemoveOdgovor(4))//uklanja odgovor Fojnica
		cout << "Odgovor uspjesno uklonjen!" << crt;

	/*
	-------------------------------
	1. U kojem gradu se nalazi FIT?
	-------------------------------
	1. Mostar
	2. Sarajevo
	3. Tuzla
	4. Zenica
	-------------------------------
	*/
	pitanja[0].Ispis();//prethodno je prikazan ocekivani ispis pitanja

	pitanja[1].Unos("Odaberite tacne odgovore.", 5);

	if (pitanja[1].AddOdgovor("FIT se nalazi u Mostaru", true))
		cout << "Odgovor uspjesno dodan!" << crt;
	if (pitanja[1].AddOdgovor("FIT je clanica UNMO-a", true))
		cout << "Odgovor uspjesno dodan!" << crt;
	if (pitanja[1].AddOdgovor("U Mostaru je uvijek jako vruce", false))
		cout << "Odgovor uspjesno dodan!" << crt;

	pitanja[2].Unos("Studij na FIT-u traje 4 godine?", 2);
	if (pitanja[2].AddOdgovor("Ne", true))
		cout << "Odgovor uspjesno dodan!" << crt;
	if (pitanja[2].AddOdgovor("Da", false))
		cout << "Odgovor uspjesno dodan!" << crt;
	/*....*/


	PrijemniIspit prijemni2017;
	//parametri: pragProlaznosti, kazneniBodovi
	prijemni2017.Unos(55, 0.30);
	/*dodaje novog kandidata u listu prijavljenih. onemoguciti dodavanje kandidata sa istim jmbg-om*/
	if (prijemni2017.DodajKandidata(jasmin))
		cout << "Kandidat uspjesno dodan" << crt;
	if (prijemni2017.DodajKandidata(adel))
		cout << "Kandidat uspjesno dodan" << crt;

	/*kandidatu sa JMBG 1111111111111 pokrece ispit na nacin da mu se prikazuju pitanja, od njega zahtijevaju odgovori, te izracunava ostvareni uspjeh (da li je polozio ili ne, te sa kojim procentom uspjesnosti). prilikom izracuvanja uspjesnosti voditi racuna o kaznenim bodovima. po okoncanju ispita, uspjeh kandidata se pohranjuje u niz _uspjehKandidata. za pokretanje ispita kandidat prethodno mora biti dodan u listu. po vlastitoj zelji implementirajte naci oznacavanja veceg broja tacnih odgovoran na postavljeno pitanje (jedno pitanje moze imati vise tacnih odgovora)*/
	if (prijemni2017.PokreniIspit("1111111111111", pitanja, brojPitanja))
		cout << "Ispit uspjesno okoncan!" << crt;

	jasmin.Dealociraj();
	adel.Dealociraj();
	for (size_t i = 0; i < brojPitanja; i++)
		pitanja[i].Dealociraj();
	delete[] pitanja;
	return 0;
}