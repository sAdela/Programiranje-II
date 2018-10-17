#include<iostream>
#include<tuple>
using namespace std;

/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NECE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/

const char * not_set = "NEMA_VRIJEDNOST";
const char * crt = "\n-------------------------------------------\n";
const int min_polgavlja = 3;
const int max_zavrsnih = 30;
const char * zabranjeneRijeci[] = { "ubiti","mrziti", "NC", "RE" };

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

struct Poglavlje {
	char * _naslov;
	char * _sadrzaj;
	bool _prihvaceno;
	int _ocjena;//da bi se poglavlje smatralo prihvacenim ocjena mora biti u opsegu od 6 - 10
	void Unos(const char * naslov = nullptr, const char * sadrzaj = nullptr) {
		_ocjena = 0; _prihvaceno = false;
		_naslov = AlocirajNizKaraktera(naslov);
		_sadrzaj = AlocirajNizKaraktera(sadrzaj);
	}
	void Dealociraj() {
		delete[] _naslov; _naslov = nullptr;
		delete[] _sadrzaj; _sadrzaj = nullptr;
	}
	void Ispis() {
		if (_naslov == nullptr || _sadrzaj == nullptr)
			return;
		cout << endl << "Poglavlje -> " << _naslov << endl << _sadrzaj << endl;
		if (_prihvaceno)
			cout << "Ocjena: " << _ocjena << endl;;
	}
	void OcijeniPoglavlje(int ocjena) {
		_ocjena = ocjena;
		if (_ocjena > 5 && ocjena <= 10)
			_prihvaceno = true;
		//else _prihvaceno = false;
	}
};

struct ZavrsniRad {
	char _brojIndeksa[15];
	char * _tema;
	shared_ptr<Poglavlje> * _poglavljaRada;
	int _trenutnoPoglavlja;
	char * _datumOdbrane;
	float _konacnaOcjena; //konacna ocjena zavrsnog rada predstavlja prosjek ocjena svih poglavlja u zavrsnom radu koja se izracunava u momentu zakazivanja odbrane



	void OcijeniPoglavlje(const char* nazivPoglavlja, float ocjena) {

		for (int i = 0; i < _trenutnoPoglavlja; i++) {

			if (strcmp(nazivPoglavlja, _poglavljaRada[i]->_naslov) == 0) {

				_poglavljaRada[i]->_ocjena = ocjena;

				if (ocjena >= 6 || ocjena <= 10) {
					_poglavljaRada[i]->_prihvaceno = true;
				}
				else
					_poglavljaRada[i]->_prihvaceno = false;
				return;
			}
		}

		cout << "Ne postoji poglavlje sa tim nazivom " << endl;
	}

