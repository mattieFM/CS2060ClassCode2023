/**
@author: Mattie Fuller
Class: CS2060-002 
Project: Project Iteration: 2
Description: This is a project that will handle "airbnb" style bookings for a fictional renting service "airuccs." This is the second iteration and is fully GUI dependant and is attempting to implement
private public key authentication. Note that in the spirit of staying as native to c as possible I have opted away from using xml (html-esque) GUI builders that gtk+ provides, these are much easier but
are not really in the spirit of pure c program so I wont be using them.

Note: it is really clunky having this entire thing in one file, so sorry about that

Documentation style: comments are following doxgen C standard as I have found this is the most common style that tends to be used, let me know if you would prefer a different style.

Security Standard: SEI CERT C Standard

*/

/*-------------------------------------------------------------
 Air UCCS backend code
 ------------------------------------------------------------*/



/*-------------------------------------------------------------
 Includes
 ------------------------------------------------------------*/
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//Preprocessor constants
#define NUMBER_OF_PRICE_RANGES 3
#define NUMBER_OF_REVIEW_CATS 5
#define NUMBER_OF_REVIEW_CATS 5
#define STRING_SIZE 80
#define BASE_DISCOUNT 50

//boolean
#define ADD_MOCK_REVIEWS 1
#define MAX_REVIEW 5
#define MIN_REVIEW 1

//setup a generic linked list macro
#define DEFINE_LL_OF_TYPE_ARR_WITH_SIZE(TYPE, size, typeName, structName) \
typedef struct structName {\
  struct structName * next;\
  TYPE data[size];\
} typeName;

//global defualts for struct inits
const char DEFAULT_REVIEW_HEADERS[NUMBER_OF_REVIEW_CATS][STRING_SIZE] = {"Happiness", "Cleanliness", "Saftey", "Location", "Amenities"};
const char DEFAULT_NAME[STRING_SIZE] = "AIR UCCS";
int unsigned const DEFAULT_MIN_RENTAL_NIGHTS = 1;
unsigned int const DEFAULT_MAX_RENTAL_NIGHTS = 14;
unsigned int const DEFAULT_INTERVAL_1_NIGHTS = 3;
unsigned int const DEFAULT_INTERVAL_2_NIGHTS = 6;
double const DEFAULT_RENTAL_RATE = 400;
double const DEFAULT_DISCOUNT = 50;
const int DEFAULT_DAY_RANGES[NUMBER_OF_PRICE_RANGES] = {0, DEFAULT_INTERVAL_1_NIGHTS, DEFAULT_INTERVAL_2_NIGHTS };
const int DEFAULT_DISCOUNTS[NUMBER_OF_PRICE_RANGES] = {0, DEFAULT_DISCOUNT, DEFAULT_DISCOUNT*2};


/*---------------------------
Structs
-----------------------------*/
//use macro to setup linked list with type of int array
DEFINE_LL_OF_TYPE_ARR_WITH_SIZE(int, NUMBER_OF_REVIEW_CATS, ReviewNode, reviewNode);

/// @brief the location of the property
/// @initalizer: initLocation
typedef struct location {
	/* @description the human readable address of this location*/
	char address[STRING_SIZE]; //(80 size as told for all strings)

	/* @description the latitude of this location*/
	int lat;

	/*@description the longitude of this location*/
	int lon;
} Location;

void initLocation(Location * loc, char address[STRING_SIZE], int lat, int lon){
  //copy adress
  strcpy(loc->address, address);
  loc->lat = lat;
  loc->lon = lon;
}

/// @brief A struct representing a property
/// @initalizer: initProperty
typedef struct  property {
	//--Property Info-
	/*@description the name of this property */
	char propertyName[STRING_SIZE]; //(80 size as told for all strings)
	/* @description the location of this property*/
	Location location;

	//--Discounts--

	//-ranges-
	/* @description the number of days when the first discount period should start */
	int dayRanges[NUMBER_OF_PRICE_RANGES];

  /** @description the discounts for each range*/
  double discounts[NUMBER_OF_PRICE_RANGES];

	//-discount values-
	/* @description the base discount for range 1. This value is multiplied by the range */
	double baseDiscount;

  /** @description how much money has this property made durring this runtime*/
  double totalMoney;
  /** @description how many nights has this property booked durring this runtime*/
  double totalNights;

  /// @brief the head of the review's linked list
  ReviewNode * reviewsHead;

  /// @brief an array of the headers of the review categories
  char headers[NUMBER_OF_REVIEW_CATS][STRING_SIZE] ;
} Property;

void initProperty(Property * property){
  property->baseDiscount = BASE_DISCOUNT;
  memcpy(&property->dayRanges, &DEFAULT_DAY_RANGES, sizeof(DEFAULT_DAY_RANGES));
  memcpy(&property->discounts, &DEFAULT_DISCOUNTS, sizeof(DEFAULT_DISCOUNTS));
  memcpy(&property->headers, &DEFAULT_REVIEW_HEADERS, sizeof(DEFAULT_REVIEW_HEADERS));
  property->totalMoney = 0;
  property->totalNights = 0;
  ReviewNode * nodePtr = malloc(sizeof(ReviewNode));
  nodePtr->next = NULL;
  nodePtr->data[0] = NULL;
  property->reviewsHead = nodePtr;
}


void cleanUpReviews(Property * propertyPtr){
  while(propertyPtr->reviewsHead->next != NULL){
    ReviewNode * currentPtr = propertyPtr->reviewsHead;

    //traverse to one away from end of ll
    while(currentPtr->next->next != NULL){
      currentPtr = currentPtr->next;
    }
    
    //free the memory of the next node
    free(currentPtr->next);
    currentPtr->next = NULL;
  }
  
}

