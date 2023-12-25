#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CARS 100
#define MAX_RENTALS 50

struct Car {
    char model[50];
    char plateNumber[15];
    float mileage;
    int isRented;  // 0 si disponible, 1 si louée
    int isDamaged; // 0 si en bon état, 1 si en panne
};

struct Rental {
    struct Car *car;
    char renterName[50];
    int days;
    float mileageAtRent; // Kilométrage 
};

void addCar(struct Car fleet[], int *carCount, const char *model, const char *plateNumber, float mileage)
{
    struct Car newCar;
    strcpy(newCar.model, model);
    strcpy(newCar.plateNumber, plateNumber);
    newCar.mileage = mileage;
    newCar.isRented = 0;
    newCar.isDamaged = 0;

    fleet[*carCount] = newCar;
    (*carCount)++;

    printf("Voiture ajoutée avec succès\n");
}

void displayCar(const struct Car *car)
{
    printf("Modèle : %s, Plaque : %s, Kilométrage : %.2f, %s, %s\n",
           car->model, car->plateNumber, car->mileage,
           car->isRented ? "Louée" : "Disponible",
           car->isDamaged ? "En panne" : "En bon état");
}

void removeDamagedCar(struct Car fleet[], int *carCount, const char *plateNumber)
{
    for (int i = 0; i < *carCount; i++) 
    {
        if (strcmp(fleet[i].plateNumber, plateNumber) == 0) 
        {
            if (fleet[i].isDamaged)
            {
                for (int j = i; j < (*carCount - 1); j++)
                {

                    fleet[j] = fleet[j + 1];
                }
                (*carCount)--;
                printf("Voiture supprimée\n");
                return;
            } else 
            {
                printf("La voiture n'est pas en panne\n");
                return;
            }
        }
    }
    printf("Voiture non trouvée\n");
}

void modifyCar(struct Car fleet[], int carCount, const char *plateNumber, float newMileage, int isDamaged)
{
    for (int i = 0; i < carCount; i++)
    {
        if (strcmp(fleet[i].plateNumber, plateNumber) == 0)
        {
            fleet[i].mileage = newMileage;
            fleet[i].isDamaged = isDamaged;
            printf("modification de la voiture avec succès\n");
            return;
        }
    }
    printf("Voiture non trouvée\n");
}

void rentCar(struct Car fleet[], int carCount, struct Rental rentals[], int *rentalCount)
{
    int carIndex;
    displayCars(fleet, carCount);

    printf("donner le numéro de la voiture à louer : ");
    scanf("%d", &carIndex);

    if (carIndex >= 0 && carIndex < carCount)
    {
        if (fleet[carIndex].isRented)
        {
            printf("la voiture est louée\n");
        }
         else
         {
            struct Rental newRental;
            newRental.car = &fleet[carIndex];

            printf("Nom du locataire : ");
            scanf("%s", newRental.renterName);

            printf("Nombre de jours de location : ");
            scanf("%d", &newRental.days);

            newRental.mileageAtRent = fleet[carIndex].mileage;

            fleet[carIndex].isRented = 1;
            rentals[*rentalCount] = newRental;
            (*rentalCount)++;

            printf("Voiture louée\n");
        }
    } else
    {
        printf("Numéro de voiture introuvable\n");
    }
}

void returnCar(struct Car fleet[], int carCount, struct Rental rentals[], int *rentalCount, const char *plateNumber)
{
    for (int i = 0; i < *rentalCount; i++)
    {
        if (strcmp(rentals[i].car->plateNumber, plateNumber) == 0)
        {
            fleet[carCount].isRented = 0; 
            float mileageAtReturn = rentals[i].car->mileage;
            int daysRented = rentals[i].days;

            float cost = calculateRentalCost(mileageAtReturn - rentals[i].mileageAtRent, daysRented);

            printf("Voiture retournée\n");
            printf("Coût de la location : %.2f euros\n", cost);
            (*rentalCount)--;
            return;
        }
    }
    printf("Voiture introuvable\n");
}

float calculateRentalCost(float mileage, int days)
{
    const float costPerKilometer = 0.2;
    const float dailyRentalRate = 30.0;

    float mileageCost = mileage * costPerKilometer;
    float dailyCost = days * dailyRentalRate;

    return mileageCost + dailyCost;
}

