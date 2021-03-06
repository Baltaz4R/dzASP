// dz.cpp : Defines the entry point for the console application.
//


#include "windows.h"
#include <fstream>
#include <iostream>
#include <random>
using namespace std;

class PerformanceCalculator {
	LARGE_INTEGER startCounter;
	LARGE_INTEGER stopCounter;
	LARGE_INTEGER frequency;
public:
	PerformanceCalculator() {
		startCounter.QuadPart = stopCounter.QuadPart = 0;
		QueryPerformanceFrequency(&frequency);
	}
	void start() {
		QueryPerformanceCounter(&startCounter);
	}
	void stop() {
		QueryPerformanceCounter(&stopCounter);
	}
	double elapsedMillis() const {
		return (stopCounter.QuadPart - startCounter.QuadPart)*1000. / frequency.QuadPart;
	}
};

class Skup {
	struct Elem {
		int broj;
		Elem* next;
		Elem(int b, Elem* p = nullptr) {
			broj = b;
			next = p;
		}
	};
	Elem *prvi, *posl;
	int brelem;

	void kopiraj(const Skup& s) {
		if (s.brelem != 0) {
			Elem* novi = new Elem(s.prvi->broj);
			prvi = posl = novi;
			brelem = 1;
			for (Elem *tek = s.prvi->next; tek != nullptr; tek = tek->next) {
				novi = new Elem(tek->broj);
				posl->next = novi;
				posl = posl->next;
				brelem++;
			}
		}
		else {
			brelem = 0;
			prvi = posl = nullptr;
		}
	}
	void premesti(Skup& s) {
		brelem = s.brelem;
		prvi = s.prvi;
		posl = s.posl;
		s.prvi = s.posl = nullptr;
	}
	void brisi() {
		while (prvi) {
			Elem* stari = prvi;
			prvi = prvi->next;
			delete stari;
		}
		prvi = posl = nullptr;
		brelem = 0;
	}

public:
	Skup() {
		brelem = 0;
		prvi = nullptr;
		posl = nullptr;
	}
	Skup(const Skup& s) {
		kopiraj(s);
	}
	Skup(Skup&& s) {
		premesti(s);
	}
	~Skup() {
		brisi();
	}

	Skup& operator=(const Skup& s) {
		if (this != &s) {
			brisi();
			kopiraj(s);
		}
		return *this;
	}
	Skup& operator=(Skup&& s) {
		if (this != &s) {
			brisi();
			premesti(s);
		}
		return *this;
	}

	bool pripada(int x) const {
		bool p = false;
		Elem* tek = prvi;
		while (tek != nullptr) {
			if (tek->broj > x) {
				break;
			}
			else if (tek->broj == x) {
				p = true;
				break;
			}
			else {
				tek = tek->next;
			}
		}
		return p;
	}

	void stavi(int x) {
		Elem *tek = prvi, *pret = nullptr;
		while (tek && tek->broj < x) {
			pret = tek;
			tek = tek->next;
		}
		if (tek && tek->broj == x) return;
		Elem* novi = new Elem(x, tek);
		brelem++;
		if (!pret) {
			prvi = novi;
			if (!posl) posl = novi;
		}
		else pret->next = novi;
	}

	void brisi(int x) {
		Elem *tek = prvi, *pret = nullptr;
		while (tek && tek->broj < x) {
			pret = tek;
			tek = tek->next;
		}
		if (tek && tek->broj == x) {
			if (!pret) {
				prvi = tek->next;
				if (!prvi) posl = nullptr;
			}
			else pret->next = tek->next;
			delete tek;
			brelem--;
		}
	}

	void brisi(int x, int y) {
		Elem *tek = prvi, *pret = nullptr, *pom = nullptr;
		while (tek && tek->broj <= x) {
			pret = tek;
			tek = tek->next;
		}
		while (tek && tek->broj < y) {
			pom = tek;
			tek = tek->next;
			delete pom;
			brelem--;
		}
		if (pret) pret->next = tek;
		else prvi = tek;
		if (tek) posl = tek;
		else posl = pret;
	}

