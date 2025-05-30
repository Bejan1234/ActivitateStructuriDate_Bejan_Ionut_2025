#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina { //Seminar 6 - Hash Table
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
	Masina info;
	struct Nod* next;
};

typedef struct Nod Nod;

struct HashTable {
	int dim;
	Nod** vector;
};
typedef struct HashTable HashTable;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* lista) {
	Nod* p = lista;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {

	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	if (*lista) {
		Nod* p = *lista;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*lista = nou;
	}
}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(char key, int dimensiune) {

	int hash = 0;
	if (dimensiune != 0)
	{
		hash = key % dimensiune;
	}
	else
	{
		return 0;
	}
	return hash;


}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	int pozitie = calculeazaHash(masina.serie, hash.dim);
	if (pozitie < hash.dim)
	{
		if (hash.vector[pozitie] != NULL) // AVEM COLIZIUNE !!!!
		{
			adaugaMasinaInLista(&(hash.vector[pozitie]), masina);
		}
		else
		{
			hash.vector[pozitie] = (Nod*)malloc(sizeof(Nod));
			hash.vector[pozitie]->info = masina;
			hash.vector[pozitie]->next = NULL;
		}
	}
}

HashTable citireMasiniDinFisier(const char* numeFisier, int dim) {
	FILE* f = fopen(numeFisier, "r");
	HashTable ht = initializareHashTable(dim);
	while (!feof(f))
	{
		Masina m = citireMasinaDinFisier(f);
		inserareMasinaInTabela(ht, m);
	}

	fclose(f);
	return ht;

}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++)
	{
		printf("\nSuntem pe pozitia: %d\n", i);
		afisareListaMasini(ht.vector[i]);
	}
}

void dezalocareTabelaDeMasini(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		Nod* p = ht->vector[i];
		while (p) {
			Nod* temp = p;
			p = p->next;
			free(temp->info.model);
			free(temp->info.numeSofer);
			free(temp);
		}
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	float* vector = (float*)malloc(sizeof(float) * ht.dim);
	*nrClustere = 0;

	for (int i = 0; i < ht.dim; i++) {
		Nod* p = ht.vector[i];
		if (p != NULL) {
			float suma = 0;
			int count = 0;
			while (p) {
				suma += p->info.pret;
				count++;
				p = p->next;
			}
			vector[*nrClustere] = suma / count;
			(*nrClustere)++;
		}
	}
	return vector;
}

Masina getMasinaDupaCheie(HashTable ht, char serie) {
	Masina m;
	int pozitie = calculeazaHash(serie, ht.dim);
	if (ht.vector[pozitie] != NULL)
	{
		Nod* p = ht.vector[pozitie];
		while (p != NULL, p->info.serie != serie)
		{
			p = p->next;
		}
		if (p) {
			m = p->info;
			m.model = (char*)malloc(strlen(p->info.model) + 1);
			strcpy_s(m.model, strlen(p->info.model) + 1, p->info.model);
			m.numeSofer = (char*)malloc(strlen(p->info.numeSofer) + 1);
			strcpy_s(m.numeSofer, strlen(p->info.numeSofer) + 1, p->info.numeSofer);
		}
	}
	return m;
}

int main() {
	HashTable ht;
	ht = citireMasiniDinFisier("masini.txt", 7);
	afisareTabelaDeMasini(ht);
	Masina m = getMasinaDupaCheie(ht, 'A');
	if (m.id != -1) {
		printf("\nMasina cautata este: \n");
		afisareMasina(m);
		free(m.model);
		free(m.numeSofer);
	}
	else {
		printf("\nMasina nu a fost gasita\n");
	}
	int nrClustere = 0;
	float* medii = calculeazaPreturiMediiPerClustere(ht, &nrClustere);
	printf("\nPreturi medii pe clustere:\n");
	for (int i = 0; i < nrClustere; i++) {
		printf("Cluster %d: %.2f\n", i + 1, medii[i]);
	}
	free(medii);
	dezalocareTabelaDeMasini(&ht);

	return 0;
}