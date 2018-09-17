#include<iostream>
using namespace std;
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NE�E BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VA�IM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJE�ENJA VA�IH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORI�TENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI �ETE KOPIRATI VA�A RJE�ENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URA�ENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/

//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

const char *crt = "\n-------------------------------------------\n";

enum eNacinStudiranja { REDOVAN, DL };
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };

struct Datum {
	int _dan, _mjesec, _godina;
	void Unos(int d, int m, int g) { _dan = d; _mjesec = m; _godina = g; }
	void Ispis() { cout << _dan << "/" << _mjesec << "/" << _godina << endl; }
	int PretvoriUDane() {
		return _dan + _mjesec * 30 + _godina * 365;
	}
};

Datum rokZaPrijavu = { 5, 7, 2017 };

const char * errVecDodan = "Istoimeni predmet se moze dodati najvise dva puta!";
const char * errProsaoRok = "Zao nam je. Rok za prijavu je vec prosao!";
const char * uspjesnoDodan = "Predmet uspjesno dodan!";

struct Predmet {
	char * _naziv;
	int * _ocjena;
	Datum _datumUnosa;
	void Unos(const char * naziv, int ocjena, Datum datumUnosa) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_ocjena = new int(ocjena);
		_datumUnosa.Unos(datumUnosa._dan, datumUnosa._mjesec, datumUnosa._godina);
	}
	void Dealociraj() {
		delete[] _naziv; _naziv = nullptr;
		delete _ocjena; _ocjena = nullptr;
	}

	void Ispis() {
		cout << _naziv << " (" << *_ocjena << ") "; _datumUnosa.Ispis();
	}
	void PromijeniOcjenu(int ocjena) { *_ocjena = ocjena; }
};

struct Uspjeh {
	eRazred  _razred;
	bool _najboljiUcenik; // ako je kandidat u tom razredu proglasen najboljim ucenikom
	shared_ptr<Predmet> * _predmeti;
	int _brojPredmeta;
	void Unos(eRazred razred, bool najboljiUcenik) {
		_razred = razred;
		_najboljiUcenik = najboljiUcenik;
		_brojPredmeta = 0;
		_predmeti = nullptr;
	}
	void Dealociraj() {
		for (int i = 0; i < _brojPredmeta; i++)
			_predmeti[i]->Dealociraj();
		delete[] _predmeti; _predmeti = nullptr;
	}
	void Ispis() {
		cout << crt << "Razred -> " << _razred << "Najbolji -> " << _najboljiUcenik << crt;
		for (size_t i = 0; i < _brojPredmeta; i++)
			_predmeti[i]->Ispis();
	}
};

struct Kandidat {
	eNacinStudiranja _nacinStudiranja;
	char * _imePrezime;
	Uspjeh * _uspjeh[4];

	void Unos(eNacinStudiranja nacinStudiranja, const char * imePrezime) {
		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);
		_nacinStudiranja = nacinStudiranja;
		for (size_t i = 0; i < 4; i++)
			_uspjeh[i] = nullptr;
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < 4; i++) {
			if (_uspjeh[i] != nullptr) {
				_uspjeh[i]->Dealociraj();
				delete _uspjeh[i];
			}
		}
	}
	void Ispis() {
		cout << crt << _imePrezime << " " << _nacinStudiranja;
		for (size_t i = 0; i < 4; i++)
			if(_uspjeh[i] != nullptr)
			_uspjeh[i]->Ispis();
	}

	bool DodajPredmet(eRazred razred, Predmet predmet) {

		//uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda. prilikom dodavanja onemoguciti:
		//- istoimeni predmet se moze dodati samo dva puta, bez obzira sto se dodaje na razlicitim godinama,
		//- dodavanje predmeta nakon dozvoljenog roka za prijavu (rokZaPrijavu).
		//razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom 
		//(npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.). 
		//Funkcija vraca neku od predefinisanih poruka u zavisnosti od (ne)uspjesnost izvrsenja
		int br = 0;

		for (int i = 0; i < 4; i++) {

			if (_uspjeh[i] != nullptr) {

				for (int j = 0; j < _uspjeh[i]->_brojPredmeta; j++) {

					if (strcmp(_uspjeh[i]->_predmeti[j]->_naziv, predmet._naziv) == 0)

						br++;
				}
			}
		}

		if (br > 2) return false;

		if (rokZaPrijavu.PretvoriUDane() <= predmet._datumUnosa.PretvoriUDane())
			return false;

		int raz = int(razred) - 1;

		if (_uspjeh[raz] == nullptr) {
			_uspjeh[raz] = new Uspjeh;
			_uspjeh[raz]->Unos(razred, false);
		}

		shared_ptr<Predmet> *novi = new shared_ptr<Predmet>[_uspjeh[raz]->_brojPredmeta + 1];

		for (int i = 0; i < _uspjeh[raz]->_brojPredmeta; i++) {

			novi[i] = make_shared<Predmet>();
			novi[i]->Unos(_uspjeh[raz]->_predmeti[i]->_naziv, *_uspjeh[raz]->_predmeti[i]->_ocjena, _uspjeh[raz]->_predmeti[i]->_datumUnosa);
			_uspjeh[raz]->_predmeti[i]->Dealociraj();
		}
		novi[_uspjeh[raz]->_brojPredmeta] = make_shared<Predmet>();
		novi[_uspjeh[raz]->_brojPredmeta]->Unos(predmet._naziv, *predmet._ocjena, predmet._datumUnosa);
		delete[] _uspjeh[raz]->_predmeti;
		_uspjeh[raz]->_predmeti = novi;
		_uspjeh[raz]->_brojPredmeta++;


		return true;
	}
};