	int br_elemenata() const {
		return brelem;
	}

	friend  Skup unija(const Skup& s1, const Skup& s2) {
		Skup s;
		Elem *tek1 = s1.prvi, *tek2 = s2.prvi, *tek = nullptr;
		if (tek1 && tek2 && tek1->broj == tek2->broj) {
			Elem* novi = new Elem(tek1->broj);
			tek1 = tek1->next;
			tek2 = tek2->next;
			s.prvi = tek = novi;
			s.brelem++;
		}
		else if (tek1 && tek2 && tek1->broj < tek2->broj) {
			Elem* novi = new Elem(tek1->broj);
			tek1 = tek1->next;
			s.prvi = tek = novi;
			s.brelem++;
		}
		else if (tek1 && tek2 && tek1->broj > tek2->broj) {
			Elem* novi = new Elem(tek2->broj);
			tek2 = tek2->next;
			s.prvi = tek = novi;
			s.brelem++;
		}
		while (tek1 && tek2) {
			if (tek1->broj == tek2->broj) {
				Elem* novi = new Elem(tek1->broj);
				tek1 = tek1->next;
				tek2 = tek2->next;
				tek->next = novi;
				tek = tek->next;
				s.brelem++;
			}
			else if (tek1->broj < tek2->broj) {
				Elem* novi = new Elem(tek1->broj);
				tek1 = tek1->next;
				tek->next = novi;
				tek = tek->next;
				s.brelem++;
			}
			else {
				Elem* novi = new Elem(tek2->broj);
				tek2 = tek2->next;
				tek->next = novi;
				tek = tek->next;
				s.brelem++;
			}
		}
		while (tek2) {
			Elem* novi = new Elem(tek2->broj);
			tek2 = tek2->next;
			if (!tek) s.prvi = tek = novi;
			else {
				tek->next = novi;
				tek = tek->next;
			}
			s.brelem++;
		}
		while (tek1) {
			Elem* novi = new Elem(tek1->broj);
			tek1 = tek1->next;
			if (!tek) s.prvi = tek = novi;
			else {
				tek->next = novi;
				tek = tek->next;
			}
			s.brelem++;
		}
		s.posl = tek;
		return s;
	}

	void pisi() const {
		cout << "\n";
		for (Elem* tek = prvi; tek; tek = tek->next)
			cout << tek->broj << " ";
		cout << "\n";
	}


};
class SkupSBP {
	struct Elem {
		int broj;
		Elem *levo, *desno;
		Elem(int b, Elem* levo = nullptr, Elem* desno = nullptr) {
			broj = b;
			this->levo = levo;
			this->desno = desno;
		}
	};

	struct Stek {
		Elem* stari;
		Elem* novi;
		Stek* pret;
		Stek(Elem* s = nullptr, Elem* n = nullptr, Stek* p = nullptr) {
			stari = s;
			novi = n;
			pret = p;
		}
	};
	struct Stekic {
		Elem* elem;
		Stekic* pret;
		Stekic(Elem* e = nullptr, Stekic* p = nullptr) {
			elem = e;
			pret = p;
		}
	};
	Stek* stek = nullptr;
	Stekic* s = nullptr;
	void push(Elem* s, Elem* n) {
		Stek* novi = new Stek(s, n, stek);
		stek = novi;
		novi = nullptr;
	}
	void pop(Elem** s, Elem** n) {
		if (!stek) {
			cout << "Greska! Stek je prazan.";
			return;
		}
		*s = stek->stari;
		stek->stari = nullptr;
		*n = stek->novi;
		stek->novi = nullptr;
		Stek* obrisi = stek;
		stek = stek->pret;
		delete obrisi;
	}
	void push(Elem* e) {
		Stekic* novi = new Stekic(e, s);
		s = novi;
		novi = nullptr;
	}
	Elem* pop() {
		if (!s) {
			cout << "Greska! Stek je prazan.";
			return nullptr;
		}
		Elem* pom = s->elem;
		s->elem = nullptr;
		Stekic* obrisi = s;
		s = s->pret;
		delete obrisi;
		return pom;
	}

