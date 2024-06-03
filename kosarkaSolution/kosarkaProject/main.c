#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "quiz.h"
#include "file_utils.h"


typedef enum {
	POKRENI_KVIZ = 1,
	PREGLEDAJ_REZULTATE,
	PRETRAZI_REZULTATE,
	KOPIRAJ_DATOTEKU,
	OBRISI_DATOTEKU,
	IZLAZ
} IzbornikOpcija;

void prikaziIzbornik();

int main() {
	int opcija;
	char izvornaDatoteka[256];
	char odredisnaDatoteka[256];

	while (1) {
		prikaziIzbornik();
		printf("Odaberite opciju: ");
		scanf("%d", &opcija);
		char ime[50];

		IzbornikOpcija izbor = opcija;

		switch (izbor) {
		case POKRENI_KVIZ:
			pokreniKviz();
			break;
		case PREGLEDAJ_REZULTATE:
			pregledajRezultate();
			break;
		case PRETRAZI_REZULTATE:
			printf("Unesite ime igraca za pretragu: ");
			scanf("%49s", ime);
			pretraziRezultat(ime);
			break;
		case KOPIRAJ_DATOTEKU:
			printf("Unesite naziv izvorne datoteke: ");
			scanf("%255s", izvornaDatoteka);
			printf("Unesite naziv odredisne datoteke: ");
			scanf("%255s", odredisnaDatoteka);
			kopirajDatoteku(izvornaDatoteka, odredisnaDatoteka);
			break;
		case OBRISI_DATOTEKU:
			printf("Unesite naziv datoteke za brisanje: ");
			scanf("%255s", izvornaDatoteka);
			obrisiDatoteku(izvornaDatoteka);
			break;
		case IZLAZ:
			printf("Izlaz iz programa.\n");
			exit(0);
		default:
			printf("Neispravan odabir. Molim pokusajte ponovo.\n");
		}
	}

	return 0;
}

void prikaziIzbornik() {
	printf("--- IZBORNIK ---\n");
	printf("1. Pokreni kviz\n");
	printf("2. Pregledaj rezultate\n");
	printf("3. Pretrazi rezultate\n");
	printf("4. Kopiraj datoteku\n");
	printf("5. Obrisi datoteku\n");
	printf("6. Izlaz\n");
}