	void DodajPoglavlje(const char* nazivPoglavlja, const char* sadrzajPoglavlja) {

		/*u zavrsni rad dodaje novo poglavlje i njegov sadrzaj.
		ukoliko poglavlje vec postoji u zavrsnom radu,
		funkcija tom poglavlju treba dodati novi sadrzaj i pri tome zadrzi postojeci
		(izmedju postojeceg i novog sadrzaja se dodaje prazan prostor).
		u slucaju da poglavlje ne postoji, ono se dodaje zajedno sa sadrzajem*/

		int vel = 0, vel1 = 0;
		char *kopiraj;

		for (int i = 0; i < _trenutnoPoglavlja; i++) {

			if (strcmp(_poglavljaRada[i]->_naslov, nazivPoglavlja) == 0) {

				//Ako vec postoji poglavlje u radu

				if (_poglavljaRada[i]->_sadrzaj != nullptr)
					vel1 = strlen(_poglavljaRada[i]->_sadrzaj) + 1;

				vel = vel1 + strlen(sadrzajPoglavlja) + 1;

				kopiraj = new char[vel];
				strcpy_s(kopiraj, vel, _poglavljaRada[i]->_sadrzaj);
				strcat_s(kopiraj, vel, " ");
				strcat_s(kopiraj, vel, sadrzajPoglavlja);

				delete[] _poglavljaRada[i]->_sadrzaj;
				_poglavljaRada[i]->_sadrzaj = new char[vel];
				strcpy_s(_poglavljaRada[i]->_sadrzaj, vel, kopiraj);
				return;
			}
		}

		shared_ptr<Poglavlje> *novi = new shared_ptr<Poglavlje>[_trenutnoPoglavlja + 1];

		for (int j = 0; j < _trenutnoPoglavlja; j++) {

			novi[j] = make_shared<Poglavlje>();
			novi[j]->Unos(_poglavljaRada[j]->_naslov, _poglavljaRada[j]->_sadrzaj);
			novi[j]->_prihvaceno = _poglavljaRada[j]->_prihvaceno;
			novi[j]->_ocjena = _poglavljaRada[j]->_ocjena;

			_poglavljaRada[j]->Dealociraj();
		}
		delete[] _poglavljaRada;

		novi[_trenutnoPoglavlja] = make_shared<Poglavlje>();
		novi[_trenutnoPoglavlja]->Unos(nazivPoglavlja, sadrzajPoglavlja);

		_poglavljaRada = novi;
		_trenutnoPoglavlja++;
		return;

	}


	void Unos(const char * brojIndeksa, const char * nazivTeme = nullptr) {
		_konacnaOcjena = 0;
		strcpy_s(_brojIndeksa, 15, brojIndeksa);
		_tema = AlocirajNizKaraktera(nazivTeme);
		_trenutnoPoglavlja = 0;
		_datumOdbrane = AlocirajNizKaraktera(not_set);
		_poglavljaRada = nullptr;
	}
	void Dealociraj() {
		delete[] _tema; _tema = nullptr;
		delete[] _datumOdbrane; _datumOdbrane = nullptr;
		for (size_t i = 0; i < _trenutnoPoglavlja; i++)
			_poglavljaRada[i]->Dealociraj();
		delete[] _poglavljaRada; _poglavljaRada = nullptr;
	}
	void Ispis() {
		cout << "Tema rada: " << _tema << endl;
		cout << "Sadrzaj: " << endl;
		for (size_t i = 0; i < _trenutnoPoglavlja; i++)
			_poglavljaRada[i]->Ispis();
		cout << "Datum odbrane rada: " << _datumOdbrane << endl << " Ocjena: " << _konacnaOcjena << endl;
	}
};
struct Nastavnik {
	char * _imePrezime;
	shared_ptr<ZavrsniRad> _teme[max_zavrsnih] = { nullptr };

	ZavrsniRad* ZakaziOdbranuRada(const char* brojIndexa, const char* datumOdbrane) {

		/*
		funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada sto podrazumijeva
		izracunavanje konacne ocjene i definisanje datuma odbrane. odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio.
		Za zakazivanje odbrane rada moraju biti zadovoljeni sljedeci uslovi:
		1. zavrsni rad mora imati broj poglavlja veci od minimalnog
		2. sadrzaj bilo kojeg poglavlja u zavrsnom radu ne smije sadrzavati neku od zabranjenih rijeci (definisanih u nizu zabranjeneRijeci)
		3. svako poglavlje mora biti prihvaceno/odobreno

		ukoliko su zadovoljeni prethodni kriteriji, izracunava se konacna ocjena rada (prosjek ocjena svih poglavlja),
		postavlja datum odbrane rada i vraca pokazivac na rad kome je zakazana odbrana.
		u slucaju da student sa primljenim brojem indeksa nije prijavio zavrsni rad ili neki od postavljenih kriterija
		nije zadovoljen, funkcija vraca nullptr.
		*/


		ZavrsniRad *novi = new ZavrsniRad;

		for (int i = 0; i < max_zavrsnih; i++) {
			if (_teme[i] != nullptr) {


				if (strcmp(brojIndexa, _teme[i]->_brojIndeksa) == 0) {

					if (min_polgavlja > _teme[i]->_trenutnoPoglavlja)
						return nullptr;

					for (int z = 0; z < _teme[i]->_trenutnoPoglavlja; z++) {
						for (int j = 0; j < 4; j++) {
							if (strstr(_teme[i]->_poglavljaRada[z]->_sadrzaj, zabranjeneRijeci[j]) != nullptr)
								return nullptr;
						}
						if (_teme[i]->_poglavljaRada[z]->_prihvaceno == false)
							return nullptr;
					}
				}
				float suma = 0;

				_teme[i]->_datumOdbrane = AlocirajNizKaraktera(datumOdbrane);

				for (int j = 0; j < _teme[i]->_trenutnoPoglavlja; j++) {
					suma += _teme[i]->_poglavljaRada[j]->_ocjena;
				}

				_teme[i]->_konacnaOcjena = float(suma) / _teme[i]->_trenutnoPoglavlja;


				return &(*_teme[i]);
			}

		}

		return nullptr;

	}
	
