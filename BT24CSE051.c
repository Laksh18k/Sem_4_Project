#include "BT24CSE051.h"


// helpers
Driver* split(Driver* head) {
    Driver *slow=head, *fast=head->next;

    while (fast && fast->next) {
        slow=slow->next;
        fast=fast->next->next;
    }

    Driver* second=slow->next;
    slow->next=NULL;
    return second;
}

Driver* merge(Driver* a, Driver* b) {
    if (!a) return b;
    if (!b) return a;
    Driver* result = NULL;
    if (a->total_Earnings > b->total_Earnings) {
        result = a;
        result->next = merge(a->next, b);
    } else {
        result = b;
        result->next = merge(a, b->next);
    }
    return result;
}

Driver* mergeSortDrivers(Driver* head) {
    if (!head || !head->next)
        return head;

    Driver* second=split(head);

    head=mergeSortDrivers(head);
    second=mergeSortDrivers(second);

    return merge(head, second);
}

Passenger* findPassengerByID(Passenger *head, int p_id) {
    while (head) {
        if (head->p_Id == p_id) return head;
        head = head->next;
    }
    return NULL;
}

Driver* findDriverByID(Driver *head, int d_id) {
    while (head) {
        if (head->d_Id == d_id) return head;
        head = head->next;
    }
    return NULL;
}

Booking* findBookingByID(Booking *head, int b_id) {
    while (head) {
        if (head->b_Id == b_id) return head;
        head = head->next;
    }
    return NULL;
}

float calculateDistance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}


/// File handling

void saveDrivers(Driver *head) {
    FILE *fp = fopen("drivers.csv", "w");
    if (!fp) return;
    // Header
    fprintf(fp, "id,name,type,status,x,y,earnings\n");
    while (head) {
        fprintf(fp, "%d,%s,%d,%d,%d,%d,%.2f\n", 
                head->d_Id, head->name, head->v_Type, head->d_Status, 
                head->loc.x, head->loc.y, head->total_Earnings);
        head = head->next;
    }
    fclose(fp);
}

void loadDrivers(Driver **head) {
    FILE *fp = fopen("drivers.csv", "r");
    if (!fp) return;
    char line[200];
    fgets(line, 200, fp); // Skip header
    while (fgets(line, 200, fp)) {
        Driver *newDriver = (Driver *)malloc(sizeof(Driver));
        sscanf(line, "%d,%[^,],%u,%u,%d,%d,%f", 
               &newDriver->d_Id, newDriver->name, (unsigned int*)&newDriver->v_Type, 
               (unsigned int*)&newDriver->d_Status, &newDriver->loc.x, &newDriver->loc.y, 
               &newDriver->total_Earnings);
        newDriver->next = *head;
        *head = newDriver;
    }
    fclose(fp);
}

void savePassengers(Passenger *head) {
    FILE *fp = fopen("passengers.csv", "w");
    if (!fp) return;
    fprintf(fp, "id,name,number,frequency\n");
    while (head) {
        fprintf(fp, "%d,%s,%s,%d\n", head->p_Id, head->name, head->number, head->frequency);
        head = head->next;
    }
    fclose(fp);
}

void loadPassengers(Passenger **head) {
    FILE *fp = fopen("passengers.csv", "r");
    if (!fp) return;
    char line[200];
    fgets(line, 200, fp); // Skip header
    while (fgets(line, 200, fp)) {
        Passenger *newNode = (Passenger*)malloc(sizeof(Passenger));
        sscanf(line, "%d,%[^,],%[^,],%d", &newNode->p_Id, newNode->name, newNode->number, &newNode->frequency);
        newNode->next = *head;
        *head = newNode;
    }
    fclose(fp);
}

void saveBookings(Booking *head) {
    FILE *fp = fopen("bookings.csv", "w");
    if (!fp) return;
    fprintf(fp, "b_id,d_id,p_id,type,dist,fare\n");
    while (head) {
        fprintf(fp, "%d,%d,%d,%d,%.2f,%.2f\n", 
                head->b_Id, head->d_Id, head->p_Id, head->v_Type, head->distance, head->fare);
        if (head->b_Id >= BookingId) BookingId = head->b_Id + 1; // Sync ID tracker
        head = head->next;
    }
    fclose(fp);
}

