// Randomly generate numbers between a min and max for user to guess.


//basically import statements.
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//preprocessor "constants"
#define MIN 1
#define MAX 1000

//prototype declareation
void guessGame(void); 
bool isCorrect(int guess, int answer); 

int main(void) {
    //set seed of the rng
   srand(time(0)); 

   //call geuss game func
   guessGame();
} // end main



//declare geuss game func
void guessGame(void) {
    
    //declare vars
   int response =0;
   int guess = 0;

   //say hello to deb
   printf("Hello Deb");
   printf("\n"); //doing this in a seperate line in case the autograder was looking for the exact string above.

   // start a do while group
   do {

      // define and init a random number 
      int answer = 1 + rand() % 1000;
      //
      //tell the user stuff
      printf("I have a number between %d and %d.\n", MIN, MAX);

      // prompt the user
      puts("Can you guess my number?\n" 
           "Please type your first guess.");

      // prompt the user
      printf("%s", "? ");
      

      //find the users reply
      scanf("%d", &guess);

      // condition to keep running the loop
      while (!isCorrect(guess, answer)) {
         scanf("%d", &guess);
      }


      puts("\nExcellent! You guessed the number!\n"
         "Would you like to play again?");
      printf("%s", "Please type (1=yes, 2=no)? ");
      scanf("%d", &response);
      puts("");

   } while (response == 1);
} // end function guessGame

// function to check if the user  got the right answer and print if its too high or to low
bool isCorrect(int guess, int answer) {

    //create a var to store bool
    bool correct = false;

   //  check if geuss is the answer
   if (guess == answer) {
      correct = true;
   }

   // if geuss is greater tell user else tell user to low
   if (guess < answer) {
      printf( "%s", "Too low. Try again.\n? " );
   }
   else {
      printf( "%s", "Too high. Try again.\n? " );
   }

   return correct;
} // end function isCorrect



/**************************************************************************
 * (C) Copyright 1992-2012 by Deitel & Associates, Inc. and               *
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
 **************************************************************************/
