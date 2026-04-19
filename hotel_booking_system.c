#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ---- Constants ---- */
#define MAX_ROOMS     10
#define FILENAME      "hotel_data.txt"
#define DISCOUNT_RATE 0.10
#define DISCOUNT_DAYS 7

/* ---- Room Structure ---- */
struct Room {
    int   room_number;
    char  type[20];
    float price;
    int   is_booked;
    char  guest_name[50];
    int   days;
};

struct Room rooms[MAX_ROOMS];

/* ---- Utility Functions ---- */
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

/* ---- Initialize Rooms ---- */
void initRooms() {
    int numbers[MAX_ROOMS] = {101,102,103,104,105,201,202,203,301,302};
    char *types[MAX_ROOMS] = {"AC","AC","AC","AC","AC",
                             "Non-AC","Non-AC","Non-AC","Non-AC","Non-AC"};
    float prices[MAX_ROOMS] = {2500,2500,2500,2500,2500,
                              1200,1200,1200,1200,1200};

    for (int i = 0; i < MAX_ROOMS; i++) {
        rooms[i].room_number = numbers[i];
        strcpy(rooms[i].type, types[i]);
        rooms[i].price = prices[i];
        rooms[i].is_booked = 0;
        strcpy(rooms[i].guest_name, "");
        rooms[i].days = 0;
    }
}

/* ---- Display Rooms ---- */
void displayRooms() {
    printf("\n========================================================\n");
    printf("  Room No  |   Type   |  Price/Night  |    Status      \n");
    printf("========================================================\n");

    for (int i = 0; i < MAX_ROOMS; i++) {
        printf("   %-8d| %-9s| Rs %-10.0f| %s\n",
               rooms[i].room_number,
               rooms[i].type,
               rooms[i].price,
               rooms[i].is_booked ? "BOOKED" : "Available");
    }

    printf("========================================================\n");
}

/* ---- Find Room ---- */
int findRoom(int room_no) {
    for (int i = 0; i < MAX_ROOMS; i++) {
        if (rooms[i].room_number == room_no)
            return i;
    }
    return -1;
}

/* ---- Book Room ---- */
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
        printf("\n[ERROR] Room already booked!\n");
        return;
    }

    getchar();
    printf("Enter Guest Name        : ");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Enter Number of Days    : ");
    scanf("%d", &days);

    if (days <= 0) {
        printf("\n[ERROR] Days must be > 0\n");
        return;
    }

    rooms[idx].is_booked = 1;
    strcpy(rooms[idx].guest_name, name);
    rooms[idx].days = days;

    printf("\n[SUCCESS] Room booked successfully!\n");
}

/* ---- Cancel Booking ---- */
void cancelBooking() {
    int room_no, idx;

    printf("\nEnter Room Number to Cancel: ");
    scanf("%d", &room_no);

    idx = findRoom(room_no);

    if (idx == -1) {
        printf("\n[ERROR] Room does not exist!\n");
        return;
    }
    if (!rooms[idx].is_booked) {
        printf("\n[ERROR] Room is not booked!\n");
        return;
    }

    printf("\nCancelling booking for %s...\n", rooms[idx].guest_name);

    rooms[idx].is_booked = 0;
    strcpy(rooms[idx].guest_name, "");
    rooms[idx].days = 0;

    printf("[SUCCESS] Booking cancelled!\n");
}

/* ---- Generate Bill ---- */
void generateBill() {
    int room_no, idx;
    float total, discount, final;

    printf("\nEnter Room Number for Bill: ");
    scanf("%d", &room_no);

    idx = findRoom(room_no);

    if (idx == -1 || !rooms[idx].is_booked) {
        printf("\n[ERROR] Invalid room!\n");
        return;
    }

    total = rooms[idx].price * rooms[idx].days;

    printf("\n============================================\n");
    printf("             HOTEL BILL RECEIPT             \n");
    printf("============================================\n");
    printf("  Guest Name  : %s\n", rooms[idx].guest_name);
    printf("  Room Number : %d\n", rooms[idx].room_number);
    printf("  Room Type   : %s\n", rooms[idx].type);
    printf("  Days Stayed : %d\n", rooms[idx].days);
    printf("  Rate/Night  : Rs %.0f\n", rooms[idx].price);
    printf("--------------------------------------------\n");
    printf("  Subtotal    : Rs %.2f\n", total);

    if (rooms[idx].days > DISCOUNT_DAYS) {
        discount = total * DISCOUNT_RATE;
        final = total - discount;
        printf("  Discount(10%%): - Rs %.2f\n", discount);
        printf("--------------------------------------------\n");
        printf("  TOTAL AMOUNT : Rs %.2f\n", final);
    } else {
        printf("--------------------------------------------\n");
        printf("  TOTAL AMOUNT : Rs %.2f\n", total);
    }

    printf("============================================\n");
}

/* ---- View Booking ---- */
void viewBooking() {
    int room_no, idx;

    printf("\nEnter Room Number to View: ");
    scanf("%d", &room_no);

    idx = findRoom(room_no);

    if (idx == -1 || !rooms[idx].is_booked) {
        printf("\n[INFO] Room not booked.\n");
        return;
    }

    printf("\n--- Booking Details ---\n");
    printf("Room Number : %d\n", rooms[idx].room_number);
    printf("Guest Name  : %s\n", rooms[idx].guest_name);
    printf("Days        : %d\n", rooms[idx].days);
}

/* ---- Summary ---- */
void roomSummary() {
    int booked = 0;

    for (int i = 0; i < MAX_ROOMS; i++)
        if (rooms[i].is_booked) booked++;

    printf("\n--- Room Summary ---\n");
    printf("Total Rooms    : %d\n", MAX_ROOMS);
    printf("Booked Rooms   : %d\n", booked);
    printf("Available Rooms: %d\n", MAX_ROOMS - booked);
}

/* ---- Save ---- */
void saveToFile() {
    FILE *fp = fopen(FILENAME, "w");

    if (!fp) return;

    for (int i = 0; i < MAX_ROOMS; i++) {
        fprintf(fp, "%d|%s|%.0f|%d|%s|%d\n",
                rooms[i].room_number,
                rooms[i].type,
                rooms[i].price,
                rooms[i].is_booked,
                rooms[i].guest_name,
                rooms[i].days);
    }

    fclose(fp);
}

/* ---- Load ---- */
void loadFromFile() {
    FILE *fp = fopen(FILENAME, "r");
    char line[200];
    int i = 0;

    if (!fp) return;

    while (fgets(line, sizeof(line), fp) && i < MAX_ROOMS) {
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
}

/* ---- Menu ---- */
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
    printf("|    7. Exit                   |\n");
    printf("|==============================|\n");
    printf("  Enter your choice: ");
}

/* ---- MAIN ---- */
int main() {
    int choice;

    initRooms();
    loadFromFile();

    while (1) {
        clearScreen();
        printMenu();
        scanf("%d", &choice);

        clearScreen();

        switch (choice) {
            case 1: displayRooms(); break;
            case 2: bookRoom(); break;
            case 3: cancelBooking(); break;
            case 4: generateBill(); break;
            case 5: viewBooking(); break;
            case 6: roomSummary(); break;

            case 7:
                saveToFile();
                printf("\nThank you! Goodbye.\n");
                return 0;

            default:
                printf("\n[ERROR] Invalid choice. Enter 1-7.\n");
        }

        pauseScreen();
    }
}