	Elem* koren;
	int brelem;

	void kopiraj(const SkupSBP& s) {
		if (s.koren) {
			koren = new Elem(s.koren->broj);
		}
		else {
			return;
		}
		Elem *poms = s.koren, *pomn = koren;
		push(poms, pomn);
		while (stek) {					//PREORDER
			pop(&poms, &pomn);
			while (poms) {
				if (poms->desno) {
					pomn->desno = new Elem(poms->desno->broj);
					push(poms->desno, pomn->desno);
				}
				if (poms->levo) {
					poms = poms->levo;
					pomn->levo = new Elem(poms->broj);
					pomn = pomn->levo;
				}
				else {
					poms = poms->levo;
				}
			}

		}
		brelem = s.brelem;
	}
	void premesti(SkupSBP& s) {
		brelem = s.brelem;
		koren = s.koren;
		s.koren = nullptr;
	}
	void brisi() {
		Elem* pom = koren;
		while (1) {
			while (pom) {
				push(pom);
				pom = pom->levo;
			}
			if (s) {
				pom = pop();
				Elem* brisi = pom;
				pom = pom->desno;
				delete brisi;
			}
			else {
				break;
			}
		}
		koren = nullptr;
		brelem = 0;
	}

public:

	SkupSBP() {
		brelem = 0;
		koren = nullptr;
	}
	SkupSBP(const SkupSBP& s) {
		kopiraj(s);
	}
	SkupSBP(SkupSBP&& s) {
		premesti(s);
	}
	~SkupSBP() {
		brisi();
	}

	SkupSBP& operator=(const SkupSBP& s) {
		if (this != &s) {
			brisi();
			kopiraj(s);
		}
		return *this;
	}
	SkupSBP& operator=(SkupSBP&& s) {
		if (this != &s) {
			brisi();
			premesti(s);
		}
		return *this;
	}

	bool pripada(int x) const {
		Elem* tek = koren;
		bool p = false;
		while (tek != nullptr) {
			if (tek->broj > x) {
				tek = tek->levo;
			}
			else if (tek->broj == x) {
				p = true;
				break;
			}
			else {
				tek = tek->desno;
			}
		}
		return p;
	}

	void stavi(int x) {
		Elem *tek = koren, *pret = nullptr;
		while (tek) {
			pret = tek;
			if (x == tek->broj) {
				return;  //vec postoji u skupu
			}
			else if (x < tek->broj) {
				tek = tek->levo;
			}
			else {
				tek = tek->desno;
			}
		}
		Elem* novi = new Elem(x);
		brelem++;
		if (pret == nullptr) {
			koren = novi;
		}
		else if (x < pret->broj) {
			pret->levo = novi;
		}
		else {
			pret->desno = novi;
		}

	}

	void brisi(int x) {
		Elem *tek = koren, *pret = nullptr;
		while (tek && x != tek->broj) {
			pret = tek;
			if (x < tek->broj) {
				tek = tek->levo;
			}
			else {
				tek = tek->desno;
			}
		}
		if (!tek) {
			return; //kljuc nije pronadjen
		}
		Elem* nastavak = nullptr;
		if (!tek->levo) {
			nastavak = tek->desno;;
		}
		else if (!tek->desno) {
			nastavak = tek->desno;
		}
		else {
			Elem* pom = tek, *levi;
			nastavak = tek->desno;
			levi = nastavak->levo;
			while (levi) {
				pom = nastavak;
				nastavak = levi;
				levi = levi->levo;
			}
			if (pom != tek) {
				pom->levo = nastavak->desno;
				nastavak->desno = tek->desno;
			}
			nastavak->levo = tek->levo;
		}
		if (!pret) {
			koren = nastavak;
		}
		else if (tek == pret->levo) {
			pret->levo = nastavak;
		}
		else {
			pret->desno = nastavak;
		}
		delete tek;
		brelem--;
	}