Kandidat *rekNajboljiProsjek(Kandidat *kandidat, int brojKandidata, eRazred razred, Kandidat *najbolji = nullptr, int brojPredmeta=0, float prosjek=0, int suma = 0, int trenutni=0) {

	//napisati rekurzivnu funkciju koja ce vratiti pokazivac na kandidata sa najvecim prosjekom u razredu 
 	//koji je proslijedjen kao parametar. ukoliko je vise kandidata ostvarilo isti prosjek, 
	//funkcija treba da vrati posljednje pronadjenog kandidata.
	//u slucaju da niti jedan kandidat nije evidentirao uspjeh u trazenom razredu, funkcija vraca nullptr. 

	if (brojKandidata < 1) return najbolji; 
	if (kandidat[brojKandidata - 1]._uspjeh[razred - 1] != nullptr) {
		if (brojPredmeta == kandidat[brojKandidata - 1]._uspjeh[razred - 1]->_brojPredmeta) {
			prosjek = suma / brojPredmeta;
			if (prosjek >= trenutni) {
				trenutni = prosjek;
				najbolji = &kandidat[brojKandidata - 1];
			}

			return rekNajboljiProsjek(kandidat, brojKandidata - 1, razred, najbolji, 0, 0, 0, trenutni);
		}
		else if (brojPredmeta < kandidat[brojKandidata - 1]._uspjeh[razred-1]->_brojPredmeta) {
			suma += *kandidat[brojKandidata - 1]._uspjeh[razred - 1]->_predmeti[brojPredmeta]->_ocjena;
			return rekNajboljiProsjek(kandidat, brojKandidata, razred, najbolji, brojPredmeta + 1, prosjek, suma, trenutni);
		}
	}
	else return rekNajboljiProsjek(kandidat, brojKandidata - 1, razred, najbolji, 0, 0, 0, trenutni);

}

