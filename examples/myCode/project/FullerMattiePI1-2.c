/**
@auther: Mattie Fuller
Class: CS2060-002 
Project: Project Iteration: 1
Description: This is a project that will handle "aribnb" style bookings for a fictional renting service "airuccs." This is the first iteration and will have more functionality built
upon it as time goes on and the "customer" adds more requirements in the form of user stories.

Documentation style: comments are following doxgen C standard as I have found this is the most common style that tends to be used, let me know if you would perfer a differnt style.

Security Standard: SEI CERT C Standard

@noteToGrader: we covered arrays in the lecture right before this was due, so I should be good to use them, right?
*/

/*-------------------------------------------------------------
 Includes
 ------------------------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>
#include <gtk/gtk.h>


/*-------------------------------------------------------------
 Prototypes
 ------------------------------------------------------------*/

/**
 Breif:
 Get a int that is either within [min-max] or in a list of sentinal values, from the user, prompting them with the prompt passed
 into this function and displaying the err prompt if they enter an invalid input.

 @Author MattieFM

 @param prompt {char[]} a string like array of chars to display to the user as a prompt
 @param err {char[]} a string like array to display to the user when they enter an invalid value,
 @param minAsciiCode {signed int} the lowest ascii code allowed to be entered
 @param maxAsciiCode {signed int} the highest ascii code allowed to be entered
 @param sentinalValues {signed int[]} an array of explicitly allowed ascii codes
 @param length, {size_t} the length of the sentinal_values array

 @extends getValidSignedInt

 @returns a signed int between [min-max]

*/
int getValidIntInputWithSentinals(char const PROMPT[], int min, int max, int const SENTINAL_VALUES[], size_t length);

/**
 Breif:
 A function that will calculate the discount of a day

 @param night {int} the night to find the discount of
 @param discountRanges[] {int[]) an array of ints in the format [discountRange1Start,discountRange1End,DiscountRange2End,...) where the start of each range after 1 is the end of the last
 @param discounts[] {floats} an array of the discounts for each range
 @param length, {size_t} the length of both the discountRRanges and discounts array, they must be same length

 @returns double, a dollar amount of the discount for this night
*/
double calculateDiscount(int night, int const DISCOUNT_RANGES[], double const DISCOUNTS[], size_t length);

/**
 Breif:
 A function to calculate the cost of a stay in a AirUCCS rented building. Cost is based on nights stayed, the base price per night and the discounts applied.
 
 @param nights {int} how many nights the user has stayed
 @param basePrice {float} the base price per night before any discounts are applied.
 @param discountRanges[] {int[]) an array of ints in the format [discountRange1Start,discountRange1End,DiscountRange2End,...) where the start of each range after 1 is the end of the last
 @param discounts[] {floats} an array of the discounts for each range
 @param length, {size_t} the length of both the discountRRanges and discounts array, they must be same length

 @returns {double} a double value represnting how much the stay would cost.
*/
double calculateCost(int nights, double basePrice, int const DISCOUNT_RANGES[], double const DISCOUNTS[], size_t length);

/**
 Breif:
 A function to display how much the user's stay cost in a user friendly way

 @param nights {int} how many nights the user has stayed
 @param cost {double} the total cost of the user's stay

 @returns nothing, prints info to the console for the user. in the format:
 Nights          Charge
 5               $nnnn.nn

*/
void displayCost(int nights, double cost);

/**
 brief:
 A function that will start the process of booking a room. 

 @param valuePt a pointer to a variable to store the value the user entered.
 @param sentinals[] an array of sentinal values that are allowed
 @param min, the min value for nights allowed
 @param max, the max value for nights allowed
 @param length, {size_t} the length of the sentinal_values array
 
 @returns bool, this will return true if the user has made a valid booking and false if they have entered a sentinal value.
*/
bool getBooking(int* valuePt, int const SENTINALS[], int min, int max, size_t length);

/**
 Brief: check if an array of ints contains a value.

 @param ARR[] the array to look through
 @param val the value to check for
 @param length, {size_t} the length of the array

 @returns bool, true if value is in arr false if not.

*/
bool intArrContains(int const ARR[], int val, size_t length);

