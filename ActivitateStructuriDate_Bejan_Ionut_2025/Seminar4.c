#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod
{
	Masina info;
	struct Nod* next;
};

typedef struct Nod Nod;

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

void afisareListaMasini(Nod* nod) {
	while (nod)
	{
		afisareMasina(nod->info);
		nod = nod->next;
	}
}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod) );
	nodNou->info = masinaNoua;
	nodNou->next = NULL;

	if ((*lista) == NULL) {
		*lista = nodNou;
	}
	else
	{
		Nod* aux = (*lista);
		while (aux->next)
		{
			aux = aux->next;
		}
		aux->next = nodNou;
	}

}
//*adaugate recent
void adaugaLaInceputInLista(Nod** lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = *lista;
	*lista = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {

	FILE* file = fopen(numeFisier, "r");
	Nod* lista = NULL;
	while (!feof(file))
	{
		Masina m = citireMasinaDinFisier(file);
		adaugaMasinaInLista(&lista, m);
	}
	fclose(file);
	return lista;
}

void dezalocareListaMasini(Nod** lista) {
	while ((*lista) != NULL) {
		Nod* temp = (*lista);
		(*lista) = (*lista)->next;
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
	}
}

float calculeazaPretMediu(Nod* lista) {
	float suma = 0;
	int contor = 0;
	float pret = 0;

	while (lista) {
		suma += lista->info.pret;
		contor++;
		lista = lista->next;
	}

	if (contor == 0) {
		return 0;
	}

	return suma / contor;

}
//*adaugate recent
void stergeMasiniDinSeria(Nod** lista, char serieCautata) {
	Nod* curent = *lista;
	Nod* anterior = NULL;

	while (curent != NULL) {
		if (curent->info.serie == serieCautata) {
			Nod* deSters = curent;

			if (anterior == NULL) {
				// Elementul de sters este primul
				*lista = curent->next;
				curent = *lista;
			}
			else {
				anterior->next = curent->next;
				curent = curent->next;
			}

			free(deSters->info.model);
			free(deSters->info.numeSofer);
			free(deSters);
		}
		else {
			anterior = curent;
			curent = curent->next;
		}
	}
}


float calculeazaPretulMasinilorUnuiSofer(Nod* lista, const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	float suma = 0;
	while (lista) {
		if (strcmp(lista->info.numeSofer, numeSofer) == 0) {
			suma += lista->info.pret;
		}
		lista = lista->next;
	}
	return suma;
}

int getNrUsiMasinaScumpa(Nod* lista) {
	if (lista) {
		float pretMaxim = lista->info.pret;
		int nrUsi = lista->info.nrUsi;
		lista = lista->next;
		while (lista != NULL) {
			if (lista->info.pret > pretMaxim)
			{
				nrUsi = lista->info.nrUsi;
				pretMaxim = lista->info.pret;
			}
			lista = lista->next;
		}
		return nrUsi;
	}
	return 0;
}

int main() {

	Nod* nod = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(nod);
	float medie = calculeazaPretMediu(nod);
	printf("Pretul mediu al masinilor este: %.2f\n", medie);
	float sumaSofer = calculeazaPretulMasinilorUnuiSofer(nod, "Ionescu");
	printf("\nSuma soferului este: %.2f\n", sumaSofer);
	int nrUsi = getNrUsiMasinaScumpa(nod);
	printf("\nNumarul de usi ale celei mai scumpe masini este: %i\n", nrUsi);

	dezalocareListaMasini(&nod);

	return 0;
}