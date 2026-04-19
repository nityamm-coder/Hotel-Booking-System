/*
 * ============================================================
 *        HOTEL BOOKING SYSTEM - Mini Project (B.Tech)
 *        Language : C
 *        Features : Room Booking, Cancellation, Bill with
 *                   Discount, File Storage (save/load)
 * ============================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ---- Constants ---- */
#define MAX_ROOMS     10
#define FILENAME      "hotel_data.txt"
#define DISCOUNT_RATE 0.10   /* 10% discount for stays > 7 days */
#define DISCOUNT_DAYS 7

/* ---- Room Structure ---- */
struct Room {
    int   room_number;
    char  type[20];       /* "AC" or "Non-AC"    */
    float price;          /* price per night (Rs) */
    int   is_booked;      /* 0 = free, 1 = booked */
    char  guest_name[50];
    int   days;
};

/* Global array of rooms */
struct Room rooms[MAX_ROOMS];

/* ================================================================
   FUNCTION: initRooms
   Sets up the 10 default rooms with room number, type, and price.
   Called once at the start.
   ================================================================ */
void initRooms() {
    int i;
    int numbers[MAX_ROOMS] = {101, 102, 103, 104, 105,
                               201, 202, 203, 301, 302};
    char *types[MAX_ROOMS] = {"AC","AC","AC","AC","AC",
                               "Non-AC","Non-AC","Non-AC","Non-AC","Non-AC"};
    float prices[MAX_ROOMS] = {2500, 2500, 2500, 2500, 2500,
                                1200, 1200, 1200, 1200, 1200};

    for (i = 0; i < MAX_ROOMS; i++) {
        rooms[i].room_number = numbers[i];
        strcpy(rooms[i].type, types[i]);
        rooms[i].price      = prices[i];
        rooms[i].is_booked  = 0;
        strcpy(rooms[i].guest_name, "");
        rooms[i].days = 0;
    }
}

/* ================================================================
   FUNCTION: displayRooms
   Prints all rooms in a neat table with status.
   ================================================================ */
void displayRooms() {
    int i;
    printf("\n");
    printf("========================================================\n");
    printf("  Room No  |   Type   |  Price/Night  |    Status      \n");
    printf("========================================================\n");
    for (i = 0; i < MAX_ROOMS; i++) {
        char *status = rooms[i].is_booked ? "BOOKED" : "Available";
        printf("   %-8d| %-9s| Rs %-10.0f| %s\n",
               rooms[i].room_number,
               rooms[i].type,
               rooms[i].price,
               status);
    }
    printf("========================================================\n");
}

/* ================================================================
   FUNCTION: findRoom (Helper)
   Searches for a room by room number.
   Returns the index if found, -1 if not found.
   ================================================================ */
int findRoom(int room_no) {
    int i;
    for (i = 0; i < MAX_ROOMS; i++) {
        if (rooms[i].room_number == room_no)
            return i;
    }
    return -1;
}

/* ================================================================
   FUNCTION: bookRoom
   Lets a guest book an available room.
   Asks for: room number, guest name, number of days.
   ================================================================ */
void bookRoom() {
    int room_no, idx, days;
    char name[50];

    displayRooms();

    printf("\nEnter Room Number to Book: ");
    scanf("%d", &room_no);

    idx = findRoom(room_no);

    if (idx == -1) {
        printf("\n[ERROR] Room %d does not exist!\n", room_no);
        return;
    }
    if (rooms[idx].is_booked) {
        printf("\n[ERROR] Room %d is already booked by %s.\n",
               room_no, rooms[idx].guest_name);
        return;
    }

    /* Clear leftover newline before reading string */
    getchar();
    printf("Enter Guest Name        : ");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = '\0';  /* remove trailing newline */

    printf("Enter Number of Days    : ");
    scanf("%d", &days);

    if (days <= 0) {
        printf("\n[ERROR] Days must be greater than 0.\n");
        return;
    }

    /* Save details into the rooms array */
    rooms[idx].is_booked = 1;
    strcpy(rooms[idx].guest_name, name);
    rooms[idx].days = days;

    printf("\n[SUCCESS] Room %d booked for %s (%d days).\n",
           room_no, name, days);
}