/**
 Brief: display the rental propery info to the user before they are prompted to make a purchace.

 @param min, the minimum number of nights a user can stay
 @param max the maximum number of nights a user can stay
 @param basePrice the base price per night of the property
 @param DISCOUNT_RANGES[] {int[]) an array of ints in the format [discountRange1Start,discountRange1End,DiscountRange2End,...) where the start of each range after 1 is the end of the last
 @param DISCOUNTS[] {double[]} an array of the discounts for each range,
 @param length, {size_t} the length of both the discountRRanges and discounts array, they must be same length

*/
void displayRentalProperyInfo(int min, int max, double basePrice, int const DISCOUNT_RANGES[], double const DISCOUNTS[], size_t length);


/*-------------------------------------------------------------
 Function Declarations
 ------------------------------------------------------------*/


/**
@brief the main function for the program that will handle initalization/entry.
@author MattieFM
*/
int main(void) {
    // -Constants Delcarations-
    int const SENTINAL_NEG1 = -1;
    int unsigned const MIN_RENTAL_NIGHTS = 1;
    unsigned int const MAX_RENTAL_NIGHTS = 14;
    unsigned int const INTERVAL_1_NIGHTS = 3;
    unsigned int const INTERVAL_2_NIGHTS = 6;
    double const RENTAL_RATE = 400;
    double const DISCOUNT = 50;

    //define our config arrays, these are a bit overkill at this stage but will likely be useful later.
    unsigned int const DISCOUNT_RANGES[] = {0, INTERVAL_1_NIGHTS, INTERVAL_2_NIGHTS };
    double const DISCOUNTS[] = {0, DISCOUNT, DISCOUNT*2}; //with something like this discounts are usually picked by hand to be just right, rather than based on a curve.
    int const SENTINALS[] = { SENTINAL_NEG1 };
    // -Constants Ends-

    //find the size of both our arrays by checking the total bit size of the array then dividing it by the bit size of the first member.
    size_t discountsLen = sizeof(DISCOUNTS) / sizeof(DISCOUNTS[0]); //get len with basic bit math
    size_t sentinalLen = sizeof(SENTINALS) / sizeof(SENTINALS[0]); //get len with basic bit math

    //these vars will track total rev/nights
    double totalCost = 0;
    int totalNights = 0;

    bool shouldContinue = true;

    //main loop
    while (shouldContinue) {
        int userInputVal = 0;
        //display info to user
        displayRentalProperyInfo(MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, RENTAL_RATE, DISCOUNT_RANGES, DISCOUNTS, discountsLen);

        //get input from user, storing their input into the userinputval and whether they entered a sentinal val into the isSentinal var
        bool isSentinal = !getBooking(&userInputVal, SENTINALS, MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, sentinalLen);

        if (isSentinal) { //if a sentinal value was entered
            int sentinalVal = userInputVal; //create a new variable for specificity sake
            switch (sentinalVal)
            {
            case -1: //we would have to use preprocessor define macro to use a const here
                //end our loop and display owner information
                shouldContinue = false;
                printf("%s", "-Rental Owner Summery-\n");
                displayCost(totalNights, totalCost);
                break;

            default:
                break;
            }
        }
        else { //if a sentinal value was not entered
            int nightsStayed = userInputVal; //create a new variable for clarity that this is what this variable contains
            double cost = calculateCost(nightsStayed, RENTAL_RATE, DISCOUNT_RANGES, DISCOUNTS, discountsLen);
            totalCost += cost;
            totalNights += nightsStayed;
            displayCost(nightsStayed, cost);
        }
    }
}

bool intArrContains(int const ARR[], int val, size_t length) {
    bool returnValue = false;
    for (size_t i = 0; i < length; i++)
    {
        if (ARR[i] == val) {
            returnValue = true; //we should just return here to not waste cpu cycles, but w/e
        }
    }
    return returnValue;
}

bool getBooking(int* valuePt, int const SENTINALS[], int min, int max, size_t length) {
    bool returnVal = true;
    char const prompt[] = "Please enter the number of nights you would like to stay:";
    int val = getValidIntInputWithSentinals(prompt, min, max, SENTINALS, length);

    if (intArrContains(SENTINALS, val, length)) {
        //if the user entered a sentinal value set the return value to false and edit the sentinal val pointer
        returnVal = false;
    }

    *valuePt = val;


    return returnVal;
}