void displayRentals(const struct Rental rentals[], int rentalCount)
{
    printf("Historique des locations :\n");
    for (int i = 0; i < rentalCount; i++)
    {
        printf("Voiture : %s, Loueur : %s, Jours : %d\n",
               rentals[i].car->model, rentals[i].renterName, rentals[i].days, rentals[i].mileageAtRent);
    }
}

void displayCars(const struct Car fleet[], int carCount)
{
    printf("Liste des voitures :\n");
    for (int i = 0; i < carCount; i++)
    {
        displayCar(&fleet[i]);
    }
}
void displayInvoice(const struct Rental rentals[], int rentalCount, const char *plateNumber)
{
    for (int i = 0; i < rentalCount; i++)
    {
        if (strcmp(rentals[i].car->plateNumber, plateNumber) == 0)
        {
            float mileageAtReturn = rentals[i].car->mileage;
            int daysRented = rentals[i].days;

            float cost = calculateRentalCost(mileageAtReturn - rentals[i].mileageAtRent, daysRented);

            printf("Facture pour la location de la voiture %s :\n", plateNumber);
            printf("Kilométrage au début : %.2f\n", rentals[i].mileageAtRent);
            printf("Kilométrage à la fin : %.2f\n", mileageAtReturn);
            printf("Nombre de jours : %d\n", daysRented);
            printf("Coût total : %.2f euros\n", cost);

            return;
        }
    }
    printf("Voiture introuvable\n");
}

int main()
{
    struct Car carFleet[MAX_CARS];
    struct Rental currentRentals[MAX_RENTALS];
    int numberOfCars = 0;
    int numberOfRentals = 0;

    int choice;

    do
    {
        printf("1. Ajouter une voiture\n");
        printf("2. Afficher les voitures\n");
        printf("3. Louer une voiture\n");
        printf("4. Retourner une voiture\n");
        printf("5. Supprimer une voiture en panne\n");
        printf("6. Modifier la description d'une voiture\n");
        printf("7. Afficher l'historique des locations\n");
        printf("8. Afficher la facture pour une location\n");
        printf("Choisissez une option : ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                {
                    char model[50];
                    char plateNumber[15];
                    float mileage;

                    printf("Modèle de la voiture : ");
                    scanf("%s", model);

                    printf("Plaque d'immatriculation : ");
                    scanf("%s", plateNumber);

                    printf("Kilométrage : ");
                    scanf("%f", &mileage);

                    addCar(carFleet, &numberOfCars, model, plateNumber, mileage);
                }
                break;
            case 2:
                displayCars(carFleet, numberOfCars);
                break;
            case 3:
                rentCar(carFleet, numberOfCars, currentRentals, &numberOfRentals);
                break;
            case 4:
                {
                    char plateNumber[15];
                    printf("Plaque d'immatriculation de la voiture à retourner : ");
                    scanf("%s", plateNumber);
                    returnCar(carFleet, numberOfCars, currentRentals, &numberOfRentals, plateNumber);
                }
                break;
            case 5:
                {
                    char plateNumber[15];
                    printf("Plaque d'immatriculation de la voiture en panne : ");
                    scanf("%s", plateNumber);
                    removeDamagedCar(carFleet, &numberOfCars, plateNumber);
                }
                break;
            case 6:
                {
                    char plateNumber[15];
                    float newMileage;
                    int isDamaged;
                    printf("Plaque d'immatriculation de la voiture à modifier : ");
                    scanf("%s", plateNumber);
                    printf("Nouveau kilométrage : ");
                    scanf("%f", &newMileage);
                    printf("La voiture est-elle en panne ? : ");
                    scanf("%d", &isDamaged);
                    modifyCar(carFleet, numberOfCars, plateNumber, newMileage, isDamaged);
                }
                break;
            case 7:
                displayRentals(currentRentals, numberOfRentals);
                break;
            case 8:
                {
                    char plateNumber[15];
                    printf("Plaque d'immatriculation de la voiture pour afficher la facture : ");
                    scanf("%s", plateNumber);
                    displayInvoice(currentRentals, numberOfRentals, plateNumber);
                }
                break;
            default:
                printf("Option invalide.\n");
        }

    } while (choice != 8);

    return 0;
}