void loadBookings(Booking **head) {
    FILE *fp = fopen("bookings.csv", "r");
    if (!fp) return;
    char line[200];
    fgets(line, 200, fp);
    while (fgets(line, 200, fp)) {
        Booking *newB = (Booking*)malloc(sizeof(Booking));
        sscanf(line, "%d,%d,%d,%u,%f,%f", 
               &newB->b_Id, &newB->d_Id, &newB->p_Id, (unsigned int*)&newB->v_Type, 
               &newB->distance, &newB->fare);
        newB->next = *head;
        *head = newB;
        if (newB->b_Id >= BookingId) BookingId = newB->b_Id + 1;
    }
    fclose(fp);
}
// main functions
void addDriver(Driver **head, int id, char name[], int type, int x, int y) {
    Driver *newDriver = (Driver *)malloc(sizeof(Driver));
    newDriver->d_Id = id;
    strcpy(newDriver->name, name);
    newDriver->v_Type = (VehicleType)type;
    newDriver->d_Status = Free;
    newDriver->loc.x = x;
    newDriver->loc.y = y;
    newDriver->total_Earnings = 0.0;
    newDriver->next = *head;
    *head = newDriver;
}

void addPassenger(Passenger **head, int id, char name[], char mobile[]) {
    if (findPassengerByID(*head, id)) return;
    Passenger *newNode = (Passenger*)malloc(sizeof(Passenger));
    newNode->p_Id = id;
    strcpy(newNode->name, name);
    strcpy(newNode->number, mobile);
    newNode->frequency = 0;
    newNode->next = *head;
    *head = newNode;
}

Driver* findNearestVehicle(Driver *dHead, int p_x, int p_y, int prefType) {
    Driver *best = NULL;
    float minDist = __INT_MAX__;

    while (dHead) {
        if (dHead->d_Status == 0 &&
           (prefType == -1 || dHead->v_Type == prefType)) {

            float dist = calculateDistance(dHead->loc.x, dHead->loc.y, p_x, p_y);

            if (dist <= 5 && dist < minDist) {
                minDist = dist;
                best = dHead;
            }
        }
        dHead = dHead->next;
    }
    return best;
}

int requestRide(Driver *dHead, Passenger *pHead, Booking **bHead, int p_id, int p_x, int p_y,int prefType){
    int ret_val;
    if(!findPassengerByID(pHead,p_id)){
        printf(" Passenger does not exist");
        ret_val=-1;
    }
    else{
        Driver* driver=findNearestVehicle(dHead,p_x,p_y,prefType);
        if(driver == NULL) ret_val= -1;
        else{
            Booking* new=(Booking*)malloc(sizeof(Booking));
            new->b_Id=BookingId++;
            new->d_Id=driver->d_Id;
            new->p_Id=p_id;
            new->v_Type=driver->v_Type;
            new->distance=0;
            new->fare=0;
            driver->d_Status=booked;
            new->next=*bHead;
            *bHead=new;
            ret_val=new->b_Id;

            // date;
        }
        return ret_val;
    }
}

void completeRide(Driver *dHead, Passenger *pHead, Booking *bHead, int booking_id, float distance){
    Booking* booking=findBookingByID(bHead,booking_id);
    booking->distance=distance;
    float fare;
    if(booking->v_Type=Cab) fare=distance*10;
    else fare=distance*5;
    Driver* driver=findDriverByID(dHead,booking->d_Id);
    driver->total_Earnings+=fare;
    booking->fare=fare;
    Passenger* pass=findPassengerByID(pHead,booking->p_Id);
    pass->frequency=pass->frequency+1;
    driver->d_Status=Free;

    // date 
}

float calculateDriverEarnings(Driver *dHead, int d_id){
    Driver* driver=findDriverByID(dHead,d_id);
    return driver->total_Earnings;
}
void displayTopDrivers(Driver *dHead, Booking *bHead){
    dHead=mergeSortDrivers(dHead);
    int count=0;
    while(dHead && count<5){
        printf("Driver ID:%d Name:%s Earnings:%.2f\n",dHead->d_Id,dHead->name,dHead->total_Earnings);
        dHead=dHead->next;
        count++;
    }
}

void displayFrequentPairs(Driver *dHead, Passenger *pHead, Booking *bHead) {

    int maxCount = 0;
    int best_d = -1, best_p = -1;

    for (Booking *i = bHead; i != NULL; i = i->next) {
        int count = 0;

        for (Booking *j = bHead; j != NULL; j = j->next) {
            if (i->d_Id == j->d_Id && i->p_Id == j->p_Id) {
                count++;
            }
        }

        if (count > maxCount) {
            maxCount = count;
            best_d = i->d_Id;
            best_p = i->p_Id;
        }
    }

    if (maxCount == 0) {
        printf("No rides found.\n");
        return;
    }

    Driver *d = findDriverByID(dHead, best_d);
    Passenger *p = findPassengerByID(pHead, best_p);

    if (d && p) {
        printf("Most frequent pair: %s & %s (%d rides)\n",
               d->name, p->name, maxCount);
    }
}