//note, we could use macros to make this a generic-like function so that we could reuse it for all types. see below:
// in header file
// TYPE getValidIn(char prompt[], char err[], TYPE min, TYPE max)
// out of header file
// #define TYPE int
// #include header.h
// #undef TYPE
// then repeate for all the types we would want, I am aware of this, but we have not covered this and macros are a bit questionable anyways
int getValidIntInputWithSentinals(char const PROMPT[], int min, int max, int const SENTINAL_VALUES[], size_t length) {
    //define our prompts as constants 
    char const RANGE_ERR[] = "\nError: not within %d and %d. Please enter the value again:";
    char const TYPE_ERR[] = "\nError: not an integer. Please enter the value again:";


    bool validInput = false;
    int userInput = min - 1;

    printf("%s", PROMPT);

    //loop while we dont have valid input
    while (!validInput) {
        bool input = false;
        input = scanf("%d", &userInput);
        if (input) {
            //if input is valid check if it is in range if so end and return
            if (userInput >= min && userInput <= max) {
                validInput = true;
            }
            else if (intArrContains(SENTINAL_VALUES, userInput, length)) {
                //if the value is a sentinal value it is also valid
                validInput = true; 
            }
            else { //out of range
                printf(RANGE_ERR, min, max);
            }
        }
        else { //not an integer
            printf("%s", TYPE_ERR);
        }

        while (getchar() != '\n'); //clear buffer so there is no leftover data.
    }


    return userInput;
}


void displayRentalProperyInfo(int min, int max, double basePrice, int const DISCOUNT_RANGES[], double const DISCOUNTS[], size_t length) {
    //display header info
    printf("%s", "-Rental Propterty Info-\n");
    printf("Rental Property can be rented for %d to %d nights.\n", min, max);
    printf("$%.2f rate a night for first 3 nights.\n", basePrice);

    //display mid ranges
    for (size_t i = 1; i < length -1; i++) //skip last index since we handle that below
    {
        int rangeMin = DISCOUNT_RANGES[i]+1;//min is exlusive so add one to make it inclusive
        int rangeMax = DISCOUNT_RANGES[i+1];
        printf("$%.2f rate a night for nights %d to %d.\n", DISCOUNTS[i], rangeMin, rangeMax);
    }

    //display final range
    printf("$%.2f rate a night for nights over %d.\n\n", DISCOUNTS[length -1], DISCOUNT_RANGES[length -1]);
}

double calculateDiscount(int night, int const DISCOUNT_RANGES[], double const DISCOUNTS[], size_t length) {
    double discount = 0;

    //get the last day of the last range
    int lastDay = DISCOUNT_RANGES[length - 1];

    //loop through our array finding the discount
    if (night < lastDay) { //if not the last discount
        for (size_t i = 0; i < length - 1; i++)
        {
            //loop through our array of discounts checking if the night is in the range.
            int rangeMin = DISCOUNT_RANGES[i];
            int rangeMax = DISCOUNT_RANGES[i + 1];
            if (night > rangeMin && night <= rangeMax) {//min is exclusive max is inclusive
                discount = DISCOUNTS[i]; //we should just return here to save cpu cycles
            }
        }
    }
    else { //if last discount
        discount = DISCOUNTS[length - 1];
    }
    
    

    return discount;
}

double calculateCost(int nights, double basePrice, int const DISCOUNT_RANGES[], double const DISCOUNTS[], size_t length) {
    double totalCost = 0;

    for (int i = 1; i <= nights; i++) //note that we start at 1 and loop till less than or equal to 1 so that we have the number of the night, not its index
    {
        double discount = calculateDiscount(i, DISCOUNT_RANGES, DISCOUNTS, length); //find the discount for this night
        double nightCost = basePrice - discount;
        totalCost += nightCost; //sum all costs
    }

    return totalCost;

}

void displayCost(int nights, double cost) {
    //display simple table formated info
    printf("%s", "\n-Rental Charges-\n");
    printf("%-10s %s%-10s\n", "Nights","", "Charge");
    printf("%-10d %s%-.2f\n", nights,"$", cost);
    printf("%s", "\n");
}