#define INHERIT_BASE_WIN \
  GtkWidget* window;


/// @brief the login window of the app
typedef struct logInWindow {
  //"inherit" from our base win macro
  INHERIT_BASE_WIN

  //both of these are temp for testing guis
  GtkEntry * passwordField;
  GtkEntry * usernameField;


  /** @brief how many times has the user attempted a login so fare*/
  int attempts;

  /** @brief max times the user can try to login --needs refactor to config.json eventually*/
  int maxLoginAttempts;

  /** TEMP a string password just for testing guis */
  char * password;


} LoginWindow;

/// @brief the config window of the app
typedef struct configWindow {
  //"inherit" from our base win macro
  INHERIT_BASE_WIN
} ConfigWindow;


/// @brief the admin window of the app
typedef struct adminWindow {
  //"inherit" from our base win macro
  INHERIT_BASE_WIN

  /** @brief the window to login to the admin menu*/
  LoginWindow loginWindow;

  //the inputs fileds of the admin menu
  GtkEntry * propertyNameInput;
  GtkEntry * baseDiscountInput;
  GtkLabel * totalMoneyLabel;
  GtkLabel * totalNightsLabel;


} AdminWindow;

/// @brief the review Window of the app
typedef struct reviewWindow {
  //"inherit" from our base win macro
  INHERIT_BASE_WIN

  //the inputs fileds for the review
  GtkEntry * inputs[NUMBER_OF_REVIEW_CATS];

  //the submit btn
  GtkButton * submitBtn;

} ReviewWindow;


/// @brief the customer window of the app
typedef struct customerWindow {
  //"inherit" from our base win macro
  INHERIT_BASE_WIN
  /** the label widget to store price within*/
  GtkWidget * priceRendererWindow;
  /** the entry widget for the input of number of night*/
  GtkWidget * inputEntry;
  /**the review table*/
  GtkWidget * reviewTable;
  /**the review table*/
  GtkWidget * grid;
} CustomerWindow;

/// @brief the main struct containing all app info that we need
typedef struct app {
  //"inherit" from our base win macro
  INHERIT_BASE_WIN

  // a reference to our main gtk app
  GtkApplication* app;

  /** the property*/
  Property property;
  /** the main window*/
  CustomerWindow customerWindow;
  /** the admin Window*/
  AdminWindow adminWindow;
  /**the config window*/
  ConfigWindow configWindow;
  /** the review window*/
  ReviewWindow reviewWindow;
} App;

/*-------------------------------------------------------------
 Prototypes
 ------------------------------------------------------------*/

/// @brief free all memory held by review ll
/// @param propertyPtr the property
void cleanUpReviews(Property * propertyPtr);

/// @brief Add a review to the linked list of reviews
/// @param property the property to add the review to
/// @param review the review to add
void addReview(App * mainApp, const int review[NUMBER_OF_REVIEW_CATS]);

/// @brief Initalzize a property with default values
/// @param property the property struct to initalize
void initProperty(Property * property);

/// @brief Initalize a location structure
/// @param loc the location struct to init
/// @param address the adress in the form of a string of the location
/// @param lat the latitude of the location
/// @param lon the longitude of the location
void initLocation(Location * loc, char address[STRING_SIZE], int lat, int lon);

/// @brief update all entrys on the admin app page
/// @param mainApp the main app struct
static void updateAdminInputs(App * mainApp);

/**
 Brief:
 Get a int that is either within [min-max] or in a list of sentinel values, from the user, prompting them with the prompt passed
 into this function and displaying the err prompt if they enter an invalid input.

 @Author MattieFM

 @param prompt {char[]} a string like array of chars to display to the user as a prompt
 @param err {char[]} a string like array to display to the user when they enter an invalid value,
 @param minAsciiCode {signed int} the lowest ascii code allowed to be entered
 @param maxAsciiCode {signed int} the highest ascii code allowed to be entered
 @param sentinelValues {signed int[]} an array of explicitly allowed ascii codes
 @param length, {size_t} the length of the sentinel_values array

 @extends getValidSignedInt

 @returns a signed int between [min-max]

*/
int getValidIntInputWithSentinels(char const PROMPT[], int min, int max, int const SENTINEL_VALUES[], size_t length);

/**
 Brief:
 A function that will calculate the discount of a day

 @param night {int} the night to find the discount of
 @param discountRanges[] {int[]) an array of ints in the format [discountRange1Start,discountRange1End,DiscountRange2End,...) where the start of each range after 1 is the end of the last
 @param discounts[] {floats} an array of the discounts for each range
 @param length, {size_t} the length of both the discountRRanges and discounts array, they must be same length

 @returns double, a dollar amount of the discount for this night
*/
double calculateDiscount(int night, int const DISCOUNT_RANGES[], double const DISCOUNTS[], size_t length);

/**
 Brief:
 A function to calculate the cost of a stay in a AirUCCS rented building. Cost is based on nights stayed, the base price per night and the discounts applied.
 
 @param nights {int} how many nights the user has stayed
 @param basePrice {float} the base price per night before any discounts are applied.
 @param discountRanges[] {int[]) an array of ints in the format [discountRange1Start,discountRange1End,DiscountRange2End,...) where the start of each range after 1 is the end of the last
 @param discounts[] {floats} an array of the discounts for each range
 @param length, {size_t} the length of both the discountRRanges and discounts array, they must be same length

 @returns {double} a double value representing how much the stay would cost.
*/
double calculateCost(int nights, double basePrice, int const DISCOUNT_RANGES[], double const DISCOUNTS[], size_t length);

/**
 Brief:
 A function to display how much the user's stay cost in a user friendly way

 @param nights {int} how many nights the user has stayed
 @param cost {double} the total cost of the user's stay

 @returns nothing, prints info to the console for the user. in the format:
 Nights          Charge
 5               $nnnn.nn

*/
void displayCost(int nights, double cost);

