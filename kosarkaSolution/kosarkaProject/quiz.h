#ifndef QUIZ_H
#define QUIZ_H

typedef struct {
	char pitanje[256];
	char opcije[4][128];
	int tocanOdgovor;
} Pitanje;

void randomPitanja();
void pokreniKviz();
void pregledajRezultate();
void spremiRezultat(char* imeIgraca, int rezultat);
int usporediRezultate(const void* a, const void* b);
int usporediPoImenu(const void* a, const void* b);
void pretraziRezultat(const char* imeIgraca);
void oslobodiMemoriju();


extern Pitanje pitanja[];
extern int brojPitanja;

#endif 
