#include <stdio.h>
#include <string.h>

#define MAXPATIENTS 100

const char SPECIALTY_NAMES[4][30] = {"General Practice", "Paediatrics", "Cardiology", "Neurology"};
const float BASE_FEES[4] = {1500.0, 2500.0, 4500.0, 5000.0};
const int AVG_TIMES[4] = {15, 20, 30, 30};
const int DAILY_CAPS[4] = {30, 20, 12, 10};

const char WARD_NAMES[4][30] = {"General Ward", "Paediatric Ward", "Surgical Ward", "ICU"};
const float WARD_RATES[4] = {3000.0, 6000.0, 12000.0, 25000.0};
const int WARD_CAPACITY[4] = {20, 10, 10, 5};

int bedOccupancy[4][20];

void setupBeds() {
    int i, j;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 20; j++) {
            bedOccupancy[i][j] = 0;
        }
    }
}

int main() {
    setupBeds();
    printf("Smart Hospital Management System ...\n");
    return 0;
}