	bool ProvjeriUslove(ZavrsniRad rad){
		if (_teme[max_zavrsnih - 1] != nullptr)
			return false;

		for (int i = 0; i < max_zavrsnih; i++) {
			if (_teme[i] != nullptr) {

				if (strcmp(rad._tema, _teme[i]->_tema) == 0)
					return false;
			}
		}

		for (int i = 0; i < max_zavrsnih; i++) {

			if (_teme[i] != nullptr) {

				if (strcmp(_teme[i]->_brojIndeksa, rad._brojIndeksa) == 0)
					return false;
			}
		}
		return true;
	}

	bool DodajZavrsniRad(ZavrsniRad rad) {
		/*funkcija DodajZavrsniRad ima zadatak da odredjenom nastavniku dodijeli mentorstvo na zavrsnom radu.
		sprijeciti dodavanje zavrsnih radova sa istom temom kao i mogucnost da jedan student kod istog
		nastavnika posjeduje vise zavrsnih radova*/

		if (!ProvjeriUslove(rad))
			return false;
		
		for (int i = 0; i < max_zavrsnih; i++) {
			if (_teme[i] == nullptr) {

				_teme[i] = make_shared<ZavrsniRad>();
				_teme[i]->Unos(rad._brojIndeksa, rad._tema);
				_teme[i]->_datumOdbrane = AlocirajNizKaraktera(rad._datumOdbrane);
				_teme[i]->_konacnaOcjena = rad._konacnaOcjena;
				
				for (int j = 0; j < rad._trenutnoPoglavlja; j++) {

					_teme[i]->DodajPoglavlje(rad._poglavljaRada[j]->_naslov, rad._poglavljaRada[j]->_sadrzaj);
					_teme[i]->OcijeniPoglavlje(rad._poglavljaRada[j]->_naslov, rad._poglavljaRada[j]->_ocjena);
				}
				return true;
			}
		}

		return true;

	}

	void Unos(const char * imePrezime) {
		_imePrezime = AlocirajNizKaraktera(imePrezime);
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < max_zavrsnih; i++)
			if (_teme[i] != nullptr)
				_teme[i]->Dealociraj();
	}
	void Ispis() {
		cout << crt << _imePrezime << crt;
		for (size_t i = 0; i < max_zavrsnih; i++)
			if (_teme[i] != nullptr)
				_teme[i]->Ispis();
	}
};