void displayAvailableVehicles(Driver *dHead){
    while(dHead){
        if(dHead->d_Status==Free){
            printf("Id:-%d\n",dHead->d_Id);
            printf("Name:-%s\n",dHead->name);
            printf("Vehicle:-%d\n",dHead->v_Type);
            printf("Location:- %d %d\n",dHead->loc.x,dHead->loc.y);
        }
        dHead=dHead->next;
    }
}

void updateDriverLocation(Driver *dHead, int d_id, int new_x, int new_y){
    Driver* driver=findDriverByID(dHead,d_id);
    if(!driver){
        printf("Driver not found\n");
        return;
    }
    driver->loc.x=new_x;
    driver->loc.y=new_y;
    printf("Location Updated\n");
}

void deleteDriver(Driver **head, int d_id) {
    Driver *temp = *head, *prev = NULL;
    while (temp) {
        if (temp->d_Id == d_id) {
            if (temp->d_Status == 1) {
                printf("Driver is currently booked. Cannot delete.\n");
                return;
            }
            if (prev == NULL) {
                *head = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            printf("Driver deleted.\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Driver not found.\n");
}

void displayBookingHistory(Booking *bHead) {
    while (bHead) {
        printf("BookingID:%d Driver:%d Passenger:%d Distance:%.2f Fare:%.2f\n",
            bHead->b_Id, bHead->d_Id, bHead->p_Id,
            bHead->distance, bHead->fare);
        bHead = bHead->next;
    }
}


int main() {
    Driver *dHead = NULL;
    Passenger *pHead = NULL;
    Booking *bHead = NULL;

    loadDrivers(&dHead);
    loadPassengers(&pHead);
    loadBookings(&bHead);

    int choice;
    while (1) {
        printf("\n===== Ride Hailing System (CSV Enabled) =====\n");
        printf("1. Add Driver          2. Add Passenger\n");
        printf("3. Request Ride        4. Complete Ride\n");
        printf("5. Top Drivers         6. Frequent Pairs\n");
        printf("7. Available Vehicles  8. Update Location\n");
        printf("9. Delete Driver       10. History\n");
        printf("0. Save & Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 0) {
            printf("Saving data and exiting...\n");
            saveDrivers(dHead);
            savePassengers(pHead);
            saveBookings(bHead);
            break;
        }
        switch (choice) {

            case 1: {
                int id, type, x, y;
                char name[50];

                printf("Enter :- Driver ID, Name, Type (0=Cab,1=Bike), x, y: ");
                scanf("%d %s %d %d %d", &id, name, &type, &x, &y);

                addDriver(&dHead, id, name, type, x, y);
                break;
            }

            case 2: {
                int id;
                char name[50], mobile[15];

                printf("Enter Passenger ID, Name, Mobile: ");
                scanf("%d %s %s", &id, name, mobile);

                addPassenger(&pHead, id, name, mobile);
                break;
            }

            case 3: {
                int p_id, x, y, prefType;

                printf("Enter Passenger ID, Location(x y), PrefType (0=Cab,1=Bike,-1=Any): ");
                scanf("%d %d %d %d", &p_id, &x, &y, &prefType);

                int booking_id = requestRide(dHead, pHead, &bHead, p_id, x, y, prefType);

                if (booking_id == -1)
                    printf("Ride request failed.\n");
                else
                    printf("Ride booked! Booking ID: %d\n", booking_id);

                break;
            }

            case 4: {
                int booking_id;
                float distance;

                printf("Enter Booking ID and Distance travelled: ");
                scanf("%d %f", &booking_id, &distance);

                completeRide(dHead, pHead, bHead, booking_id, distance);
                printf("Ride completed.\n");
                break;
            }

            case 5:
                displayTopDrivers(dHead, bHead);
                break;

            case 6:
                displayFrequentPairs(dHead, pHead, bHead);
                break;

            case 7:
                displayAvailableVehicles(dHead);
                break;

            case 8: {
                int id, x, y;
                printf("Enter Driver ID and new location (x y): ");
                scanf("%d %d %d", &id, &x, &y);

                updateDriverLocation(dHead, id, x, y);
                break;
            }

            case 9: {
                int id;
                printf("Enter Driver ID to delete: ");
                scanf("%d", &id);

                deleteDriver(&dHead, id);
                break;
            }

            case 10:
                displayBookingHistory(bHead);
                break;

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}