/* ================================================================
   FUNCTION: cancelBooking
   Cancels an existing booking and frees the room.
   ================================================================ */
void cancelBooking() {
    int room_no, idx;

    printf("\nEnter Room Number to Cancel: ");
    scanf("%d", &room_no);

    idx = findRoom(room_no);

    if (idx == -1) {
        printf("\n[ERROR] Room %d does not exist!\n", room_no);
        return;
    }
    if (!rooms[idx].is_booked) {
        printf("\n[ERROR] Room %d is not booked.\n", room_no);
        return;
    }

    printf("\n[INFO] Cancelling booking for %s in Room %d...\n",
           rooms[idx].guest_name, room_no);

    /* Reset room to free state */
    rooms[idx].is_booked = 0;
    strcpy(rooms[idx].guest_name, "");
    rooms[idx].days = 0;

    printf("[SUCCESS] Booking cancelled. Room %d is now available.\n", room_no);
}

/* ================================================================
   FUNCTION: generateBill
   Calculates and prints the bill for a booked room.
   Applies 10% discount if stay is more than 7 days.
   ================================================================ */
void generateBill() {
    int room_no, idx;
    float total, discount_amount, final_amount;

    printf("\nEnter Room Number for Bill: ");
    scanf("%d", &room_no);

    idx = findRoom(room_no);

    if (idx == -1) {
        printf("\n[ERROR] Room %d does not exist!\n", room_no);
        return;
    }
    if (!rooms[idx].is_booked) {
        printf("\n[ERROR] Room %d has no active booking.\n", room_no);
        return;
    }

    total = rooms[idx].price * rooms[idx].days;

    printf("\n");
    printf("============================================\n");
    printf("             HOTEL BILL RECEIPT             \n");
    printf("============================================\n");
    printf("  Guest Name  : %s\n",   rooms[idx].guest_name);
    printf("  Room Number : %d\n",   rooms[idx].room_number);
    printf("  Room Type   : %s\n",   rooms[idx].type);
    printf("  Days Stayed : %d\n",   rooms[idx].days);
    printf("  Rate/Night  : Rs %.0f\n", rooms[idx].price);
    printf("--------------------------------------------\n");
    printf("  Subtotal    : Rs %.2f\n", total);

    /* Apply discount if stay is more than DISCOUNT_DAYS */
    if (rooms[idx].days > DISCOUNT_DAYS) {
        discount_amount = total * DISCOUNT_RATE;
        final_amount    = total - discount_amount;
        printf("  Discount(10%%): - Rs %.2f   [Stay > 7 days!]\n",
               discount_amount);
        printf("--------------------------------------------\n");
        printf("  TOTAL AMOUNT : Rs %.2f\n", final_amount);
    } else {
        printf("--------------------------------------------\n");
        printf("  TOTAL AMOUNT : Rs %.2f\n", total);
        printf("  (Stay > 7 days for 10%% discount)\n");
    }

    printf("============================================\n");
    printf("      Thank you for staying with us!        \n");
    printf("============================================\n");
}

/* ================================================================
   FUNCTION: viewBooking
   Shows the details of one specific booked room.
   ================================================================ */
void viewBooking() {
    int room_no, idx;

    printf("\nEnter Room Number to View: ");
    scanf("%d", &room_no);

    idx = findRoom(room_no);

    if (idx == -1) {
        printf("\n[ERROR] Room %d does not exist!\n", room_no);
        return;
    }
    if (!rooms[idx].is_booked) {
        printf("\n[INFO] Room %d is currently available (not booked).\n", room_no);
        return;
    }

    printf("\n--- Booking Details ---\n");
    printf("  Room Number : %d\n",   rooms[idx].room_number);
    printf("  Room Type   : %s\n",   rooms[idx].type);
    printf("  Guest Name  : %s\n",   rooms[idx].guest_name);
    printf("  Days Booked : %d\n",   rooms[idx].days);
    printf("  Price/Night : Rs %.0f\n", rooms[idx].price);
}

