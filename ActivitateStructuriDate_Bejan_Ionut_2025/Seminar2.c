#include<stdio.h>
#include<malloc.h>
#include<string.h>

struct Masina {
    int id;
    int anFabricatie;
    char* sofer;
    float kilometriiParcursi;
    char initialaProducator;
};

struct Masina initializare(int id, int anFabricatie, const char* sofer, float kilometriiParcursi, char initialaProducator) {
    struct Masina m;
    m.id = id;
    m.anFabricatie = anFabricatie;
    if (sofer != NULL) {
        m.sofer = (char*)malloc(strlen(sofer) + 1);
        if (m.sofer) {
            strcpy_s(m.sofer, strlen(sofer) + 1, sofer);
        }
    }
    else {
        m.sofer = NULL;
    }
    m.kilometriiParcursi = kilometriiParcursi;
    m.initialaProducator = initialaProducator;
    return m;
}

void afisare(struct Masina m) {
    printf("ID: %d \n", m.id);
    printf("An fabricatie: %d \n", m.anFabricatie);
    printf("Sofer: %s \n", m.sofer ? m.sofer : "N/A");
    printf("Kilometrii Parcursi: %f \n", m.kilometriiParcursi);
    printf("Initiala producator: %c \n", m.initialaProducator);
}

void afisareVector(struct Masina* vector, int nrElemente) {
    if (vector != NULL && nrElemente > 0) {
        for (int i = 0; i < nrElemente; i++) {
            afisare(vector[i]);
            printf("\n");
        }
    }
}

struct Masina* copiazaPrimeleNElemente(struct Masina* vector, int nrElemente, int nrElementeCopiate) {
    struct Masina* vectorNou = NULL;
    if (vector != NULL && nrElementeCopiate <= nrElemente && nrElementeCopiate > 0) {
        vectorNou = (struct Masina*)malloc(sizeof(struct Masina) * nrElementeCopiate);
        if (vectorNou) {
            for (int i = 0; i < nrElementeCopiate; i++) {
                vectorNou[i] = initializare(vector[i].id, vector[i].anFabricatie, vector[i].sofer, vector[i].kilometriiParcursi, vector[i].initialaProducator);
            }
        }
    }
    return vectorNou;
}

void dezalocare(struct Masina** vector, int* nrElemente) {
    if (*vector != NULL && *nrElemente > 0) {
        for (int i = 0; i < *nrElemente; i++) {
            if ((*vector)[i].sofer != NULL) {
                free((*vector)[i].sofer);
            }
        }
        free(*vector);
        *vector = NULL;
        *nrElemente = 0;
    }
}

void copiazaMasiniCuMultiKM(struct Masina* vector, int nrElemente, float prag, struct Masina** vectorNou, int* dimensiune) {
    if (vector != NULL && nrElemente > 0) {
        (*dimensiune) = 0;
        for (int i = 0; i < nrElemente; i++) {
            if (vector[i].kilometriiParcursi > prag) {
                (*dimensiune)++;
            }
        }

        *vectorNou = (struct Masina*)malloc(sizeof(struct Masina) * (*dimensiune));
        if (*vectorNou) {
            int contor = 0;
            for (int i = 0; i < nrElemente; i++) {
                if (vector[i].kilometriiParcursi > prag) {
                    (*vectorNou)[contor] = initializare(vector[i].id, vector[i].anFabricatie, vector[i].sofer, vector[i].kilometriiParcursi, vector[i].initialaProducator);
                    contor++;
                }
            }
        }
    }
}

struct Masina getPrimaMasinaDupaSofer(struct Masina* vector, int nrElemente, const char* conditie) {
    for (int i = 0; i < nrElemente; i++) {
        if (strcmp(vector[i].sofer, conditie) == 0) {
            return initializare(vector[i].id, vector[i].anFabricatie, vector[i].sofer, vector[i].kilometriiParcursi, vector[i].initialaProducator);
        }
    }
    return initializare(-1, 0, "", 0, '-');
}

int main() {
    int nrElemente = 3;
    struct Masina* vector = (struct Masina*)malloc(sizeof(struct Masina) * nrElemente);
    if (!vector) return 1;

    vector[0] = initializare(1, 2000, "Ion", 5000, 'A');
    vector[1] = initializare(2, 2001, "Ana", 3000, 'F');
    vector[2] = initializare(3, 2002, "Stefan", 7500, 'M');
    afisareVector(vector, nrElemente);

    int nrElementeNoi = 3;
    struct Masina* vectorNou = copiazaPrimeleNElemente(vector, nrElemente, nrElementeNoi);
    printf("Masini copiate: \n");
    afisareVector(vectorNou, nrElementeNoi);

    dezalocare(&vector, &nrElemente);

    float prag = 3500;
    copiazaMasiniCuMultiKM(vectorNou, nrElementeNoi, prag, &vector, &nrElemente);
    printf("Masini cu multi km: \n");
    afisareVector(vector, nrElemente);

    dezalocare(&vector, &nrElemente);
    dezalocare(&vectorNou, &nrElementeNoi);

    struct Masina primaMasina = getPrimaMasinaDupaSofer(vector, nrElemente, "Ana");
    afisare(primaMasina);
    free(primaMasina.sofer);


    return 0;
}