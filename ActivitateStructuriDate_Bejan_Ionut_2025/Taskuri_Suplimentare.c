#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct BibliotecaInteractiva {
    int id;
    char* nume;
    int nrCarti;
    float* ratinguri;
};

typedef struct BibliotecaInteractiva BI;

 

BI initBiblioteca(int id, const char* nume, int nrCarti, float* ratinguri) {
    BI b;
    b.id = id;
    b.nume = (char*)malloc(strlen(nume) + 1);
    if (b.nume) strcpy(b.nume, nume);

    b.nrCarti = nrCarti;
    b.ratinguri = (float*)malloc(sizeof(float) * nrCarti);
    if (b.ratinguri) {
        for (int i = 0; i < nrCarti; i++) {
            b.ratinguri[i] = ratinguri[i];
        }
    }
    return b;
}

void afisare(BI b) {
    printf("ID: %d\nNume: %s\nNumar carti: %d\n", b.id, b.nume, b.nrCarti);
    for (int i = 0; i < b.nrCarti; i++) {
        printf("Rating[%d]: %.2f\n", i + 1, b.ratinguri[i]);
    }
    printf("\n");
}

void modificaNume(BI* b, const char* nou) {
    if (b->nume) free(b->nume);
    b->nume = (char*)malloc(strlen(nou) + 1);
    strcpy(b->nume, nou);
}

float calculeazaMedie(BI b) {
    float s = 0;
    for (int i = 0; i < b.nrCarti; i++) s += b.ratinguri[i];
    return (b.nrCarti > 0) ? s / b.nrCarti : 0;
}

void elibereaza(BI* b) {
    if (b->nume) free(b->nume);
    if (b->ratinguri) free(b->ratinguri);
    b->nume = NULL;
    b->ratinguri = NULL;
}



void afisareVector(BI* v, int n) {
    for (int i = 0; i < n; i++) afisare(v[i]);
}

BI* copiazaBiblioteciRatingMare(BI* v, int n, int* dimNoua) {
    *dimNoua = 0;
    for (int i = 0; i < n; i++)
        if (calculeazaMedie(v[i]) >= 4.5f) (*dimNoua)++;

    BI* r = (BI*)malloc(sizeof(BI) * (*dimNoua));
    int k = 0;
    for (int i = 0; i < n; i++)
        if (calculeazaMedie(v[i]) >= 4.5f)
            r[k++] = initBiblioteca(v[i].id, v[i].nume, v[i].nrCarti, v[i].ratinguri);
    return r;
}

BI* mutaBiblioteciCuPutineCarti(BI* v, int n, int* dimNoua) {
    *dimNoua = 0;
    for (int i = 0; i < n; i++)
        if (v[i].nrCarti < 3) (*dimNoua)++;

    BI* r = (BI*)malloc(sizeof(BI) * (*dimNoua));
    int k = 0;
    for (int i = 0; i < n; i++)
        if (v[i].nrCarti < 3)
            r[k++] = initBiblioteca(v[i].id, v[i].nume, v[i].nrCarti, v[i].ratinguri);
    return r;
}

BI* concateneaza(BI* v1, int d1, BI* v2, int d2, int* dFinal) {
    *dFinal = d1 + d2;
    BI* r = (BI*)malloc(sizeof(BI) * (*dFinal));
    for (int i = 0; i < d1; i++) r[i] = initBiblioteca(v1[i].id, v1[i].nume, v1[i].nrCarti, v1[i].ratinguri);
    for (int i = 0; i < d2; i++) r[d1 + i] = initBiblioteca(v2[i].id, v2[i].nume, v2[i].nrCarti, v2[i].ratinguri);
    return r;
}



void salveazaVectorInFisier(BI* v, int n, const char* numeFisier) {
    FILE* f = fopen(numeFisier, "w");
    if (!f) return;
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d,%s,%d", v[i].id, v[i].nume, v[i].nrCarti);
        for (int j = 0; j < v[i].nrCarti; j++) fprintf(f, ",%.2f", v[i].ratinguri[j]);
        fprintf(f, "\n");
    }
    fclose(f);
}

BI* citesteVectorDinFisier(const char* numeFisier, int* dim) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) return NULL;
    BI* v = NULL;
    char linie[256];
    *dim = 0;
    while (fgets(linie, sizeof(linie), f)) {
        int id, nr;
        char* t = strtok(linie, ",");
        id = atoi(t);
        t = strtok(NULL, ",");
        char* nume = (char*)malloc(strlen(t) + 1); strcpy(nume, t);
        t = strtok(NULL, ",");
        nr = atoi(t);
        float* r = (float*)malloc(sizeof(float) * nr);
        for (int i = 0; i < nr; i++) {
            t = strtok(NULL, ",");
            r[i] = atof(t);
        }
        BI b = initBiblioteca(id, nume, nr, r);
        BI* temp = realloc(v, sizeof(BI) * (*dim + 1));
        if (temp) {
            v = temp;
            v[*dim] = b;
            (*dim)++;
        }
        free(nume); free(r);
    }
    fclose(f);
    return v;
}