/* ================================================================
   FUNCTION: roomSummary
   Shows a quick count of available vs booked rooms.
   ================================================================ */
void roomSummary() {
    int i, booked = 0, available = 0;
    for (i = 0; i < MAX_ROOMS; i++) {
        if (rooms[i].is_booked) booked++;
        else available++;
    }
    printf("\n--- Room Summary ---\n");
    printf("  Total Rooms    : %d\n", MAX_ROOMS);
    printf("  Booked Rooms   : %d\n", booked);
    printf("  Available Rooms: %d\n", available);
}

/* ================================================================
   FUNCTION: saveToFile
   Saves all room data to "hotel_data.txt" so data is not lost
   when the program exits.
   ================================================================ */
void saveToFile() {
    FILE *fp;
    int i;

    fp = fopen(FILENAME, "w");  /* "w" = open for writing */
    if (fp == NULL) {
        printf("\n[ERROR] Could not save data to file.\n");
        return;
    }

    for (i = 0; i < MAX_ROOMS; i++) {
        fprintf(fp, "%d|%s|%.0f|%d|%s|%d\n",
                rooms[i].room_number,
                rooms[i].type,
                rooms[i].price,
                rooms[i].is_booked,
                rooms[i].guest_name,
                rooms[i].days);
    }

    fclose(fp);  /* Always close the file after writing */
    printf("\n[SUCCESS] Data saved to '%s'.\n", FILENAME);
}

/* ================================================================
   FUNCTION: loadFromFile
   Loads room data from "hotel_data.txt" at program start.
   If file doesn't exist, uses default room data.
   ================================================================ */
void loadFromFile() {
    FILE *fp;
    int i = 0;
    char line[200];

    fp = fopen(FILENAME, "r");  /* "r" = open for reading */
    if (fp == NULL) {
        /* File doesn't exist yet — first time running */
        printf("[INFO] No saved data found. Starting fresh.\n");
        return;
    }

    while (fgets(line, sizeof(line), fp) && i < MAX_ROOMS) {
        /* Parse each line using sscanf with '|' delimiter */
        sscanf(line, "%d|%19[^|]|%f|%d|%49[^|]|%d",
               &rooms[i].room_number,
               rooms[i].type,
               &rooms[i].price,
               &rooms[i].is_booked,
               rooms[i].guest_name,
               &rooms[i].days);
        i++;
    }

    fclose(fp);
    printf("[INFO] Previous data loaded from '%s'.\n", FILENAME);
}

/* ================================================================
   FUNCTION: printMenu
   Displays the main menu options.
   ================================================================ */
void printMenu() {
    printf("\n");
    printf("|==============================|\n");
    printf("|     HOTEL BOOKING SYSTEM     |\n");
    printf("|==============================|\n");
    printf("|    1. View All Rooms         |\n");
    printf("|    2. Book a Room            |\n");
    printf("|    3. Cancel Booking         |\n");
    printf("|    4. Generate Bill          |\n");
    printf("|    5. View Booking Details   |\n");
    printf("|    6. Room Summary           |\n");
    printf("|    7. Save Data              |\n");
    printf("|    8. Exit                   |\n");
    printf("|==============================|\n");
    printf("  Enter your choice: ");
}

/* ================================================================
   MAIN FUNCTION
   Entry point: loads data, shows menu, runs chosen option in loop.
   ================================================================ */
int main() {
    int choice;

    /* Step 1: Set up default rooms */
    initRooms();

    /* Step 2: Load any previously saved bookings from file */
    loadFromFile();

    /* Step 3: Main menu loop — keeps running until user exits */
    while (1) {
        printMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayRooms();  break;
            case 2: bookRoom();      break;
            case 3: cancelBooking(); break;
            case 4: generateBill();  break;
            case 5: viewBooking();   break;
            case 6: roomSummary();   break;
            case 7: saveToFile();    break;
            case 8:
                saveToFile();  /* Auto-save on exit */
                printf("\nThank you! Goodbye.\n");
                return 0;
            default:
                printf("\n[ERROR] Invalid choice. Enter 1-8.\n");
        }
    }

    return 0;
}