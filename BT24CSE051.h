#ifndef BT24CSE051_H
#define BT24CSE051_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>     
#include <math.h>

static int BookingId=101;

typedef enum VehicleType{
    Cab,
    Bike
} VehicleType;

typedef enum DriverStatus{
    Free,
    booked
} DriverStatus;

typedef struct Location_tag{
    int x;
    int y;
} Location;

typedef struct Driver_tag{
    int d_Id;
    char name[50];
    VehicleType v_Type;
    DriverStatus d_Status;
    Location loc;
    float total_Earnings;
    struct Driver_tag* next;
}Driver;

typedef struct Passenger_tag{
    int p_Id;
    char name[50];
    char number[10];
    int frequency;
    struct Passenger_tag* next;
}Passenger;

typedef struct Booking_tag{
    int b_Id;
    int p_Id;
    int d_Id;
    VehicleType v_Type;
    float distance;
    int date;
    float fare;
    struct Booking_tag* next;
}Booking;

void addDriver(Driver **head, int id, char name[], int type, int x, int y);
void addPassenger(Passenger **head, int id, char name[], char mobile[]);
Driver* findNearestVehicle(Driver *dHead, int p_x, int p_y, int prefType);
int requestRide(Driver *dHead, Passenger *pHead, Booking **bHead, int p_id, int p_x, int p_y,int prefType);
void completeRide(Driver *dHead, Passenger *pHead, Booking *bHead, int booking_id, float distance);
float calculateDriverEarnings(Driver *dHead, int d_id);
void displayTopDrivers(Driver *dHead, Booking *bHead);
void displayFrequentPairs(Driver *dHead, Passenger *pHead, Booking *bHead);
void displayAvailableVehicles(Driver *dHead);
void updateDriverLocation(Driver *dHead, int d_id, int new_x, int new_y);
void deleteDriver(Driver **dHead, int d_id);
void displayBookingHistory(Booking *bHead);

// helpers
Passenger* findPassengerByID(Passenger *pHead, int p_id);
Driver* findDriverByID(Driver *dHead, int d_id);
Booking* findBookingByID(Booking *bHead, int b_id);
float calculateDistance(int x1, int y1, int x2, int y2);

// MergeSort
Driver* split(Driver* head);
Driver* merge(Driver* a, Driver* b);
Driver* mergeSortDrivers(Driver* head);

// File handling
void saveDrivers(Driver *head);
void loadDrivers(Driver **head);
void savePassengers(Passenger *head);
void loadPassengers(Passenger **head);
void saveBookings(Booking *head);
void loadBookings(Booking **head);
#endif