/**
 Brief: a method that would charge the user money for their stay. 

 @param uuid, the id of the user's stay
 @param cost, the total cost to charge them
 @param cardNum the card to charge the cost to
*/
void chargeUser(char const uuid[], float cost, char const cardNum[]);

/**
 brief:
 A function that will start the process of booking a room. 

 @param valuePt a pointer to a variable to store the value the user entered.
 @param sentinels[] an array of sentinel values that are allowed
 @param min, the min value for nights allowed
 @param max, the max value for nights allowed
 @param length, {size_t} the length of the sentinel_values array
 
 @returns bool, this will return true if the user has made a valid booking and false if they have entered a sentinel value.
*/
bool getBooking(int* valuePt, int const SENTINELS[], int min, int max, size_t length);

/**
 Brief: check if an array of ints contains a value.

 @param ARR[] the array to look through
 @param val the value to check for
 @param length, {size_t} the length of the array

 @returns bool, true if value is in arr false if not.

*/
bool intArrContains(int const ARR[], int val, size_t length);

/**
 Brief: display the rental property info to the user before they are prompted to make a perchance.

 @param min, the minimum number of nights a user can stay
 @param max the maximum number of nights a user can stay
 @param basePrice the base price per night of the property
 @param DISCOUNT_RANGES[] {int[]) an array of ints in the format [discountRange1Start,discountRange1End,DiscountRange2End,...) where the start of each range after 1 is the end of the last
 @param DISCOUNTS[] {double[]} an array of the discounts for each range,
 @param length, {size_t} the length of both the discountRRanges and discounts array, they must be same length

*/
void displayRentalPropertyInfo(int min, int max, double basePrice, int const DISCOUNT_RANGES[], double const DISCOUNTS[], size_t length);


/**
 * brief: initialize the login window "object-like" but do not spawn it into the current scene
 * @param mainWindow a GtkWidget pointer to initialize the mainWindow body onto
 * @param app the main gtk app.
*/
static void loginWindowInit(GtkApplication* app, App * mainApp);

/// @brief setup the review window
/// @param app the app
/// @param mainApp the main app 
static void reviewWindowInit(GtkApplication* app, App * mainApp);

/**
 * Brief: make a button with a callback when it is clicked
 * @param label the string to display on this button
 * @param cb, the function pointer to the function to call on this clicked. this function must take a gtk widget and the data pointer
*/
static GtkWidget * makeBtnWithCb(char* label, void (*cb) (GtkWidget *widget, gpointer data));

/**
 * Brief: the class of our tab headers widget that handles switching between tabs within the app
*/
static GtkWidget * TabHeaders(void);

/// @brief update the customer window display of reviews
static void mainWindowUpdateReviews(App * mainApp);

/// @brief this is called when the input box for nights changes its value. to show a preview of the pice
/// @param widget the widget this was called from IE: the entry
/// @param app the pointer to our app struct
static void onChange(GtkWidget *widget, App * app);

/// @brief this is called when the input box for any entry is edited, it ensures the value is within the min/max value
/// @param widget the widget this was called from IE: the entry
/// @param app the pointer to our app struct
static void onChangeValidateRangeOfReview(GtkWidget *widget, App * app);

/// @brief create a display that looks like Header: text
/// @param headerText the header text
/// @param displayText the display text
/// @return the widget
static GtkWidget * makeHtmlStyleLabelWithLabel(char * headerText, char * displayText);

/// @brief the method that will be called to process a charge when the user clicks the pay button
/// @param widget the input button this was called from
/// @param app the pointer to our main app struct
static void onCharge(GtkWidget *widget, App * app);

/// @brief render a 2d array of strings as labels in a grid
/// @param array 2d string array
/// @param rows the rows in the array
/// @param cols the cols in the array
/// @return the widget
static GtkWidget * makeGridOfLabelsFromHeadersAndLinkedList(char headers[NUMBER_OF_REVIEW_CATS][STRING_SIZE], ReviewNode * head);

/*-------------------------------------------------------------
 Function Declarations
 ------------------------------------------------------------*/


void addReview(App * mainApp, const int review[NUMBER_OF_REVIEW_CATS]){
  Property * propertyPtr = &mainApp->property;
  ReviewNode * currentPtr = propertyPtr->reviewsHead;

  //traverse to end of ll
  while(currentPtr->next != NULL){
    currentPtr = currentPtr->next;
  }

  //current node is now the end of the linked list
  ReviewNode * nodePtr = malloc(sizeof(ReviewNode));
  nodePtr->next = NULL;
  for (size_t i = 0; i < NUMBER_OF_REVIEW_CATS; i++)
  {
    printf("val: %d\n", review[i]);
    nodePtr->data[i] = review[i];
  }

  if(propertyPtr->reviewsHead->data[0] != NULL){
    //if the last node is not the head
    currentPtr->next = nodePtr;
    printf("here");
  } else {
    free(propertyPtr->reviewsHead);
    propertyPtr->reviewsHead = nodePtr;
  }

  mainWindowUpdateReviews(mainApp);
  
}

