#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Booking structure
typedef struct {
    float duration;
    int bill;
    int ticketID;
} Booking;

// Vehicle structure
typedef struct {
    char name[30];
    char phone[20];
    char number[20];
    char type[10];
    int occupied;
    Booking b;
} Vehicle;

Vehicle *slots;
int max = 10;
int availableSlots;
int totalEarnings = 0;

// Convert to uppercase
void toUpper(char str[]) {
    for(int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

// Validate phone
int isValidPhone(char phone[]) {
    if(strlen(phone) != 10) return 0;
    for(int i = 0; i < 10; i++) {
        if(phone[i] < '0' || phone[i] > '9')
            return 0;
    }
    return 1;
}

// Validate vehicle number
int isValidNumber(char num[]) {
    if(strlen(num) != 10) return 0;

    if(!(num[0]>='A'&&num[0]<='Z')) return 0;
    if(!(num[1]>='A'&&num[1]<='Z')) return 0;
    if(!(num[2]>='0'&&num[2]<='9')) return 0;
    if(!(num[3]>='0'&&num[3]<='9')) return 0;
    if(!(num[4]>='A'&&num[4]<='Z')) return 0;
    if(!(num[5]>='A'&&num[5]<='Z')) return 0;

    for(int i=6;i<10;i++) {
        if(!(num[i]>='0'&&num[i]<='9')) return 0;
    }
    return 1;
}

// Initialize
void init() {
    for(int i = 0; i < max; i++) {
        slots[i].occupied = 0;
    }
}

// Save to file
void saveToFile() {
    FILE *fp = fopen("parking.txt", "w");

    if(fp == NULL) {
        printf("Error saving file!\n");
        return;
    }

    for(int i = 0; i < max; i++) {
        if(slots[i].occupied == 1) {
            fprintf(fp, "%s %s %s %s %.2f %d %d\n",
                slots[i].name,
                slots[i].phone,
                slots[i].number,
                slots[i].type,
                slots[i].b.duration,
                slots[i].b.bill,
                slots[i].b.ticketID);
        }
    }

    fclose(fp);
}

// Load from file
void loadFromFile() {
    FILE *fp = fopen("parking.txt", "r");
    if(fp == NULL) return;

    char name[30], phone[20], num[20], type[10];
    float duration;
    int bill, ticket;

    while(fscanf(fp,"%s %s %s %s %f %d %d",
        name, phone, num, type, &duration, &bill, &ticket) != EOF) {

        for(int i=0;i<max;i++) {
            if(slots[i].occupied==0) {
                strcpy(slots[i].name,name);
                strcpy(slots[i].phone,phone);
                strcpy(slots[i].number,num);
                strcpy(slots[i].type,type);
                slots[i].b.duration=duration;
                slots[i].b.bill=bill;
                slots[i].b.ticketID=ticket;
                slots[i].occupied=1;
                availableSlots--;
                break;
            }
        }
    }

    fclose(fp);
}

// View file data
void viewFile() {
    FILE *fp = fopen("parking.txt", "r");

    if(fp == NULL) {
        printf("No data found!\n");
        return;
    }

    char line[200];
    printf("\n--- FILE DATA ---\n");

    while(fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }

    fclose(fp);
}

// Park vehicle
void parkVehicle() {
    char name[30], phone[20], num[20], type[10];
    float duration;

    if(availableSlots == 0) {
        printf("Parking Full!\n");
        return;
    }

    printf("Enter your name: ");
    scanf("%s", name);

    printf("Enter phone number: ");
    scanf("%s", phone);

    if(!isValidPhone(phone)) {
        printf("Invalid phone number!\n");
        return;
    }

    char fullPhone[20] = "+91";
    strcat(fullPhone, phone);

    printf("Enter vehicle number: ");
    scanf("%s", num);
    toUpper(num);

    if(!isValidNumber(num)) {
        printf("Invalid vehicle number!\n");
        return;
    }

    printf("Enter type (car/bike): ");
    scanf("%s", type);
    toUpper(type);

    printf("Enter duration: ");
    scanf("%f", &duration);

    int start,end,rate;

    if(strcmp(type,"CAR")==0) {
        start=0; end=5; rate=1000;
    } else if(strcmp(type,"BIKE")==0) {
        start=5; end=10; rate=500;
    } else {
        printf("Invalid type!\n");
        return;
    }

    int bill = duration * rate;

    if(duration > 3) {
        printf("Original bill: Rupees %d\n", bill);
        bill -= bill * 0.30;
        printf("After discount: Rupees %d\n", bill);
    }

    for(int i=start;i<end;i++) {
        if(slots[i].occupied==0) {

            strcpy(slots[i].name,name);
            strcpy(slots[i].phone,fullPhone);
            strcpy(slots[i].number,num);
            strcpy(slots[i].type,type);

            slots[i].b.duration=duration;
            slots[i].b.bill=bill;
            slots[i].b.ticketID=rand()%10000;

            slots[i].occupied=1;
            availableSlots--;

            saveToFile();

            printf("\nVehicle parked at slot %d\n", i+1);
            printf("Ticket ID: %d\n", slots[i].b.ticketID);
            return;
        }
    }

    printf("No slots available!\n");
}

// Remove vehicle
void removeVehicle() {
    int ticket;

    printf("Enter Ticket ID: ");
    scanf("%d",&ticket);

    for(int i=0;i<max;i++) {
        if(slots[i].occupied && slots[i].b.ticketID==ticket) {

            printf("\n----- RECEIPT -----\n");
            printf("Name: %s\n", slots[i].name);
            printf("Phone: %s\n", slots[i].phone);
            printf("Vehicle: %s\n", slots[i].number);
            printf("Type: %s\n", slots[i].type);
            printf("Duration: %.2f\n", slots[i].b.duration);
            printf("Bill: Rupees %d\n", slots[i].b.bill);
            printf("Ticket ID: %d\n", slots[i].b.ticketID);

            totalEarnings += slots[i].b.bill;

            slots[i].occupied=0;
            availableSlots++;

            saveToFile();

            printf("Thank you! Visit again 😊\n");
            return;
        }
    }

    printf("Invalid Ticket ID!\n");
}

// Display
void display() {
    for(int i=0;i<max;i++) {
        if(slots[i].occupied)
            printf("Slot %d: %s (%s)\n", i+1, slots[i].number, slots[i].type);
        else
            printf("Slot %d: Empty\n", i+1);
    }
}

// MAIN
int main() {
    int choice;

    slots=(Vehicle*)malloc(max*sizeof(Vehicle));
    availableSlots=max;

    init();
    loadFromFile();

    printf("Welcome to Smart Parking System\n");

    while(1) {
        printf("\nTotal Slots: %d | Available: %d\n", max, availableSlots);
        printf("1. Park Vehicle\n");
        printf("2. Remove Vehicle\n");
        printf("3. Display Parking\n");
        printf("4. View File Data\n");
        printf("5. Exit\n");

        scanf("%d",&choice);

        switch(choice) {
            case 1: parkVehicle(); break;
            case 2: removeVehicle(); break;
            case 3: display(); break;
            case 4: viewFile(); break;
            case 5: free(slots); return 0;
            default: printf("Invalid choice!\n");
        }
    }
}