	void brisi(int x, int y) {
		for (int i = x + 1; i < y; i++) {
			brisi(i);
		}
	}

	int br_elemenata() const {
		return brelem;
	}

	friend  SkupSBP unija(const SkupSBP& s1, const SkupSBP& s2) {
		SkupSBP skup;
		Elem* pom;
		if (s1.brelem > s2.brelem) {
			skup = s1;
			pom = s2.koren;
		}
		else {
			skup = s2;
			pom = s1.koren;
		}
		while (1) {
			while (pom) {
				skup.push(pom);
				pom = pom->levo;
			}
			if (skup.s) {
				pom = skup.pop();
				skup.stavi(pom->broj);
				pom = pom->desno;
			}
			else {
				break;
			}
		}
		return skup;
	}

	void pisi() {
		cout << "\n";
		Elem* pom = koren;
		while (1) {
			while (pom) {
				push(pom);
				pom = pom->levo;
			}
			if (s) {
				pom = pop();
				cout << pom->broj << " ";
				pom = pom->desno;
			}
			else {
				break;
			}
		}
		cout << "\n";
	}
};

int main()
{
	cout << "\n1. Implementacija skupa koriscenjem ulancane liste\n"
		"2. Implementacija skupa koriscenjem stabla binarnog pretrazivanja\n"
		"3. Merenje performansi izvrsavanja\n"
		"Vas izbor? ";
	int iz;
	cin >> iz;
	if (iz == 1) {
		Skup s1, s2, s3;
		bool kraj = false;
		int broj;
		int broj1, broj2;
		bool b;
		ifstream dat;
		while (!kraj) {
			cout << "\n1. Ispisi skup1\n"
				"2. Ispisi skup2\n"
				"3. Pretrazi skup1\n"
				"4. Pretrazi skup2\n"
				"5. Dodaj element u skup1\n"
				"6. Dodaj element u skup2\n"
				"7. Izbrisi element iz skupa1\n"
				"8. Izbrisi element iz skupa2\n"
				"9. Izbrisi elemente u zadatom opsegu iz skupa1\n"
				"10. Izbrisi elemente u zadatom opsegu iz skupa2\n"
				"11. Broj elemenata u skupu1\n"
				"12. Broj elemenata u skupu2\n"
				"13. Unija skupa1 i skupa2\n"
				"14. Dodaj vise elemenata skup1\n"
				"15. Dodaj vise elemenata skup2\n"
				"16. Dodaj elemenate iz datoteke ucitaj.txt u skup1\n"
				"17. Dodaj elemenate iz datoteke ucitaj.txt u skup2\n"
				"0. Zavrsetak rada\n\n"
				"Vas izbor? ";
			int izbor;
			cin >> izbor;
			switch (izbor)
			{
			case 1:
				s1.pisi();
				break;
			case 2:
				s2.pisi();
				break;
			case 3:
				cout << "Broj?			";
				cin >> broj;
				b = s1.pripada(broj);
				if (b) {
					cout << "Broj " << broj << " pripada skupu 1";
				}
				else {
					cout << "Broj " << broj << " ne pripada skupu 1";
				}

				break;
			case 4:
				cout << "Broj?			";
				cin >> broj;
				b = s2.pripada(broj);
				if (b) {
					cout << "Broj " << broj << " pripada skupu 2";
				}
				else {
					cout << "Broj " << broj << " ne pripada skupu 2";
				}

				break;
			case 5:
				cout << "Broj?			";
				cin >> broj;
				s1.stavi(broj);
				break;
			case 6:
				cout << "Broj?			";
				cin >> broj;
				s2.stavi(broj);
				break;
			case 7:
				cout << "Broj?			";
				cin >> broj;
				s1.brisi(broj);
				break;
			case 8:
				cout << "Broj?			";
				cin >> broj;
				s2.brisi(broj);
				break;
			case 9:
				cout << "Opseg?			";
				cin >> broj1 >> broj2;
				s1.brisi(broj1, broj2);
				break;
			case 10:
				cout << "Opseg?			";
				cin >> broj1 >> broj2;
				s2.brisi(broj1, broj2);
				break;
			case 11:
				cout << s1.br_elemenata();
				break;
			case 12:
				cout << s2.br_elemenata();
				break;
			case 13:
				s3 = unija(s1, s2);
				s3.pisi();
				break;
			case 14:
				cout << "Koliko elemenata zelite da unesete?			";
				cin >> broj;
				for (int i = 1; i <= broj; i++) {
					cin >> broj1;
					s1.stavi(broj1);
				}
				break;
			case 15:
				cout << "Koliko elemenata zelite da unesete?			";
				cin >> broj;
				for (int i = 1; i <= broj; i++) {
					cin >> broj1;
					s2.stavi(broj1);
				}
				break;
			case 16:
				dat.open("ucitaj.txt");
				if (!dat.is_open()) cout << "Greska! Datoteka se nije otvorila.";
				while(!dat.eof()) {
					dat >> broj;
					s1.stavi(broj);
				}
				dat.close();
				break;
			case 17:
				dat.open("ucitaj.txt");
				if (!dat.is_open()) cout << "Greska! Datoteka se nije otvorila.";
				while (!dat.eof()) {
					dat >> broj;
					s2.stavi(broj);
				}
				dat.close();
				break;
			case 0:
				kraj = true;
				break;
			default:
				cout << "*** Nedozvoljen izbor! ***\a\n";
				break;
			}
		}
	}
	else if (iz == 2) {
		SkupSBP s1, s2, s3;
		bool kraj = false;
		int broj;
		int broj1, broj2;
		bool b;
		ifstream dat;
		while (!kraj) {
			cout << "\n1. Ispisi skup1\n"
				"2. Ispisi skup2\n"
				"3. Pretrazi skup1\n"
				"4. Pretrazi skup2\n"
				"5. Dodaj element u skup1\n"
				"6. Dodaj element u skup2\n"
				"7. Izbrisi element iz skupa1\n"
				"8. Izbrisi element iz skupa2\n"
				"9. Izbrisi elemente u zadatom opsegu iz skupa1\n"
				"10. Izbrisi elemente u zadatom opsegu iz skupa2\n"
				"11. Broj elemenata u skupu1\n"
				"12. Broj elemenata u skupu2\n"
				"13. Unija skupa1 i skupa2\n"
				"14. Dodaj vise elemenata skup1\n"
				"15. Dodaj vise elemenata skup2\n"
				"16. Dodaj elemenate iz datoteke ucitaj.txt u skup1\n"
				"17. Dodaj elemenate iz datoteke ucitaj.txt u skup2\n"
				"0. Zavrsetak rada\n\n"
				"Vas izbor? ";
			int izbor;
			cin >> izbor;
			switch (izbor)
			{
			case 1:
				s1.pisi();
				break;
			case 2:
				s2.pisi();
				break;
			case 3:
				cout << "Broj?			";
				cin >> broj;
				b = s1.pripada(broj);
				if (b) {
					cout << "Broj " << broj << " pripada skupu 1";
				}
				else {
					cout << "Broj " << broj << " ne pripada skupu 1";
				}

				break;
			case 4:
				cout << "Broj?			";
				cin >> broj;
				b = s2.pripada(broj);
				if (b) {
					cout << "Broj " << broj << " pripada skupu 2";
				}
				else {
					cout << "Broj " << broj << " ne pripada skupu 2";
				}

				break;
			case 5:
				cout << "Broj?			";
				cin >> broj;
				s1.stavi(broj);
				break;
			case 6:
				cout << "Broj?			";
				cin >> broj;
				s2.stavi(broj);
				break;
			case 7:
				cout << "Broj?			";
				cin >> broj;
				s1.brisi(broj);
				break;
			case 8:
				cout << "Broj?			";
				cin >> broj;
				s2.brisi(broj);
				break;
			case 9:
				cout << "Opseg?			";
				cin >> broj1 >> broj2;
				s1.brisi(broj1, broj2);
				break;
			case 10:
				cout << "Opseg?			";
				cin >> broj1 >> broj2;
				s2.brisi(broj1, broj2);
				break;
			case 11:
				cout << s1.br_elemenata();
				break;
			case 12:
				cout << s2.br_elemenata();
				break;
			case 13:
				s3 = unija(s1, s2);
				s3.pisi();
				break;
			case 14:
				cout << "Koliko elemenata zelite da unesete?			";
				cin >> broj;
				for (int i = 1; i <= broj; i++) {
					cin >> broj1;
					s1.stavi(broj1);
				}
				break;
			case 15:
				cout << "Koliko elemenata zelite da unesete?			";
				cin >> broj;
				for (int i = 1; i <= broj; i++) {
					cin >> broj1;
					s2.stavi(broj1);
				}
				break;
			case 16:
				dat.open("ucitaj.txt");
				if (!dat.is_open()) cout << "Greska! Datoteka se nije otvorila.";
				while (!dat.eof()) {
					dat >> broj;
					s1.stavi(broj);
				}
				dat.close();
				break;
			case 17:
				dat.open("ucitaj.txt");
				if (!dat.is_open()) cout << "Greska! Datoteka se nije otvorila.";
				while (!dat.eof()) {
					dat >> broj;
					s2.stavi(broj);
				}
				dat.close();
				break;
			case 0:
				kraj = true;
				break;
			default:
				cout << "*** Nedozvoljen izbor! ***\a\n";
				break;
			}
		}
	}
	else if (iz == 3) {
		PerformanceCalculator pc;
		int broj;

		default_random_engine generator;
		uniform_int_distribution<int> distribution(-1000000, 1000000);

		double** m1 = new double*[5];
		for (int i = 0; i < 5; i++)
			m1[i] = new double[6];
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 6; j++)
				m1[i][j] = 0;

		double** m2 = new double*[5];
		for (int i = 0; i < 5; i++)
			m2[i] = new double[6];
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 6; j++)
				m2[i][j] = 0;

		int kap[] = { 200, 500, 1000, 10000, 100000/*, 1000000*/ };

		for (int k = 0; k < 5; k++) {					//Za milion treba k<6
			for (int j = 0; j < 3; j++) {
				long *niz = new long[kap[k]];
				for (int i = 0; i < kap[k]; i++) niz[i] = distribution(generator);
				SkupSBP s1, s2, s3;
				Skup s4, s5, s6;

				pc.start();
				for (int i = 0; i < kap[k]; i++) {
					s1.stavi(niz[i]);
				}
				pc.stop();
				m1[0][k] += pc.elapsedMillis();
				pc.start();
				for (int i = 0; i < kap[k]; i++) {
					s4.stavi(niz[i]);
				}
				pc.stop();
				m2[0][k] += pc.elapsedMillis();

				for (int i = 0; i < kap[k]; i++) {
					long l = distribution(generator);
					s2.stavi(l);
					s5.stavi(l);
				}

				pc.start();
				s1.stavi(13);
				pc.stop();
				m1[1][k] += pc.elapsedMillis();
				pc.start();
				s4.stavi(13);
				pc.stop();
				m2[1][k] += pc.elapsedMillis();

				pc.start();
				s1.brisi(13);
				pc.stop();
				m1[2][k] += pc.elapsedMillis();
				pc.start();
				s4.brisi(13);
				pc.stop();
				m2[2][k] += pc.elapsedMillis();

				pc.start();
				s1.pripada(321542);
				pc.stop();
				m1[3][k] += pc.elapsedMillis();
				pc.start();
				s4.pripada(321542);
				pc.stop();
				m2[3][k] += pc.elapsedMillis();

				pc.start();
				s3 = unija(s1, s2);
				pc.stop();
				m1[4][k] += pc.elapsedMillis();
				pc.start();
				s6 = unija(s4, s5);
				pc.stop();
				m2[4][k] += pc.elapsedMillis();

				delete[] niz;
			}
		}

		for (int i = 0; i < 5; i++)									//Za milion treba j<6
			for (int j = 0; j < 5; j++)
				m1[i][j] = m1[i][j] / 3;
		for (int i = 0; i < 5; i++)									//Za milion treba j<6
			for (int j = 0; j < 5; j++)
				m2[i][j] = m2[i][j] / 3;

		cout << "\nImplementacija skupa koriscenjem stabla binarnog pretrazivanja:";
		cout << "\n----------------------------------------------------------------------------------------------------------------------\n";
		printf("|%-20s|%15s|%15s|%15s|%15s|%15s|%15s|", "Velicina skupa", "200", "500", "1000", "10000", "100000", "1000000");
		cout << "\n----------------------------------------------------------------------------------------------------------------------";
		printf("\n|%-20s|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|", "Formiranje", m1[0][0], m1[0][1], m1[0][2], m1[0][3], m1[0][4], m1[0][5]);
		printf("\n|%-20s|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|", "Umetanje", m1[1][0], m1[1][1], m1[1][2], m1[1][3], m1[1][4], m1[1][5]);
		printf("\n|%-20s|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|", "Brisanje", m1[2][0], m1[2][1], m1[2][2], m1[2][3], m1[2][4], m1[2][5]);
		printf("\n|%-20s|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|", "Pretraga", m1[3][0], m1[3][1], m1[3][2], m1[3][3], m1[3][4], m1[3][5]);
		printf("\n|%-20s|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|", "Skupovna operacija", m1[4][0], m1[4][1], m1[4][2], m1[4][3], m1[4][4], m1[4][5]);
		cout << "\n----------------------------------------------------------------------------------------------------------------------\n";

		cout << "\nImplementacija skupa koriscenjem ulancane liste:";
		cout << "\n----------------------------------------------------------------------------------------------------------------------\n";
		printf("|%-20s|%15s|%15s|%15s|%15s|%15s|%15s|", "Velicina skupa", "200", "500", "1000", "10000", "100000", "1000000");
		cout << "\n----------------------------------------------------------------------------------------------------------------------";
		printf("\n|%-20s|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|", "Formiranje", m2[0][0], m2[0][1], m2[0][2], m2[0][3], m2[0][4], m2[0][5]);
		printf("\n|%-20s|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|", "Umetanje", m2[1][0], m2[1][1], m2[1][2], m2[1][3], m2[1][4], m2[1][5]);
		printf("\n|%-20s|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|", "Brisanje", m2[2][0], m2[2][1], m2[2][2], m2[2][3], m2[2][4], m2[2][5]);
		printf("\n|%-20s|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|", "Pretraga", m2[3][0], m2[3][1], m2[3][2], m2[3][3], m2[3][4], m2[3][5]);
		printf("\n|%-20s|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|%15.5f|", "Skupovna operacija", m2[4][0], m2[4][1], m2[4][2], m2[4][3], m2[4][4], m2[4][5]);
		cout << "\n----------------------------------------------------------------------------------------------------------------------\n";

		cin >> broj;
	}
	else {
		cout << "*** Nedozvoljen izbor! ***\a\n";
	}

	return 0;
}