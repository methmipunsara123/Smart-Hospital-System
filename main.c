#include <stdio.h>
#include <string.h>

#define MAXPATIENTS 100

const char SPECIALTYNAMES[4][30] = {"General Practice", "Paediatrics", "Cardiology", "Neurology"};
const float BASEFEES[4] = {1500.0, 2500.0, 4500.0, 5000.0};
const int AVGTIMES[4] = {15, 20, 30, 30};
const int DAILYCAPS[4] = {30, 20, 12, 10};

const char WARDNAMES[4][30] = {"General Ward", "Paediatric Ward", "Surgical Ward", "ICU"};
const float WARDRATES[4] = {3000.0, 6000.0, 12000.0, 25000.0};
const int WARDCAPACITY[4] = {20, 10, 10, 5};

int bedOccupancy[4][20];

void setupBeds() {
    int i, j;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 20; j++) {
            bedOccupancy[i][j] = 0;
        }
    }
}
int patientID[MAXPATIENTS];
char patientName[MAXPATIENTS][50];
int patientAge[MAXPATIENTS];
int urgencyLevel[MAXPATIENTS];
int specialtyID[MAXPATIENTS];
int isAdmitted[MAXPATIENTS];
int wardID[MAXPATIENTS];
int daysAdmitted[MAXPATIENTS];
int assignedBed[MAXPATIENTS];

int patientCount = 0;
int queueCounts[4] = {0, 0, 0, 0};

void registerPatient() {
    if(patientCount >= MAXPATIENTS) {
        printf("System full!\n");
        return;
    }
int calculateWaitingTime(int specIndex) {
    return queueCounts[specIndex - 1] * AVGTIMES[specIndex - 1];
}

    int specIndex;
    printf("\nSelect Specialty (1-OPD, 2-Paediatrics, 3-Cardiology, 4-Neurology): ");
    scanf("%d", &specIndex);

    if(queueCounts[specIndex - 1] >= DAILYCAPS[specIndex - 1]) {
        printf("Daily limit reached for this specialty!\n");
        return;
    }

    patientID[patientCount] = 1001 + patientCount;
    specialtyID[patientCount] = specIndex;


    printf("Enter Patient Name: ");
    getchar();
    fgets(patientName[patientCount], sizeof(patientName[patientCount]), stdin);
    patientName[patientCount][strcspn(patientName[patientCount], "\n")] = 0;


    printf("Enter Age: ");
    scanf("%d", &patientAge[patientCount]);

    printf("Enter Urgency Level (1-Normal, 2-Urgent, 3-Critical): ");
    scanf("%d", &urgencyLevel[patientCount]);


    printf("Is Admitted to Ward? (1-Yes, 0-No): ");
    scanf("%d", &isAdmitted[patientCount]);

    if(isAdmitted[patientCount] == 1) {
        printf("Select Ward ID (1-General, 2-Paediatric, 3-Surgical, 4-ICU): ");
        scanf("%d", &wardID[patientCount]);

        printf("Enter Days Admitted: ");
        scanf("%d", &daysAdmitted[patientCount]);

    int w = wardID[patientCount] - 1;
    int bedFound = 0;
    for(int b = 0; b < WARDCAPACITY[w]; b++) {
        if(bedOccupancy[w][b] == 0) {
            bedOccupancy[w][b] = 1;
            bedFound = b + 1;
            break;
        }
    }

    assignedBed[patientCount] = bedFound;
    }
    else {
        wardID[patientCount] = 0;
        daysAdmitted[patientCount] = 0;
        assignedBed[patientCount] = 0;
    }

    queueCounts[specIndex - 1]++;
    patientCount++;
    printf("\n[Reg-%d]\n Registered successfully!\n", patientID[patientCount - 1]);

}

int main() {
    setupBeds();
    printf("Smart Hospital Management System ...\n");
    registerPatient();
    return 0;
}

