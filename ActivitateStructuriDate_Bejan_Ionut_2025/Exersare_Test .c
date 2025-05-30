#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    unsigned int id;
    char* dataLansare;
    char* dataLivrare;
    char* numeClient;
    float valoare;
} Comanda;


typedef struct Nod {
    Comanda info;
    struct Nod* st;
    struct Nod* dr;
} Nod;


Comanda initComanda(unsigned int id, const char* lansare, const char* livrare, const char* nume, float valoare) {
    Comanda c;
    c.id = id;
    c.dataLansare = (char*)malloc(strlen(lansare) + 1);
    strcpy(c.dataLansare, lansare);
    c.dataLivrare = (char*)malloc(strlen(livrare) + 1);
    strcpy(c.dataLivrare, livrare);
    c.numeClient = (char*)malloc(strlen(nume) + 1);
    strcpy(c.numeClient, nume);
    c.valoare = valoare;
    return c;
}

void afisareComanda(Comanda c) {
    printf("Id comanda %u\n", c.id);
    printf("Data lansare %s\n", c.dataLansare);
    printf("Data livrare %s\n", c.dataLivrare);
    printf("Vume client %s\n", c.numeClient);
    printf("Valoare %.2f\n\n", c.valoare);
}


void inserare(Nod** rad, Comanda c) {
    if (*rad == NULL) {
        Nod* nou = (Nod*)malloc(sizeof(Nod));
        nou->info = c;
        nou->st = nou->dr = NULL;
        *rad = nou;
    }
    else if (c.id < (*rad)->info.id) {
        inserare(&(*rad)->st, c);
    }
    else {
        inserare(&(*rad)->dr, c);
    }
}


void postordine(Nod* rad) {
    if (rad) {
        postordine(rad->st);
        postordine(rad->dr);
        afisareComanda(rad->info);
    }
}


Comanda* cautaComanda(Nod* rad, unsigned int id) {
    if (!rad) return NULL;
    if (rad->info.id == id) return &rad->info;
    if (id < rad->info.id) return cautaComanda(rad->st, id);
    return cautaComanda(rad->dr, id);
}

float valoareMedieTreiComenzi(Nod* rad, unsigned int id1, unsigned int id2, unsigned int id3) {
    Comanda* c1 = cautaComanda(rad, id1);
    Comanda* c2 = cautaComanda(rad, id2);
    Comanda* c3 = cautaComanda(rad, id3);
    if (c1 && c2 && c3)
        return (c1->valoare + c2->valoare + c3->valoare) / 3.0f;
    return -1;
}


Comanda* extrageComenziClient(Nod* rad, const char* nume, Comanda* vector, int* dim) {
    if (!rad) return vector;
    vector = extrageComenziClient(rad->st, nume, vector, dim);
    if (strcmp(rad->info.numeClient, nume) == 0) {
        vector = (Comanda*)realloc(vector, sizeof(Comanda) * (*dim + 1));
        vector[*dim] = initComanda(rad->info.id, rad->info.dataLansare,
            rad->info.dataLivrare, rad->info.numeClient, rad->info.valoare);
        (*dim)++;
    }
    vector = extrageComenziClient(rad->dr, nume, vector, dim);
    return vector;
}


void dezalocareArbore(Nod** rad) {
    if (*rad) {
        dezalocareArbore(&(*rad)->st);
        dezalocareArbore(&(*rad)->dr);
        free((*rad)->info.dataLansare);
        free((*rad)->info.dataLivrare);
        free((*rad)->info.numeClient);
        free(*rad);
        *rad = NULL;
    }
}


void dezalocareVector(Comanda* v, int dim) {
    for (int i = 0; i < dim; i++) {
        free(v[i].dataLansare);
        free(v[i].dataLivrare);
        free(v[i].numeClient);
    }
    free(v);
}

void citireComenziDinFisier(const char* numeFisier, Nod** rad) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) return;
    unsigned int id;
    char lansare[100], livrare[100], client[100];
    float valoare;
    while (fscanf(f, "%u,%99[^,],%99[^,],%99[^,],%f\n", &id, lansare, livrare, client, &valoare) == 5) {
        Comanda c = initComanda(id, lansare, livrare, client, valoare);
        inserare(rad, c);
    }
    fclose(f);
}

int main() {
    Nod* rad = NULL;
    citireComenziDinFisier("comenzi.txt", &rad);

    printf("--- Traversare postordine ---\n");
    postordine(rad);

    printf("\n--- Valoare medie comenzi ---\n");
    float medie = valoareMedieTreiComenzi(rad, 1, 3, 5);
    if (medie >= 0)
        printf("Media celor 3 comenzi: %.2f\n", medie);
    else
        printf("Cel putin una dintre comenzile cautate nu exista.\n");

    printf("\n--- Comenzi pentru client 'Popescu' ---\n");
    int dim = 0;
    Comanda* vector = NULL;
    vector = extrageComenziClient(rad, "Popescu", vector, &dim);
    for (int i = 0; i < dim; i++) afisareComanda(vector[i]);

    dezalocareVector(vector, dim);
    dezalocareArbore(&rad);

    return 0;
}
