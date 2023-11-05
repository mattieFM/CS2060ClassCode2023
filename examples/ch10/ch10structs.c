//Chap 10 Structures
//Explore and add comments

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE 20
#define MAXEMPS 3

// Defining a structure
// Update to use typedef
typedef struct  employee {
	char firstName[NAME_SIZE];
	char lastName[NAME_SIZE];
	unsigned int age;
	double hourlySalary;
} Employee ;



#include <stdbool.h>
#define STRING_SIZE 80

typedef struct location {
	/* @description the human readable adress of this location*/
	char adress[STRING_SIZE]; //(80 size as told for all strings)

	/* @description the latitude of this location*/
	int lat;

	/*@description the logitude of this location*/
	int lon;
} Location;

typedef struct  property {
	//--Property Info-
	/*@description the name of this propery*/
	char propertyName[STRING_SIZE]; //(80 size as told for all strings)
	/* @description the location of this property*/
	Location location;

	//--Discounts--

	//-ranges-
	/* @description the number of days when the first discount period should start */
	int range1Start;
	/* @description the number of days that the first discount period should end and the second should start*/
	int range2Start;
	/* @description the number of days till the second discount period should end and the final should start*/
	int rangeFinalStart;	

	//-discount values-
	/* @description the base discount for range 1. This value is multiplied by the range */
	int baseDiscount;

	/* @description how much should the discount be multiplied each range? @default 2*/
	//int discountScaler; told that we are not using this in class 






} Property;

/// <summary>
/// get all the need info for a property via command line
/// </summary>
/// <param name="p"> a pointer to the output property</param>
/// <returns>whether the setup succeeded or failed</returns>
bool setupProperty(Property *p) {
	
	getRangeInfo("1", &(p->range1Start));
	getRangeInfo("2", &(p->range2Start));
	getRangeInfo("final", &(p->rangeFinalStart));

	printf("%s","base discount : ");
	scanf("%d", &(p->baseDiscount));
	puts("");

	printf("%s", "adress: ");
	fgets(p->location.adress, STRING_SIZE, stdin);
	puts("");

}

void displayProperty(Property* p) {
	char* formatter =
		"Discount 1 starts at night: %d\n"
		"Discount 2 starts at night: %d\n"
		"Discount 3 starts at night: %d\n"
		"Base Discount is: %d\n"

		"address is: %s\n"
		;
	printf(
		formatter,
		p->range1Start,
		p->range2Start,
		p->rangeFinalStart,
		p->baseDiscount,
		p->location
	);
}

/// <summary>
/// get info from the user about a discount range
/// </summary>
/// <param name="rangeTitle">the title of this range</param>
/// <param name="p">pointer to output var</param>
void getRangeInfo(char *rangeTitle, int* out) {
	printf("on what night does discount range %d start:", x);
	scanf("%d", out);
	puts("");
}


int x = 1;
Employee employee = {"Mattie", "fuller", 19, 22}
employee.name
passByReference(&employee)


void enterEmployees(struct employee* employeeArr, int maxEmployees);
void passStructByValue(struct employee aEmployee);
void passStructByReference(struct employee* employeePtr);


