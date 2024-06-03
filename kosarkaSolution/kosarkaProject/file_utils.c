#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "file_utils.h"

void kopirajDatoteku(const char* izvornaDatoteka, const char* odredisnaDatoteka) {
	if (izvornaDatoteka == NULL || odredisnaDatoteka == NULL) {
		fprintf(stderr, "Nevazeci pokazivaci na nazive datoteka.\n");
		return;
	}

	FILE* izvor = fopen(izvornaDatoteka, "rb");
	if (izvor == NULL) {
		perror("Neuspjelo otvaranje izvorne datoteke");
		return;
	}
	FILE* odrediste = fopen(odredisnaDatoteka, "wb");
	if (odrediste == NULL) {
		perror("Neuspjelo otvaranje odredisne datoteke");
		fclose(izvor);
		return;
	}

	fseek(izvor, 0, SEEK_END);
	long velicinaDatoteke = ftell(izvor);
	rewind(izvor);

	char* buffer = (char*)malloc(velicinaDatoteke * sizeof(char));
	if (buffer == NULL) {
		perror("Neuspjelo zauzimanje memorije za kopiranje datoteke");
		fclose(izvor);
		fclose(odrediste);
		return;
	}

	size_t bytesRead = fread(buffer, sizeof(char), velicinaDatoteke, izvor);
	fwrite(buffer, sizeof(char), bytesRead, odrediste);

	free(buffer);
	buffer = NULL;

	fclose(izvor);
	fclose(odrediste);
}

void obrisiDatoteku(const char* nazivDatoteke) {
	if (nazivDatoteke == NULL) {
		fprintf(stderr, "Nevazeci pokazivac na naziv datoteke.\n");
		return;
	}

	if (remove(nazivDatoteke) == 0) {
		printf("Datoteka %s uspjesno obrisana.\n", nazivDatoteke);
	}
	else {
		perror("Neuspješno brisanje datoteke");
	}
}