/**
@brief the main function for the program that will handle initialization/entry.
@author MattieFM
*/
int cliMain(void) {
    // -Constants Declarations-
    int const SENTINEL_NEG1 = -1;
    int unsigned const MIN_RENTAL_NIGHTS = 1;
    unsigned int const MAX_RENTAL_NIGHTS = 14;
    unsigned int const INTERVAL_1_NIGHTS = 3;
    unsigned int const INTERVAL_2_NIGHTS = 6;
    double const RENTAL_RATE = 400;
    double const DISCOUNT = 50;

    //define our config arrays
    unsigned int const DISCOUNT_RANGES[] = {0, INTERVAL_1_NIGHTS, INTERVAL_2_NIGHTS };
    double const DISCOUNTS[] = {0, DISCOUNT, DISCOUNT*2}; //with something like this discounts are usually picked by hand to be just right, rather than based on a curve.
    int const SENTINELS[] = { SENTINEL_NEG1 };
    // -Constants Ends-

    size_t discountsLen = sizeof(DISCOUNTS) / sizeof(DISCOUNTS[0]); //get len with basic bit math
    size_t sentinelLen = sizeof(SENTINELS) / sizeof(SENTINELS[0]); //get len with basic bit math

    //these vars will track total rev/nights
    double totalCost = 0;
    int totalNights = 0;

    bool shouldContinue = true;

    while (shouldContinue) {
        int userInputVal = 0;
        displayRentalPropertyInfo(MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, RENTAL_RATE, DISCOUNT_RANGES, DISCOUNTS, discountsLen);
        bool isSentinel = !getBooking(&userInputVal, SENTINELS, MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, sentinelLen);

        if (isSentinel) { //if a sentinel value was entered
            int sentinelVal = userInputVal; //create a new variable for specificity sake
            switch (sentinelVal)
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
        else { //if a sentinel value was not entered
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

bool getBooking(int* valuePt, int const SENTINELS[], int min, int max, size_t length) {
    bool returnVal = true;
    char const prompt[] = "Please enter the number of nights you would like to stay:";
    int val = getValidIntInputWithSentinels(prompt, min, max, SENTINELS, length);

    if (intArrContains(SENTINELS, val, length)) {
        //if the user entered a sentinel value set the return value to false and edit the sentinel val pointer
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
// then repeat for all the types we would want, I am aware of this, but we have not covered this and macros are a bit questionable anyways
int getValidIntInputWithSentinels(char const PROMPT[], int min, int max, int const SENTINEL_VALUES[], size_t length) {
    char const RANGE_ERR[] = "\nError: not within %d and %d. Please enter the value again:";
    char const TYPE_ERR[] = "\nError: not an integer. Please enter the value again:";
    bool validInput = false;
    int userInput = min - 1;

    printf("%s", PROMPT);
    while (!validInput) {
        bool input = false;
        input = scanf("%d", &userInput);
        if (input) {
            //if input is valid check if it is in range if so end and return
            if (userInput >= min && userInput <= max) {
                validInput = true;
            }
            else if (intArrContains(SENTINEL_VALUES, userInput, length)) {
                //if the value is a sentinel value it is also valid
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


void displayRentalPropertyInfo(int min, int max, double basePrice, int const DISCOUNT_RANGES[], double const DISCOUNTS[], size_t length) {
    //display header info
    printf("%s", "-Rental Property Info-\n");
    printf("Rental Property can be rented for %d to %d nights.\n", min, max);
    printf("$%.2f rate a night for first 3 nights.\n", basePrice);

    //display mid ranges
    for (size_t i = 1; i < length -1; i++) //skip last index since we handle that below
    {
        int rangeMin = DISCOUNT_RANGES[i]+1;//min is exclusive so add one to make it inclusive
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
    //display simple table formate info
    printf("%s", "\n-Rental Charges-\n");
    printf("%-10s %s%-10s\n", "Nights","", "Charge");
    printf("%-10d %s%-.2f\n", nights,"$", cost);
    printf("%s", "\n");
}





/*-------------------------------------------------------------
 Air UCCS front end code
 ------------------------------------------------------------*/

#define NumTabs 3


GtkWidget * windows[NumTabs] = {};
char * names[NumTabs] =  {"Main", "Analytics", "Login"};

static GtkWidget * makeBtnWithCb(char* label, void (*cb) (GtkWidget *widget, gpointer data)){
  GtkWidget * btn = gtk_button_new_with_label (label);
  g_signal_connect (btn, "clicked", G_CALLBACK (cb), btn);

  return btn;
}

static void switchWin(GtkWidget *widget, gpointer data){
  GtkRoot *root = gtk_widget_get_root (GTK_WIDGET (widget));
  const char * windowName = gtk_button_get_label(GTK_BUTTON(widget));

  GtkWindow *window = GTK_WINDOW (root);
  
  for (size_t i = 0; i < NumTabs; i++)
  {
    if(strcmp(names[i], windowName)){
      gtk_window_present(GTK_WINDOW(windows[i]));
    }
  }
  
  
  
}

static GtkWidget * TabHeaders(){
  GtkWidget *grid = gtk_grid_new ();
 
  for (size_t i = 0; i < NumTabs; i++)
  {
     GtkWidget *tab = makeBtnWithCb(names[i],switchWin);
    gtk_grid_attach (GTK_GRID (grid), tab, i, 0, 1, 1);
  }

  return grid;
}


/// @brief create a display that looks like Header: input field
/// @param displayText the header text
/// @return the widget
static GtkWidget * makeHtmlStyleEntryWithLabel(char * displayText){
    GtkWidget * widget = gtk_grid_new ();
    GtkWidget *entry = gtk_entry_new();
    GtkWidget *label = gtk_label_new_with_mnemonic (displayText);

    gtk_grid_attach (GTK_GRID (widget), entry, 1, 0, 1, 1);
    gtk_grid_attach (GTK_GRID (widget), label, 0, 0, 1, 1);

  

  return widget;
}


static GtkWidget * makeHtmlStyleLabelWithLabel(char * headerText, char * displayText){
    GtkWidget * widget = gtk_grid_new ();
    GtkWidget *entry = gtk_entry_new();
    GtkWidget *label = gtk_label_new_with_mnemonic (headerText);
    GtkWidget *label2 = gtk_label_new_with_mnemonic (displayText);

    gtk_grid_attach (GTK_GRID (widget), label2, 1, 0, 1, 1);
    gtk_grid_attach (GTK_GRID (widget), label, 0, 0, 1, 1);

  

  return widget;
}

static GtkWidget * makeGridOfLabelsFromHeadersAndLinkedList(char headers[NUMBER_OF_REVIEW_CATS][STRING_SIZE], ReviewNode * head){ //note this only works C99+ we can use a macro to do this if we need C98- support
    GtkWidget * widget = gtk_grid_new ();


    int y = 0;

    //add headers
    for(size_t x = 0; x < NUMBER_OF_REVIEW_CATS; x++){
      const char * string = headers[x];
      char newString[STRING_SIZE+20] = " | ";
      strcat(newString, string);
      GtkWidget *label = gtk_label_new_with_mnemonic (newString);
      gtk_grid_attach (GTK_GRID (widget), label, x, y, 1, 1);
    }

    y++;
    //traverse linked list adding each review
    ReviewNode * current = head;
    if(head->data[0] != NULL){
      do {
        //add row
        for(size_t x = 0; x < NUMBER_OF_REVIEW_CATS; x++){
          char string[STRING_SIZE];
          itoa(current->data[x], string, 10);
          GtkWidget *label = gtk_label_new_with_mnemonic (string);
          gtk_grid_attach (GTK_GRID (widget), label, x, y, 1, 1);
        }

        //increment row
        y++;

        //traverse ll
        current = current->next;
      } while (current != NULL);
    }
    
    
  return widget;
}

/**
 * brief: the base for every window of the app, defines all common elements such as teh tabs bar at the top
 * @param grid the gtkWidget grid child
*/
static GtkWidget * windowBase(GtkApplication* app){
  GtkWidget* win =  gtk_application_window_new (app);
  GtkWidget *grid = gtk_grid_new ();
  GtkWidget * tabs = TabHeaders();

  gtk_window_set_child (GTK_WINDOW (win), grid);

  //0,0 10 width, 1 height
  gtk_grid_attach (GTK_GRID (grid), tabs, 0, 0, 10, 1);

  return win;
}

static void onChange(GtkWidget *widget, App * app){
    Property data = app->property;
    CustomerWindow mainWin = app->customerWindow;
    int nightsStayed = atoi(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(mainWin.inputEntry))));
    printf("Calculating For %d nights\n", nightsStayed);
    double cost = calculateCost(nightsStayed, data.baseDiscount, data.dayRanges, data.discounts, NUMBER_OF_PRICE_RANGES);
    char intStr[STRING_SIZE];
    char header[STRING_SIZE] = "Cost: $";
    printf("Estimate: %.2f\n", cost);
    sprintf(intStr, "%.2f", cost);
    strcat(header, intStr);
   
    gtk_label_set_text (GTK_LABEL (mainWin.priceRendererWindow), header);
  }

static void onCharge(GtkWidget *widget, App * app){
    const Property data = app->property;
    CustomerWindow mainWin = app->customerWindow;
    int nightsStayed = atoi(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(mainWin.inputEntry))));
    printf("Paying For %d nights\n", nightsStayed);
    double cost = calculateCost(nightsStayed, data.baseDiscount, data.dayRanges, data.discounts, NUMBER_OF_PRICE_RANGES);
    app->property.totalMoney += cost;
    app->property.totalNights += nightsStayed;

    char intStr[STRING_SIZE];
    char header[STRING_SIZE] = "Cost: $";
    printf("Cost: %.2f\n", cost);
    sprintf(intStr, "%.2f", cost);
    strcat(header, intStr);
    strcat(header, "\nThank You <3");

    //open review window
    gtk_window_present(GTK_WINDOW(app->reviewWindow.window));
   
    gtk_label_set_text (GTK_LABEL (mainWin.priceRendererWindow), header);

    //update our fields
    updateAdminInputs(app);

  }

static void mainWindowUpdateReviews(App * mainApp){
  if(GTK_IS_GRID(mainApp->customerWindow.grid)){
    gtk_grid_remove(GTK_GRID(mainApp->customerWindow.grid), GTK_WIDGET(mainApp->customerWindow.reviewTable));
    mainApp->customerWindow.reviewTable = GTK_WIDGET(makeGridOfLabelsFromHeadersAndLinkedList(mainApp->property.headers, mainApp->property.reviewsHead));
    gtk_grid_attach (GTK_GRID (mainApp->customerWindow.grid), mainApp->customerWindow.reviewTable, 4, 0, 5, 5);
  }
}

/**
 * brief: initialize the main window "object-like" but do not spawn it into the current scene
 * @param mainWindow a GtkWidget pointer to initialize the mainWindow body onto
 * @param app the main gtk app.
*/
static void mainWindowInit(GtkApplication* app, App * mainApp){

  //setup our property
  Property property;
  initProperty(&property);

  mainApp->property = property;
  mainApp->customerWindow.priceRendererWindow = gtk_label_new_with_mnemonic ("type a num for an estimate");


  //this is here for easy testing and generation of reviews
  //I geuss it should probably be a macro that is only included in the compile time code if its enabled so that there is no bloat in the prod code.
  if(ADD_MOCK_REVIEWS){
    int arr[NUMBER_OF_REVIEW_CATS] = {1,5,3,2,1};
    void quickRandomize (){
      for (int i = 0; i < NUMBER_OF_REVIEW_CATS; i++)
      {
        arr[i] = (rand() % (MAX_REVIEW - 1 + 1)) + 1;
      }
    }
    addReview(mainApp, arr);
    quickRandomize();
    addReview(mainApp, arr);
    quickRandomize();
    addReview(mainApp, arr);
    quickRandomize();
    addReview(mainApp, arr);
    quickRandomize();
    addReview(mainApp, arr);
  }
 
  
  
  //init window
  mainApp->customerWindow.window = windowBase (app);

  //setup name and size
  gtk_window_set_title (GTK_WINDOW (mainApp->customerWindow.window), "Air UCCS");
  gtk_window_set_default_size (GTK_WINDOW (mainApp->customerWindow.window), 500, 500);

  //set up nights input
  GtkWidget * input = makeHtmlStyleEntryWithLabel("nights");
  GtkWidget * btn = gtk_button_new_with_label ("Pay");
  mainApp->customerWindow.inputEntry = gtk_grid_get_child_at(GTK_GRID (input), 1,0);
  
  g_signal_connect (btn, "clicked", G_CALLBACK (onCharge), mainApp);
  g_signal_connect (mainApp->customerWindow.inputEntry, "changed", G_CALLBACK (onChange), mainApp);

 
  mainApp->customerWindow.grid = gtk_window_get_child(GTK_WINDOW (mainApp->customerWindow.window));;
  mainApp->customerWindow.reviewTable = makeGridOfLabelsFromHeadersAndLinkedList(mainApp->property.headers, mainApp->property.reviewsHead);

  gtk_grid_attach (GTK_GRID (mainApp->customerWindow.grid), input, 0, 1, 1, 2);
  gtk_grid_attach (GTK_GRID (mainApp->customerWindow.grid), btn, 0, 3, 1, 1);
  gtk_grid_attach (GTK_GRID (mainApp->customerWindow.grid), mainApp->customerWindow.priceRendererWindow, 0, 4, 1, 1);
  gtk_grid_attach (GTK_GRID (mainApp->customerWindow.grid), mainApp->customerWindow.reviewTable, 4, 0, 5, 5);
  
  //hook onto the on close function of gtk to cleanup our linked list on close
  g_signal_connect(app, "delete-event", G_CALLBACK(cleanUpReviews), NULL);
}

/// @brief the method called when the login button is pressed
/// @param widget the login button
/// @param app pointer to mainapp struct
static void onLogin(GtkWidget *widget, App * app){
  printf("attempts: %d\n",app->adminWindow.loginWindow.attempts);
  app->adminWindow.loginWindow.attempts++;
  if(app->adminWindow.loginWindow.attempts < app->adminWindow.loginWindow.maxLoginAttempts){
    //if the user can make any more attempts to login
    const char * passwordIn = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(app->adminWindow.loginWindow.passwordField)));

    if(strcmp(passwordIn, app->adminWindow.loginWindow.password) == 0){
      //password correct
      //reset attempts and show window
      app->adminWindow.loginWindow.attempts = 0;
      gtk_window_present(GTK_WINDOW(app->adminWindow.window));
    } else{
      //password wrong
      //TODO: implmenet alert/popup/msgbox and display err
    }
    
  } else {
    //close the window if they cannot make any more attempts
    app->adminWindow.loginWindow.attempts = 0;
    //TODO: make this close the window in a way that it can be reopened, for now just hide
    gtk_widget_set_visible(GTK_WIDGET(app->adminWindow.loginWindow.window), false);
  }
  

}

static void onClick (GtkWidget *widget, App * mainApp){
  int review[NUMBER_OF_REVIEW_CATS] = {0,0,0,0,0};
  for (size_t i = 0; i < NUMBER_OF_REVIEW_CATS; i++)
  {
    const char * str = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(mainApp->reviewWindow.inputs[i])));
    char * end;
    int x = (int) strtol(str, &end, 10);

    if(end != NULL){
      if(*end == '\0'){
        review[i] = x;
      } else {
        review[i] = -1;
      }
    }else {
      review[i] = -1;
    }
    
  }
  
  addReview(mainApp, review);
  //TODO: make this close the window in a way that it can be reopened, for now just hide
  gtk_widget_set_visible(GTK_WIDGET(mainApp->reviewWindow.window), false);
}

static void onChangeValidateRangeOfReview(GtkWidget *widget, App * app){
  GtkEntry * entry = GTK_ENTRY(widget);
  const char * str = gtk_entry_buffer_get_text(gtk_entry_get_buffer(entry));
  char * end;
  int x = (int) strtol(str, &end, 10);

  if(end != NULL){
    if(*end == '\0'){
      //valid int input
      bool shouldUpdate = false;
      if(x > MAX_REVIEW){
        x = MAX_REVIEW;
        shouldUpdate = true;
      } else if (x < MIN_REVIEW){
        x = MIN_REVIEW;
        shouldUpdate = true;
      }

      //reset the input to the adjusted value
      if(shouldUpdate){
        //make sure we can update it properly
        if(GTK_ACCESSIBLE(widget)){
          char newStr[STRING_SIZE] = "test";
          itoa(x, newStr, 10);
          GtkEntryBuffer * newBuffer = gtk_entry_buffer_new(newStr, STRING_SIZE);
          gtk_entry_set_buffer (entry, newBuffer);
        }
        
      }
    }
  }

  
  
}

static void reviewWindowInit(GtkApplication* app, App * mainApp){
  //init window
  mainApp->reviewWindow.window = windowBase (app);

  //setup grid
  GtkWidget *grid = gtk_grid_new ();
  gtk_window_set_child (GTK_WINDOW (mainApp->reviewWindow.window), grid);

  GtkWidget * label = gtk_label_new_with_mnemonic("Please enter your review of the property");
  gtk_grid_attach (GTK_GRID (grid), label, 0, 0, 1, 1);
  //make fields for reviews
  for (size_t i = 0; i < NUMBER_OF_REVIEW_CATS; i++)
  {
    
    char * header = mainApp->property.headers[i];
    GtkWidget * entryAndLabel = makeHtmlStyleEntryWithLabel(header);
    
    //get just the entry for our array
    mainApp->reviewWindow.inputs[i] = GTK_ENTRY(gtk_grid_get_child_at(GTK_GRID (entryAndLabel), 1,0));
    //attach validation cb
    g_signal_connect (mainApp->reviewWindow.inputs[i], "changed", G_CALLBACK (onChangeValidateRangeOfReview), mainApp);

    gtk_grid_attach (GTK_GRID (grid), entryAndLabel, 0, i+1, 1, 1);
  }

  mainApp->reviewWindow.submitBtn = GTK_BUTTON(gtk_button_new_with_label ("SubmitReview"));
  //set up the on click callback
  g_signal_connect (mainApp->reviewWindow.submitBtn, "clicked", G_CALLBACK (onClick), mainApp);

  gtk_grid_attach (GTK_GRID (grid), GTK_WIDGET(mainApp->reviewWindow.submitBtn), 0, NUMBER_OF_REVIEW_CATS+1, 1, 1);
}

static void loginWindowInit(GtkApplication* app, App * mainApp){
  mainApp->adminWindow.loginWindow.window = windowBase (app);

  //setup temp login vars
  mainApp->adminWindow.loginWindow.attempts = 0;
  mainApp->adminWindow.loginWindow.maxLoginAttempts = 3;
  mainApp->adminWindow.loginWindow.password = "password";

  //setup name and size
  gtk_window_set_title (GTK_WINDOW (mainApp->adminWindow.loginWindow.window), "Air UCCS - Login");
  gtk_window_set_default_size (GTK_WINDOW (mainApp->adminWindow.loginWindow.window), 500, 500);

  //set up nights input
  GtkWidget * pass = makeHtmlStyleEntryWithLabel("Password (eventually key pair)");
  GtkWidget * username = makeHtmlStyleEntryWithLabel("UserName (eventually key pair)");
  GtkWidget * btn = gtk_button_new_with_label ("Login");

  //set up our references to the widgets
  mainApp->adminWindow.loginWindow.passwordField = GTK_ENTRY(gtk_grid_get_child_at(GTK_GRID (pass), 1,0));
  mainApp->adminWindow.loginWindow.usernameField = GTK_ENTRY(gtk_grid_get_child_at(GTK_GRID (username), 1,0));
 
  GtkWidget *grid = gtk_window_get_child(GTK_WINDOW (mainApp->adminWindow.loginWindow.window));

  //set up the on click callback
  g_signal_connect (btn, "clicked", G_CALLBACK (onLogin), mainApp);

  //add all our widgets to the window
  gtk_grid_attach (GTK_GRID (grid), pass, 0, 1, 1, 2);
  gtk_grid_attach (GTK_GRID (grid), username, 0, 3, 1, 2);
  gtk_grid_attach (GTK_GRID (grid), btn, 0, 5, 1, 1);
}

static void updateAdminInputs(App * mainApp){
  //setup default values for our inputs
  //TODO: refactor to use loop and update on payment
  
  //total nights/money display
  char tempString[STRING_SIZE] = "";
  itoa(mainApp->property.totalMoney, tempString, 10);
  gtk_label_set_text(GTK_LABEL(mainApp->adminWindow.totalMoneyLabel), tempString);

  itoa(mainApp->property.totalNights, tempString, 10);
  gtk_label_set_text(GTK_LABEL(mainApp->adminWindow.totalNightsLabel), tempString);


  //setup placeholders for current values for config fields

  //property name
  GtkEntryBuffer * propNameBuffer = gtk_entry_buffer_new(mainApp->property.propertyName, STRING_SIZE);
  gtk_entry_set_buffer (GTK_ENTRY(mainApp->adminWindow.propertyNameInput), propNameBuffer);

  //base discount
  itoa(mainApp->property.baseDiscount, tempString, 10);
  GtkEntryBuffer * basediscountBuffer = gtk_entry_buffer_new(tempString, STRING_SIZE);
  gtk_entry_set_buffer (GTK_ENTRY(mainApp->adminWindow.baseDiscountInput), basediscountBuffer);

  //TODO: write json.stringify esque thing for our arrays
  // GtkEntryBuffer * dayRangesBuffer = gtk_entry_buffer_new(mainApp->property.propertyName, STRING_SIZE);
  // gtk_entry_set_buffer (GTK_ENTRY(gtk_grid_get_child_at(GTK_GRID (dayRanges), 1,0)), dayRangesBuffer);

  // GtkEntryBuffer * discountsBuffer = gtk_entry_buffer_new(mainApp->property.propertyName, STRING_SIZE);
  // gtk_entry_set_buffer (GTK_ENTRY(gtk_grid_get_child_at(GTK_GRID (discounts), 1,0)), discountsBuffer);



}


/**
 * brief: initialize the main window "object-like" but do not spawn it into the current scene
 * @param mainWindow a GtkWidget pointer to initialize the mainWindow body onto
 * @param app the main gtk app.
*/
static void adminWindowInit(GtkApplication* app, App * mainApp){
  mainApp->adminWindow.window = windowBase (app);



  //set up admin inputs
  GtkWidget * propName = makeHtmlStyleEntryWithLabel("Property Name");
  GtkWidget * baseDiscount = makeHtmlStyleEntryWithLabel("Base Discount");
  GtkWidget * dayRanges = makeHtmlStyleEntryWithLabel("Day Ranges");
  GtkWidget * discounts = makeHtmlStyleEntryWithLabel("Discounts");

  //setup money displays
  char tempString[STRING_SIZE] = "";
  itoa(mainApp->property.totalMoney, tempString, 10);
  GtkWidget * totalMoneyDisplay = makeHtmlStyleLabelWithLabel("Gross Revenue: ", tempString);
  itoa(mainApp->property.totalNights, tempString, 10);
  GtkWidget * totalNightsDisplay = makeHtmlStyleLabelWithLabel("Nights Stayed: ", tempString);

  //set up our references
  mainApp->adminWindow.propertyNameInput = GTK_ENTRY(gtk_grid_get_child_at(GTK_GRID (propName), 1,0));
  mainApp->adminWindow.baseDiscountInput = GTK_ENTRY(gtk_grid_get_child_at(GTK_GRID (baseDiscount), 1,0));
  mainApp->adminWindow.totalMoneyLabel = GTK_LABEL(gtk_grid_get_child_at(GTK_GRID (totalMoneyDisplay), 1,0));
  mainApp->adminWindow.totalNightsLabel = GTK_LABEL(gtk_grid_get_child_at(GTK_GRID (totalNightsDisplay), 1,0));
  

  //setup grid
  GtkWidget *grid = gtk_grid_new ();
  gtk_window_set_child (GTK_WINDOW (mainApp->adminWindow.window), grid);

  //attach our widgets

  //attach our displays
  gtk_grid_attach (GTK_GRID (grid), totalMoneyDisplay, 0, 0, 2, 1);
  gtk_grid_attach (GTK_GRID (grid), totalNightsDisplay, 0, 1, 2, 1);

  //attach our inputs
  gtk_grid_attach (GTK_GRID (grid), propName, 0, 2, 2, 1);
  gtk_grid_attach (GTK_GRID (grid), baseDiscount, 0, 3, 2, 1);
  gtk_grid_attach (GTK_GRID (grid), dayRanges, 0, 4, 2, 1);
  gtk_grid_attach (GTK_GRID (grid), discounts, 0, 5, 2, 1);

  //setup login window
  loginWindowInit(app, mainApp);

  //update our fields
  updateAdminInputs(mainApp);
}

static void
activate (GtkApplication* app, gpointer user_data)
{
  //this needs to be on the heap because of how gtk call backs work, there is no other way to do this as far as I can tell
  //I didn't want to use the heap before we covered it, but this is the only solution I can think off (I spent 2 hours on this I might be dumb, but If I am I genially do not know any other way.)
  App * mainApp = malloc(sizeof(App));

  mainApp->app = app;
  mainWindowInit(app, mainApp);
  adminWindowInit(app, mainApp);
  reviewWindowInit(app, mainApp);
  GtkWidget *mainWindow = mainApp->customerWindow.window;
  GtkWidget *admin = mainApp->adminWindow.window;
  
  windows[0] = mainWindow;
  windows[1] = mainApp->adminWindow.loginWindow.window;
  windows[2] = mainApp->configWindow.window;
  gtk_window_present (GTK_WINDOW (mainWindow));


  void freeMem(){
    free(mainApp);
  }
  //hook into the on delete of our app to clean up memory
  g_signal_connect(app, "delete-event", G_CALLBACK(freeMem), NULL);

  
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);

  //app clean up
  g_object_unref (app);

  return status;
}


//I know this is javascript comment syntax but its pretty and is my preset. 
//**========================================================================
 /* ?                                RSA KEY GEN section
 * @author         Mattie --me:  
 * @email          mmfulelr22@gmail.com:  
 * @repo           https://github.com/mattieFM/CS2060ClassCode2023:  
 * @createdOn      idk:  
 * @description    RSA key generation for pub/priv key pair cypto:  
 *========================================================================**/

/**
 * First To walk anyone reading through the general methodology, recall that RSA is an algorithm for generating complex keys
 * for pub/priv key crypto
 * 
 * It asserts that it is very hard to find large integer common denominators of primes
 * it consists of 4 steps
 * 
 * 1: choose two arbitraty large primes, denoted as p and q. I will use these names below as it is what they are called within the algorithm and is rather standard 
 *  -These should be chosen at random, I will be using a probabalistic aproach to find primes as we only need to find 2 we can basically guess and assume we will be right within x 
 *   where x is usually less than 2000 guesses which is computationally insignificant for this scale
 * 2: we then compute n = p times q --> n=pq.
 * 3: then it gets harder. We now compute Carmichaels totient function of n (denoted as λ(n))
 *    Its a lot of math that will be explained in the functions below for it
 * 4: choose an integer e such taht 2 < e < λ(n) and gcd(e, λ(n)) = 1; that is, e and λ(n) are coprime
 * 5: determine d as d == e^-1 that is d is the modular multiplicitive invers of e modulo λ(n)
 * 
 * 
 * thus the public key becomes modulus n and the exponent e
 * and the private key consists of exponent d, p, q and λ(n), which are used to compute d
 * 
 * in short
 * d is the private key
 * %n^e is the public key
*/


/**======================
 *    Declarations
 *========================**/

#include <time.h>
#include <stdio.h>
#include <stdlib.h> 
#include <math.h> 

//the number of bits of our prime numbers
#define BIT_LEVEL 8

/**======================
 *    Prototypes
 *========================**/
/// @brief generate a random integer of n bits
unsigned long long nBitRandom(int numberOfBits);


/**======================
 *    Functions
 *========================**/

unsigned long long nBitRandom(int numberOfBits){
  srand(time(0));
  //this is relatively basic math. 
  
  //since binary max of n bits is 2^n -1 for unsigned numbs we can make an easy max
  unsigned long long maximum = (unsigned long long) powl(2, numberOfBits) - 1;
  //then we want the smallest n bit number which is similarly to above just 2^n-1 + 1
  unsigned long long minimum = (unsigned long long) powl(2, numberOfBits - 1) + 1;
  return minimum + (rand() % ( maximum - minimum + 1 ) );
}