tuple <char*, int, float> PronadjiStudenta(Nastavnik* nastavnik, int brojNastavnika) {
	float suma = 0, max = -1;
	int brojac = 0, brojNas = 0;
	/*funkcija PronadjiStudenta ima zadatak da pronadje studenta koji kod nastavnika/mentora sa najvecom prosjecnom ocjenom radova
	(sumaSvihOcjenaZavrsnihRadova/brojZavrsnihRadova) ima najvise neodobrenih (negativno ocijenjenih) poglavlja*/
	for (int i = 0; i < brojNastavnika; i++) {
		brojac = 0; suma = 0;
		for (int j = 0; j < max_zavrsnih; j++) {
			if (nastavnik[i]._teme[j] != nullptr) {
				if (nastavnik[i]._teme[j]->_konacnaOcjena > 0) {
					suma += nastavnik[i]._teme[j]->_konacnaOcjena;
					brojac++;
				}

			}
		}
		suma /= brojac;
		if (suma > max) {
			max = suma;
			brojNas = i;
		}
	}

	int max1 = 0;
	char *index;
	int vel = 0;
	index = nullptr;

	for (int i = 0; i < max_zavrsnih; i++) {
		brojac = 0;
		if (nastavnik[brojNas]._teme[i] != nullptr) {
			for (int j = 0; j < nastavnik[brojNas]._teme[i]->_trenutnoPoglavlja; j++) {
				if (nastavnik[brojNas]._teme[i]->_poglavljaRada[j]->_ocjena < 6)
					brojac++;
			}
			if (brojac > max1) {
				max1 = brojac;
				index = nastavnik[brojNas]._teme[i]->_brojIndeksa;
			}
		}
	}
	tuple <char*, int, float> tupl = make_tuple(index, max1, max);

	if (index == nullptr) {
		cout << "Ne postoji ";
		tupl = make_tuple(nullptr, 0, 0);
		return tupl;
	}


	return tupl;

}


