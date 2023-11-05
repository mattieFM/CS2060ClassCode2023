#include <stdio.h>
#include <stdbool.h>

// Constants for rental property information
unsigned int const MIN_RENTAL_NIGHTS = 1;/*The difference between unsigned int const and signed int
const lies in the range of values they can represent and how they handle negative and positive numbers:
    Unsigned int is an integer data type that represents non-negative whole numbers.It cannot represent negative numbers.
        Signed int is an integer data type that can represent both positive and negative whole numbers.
*/

unsigned int const MAX_RENTAL_NIGHTS = 14;
unsigned int const INTERVAL_1_NIGHTS = 3;
unsigned int const INTERVAL_2_NIGHTS = 6;
double const RENTAL_RATE = 400.00;
double const DISCOUNT = 50.00;

// Function prototypes -  they kind of act as an interface from java
void printRentalPropertyInfo(unsigned int minNights, unsigned int maxNights, unsigned int interval1Nights, unsigned int interval2Nights, double rate, double discount);
int getValidInt(int min, int max);
double calculateCharges(unsigned int nights, unsigned int interval1Nights, unsigned int interval2Nights, double rate, double discount);
void printNightsCharges(unsigned int nights, double charges);

int main() {
    //declare variables
    unsigned int totalNights = 0;
    double totalCharges = 0.0;
    int isFirstInput = 1; // Flag to track if this is the first input
    bool shouldContinue = true;



    /*Print rental property information - printRentalPropertyInfo() under main will be used to
  print out the desired strings */

    printRentalPropertyInfo(MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, INTERVAL_1_NIGHTS, INTERVAL_2_NIGHTS, RENTAL_RATE, DISCOUNT);



    while (shouldContinue) {
        /* Get the number of nights from the user - getValidInt() under main will be used to find a vaild int
  that fits the assaignment details, this is also where i will make sure that the code will not
  stop running fro some random reason such as a -1 or 000 being entered. */

        int nights = getValidInt(MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS);

        if (isFirstInput && nights == -1) {
            // Handles the first input being -1 if first input is -1
            printf("There were no rentals.\n");
            shouldContinue = false; //if this was a break nothing after this would exictue
        }
        else {
            isFirstInput = 0; /* Set the flag to false after the first input, the flag willkeep track of whether this is the first input entered.
               If the first input is -1, the program will immediately display "There were no rentals" and exit the loop.
               After the first input, the isFirstInput flag is set to 0 (false), so the program will not consider -1
               as a special case sense it will no longer be entered first
            */

            if (nights == -1) {
                // Print the total nights and charges when -1 is entered from every entree while the while loop was running
                printf("\nTotal Nights: %u\nTotal Charges: $%.2lf\n", totalNights, totalCharges);
                shouldContinue = false; // break
            } else {
                // Calculate charges and print results
                double charges = calculateCharges(nights, INTERVAL_1_NIGHTS, INTERVAL_2_NIGHTS, RENTAL_RATE, DISCOUNT);
                printNightsCharges(nights, charges);

                // Update total nights and charges to fit the latest total
                totalNights += nights;
                totalCharges += charges;
            }


        }


    }

    return 0;
}

void printRentalPropertyInfo(unsigned int minNights, unsigned int maxNights, unsigned int interval1Nights, unsigned int interval2Nights, double rate, double discount) {
    printf("Rental Property can be rented for %u to %u nights.\n", minNights, maxNights);
    printf("$%.2lf rate a night for the first %u nights\n", rate, interval1Nights);
    printf("$%.2lf discount rate a night for nights %u to %u\n", discount, interval1Nights + 1, interval2Nights);
    printf("$%.2lf discount rate a night for each remaining night over %u.\n", discount * 2, interval2Nights);
}

int getValidInt(int min, int max) {
    int input;
    char buffer[256]; // Buffer to read input as a string
    while (1) {
        printf("Enter the number of nights you want to rent the property: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            // if error reading input acurs print statemtn bellow
            printf("Error reading input. Please try again.\n");
            continue;
        }
        if (sscanf(buffer, "%d", &input) != 1) {
            // if invalid input (not an integer) is entered then print statement bellow
            printf("Error: Not an integer number. Please enter the value again.\n");
            continue;
        }
        if (input == -1) {
            // will exit loop when -1 is entered
            break;
        }
        if (input < min || input > max) {
            // if input is out of range of the min and max nights then print this statement
            printf("Error: Not within %d and %d. Please enter the value again.\n", min, max);
        }
        else {
            // Valid input was entered, break out of the loop
            break;
        }
    }
    return input;
}

double calculateCharges(unsigned int nights, unsigned int interval1Nights, unsigned int interval2Nights, double rate, double discount) {
    double totalCharge = 0.0;
    if (nights <= interval1Nights) {
        totalCharge = rate * nights;//this is the flat rate before getting to the 4th night discount
    }
    else if (nights <= interval2Nights) {
        totalCharge = (rate * interval1Nights) + ((nights - interval1Nights) * (rate - discount));
        //this is for the night 4-6 50$ discount
    }
    else {
        totalCharge = (rate * interval1Nights) + ((interval2Nights - interval1Nights) * (rate - discount)) + ((nights - interval2Nights) * (rate - discount * 2));
        //this is with all of the discounts after acheiving 7 or more nights
    }
    return totalCharge;
}

void printNightsCharges(unsigned int nights, double charges) { //strings to show the finale results
    printf("\nRental Charges\n\n");
    printf("Nights          Charge\n");
    printf("%-5u          $%.2lf\n", nights, charges);
}