void creeazaMatriceClustere(BI* v, int n, BI**** matrice, int** dimLinii, int* nrLinii) {
    int maxCarti = 0;
    for (int i = 0; i < n; i++) if (v[i].nrCarti > maxCarti) maxCarti = v[i].nrCarti;
    *nrLinii = maxCarti;
    *matrice = (BI***)malloc(sizeof(BI**) * (*nrLinii));
    *dimLinii = (int*)calloc(*nrLinii, sizeof(int));
    for (int i = 0; i < *nrLinii; i++) (*matrice)[i] = NULL;
    for (int i = 0; i < n; i++) {
        int linie = v[i].nrCarti - 1;
        (*dimLinii)[linie]++;
        (*matrice)[linie] = (BI**)realloc((*matrice)[linie], sizeof(BI*) * (*dimLinii)[linie]);
        (*matrice)[linie][(*dimLinii)[linie] - 1] = (BI*)malloc(sizeof(BI));
        *(*matrice)[linie][(*dimLinii)[linie] - 1] = initBiblioteca(v[i].id, v[i].nume, v[i].nrCarti, v[i].ratinguri);
    }
}

void afiseazaMatrice(BI*** matrice, int* dimLinii, int nrLinii) {
    for (int i = 0; i < nrLinii; i++) {
        if (dimLinii[i] > 0) {
            printf("Cluster (nrCarti = %d):\n", i + 1);
            for (int j = 0; j < dimLinii[i]; j++) afisare(*matrice[i][j]);
        }
    }
}

void sorteazaLiniiDupaDimensiune(BI**** matrice, int** dimLinii, int nrLinii) {
    for (int i = 0; i < nrLinii - 1; i++) {
        for (int j = i + 1; j < nrLinii; j++) {
            if ((*dimLinii)[i] > (*dimLinii)[j]) {
                BI** tmp = (*matrice)[i]; (*matrice)[i] = (*matrice)[j]; (*matrice)[j] = tmp;
                int td = (*dimLinii)[i]; (*dimLinii)[i] = (*dimLinii)[j]; (*dimLinii)[j] = td;
            }
        }
    }
}

void elibereazaMatrice(BI*** matrice, int* dimLinii, int nrLinii) {
    for (int i = 0; i < nrLinii; i++) {
        for (int j = 0; j < dimLinii[i]; j++) {
            elibereaza(matrice[i][j]);
            free(matrice[i][j]);
        }
        free(matrice[i]);
    }
    free(matrice);
    free(dimLinii);
}



int main() {
    int dim = 0;
    BI* vector = citesteVectorDinFisier("vector_biblioteci.txt", &dim);
    afisareVector(vector, dim);

    int dimCopiate = 0;
    BI* vCopiate = copiazaBiblioteciRatingMare(vector, dim, &dimCopiate);
    printf("--- Rating mare ---\n");
    afisareVector(vCopiate, dimCopiate);

    int dimPutine = 0;
    BI* vPutine = mutaBiblioteciCuPutineCarti(vector, dim, &dimPutine);
    printf("--- Putine carti ---\n");
    afisareVector(vPutine, dimPutine);

    int dimFinal = 0;
    BI* vFinal = concateneaza(vCopiate, dimCopiate, vPutine, dimPutine, &dimFinal);
    printf("--- Concatenat ---\n");
    afisareVector(vFinal, dimFinal);

    BI*** matrice = NULL;
    int* dimLinii = NULL;
    int nrLinii = 0;
    creeazaMatriceClustere(vector, dim, &matrice, &dimLinii, &nrLinii);
    printf("--- Matrice initiala ---\n");
    afiseazaMatrice(matrice, dimLinii, nrLinii);

    sorteazaLiniiDupaDimensiune(&matrice, &dimLinii, nrLinii);
    printf("--- Matrice sortata ---\n");
    afiseazaMatrice(matrice, dimLinii, nrLinii);

    for (int i = 0; i < dim; i++) elibereaza(&vector[i]);
    for (int i = 0; i < dimCopiate; i++) elibereaza(&vCopiate[i]);
    for (int i = 0; i < dimPutine; i++) elibereaza(&vPutine[i]);
    for (int i = 0; i < dimFinal; i++) elibereaza(&vFinal[i]);

    free(vector); free(vCopiate); free(vPutine); free(vFinal);
    elibereazaMatrice(matrice, dimLinii, nrLinii);

    return 0;
}