void main() {
	Datum datum19062017, datum20062017, datum30062017, datum05072017;
	datum19062017.Unos(19, 6, 2017);
	datum20062017.Unos(20, 6, 2017);
	datum30062017.Unos(30, 6, 2017);
	datum05072017.Unos(5, 7, 2017);

	Predmet Matematika, Fizika, Hemija, Engleski;
	//2 - ocjena na predmetu; datum - datum evidentiranja uspjeha na predmetu jer postoji krajnji rok za evidentiranje
	Matematika.Unos("Matematika", 2, datum19062017);
	Fizika.Unos("Fizika", 5, datum20062017);
	Hemija.Unos("Hemija", 2, datum20062017);
	Engleski.Unos("Engleski", 5, datum05072017);

	int brojKandidata = 2;

	Kandidat * prijave2017 = new Kandidat[brojKandidata];
	prijave2017[0].Unos(DL, "Jasmin Azemovic");
	prijave2017[1].Unos(REDOVAN, "Indira Hamulic");
	///*
	//uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda. prilikom dodavanja onemoguciti:
	//- istoimeni predmet se moze dodati samo dva puta, bez obzira sto se dodaje na razlicitim godinama,
	//- dodavanje predmeta nakon dozvoljenog roka za prijavu (rokZaPrijavu).
	//razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.). 
	//Funkcija vraca neku od predefinisanih poruka u zavisnosti od (ne)uspjesnost izvrsenja
	//*/
	if (prijave2017[0].DodajPredmet(DRUGI, Engleski)) //ne bi trebao dodati jer je prosao postavljeni rok za dodavanje predmeta
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(DRUGI, Matematika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(PRVI, Fizika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(PRVI, Hemija))
		cout << "Predmet uspjesno dodan!" << crt;

	Matematika.PromijeniOcjenu(5);
	Hemija.PromijeniOcjenu(3);

	if (prijave2017[1].DodajPredmet(PRVI, Matematika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(DRUGI, Matematika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(TRECI, Matematika))//ne bi trebalo ponovo dodati Matematiku!
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(TRECI, Hemija))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(DRUGI, Engleski))
		cout << "Predmet uspjesno dodan!" << crt;

	///*
	//koristeci Lambda izraz kreirati funkciju koja ce vratiti uspjeh kandidata koji je
	//ostvario najveci prosjek (na nivou razreda, a ne ukupni prosjek).
	//Funkcija prima parametre tipa eNacinStudiranja (pretrazuje samo kandidate za Redovan ili DL nacin studiranja) 
	//i bool kojim se omogucava pretraga samo onih razreda na kojim su kandidati bili najbolji ucenici
	//(ako se funkciji proslijedi vrijednost true, ona ce uzeti u obzir samo one razrede kada su kandidati oznaceni kao najbolji).
	//ukoliko vise kandidata ima isti prosjek funkcija vraca uspjeh (najboljeg razreda) prvog pronadjenog kandidata
	//*/
	
	auto najboljiUspjeh = [=](eNacinStudiranja nacin, bool najbolji) {
		Uspjeh *najuspjesniji = nullptr;
		float suma = 0, max = 0;
		int indexI=0, indexJ = 0;
		for (int i = 0; i < brojKandidata; i++) {
			for (int j = 0; j < 4; j++) {
				suma = 0;
				if (prijave2017[i]._uspjeh[j] != nullptr) {
					if (prijave2017[i]._uspjeh[j]->_najboljiUcenik == najbolji && prijave2017[i]._nacinStudiranja == nacin) {
						for (int z = 0; z < prijave2017[i]._uspjeh[j]->_brojPredmeta; z++) {
							suma += *prijave2017[i]._uspjeh[j]->_predmeti[z]->_ocjena;
						}
						if (suma > max) {
							max = suma;
							najuspjesniji = prijave2017[i]._uspjeh[j];
						}
					}
				}
			}
		}
	
		return najuspjesniji;
	};
	Uspjeh * najbolji = najboljiUspjeh(DL, true);
		if (najbolji != nullptr)
			najbolji->Ispis();
		else cout << "Nema najboljeg " << endl;

	///*
	//napisati rekurzivnu funkciju koja ce vratiti pokazivac na kandidata sa najvecim prosjekom u razredu koji je proslijedjen kao parametar. ukoliko je vise kandidata ostvarilo isti prosjek, funkcija treba da vrati posljednje pronadjenog kandidata. u slucaju da niti jedan kandidat nije evidentirao uspjeh u trazenom razredu, funkcija vraca nullptr. u nastavku je prikazan primjer poziva rekurzivne funkcije, a ostale parametre dodajte po potrebi. */
	Kandidat * kandidatSaNajboljimProsjekom = rekNajboljiProsjek(prijave2017, brojKandidata, DRUGI);
	if (kandidatSaNajboljimProsjekom != nullptr) {
		cout << crt;
		kandidatSaNajboljimProsjekom->Ispis();
		cout << crt;
	}
	

	for (size_t i = 0; i < brojKandidata; i++)
	{
		prijave2017[i].Ispis();
		prijave2017[i].Dealociraj();
	}
	system("pause>0");
}
