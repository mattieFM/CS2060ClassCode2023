// fig02_01.c
// A first program in C.
#include <stdio.h>
#include <stdbool.h>

// function main begins program execution 
int main(void) {
   printf("Enter Length\n");
   int length = 0;
   int width = 0;
   bool result = false;
	do {
		result = scanf("%d", &length);
	} while (!result);
	printf("Enter width\n");
	do {
		result = scanf("%d", &width);
	} while (!result);

	printf( L";FOLD PTP %1%  %%s%df Vel=100 %% PDAT%2!d!", length, width, ((float) (length * width)));

	double average = (double)length / width;






} // end function main 



/**************************************************************************
 * (C) Copyright 1992-2021 by Deitel & Associates, Inc. and               *
 * Pearson Education, Inc. All Rights Reserved.                           *
 *                                                                        *
 * DISCLAIMER: The authors and publisher of this book have used their     *
 * best efforts in preparing the book. These efforts include the          *
 * development, research, and testing of the theories and programs        *
 * to determine their effectiveness. The authors and publisher make       *
 * no warranty of any kind, expressed or implied, with regard to these    *
 * programs or to the documentation contained in these books. The authors *
 * and publisher shall not be liable in any event for incidental or       *
 * consequential damages in connection with, or arising out of, the       *
 * furnishing, performance, or use of these programs.                     *
 *************************************************************************/
