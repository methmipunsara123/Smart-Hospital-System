#include <stdio.h>
#include <string.h>

#define MAXPATIENTS 100


void loadBedStatus();
void logPatientToFile(int i);
void saveBedStatus();
void printPatientBill(int index);
void sortByPriority();
void generateAnalytics();

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

// Helper Calculation Functions
int calculateWaitingTime(int specIndex) {
    return queueCounts[specIndex - 1] * AVGTIMES[specIndex - 1];
}

float calculateSurcharge(float baseFee, int urgency) {
    if (urgency == 2) {
        return baseFee * 0.20;
    }
    else if (urgency == 3) {
        return baseFee * 0.50;
    }
    else {
        return 0.0;
    }
}

float calculateWardCost(int ward, int days) {
    if (ward <= 0 || days <= 0) return 0.0;
    else {
        return days * WARDRATES[ward - 1];
    }
}

float calculateDiscount(float grossTotal, int age) {
    if (age < 5 || age > 65) {
        float Discount = grossTotal * 0.15;
        return Discount;
    }
    return 0.0;
}


void printPatientBill(int index) {
    int spec = specialtyID[index] - 1;
    float baseFee = BASEFEES[spec];
    float surcharge = calculateSurcharge(baseFee, urgencyLevel[index]);
    float wardCost = calculateWardCost(wardID[index], daysAdmitted[index]);
    float grossTotal = baseFee + surcharge + wardCost;
    float discount = calculateDiscount(grossTotal, patientAge[index]);
    float finalPayable = grossTotal - discount;
    int waitTime = calculateWaitingTime(specialtyID[index]);

    printf("\n======================================================\n");
    printf("        SMART HOSPITAL ADMISSION & BILL\n");
    printf("======================================================\n");
    printf("Patient ID          : PAT-%d\n", patientID[index]);
    printf("Patient Name        : %s\n", patientName[index]);
    printf("Age                 : %d Years %s\n", patientAge[index], (patientAge[index]<5 || patientAge[index]>65) ? "(15% Subsidy)" : "");
    printf("Specialty           : %s\n", SPECIALTYNAMES[spec]);

    if(isAdmitted[index]) {
        printf("Assigned Ward       : %s (Bed %02d)\n", WARDNAMES[wardID[index]-1], assignedBed[index]);
    } else {
        printf("Assigned Ward       : OPD (Not Admitted)\n");
    }

    printf("Urgency Level       : Level %d\n", urgencyLevel[index]);
    printf("------------------------------------------------------\n");
    printf("Base Fee            : LKR %.2f\n", baseFee);
    printf("Emergency Surcharge : LKR %.2f\n", surcharge);
    printf("Ward Cost           : LKR %.2f\n", wardCost);
    printf("------------------------------------------------------\n");
    printf("Gross Total         : LKR %.2f\n", grossTotal);
    printf("Age Subsidy         : LKR -%.2f\n", discount);
    printf("------------------------------------------------------\n");
    printf("Final Payable       : LKR %.2f\n", finalPayable);
    printf("Estimated Wait Time : %d mins\n", waitTime);
    printf("======================================================\n");
}

void sortByPriority() {
    int i, j;
    for (i = 0; i < patientCount - 1; i++) {
        for (j = 0; j < patientCount - i - 1; j++) {
            if (urgencyLevel[j] < urgencyLevel[j+1]) {
                int tempU = urgencyLevel[j]; urgencyLevel[j] = urgencyLevel[j+1]; urgencyLevel[j+1] = tempU;
                int tempID = patientID[j]; patientID[j] = patientID[j+1]; patientID[j+1] = tempID;
                char tempN[50]; strcpy(tempN, patientName[j]); strcpy(patientName[j], patientName[j+1]); strcpy(patientName[j+1], tempN);
                int tempA = patientAge[j]; patientAge[j] = patientAge[j+1]; patientAge[j+1] = tempA;
                int tempS = specialtyID[j]; specialtyID[j] = specialtyID[j+1]; specialtyID[j+1] = tempS;
                int tempAdm = isAdmitted[j]; isAdmitted[j] = isAdmitted[j+1]; isAdmitted[j+1] = tempAdm;
                int tempW = wardID[j]; wardID[j] = wardID[j+1]; wardID[j+1] = tempW;
                int tempD = daysAdmitted[j]; daysAdmitted[j] = daysAdmitted[j+1]; daysAdmitted[j+1] = tempD;
                int tempB = assignedBed[j]; assignedBed[j] = assignedBed[j+1]; assignedBed[j+1] = tempB;
            }
        }
    }
    printf("\nSorted by Urgency Level successfully.\n");
}

