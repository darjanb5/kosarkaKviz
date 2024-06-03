#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quiz.h"
#include "file_utils.h"

#define BROJ_PITANJA 10

typedef struct {
	char imeIgraca[50];
	int rezultat;
} RezultatIgraca;

static RezultatIgraca* rezultatIgraca = NULL;
static int brojRezultata = 0;

static Pitanje pitanja[BROJ_PITANJA] = {
	{"\nTko je poznat kao 'Kralj kosarke'?", {"Michael Jordan", "LeBron James", "Kobe Bryant", "Shaquille O'Neal"}, 2},
	{"\nKoliko igraca ima kosarkaska momcad na terenu?", {"5", "6", "7", "8"}, 1},
	{"\nKoje godine je osnovana NBA?", {"1946.", "1956.", "1966.", "1976."}, 1},
	{"\nKoja momcad ima najvise NBA prvenstava?", {"Los Angeles Lakers", "Chicago Bulls", "Boston Celtics", "Golden State Warriors"}, 3},
	{"\nKoliki je promjer kosarkaskog obruca u incima?", {"16", "18", "20", "22"}, 2},
	{"\nZa koju momcad trenutno igra Luka Doncic?", {"New York Knicks", "Brooklyn Nets", "Dallas Mavericks", "Atlanta Hawks"}, 3},
	{"\nCiji je nadimak 'Mozart kosarke'?", {"Julius Erving", "Paul Pierce", "Pedja Stojakovic", "Drazen Petrovic"}, 4},
	{"\nKoja momcad nije iz Istocne Konferencije?", {"Boston Celtics", "Indiana Pacers", "Los Angeles Lakers", "Toronto Raptors"}, 3},
	{"\nZa koji tim Toni Kukoc nikad nije nastupio?", {"Dallas Mavericks", "Chicago Bulls", "Philadelphia 76ers", "Milwaukee Bucks"}, 1},
	{"\nIme trenutnog komesara NBA?", {"Mark Cuban", "Adam Silver", "Karl Malone", "Jerry West"}, 3}
};


void randomPitanja() {
	srand(time(NULL));
	for (int i = 0; i < BROJ_PITANJA; ++i) {
		int j = rand() % BROJ_PITANJA;
		Pitanje temp = pitanja[i];
		pitanja[i] = pitanja[j];
		pitanja[j] = temp;
	}
}

void pokreniKviz() {
	RezultatIgraca rezultatIgraca;
	printf("\nUnesite vase ime: ");
	scanf("%50s", rezultatIgraca.imeIgraca);
	rezultatIgraca.rezultat = 0;

	randomPitanja();
	postaviPitanja(&rezultatIgraca);
	spremiRezultat(rezultatIgraca.imeIgraca, rezultatIgraca.rezultat);
	oslobodiMemoriju();
}

static void postaviPitanja(RezultatIgraca* rezultatIgraca) {
	int odgovor;
	for (int i = 0; i < BROJ_PITANJA; ++i) {
		printf("%s\n", pitanja[i].pitanje);
		for (int j = 0; j < 4; ++j) {
			printf("%d. %s\n", j + 1, pitanja[i].opcije[j]);
		}
		printf("Vas odgovor: ");
		scanf("%s", &odgovor);
		if (provjeriOdgovor(odgovor) && odgovor == pitanja[i].tocanOdgovor) {
			rezultatIgraca->rezultat += 10;
		}
	}
	printf("\nKviz zavrsen! Vas rezultat: %d/100\n", rezultatIgraca->rezultat);
}

static int provjeriOdgovor(int odgovor) {
	if (odgovor < 1 || odgovor > 4) {
		printf("Neispravan odgovor. Molimo unesite broj izmedu 1 i 4.\n");
		return 0;
	}
	return 1;
}

void pregledajRezultate() {
	FILE* datoteka = fopen("rezultati.bin", "rb");
	if (datoteka == NULL) {
		perror("Neuspjelo otvaranje datoteke za citanje rezultata");
		return;
	}

	fseek(datoteka, 0, SEEK_END);
	long velicinaDatoteke = ftell(datoteka);
	rewind(datoteka);

	brojRezultata = velicinaDatoteke / sizeof(RezultatIgraca);
	rezultatIgraca = (RezultatIgraca*)malloc(brojRezultata * sizeof(RezultatIgraca));
	if (rezultatIgraca == NULL) {
		perror("Neuspjelo zauzimanje memorije za rezultate");
		fclose(datoteka);
		return;
	}

	fread(rezultatIgraca, sizeof(RezultatIgraca), brojRezultata, datoteka);
	qsort(rezultatIgraca, brojRezultata, sizeof(RezultatIgraca), usporediRezultate);

	printf("Rezultati:\n");
	for (int i = 0; i < brojRezultata; ++i) {
		printf("%s: %d\n", rezultatIgraca[i].imeIgraca, rezultatIgraca[i].rezultat);
	}

	free(rezultatIgraca);
	rezultatIgraca = NULL;

	fclose(datoteka);
}

void spremiRezultat(char* imeIgraca, int rezultat) {
	if (imeIgraca == NULL) {
		fprintf(stderr, "Nevazeci pokazivac na ime igraca.\n");
		return;
	}

	FILE* datoteka = fopen("rezultati.bin", "ab");
	if (datoteka == NULL) {
		perror("Neuspjelo otvaranje datoteke za spremanje rezultata");
		return;
	}

	RezultatIgraca noviRezultat;
	strncpy(noviRezultat.imeIgraca, imeIgraca, sizeof(noviRezultat.imeIgraca) - 1);
	noviRezultat.imeIgraca[sizeof(noviRezultat.imeIgraca) - 1] = '\0';
	noviRezultat.rezultat = rezultat;

	fwrite(&noviRezultat, sizeof(RezultatIgraca), 1, datoteka);
	fclose(datoteka);
}

int usporediRezultate(const void* a, const void* b) {
	return ((RezultatIgraca*)b)->rezultat - ((RezultatIgraca*)a)->rezultat;
}

static int usporediPoImenu(const void* a, const void* b) {
	return strcmp(((RezultatIgraca*)a)->imeIgraca, ((RezultatIgraca*)b)->imeIgraca);
}

void pretraziRezultat(const char* imeIgraca) {
	if (imeIgraca == NULL) {
		fprintf(stderr, "Nevazeci pokazivac na ime igraca.\n");
		return;
	}

	if (rezultatIgraca == NULL) {
		fprintf(stderr, "Rezultati nisu ucitani.\n");
		return;
	}

	RezultatIgraca kljuc;
	strncpy(kljuc.imeIgraca, imeIgraca, sizeof(kljuc.imeIgraca) - 1);
	kljuc.imeIgraca[sizeof(kljuc.imeIgraca) - 1] = '\0';

	RezultatIgraca* pronadjen = (RezultatIgraca*)bsearch(&kljuc, rezultatIgraca, brojRezultata, sizeof(RezultatIgraca), usporediPoImenu);
	if (pronadjen != NULL) {
		printf("Pronaden rezultat za %s: %d\n", pronadjen->imeIgraca, pronadjen->rezultat);
	}
	else {
		printf("Rezultat za %s nije pronaden.\n", imeIgraca);
	}
}

static void oslobodiMemoriju() {
	if (pitanja != NULL) {
		free(pitanja);
	}
}