int main() {
	const int max = 2;
	Nastavnik * nastavnici = new Nastavnik[max];

	nastavnici[0].Unos("Denis Music");
	nastavnici[1].Unos("Emina Junuz");

	ZavrsniRad multimedijalni;
	//parametri: brojIndeksa, tema
	multimedijalni.Unos("IB120021", "Multimedijalni informacijski sistem za visoko - obrazovnu ustanovu");
	ZavrsniRad podrsa_operaterima;
	podrsa_operaterima.Unos("IB130031", "Sistem za podršku rada kablovskog operatera");
	ZavrsniRad analiza_sigurnosti;
	analiza_sigurnosti.Unos("IB140041", "Prakticna analiza sigurnosti bežiènih raèunarskih mreža");
	ZavrsniRad kriptografija;
	kriptografija.Unos("IB120021", "Primjena teorije informacija u procesu generisanja kriptografskih kljuèeva");

	/*u zavrsni rad dodaje novo poglavlje i njegov sadrzaj. ukoliko poglavlje vec postoji u zavrsnom radu, funkcija tom poglavlju treba dodati novi sadrzaj i pri tome zadrzi postojeci (izmedju postojeceg i novog sadrzaja se dodaje prazan prostor). u slucaju da poglavlje ne postoji, ono se dodaje zajedno sa sadrzajem*/
	//parametri: nazivPoglavlja, sadrzajPoglavlja

	multimedijalni.DodajPoglavlje("Uvod", "U ovom poglavlju ce biti rijeci");
	multimedijalni.DodajPoglavlje("Uvod", "o multimedijalnim sistemima koji se danas koriste");
	multimedijalni.DodajPoglavlje("Uvod", "u savremenom poslovanju");
	multimedijalni.DodajPoglavlje("Vrste multimedijalnih sistema", "Danas se moze govoriti o nekoliko vrsta multimedijalnih sistema, a neke od najznacajnijih su ...");
	multimedijalni.DodajPoglavlje("Teorija multimedije", "Sadrzaj koji bi trebao stajati na pocetku treceg poglavlja zavrsnog rada o multimediji studenta IB130011");

	// parametri: nazivPoglavlja, ocjena
	multimedijalni.OcijeniPoglavlje("Uvod", 7);
	multimedijalni.OcijeniPoglavlje("Vrste multimedijalnih sistema", 1);
	multimedijalni.OcijeniPoglavlje("Teorija multimedije", 1);


	/*funkcija DodajZavrsniRad ima zadatak da odredjenom nastavniku dodijeli mentorstvo na zavrsnom radu. sprijeciti dodavanje zavrsnih radova sa istom temom kao i mogucnost da jedan student kod istog nastavnika posjeduje vise zavrsnih radova*/
	if (nastavnici[0].DodajZavrsniRad(multimedijalni))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (nastavnici[0].DodajZavrsniRad(podrsa_operaterima))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (!nastavnici[0].DodajZavrsniRad(podrsa_operaterima))//dupliranje rada, onemoguciti dodavanje
		cout << "Zavrsni rad nije dodat!" << endl;
	if (!nastavnici[0].DodajZavrsniRad(kriptografija)) //studentu vec dodijeljen rad, onemoguciti dodavanje
		cout << "Zavrsni rad nije dodat!" << endl;
	if (nastavnici[1].DodajZavrsniRad(analiza_sigurnosti))
		cout << "Zavrsni rad uspjesno dodat!" << endl;

	//
	//    /*funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada sto podrazumijeva
	//    izracunavanje konacne ocjene i definisanje datuma odbrane. odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio.
	//    Za zakazivanje odbrane rada moraju biti zadovoljeni sljedeci uslovi:
	//        1. zavrsni rad mora imati broj poglavlja veci od minimalnog
	//        2. sadrzaj bilo kojeg poglavlja u zavrsnom radu ne smije sadrzavati neku od zabranjenih rijeci (definisanih u nizu zabranjeneRijeci)
	//        3. svako poglavlje mora biti prihvaceno/odobreno
	//    ukoliko su zadovoljeni prethodni kriteriji, izracunava se konacna ocjena rada (prosjek ocjena svih poglavlja), postavlja datum odbrane rada i vraca pokazivac na rad kome je zakazana odbrana.
	//    u slucaju da student sa primljenim brojem indeksa nije prijavio zavrsni rad ili neki od postavljenih kriterija nije zadovoljen, funkcija vraca nullptr.
	//*/
	//
	//    paramteri: brojIndeksa, datumOdbrane
	ZavrsniRad *zr1 = nastavnici[0].ZakaziOdbranuRada("IB120021", "25.09.2018");
	if (zr1 != nullptr)
		zr1->Ispis();

	zr1 = nastavnici[0].ZakaziOdbranuRada("IB180081", "25.09.2018");//student sa brojem indeksa IB180081 jos uvijek nije prijavio rad
	if (zr1 == nullptr)
		cout << "Odbrana ne moze biti zakazana!" << endl;
	//
	//    ispisuje sve podatke o nastavniku i njegovim mentorstvima
	nastavnici[0].Ispis();
	nastavnici[1].Ispis();
	//
	//    /*funkcija PronadjiNajStudenta ima zadatak da pronadje studenta koji kod nastavnika/mentora sa najvecom prosjecnom ocjenom radova
	//    (sumaSvihOcjenaZavrsnihRadova/brojZavrsnihRadova) ima najvise neodobrenih (negativno ocijenjenih) poglavlja*/
	    int brojPoglavlja = -1;
	    float prosjekMentora = 0;
	    char * indeksStudenta;
	//  parametri PronadjiStudenta: nastavnici, brojNastavnika
	    tie(indeksStudenta, brojPoglavlja, prosjekMentora) = PronadjiStudenta(nastavnici, max);
	    if (indeksStudenta != nullptr)
	        cout << crt << indeksStudenta << " " << prosjekMentora << " " << brojPoglavlja << crt;
	
	    for (int i = 0; i < max; i++)
	        nastavnici[i].Dealociraj();
	    delete []nastavnici;
	    nastavnici = nullptr;

	system("pause>0");
	return 0;

}