void generateAnalytics() {
    printf("----------------------------------------------");
    printf("\n--- REPORTS & ANALYTICS ---\n");
    printf("----------------------------------------------\n");
    printf("Total Patients Registered: %d\n", patientCount);

    int level3 = 0, level2 = 0, level1 = 0;
    float totalRevenue = 0, totalDiscounts = 0;
    float maxBill = -1;
    char highestPayer[50] = "None";

    for(int i = 0; i < patientCount; i++) {
        if(urgencyLevel[i] == 3) level3++;
        else if(urgencyLevel[i] == 2) level2++;
        else level1++;

        float baseFee = BASEFEES[specialtyID[i]-1];
        float surcharge = calculateSurcharge(baseFee, urgencyLevel[i]);
        float wardCost = calculateWardCost(wardID[i], daysAdmitted[i]);
        float gross = baseFee + surcharge + wardCost;
        float disc = calculateDiscount(gross, patientAge[i]);
        float finalPay = gross - disc;

        totalRevenue += finalPay;
        totalDiscounts += disc;

        if(finalPay > maxBill) {
            maxBill = finalPay;
            strcpy(highestPayer, patientName[i]);
        }
    }

    printf("Urgency Breakdown: Level 3: %d | Level 2: %d | Level 1: %d\n", level3, level2, level1);
    printf("Total Revenue Earned: LKR %.2f\n", totalRevenue);
    printf("Total Discounts Granted: LKR %.2f\n", totalDiscounts);
    printf("Highest Paying Patient: %s (LKR %.2f)\n", highestPayer, maxBill > 0 ? maxBill : 0.0);
}

void saveBedStatus() {
    FILE *fp = fopen("beds_status.txt", "w");
    if(!fp) return;
    for(int i=0; i<4; i++) {
        for(int j=0; j<20; j++) {
            fprintf(fp, "%d ", bedOccupancy[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

void loadBedStatus() {
    FILE *fp = fopen("beds_status.txt", "r");
    if(!fp) return;
    for(int i=0; i<4; i++) {
        for(int j=0; j<20; j++) {
            fscanf(fp, "%d", &bedOccupancy[i][j]);
        }
    }
    fclose(fp);
}

void logPatientToFile(int i) {
    FILE *fp = fopen("patientrecords.txt", "a");
    if(!fp) return;

    float baseFee = BASEFEES[specialtyID[i]-1];
    float surcharge = calculateSurcharge(baseFee, urgencyLevel[i]);
    float wardCost = calculateWardCost(wardID[i], daysAdmitted[i]);
    float gross = baseFee + surcharge + wardCost;
    float disc = calculateDiscount(gross, patientAge[i]);
    float finalPay = gross - disc;

    fprintf(fp, "PAT-%d \n %s \n Age: %d \n Urgency: %d \n Bill: LKR %.2f\n",
            patientID[i], patientName[i], patientAge[i], urgencyLevel[i], finalPay);

    fclose(fp);
}

void registerPatient() {
    if(patientCount >= MAXPATIENTS) {
        printf("System full!\n");
        return;
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

void displayMenu() {
    printf("\n=== SMART HOSPITAL MANAGEMENT SYSTEM ===\n");
    printf("1. Register Patient\n");
    printf("2. Display Bills\n");
    printf("3. Sort Patients by Priority\n");
    printf("4. View Analytics\n");
    printf("5. Exit\n");
    printf("Enter choice: ");
}

int main() {
    setupBeds();
    loadBedStatus();
    int choice;

    while(1) {
        displayMenu();
        if(scanf("%d", &choice) != 1) break;

        if(choice == 1) {
            registerPatient();
            if(patientCount > 0) {
                logPatientToFile(patientCount - 1);
                saveBedStatus();
            }
        } else if(choice == 2) {
            for(int i = 0; i < patientCount; i++) {
                printPatientBill(i);
            }
        } else if(choice == 3) {
            sortByPriority();
        } else if(choice == 4) {
            generateAnalytics();
        } else if(choice == 5) {
            saveBedStatus();
            printf("Exiting system...\n");
            break;
        } else {
            printf("Invalid Option.\n");
        }
    }
    return 0;
}