int main(void)
{

	//PART 1 declare a struct

	struct employee defaultEmployee;
	puts("\n\nPart 1 declare struct employee defaultEmployee");

	char stringtest[6] = { '\0'};

	puts("Print after declaration defaultEmployee\n");



	//What is stored?
	printf("sizeof  defaultEmployee= %lu\n", sizeof(defaultEmployee));
	printf("Employee's first name: %s\n", defaultEmployee.firstName);
	printf("Employee's last name: %s\n", defaultEmployee.lastName);
	printf("Employee's age = %d\n", defaultEmployee.age);
	printf("Employee's hourly salary = %3.2f\n", defaultEmployee.hourlySalary);

	//initialize
	strncpy(defaultEmployee.firstName, "Debug", NAME_SIZE);
	strncpy(defaultEmployee.lastName, "Horror", NAME_SIZE);
	defaultEmployee.age = 0;
	defaultEmployee.hourlySalary = 0;

	printf("defalutemployee %d\n", defaultEmployee);
	printf("defalutemployee %p\n", defaultEmployee);

	puts("\nPrint after intialization of defaultEmployee");

	printf("Employee's first name: %s\n", defaultEmployee.firstName);
	printf("Employee's last name: %s\n", defaultEmployee.lastName);
	printf("Employee's age = %d\n", defaultEmployee.age);
	printf("Employee's hourly salary = %3.2f\n", defaultEmployee.hourlySalary);


	//Part 2
	puts("\n\nPart 2 Declare and Initalize struct employee defaultEmployee = { Joe, Smith, 23, 15}");

	// declare and initialize two employees
	struct employee aEmployee = { "Stack ", "Terror", 37, 65.00 };
	struct employee bEmployee = { "Binary ", "Beasts" };

	puts("\nPrint after intializing employeea ");

	puts("Print after declaration defaultEmployee");
	printf("aEmployee's first name: %s\n", aEmployee.firstName);
	printf("aEmployee's last name: %s\n", aEmployee.lastName);
	printf("aEmployee's age = %d\n", aEmployee.age);
	printf("aEmployee's hourly salary = %3.2f\n", aEmployee.hourlySalary);


	//compare members
	if (aEmployee.age > bEmployee.age)
	{
		puts("aEmployee age is greater than bEmployee age");
	}
	else if (aEmployee.age < bEmployee.age)
	{
		puts("aEmployee age is less than bEmployee age");
	}
	else
	{
		puts("aEmployee age is = bEmployee age");
	}

	// memeber assignments
	bEmployee.hourlySalary = aEmployee.hourlySalary;
	printf("Now bEmployee's hourly salary = %3.2f\n", bEmployee.hourlySalary);
	printf("&bEmployee.hourlySalary = %p", &bEmployee.hourlySalary);

	//Part 3 Pointer to employee struct
	puts("\n\nPart 3 Declare and Initalize employeePtr ");

	// Intialize employee pointer to aEmployee 
	struct employee* employeePtr = &aEmployee;

	// Arrow operators for pointers to structures 
	printf("EmployeePtr first name -> %s\n", employeePtr->firstName);
	printf("EmployeePtr last name -> %s\n", employeePtr->lastName);
	printf("EmployeePtr age -> %d\n", employeePtr->age);
	printf("EmployeePtr hourly salary -> %3.2f\n", employeePtr->hourlySalary);

	// assign bEmployee address
	puts("\nAssigne employeePtr = &bEmployee and access with (*employeePtr). ");
	employeePtr = &bEmployee;
	// equivalent to arrow operators 
	printf("(*employeePtr).firstName  %s\n", (*employeePtr).firstName);
	printf("(*employeePtr).lastName %s\n", (*employeePtr).lastName);
	printf("(*employeePtr).age %d\n", (*employeePtr).age);
	printf("(*employeePtr).hourlySalary %3.2f\n", (*employeePtr).hourlySalary);


	//part 4 passing to functions
	puts("\n\nPart 4 Passing to Functions");

	passStructByValue(aEmployee);
	puts("\nAfter passing by value");
	printf("aEmployee's first name: %s\n", aEmployee.firstName);
	printf("aEmployee's last name: %s\n", aEmployee.lastName);
	printf("aEmployee's age = %d\n", aEmployee.age);
	printf("aEmployee's hourly salary = %3.2f\n", aEmployee.hourlySalary);

	passStructByReference(&aEmployee);
	puts("\nAfter passing by reference");
	printf("aEmployee's first name: %s\n", aEmployee.firstName);
	printf("aEmployee's last name: %s\n", aEmployee.lastName);
	printf("aEmployee's age = %d\n", aEmployee.age);
	printf("aEmployee's hourly salary = %3.2f\n", aEmployee.hourlySalary);


	//part 5 Array of Employees
	puts("\n\nPart 5 Array of Structures");
	// Intialize array of employees 
	struct employee employees[MAXEMPS];
	enterEmployees(employees, MAXEMPS);

	return 0;
}

void enterEmployees(struct employee* employeeArr, int max)
{

	for (int i = 0; i < max; i++)
	{
		printf("\nEnter details of employees %d\n\n", i + 1);

		printf("%s", "Enter first name: ");
		scanf("%s", employeeArr[i].firstName);

		printf("%s", "Enter last name: ");
		scanf("%s", employeeArr[i].lastName);

		printf("%s", "Enter age: ");
		scanf("%i", &employeeArr[i].age);

		printf("%s", "Enter Hourly Salary: ");
		scanf("%lf", &employeeArr[i].hourlySalary);
	}
}

void passStructByValue(struct employee employeeTest)
{
	puts("In function call - change values in the structure");
	strncpy(employeeTest.firstName, "Dyanmic", NAME_SIZE);
	employeeTest.hourlySalary = 100.00;
}

void passStructByReference(struct employee* employeeTestPtr)
{
	Employee realEmp = *employeeTestPtr;
	puts("In function call - change values in the structure");

	strncpy(realEmp.firstName, "Dynamic", NAME_SIZE);
	realEmp.hourlySalary = 100.00;

	printf("%s","First Name: ");
	fgets(realEmp.firstName, NAME_SIZE, stdin);
	printf("\nEmployee's first name = %s\n", realEmp.firstName);

	printf("%s", "Hourly Salery: ");
	scanf("%lf", &(realEmp.hourlySalary));
	printf("\nEmployee's hourly salary = %3.2f\n\n", realEmp.hourlySalary);
	
}


