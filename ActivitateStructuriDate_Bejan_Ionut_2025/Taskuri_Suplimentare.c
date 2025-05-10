#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct BibliotecaInteractiva { //Bejan Ionut  B si I
    int id;
    char* nume;
    int nrCarti;
    float* ratinguri;
};

typedef struct BibliotecaInteractiva BI;

BI initializare(int id, const char* nume, int nrCarti, float* ratinguri) {
    BI b;
    b.id = id;
    b.nume = (char*)malloc(strlen(nume) + 1);
    strcpy(b.nume, nume);
    b.nrCarti = nrCarti;
    b.ratinguri = (float*)malloc(sizeof(float) * nrCarti);
    for (int i = 0; i < nrCarti; i++) {
        b.ratinguri[i] = ratinguri[i];
    }
    return b;
}

BI citireDeLaTastatura() {
    BI b;
    char buffer[100];

    printf("ID: ");
    scanf("%d", &b.id);
    getchar();

    printf("Nume: ");
    fgets(buffer, 100, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    b.nume = (char*)malloc(strlen(buffer) + 1);
    strcpy(b.nume, buffer);

    printf("Numar carti: ");
    scanf("%d", &b.nrCarti);

    b.ratinguri = (float*)malloc(sizeof(float) * b.nrCarti);
    for (int i = 0; i < b.nrCarti; i++) {
        printf("Rating[%d]: ", i + 1);
        scanf("%f", &b.ratinguri[i]);
    }

    return b;
}

void afisare(BI b) {
    printf("ID: %d\n", b.id);
    printf("Nume: %s\n", b.nume);
    printf("Numar carti: %d\n", b.nrCarti);
    for (int i = 0; i < b.nrCarti; i++) {
        printf("Rating[%d]: %.2f\n", i + 1, b.ratinguri[i]);
    }
    printf("\n");
}

void afisareVector(BI* vector, int n) {
    for (int i = 0; i < n; i++) {
        afisare(vector[i]);
    }
}

float calculeazaMedie(BI b) {
    float suma = 0;
    for (int i = 0; i < b.nrCarti; i++) {
        suma += b.ratinguri[i];
    }
    return b.nrCarti > 0 ? suma / b.nrCarti : 0;
}

void modificaNume(BI* b, const char* nou) {
    free(b->nume);
    b->nume = (char*)malloc(strlen(nou) + 1);
    strcpy(b->nume, nou);
}

void dezalocare(BI* b) {
    free(b->nume);
    free(b->ratinguri);
    b->nume = NULL;
    b->ratinguri = NULL;
}

BI* copiazaBiblioteciRatingMare(BI* vector, int n, int* dimNoua) {
    *dimNoua = 0;
    for (int i = 0; i < n; i++) {
        if (calculeazaMedie(vector[i]) >= 4.5f) {
            (*dimNoua)++;
        }
    }

    BI* rezultat = (BI*)malloc(sizeof(BI) * (*dimNoua));
    int k = 0;
    for (int i = 0; i < n; i++) {
        if (calculeazaMedie(vector[i]) >= 4.5f) {
            rezultat[k] = initializare(vector[i].id, vector[i].nume, vector[i].nrCarti, vector[i].ratinguri);
            k++;
        }
    }
    return rezultat;
}

BI* mutaBiblioteciCuPutineCarti(BI* vector, int n, int* dimNoua) {
    *dimNoua = 0;
    for (int i = 0; i < n; i++) {
        if (vector[i].nrCarti < 3) {
            (*dimNoua)++;
        }
    }

    BI* rezultat = (BI*)malloc(sizeof(BI) * (*dimNoua));
    int k = 0;
    for (int i = 0; i < n; i++) {
        if (vector[i].nrCarti < 3) {
            rezultat[k] = initializare(vector[i].id, vector[i].nume, vector[i].nrCarti, vector[i].ratinguri);
            k++;
        }
    }
    return rezultat;
}

BI* concateneaza(BI* v1, int d1, BI* v2, int d2, int* dFinal) {
    *dFinal = d1 + d2;
    BI* rezultat = (BI*)malloc(sizeof(BI) * (*dFinal));
    for (int i = 0; i < d1; i++) {
        rezultat[i] = initializare(v1[i].id, v1[i].nume, v1[i].nrCarti, v1[i].ratinguri);
    }
    for (int i = 0; i < d2; i++) {
        rezultat[d1 + i] = initializare(v2[i].id, v2[i].nume, v2[i].nrCarti, v2[i].ratinguri);
    }
    return rezultat;
}

int main() {
    int dim = 5;
    BI* vector = (BI*)malloc(sizeof(BI) * dim);

    float r1[] = { 4.6f, 4.8f };
    float r2[] = { 3.5f, 3.9f };
    float r3[] = { 5.0f, 5.0f };
    float r4[] = { 2.9f };
    float r5[] = { 3.0f, 3.1f };

    vector[0] = initializare(1, "Carturesti", 2, r1);
    vector[1] = initializare(2, "Universitas", 2, r2);
    vector[2] = initializare(3, "Litera", 2, r3);
    vector[3] = initializare(4, "Bibliostar", 1, r4);
    vector[4] = initializare(5, "Bibliotecus", 2, r5);

    printf("--- Vector initial ---\n");
    afisareVector(vector, dim);

    int dimCopiate = 0;
    BI* copiate = copiazaBiblioteciRatingMare(vector, dim, &dimCopiate);
    printf("--- Biblioteci cu rating >= 4.5 ---\n");
    afisareVector(copiate, dimCopiate);

    int dimMutate = 0;
    BI* mutate = mutaBiblioteciCuPutineCarti(vector, dim, &dimMutate);
    printf("--- Biblioteci cu mai putin de 3 carti ---\n");
    afisareVector(mutate, dimMutate);

    int dimFinal = 0;
    BI* concat = concateneaza(vector, dim, copiate, dimCopiate, &dimFinal);
    printf("--- Vector concatenat ---\n");
    afisareVector(concat, dimFinal);

    printf("--- Citire biblioteca de la tastatura ---\n");
    BI bNou = citireDeLaTastatura();
    afisare(bNou);
    dezalocare(&bNou);

    for (int i = 0; i < dim; i++) dezalocare(&vector[i]);
    free(vector);
    for (int i = 0; i < dimCopiate; i++) dezalocare(&copiate[i]);
    free(copiate);
    for (int i = 0; i < dimMutate; i++) dezalocare(&mutate[i]);
    free(mutate);
    for (int i = 0; i < dimFinal; i++) dezalocare(&concat[i]);
    free(concat);

    return 0;
}
