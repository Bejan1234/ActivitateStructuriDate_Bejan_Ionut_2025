#include<stdio.h>
#include<stdlib.h>
#include <string.h>

struct Masina {  // Seminar 1 - Lucru cu functii
	int id;
	int anFabricatie;
	char* sofer;
	float kmparcursi;
	char initialaProducator;
};

struct Masina initializare(int id, int anFabricatie, const char* sofer, float kmparcursi, char initialaProducator) {
	struct Masina m;
	m.id = 1;
	m.anFabricatie = anFabricatie;
	m.sofer = ((char*)malloc(strlen(sofer) + 1));
	strcpy_s(m.sofer, strlen(sofer) + 1, sofer);
	m.kmparcursi = kmparcursi;
	m.initialaProducator = initialaProducator;

	return m;
}

void afisare(struct Masina m) {
	printf("ID: %d\n", m.id);
	printf("An Fabricatie: %d\n", m.anFabricatie);
	printf("Sofer: %s\n", m.sofer);
	printf("Km Parcursi: %f \n", m.kmparcursi);
	printf("Initiala producator: %c\n", m.initialaProducator);

}

void modifica_Sofer(struct Masina* m, const char* nouSofer) {
	if (strlen(nouSofer) > 2) {
		if (m->sofer != NULL) {
			free(m->sofer);
		}
		m->sofer = (char*)malloc(strlen(nouSofer) + 1);
		strcpy_s(m->sofer, strlen(nouSofer) + 1, nouSofer);
	}
	
}

void dezalocare(struct Masina* m) {
	if (m->sofer != NULL) {
		free(m->sofer);
		m->sofer = NULL;

	}
}

int main() {
	struct Masina masina;

	masina = initializare(1, 2004, "Robert", 2000, 'P');
	afisare(masina);

	modifica_Sofer(&masina, "Alex");
	afisare(masina);


	return 0;
}