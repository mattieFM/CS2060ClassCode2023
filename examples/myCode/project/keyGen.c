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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> 
#include <math.h> 
#include <stdbool.h>

//the number of bits of our prime numbers
#define BIT_LEVEL 8

/**======================
 *    Prototypes
 *========================**/
/// @brief generate a random integer of n bits
int64_t nBitRandom(int numberOfBits);

/// @brief check if a number is prime
bool isPrime(int64_t n);

/// @brief  check if 2 numbers are co prime with eachother
/// @param a number1
/// @param b number2
/// @return whether they are coPrime or not
bool isCoPrime(int64_t a, int64_t b);

/// @brief get a random prime number
int64_t getRandPrime();

/// @brief generate a private and a public key
RSAKeyGen(int64_t * outN, int64_t * outD, int64_t * outE);

/// @brief encrypt a char with the public key
int64_t encrypt(char c, int64_t n, int64_t e);
char decrypt(char c, int64_t n, int64_t d);

/**======================
 *    Functions
 *========================**/


/**======================
 *    Prime Calc (working)
 *========================**/
//note there are much much better ways to do this, but for now this is fine.
int64_t nBitRandom(int numberOfBits){
  srand(time(0));
  //this is relatively basic math. 
  
  //since binary max of n bits is 2^n -1 for unsigned numbs we can make an easy max
  int64_t maximum = (int64_t) powl(2, numberOfBits) - 1;
  //then we want the smallest n bit number which is similarly to above just 2^n-1 + 1
  int64_t minimum = (int64_t) powl(2, numberOfBits - 1) + 1;

  //since all primes are odd lets just flip the last bit
  int64_t val = minimum + (rand() % ( maximum - minimum + 1 ));
  return val | ((int64_t)1 << numberOfBits);
}

bool isPrime(int64_t n){
    //since we only need to check from 2 - sqrt n devisible due to how primes work

    //note yes I am using multi-returns here since this is a very heavy function and we want the proformance saved
    //also note this does not actually check if something is prime, but combined with the stipulations of how we are generating our rands it should work properly
    for(int i = 2; i <= sqrt(n); i+=2){ //only odds are prime
        if(n%i==0){
            return false;
        }
    }
    return true;
}

bool isCoPrime(int64_t a, int64_t b){
    //numbers are co-prime if together their only common divisor is 1
    //hence 8 and 9 are co-prime while nither are prime
    //using the euclidian algorithem we can assert that by taking the largest number that divides both numbers, and replacing the larger of the two numbers with the difference untill we hit the smaller number being 1
    //from there we can assert that the modulus, the remainder of the division also remains the same (due to numbers being cool) and thus:

    //base case
    if(b == 0){
        //when b is 0, if a is equal to 1 the greatest common divisor was one and thus they are co prime
        return a == 1;
    }
    
    int64_t mod = a % b;
    return isCoPrime(b, mod);
}

/// @brief get a rand between min and max
/// @param min 
/// @param max 
/// @return 
int64_t randBetween(int64_t min, int64_t max){
    return (rand() % (max - 1 + 1)) + min;
}


int64_t getRandPrime(){
    int64_t n = 6;
    do{
        n = nBitRandom(BIT_LEVEL);
    } while (!isPrime(n));
    return n;
}

/**
 * @brief Finds the multiplicative inverses of the two numbers
 *
 * @param a
 * @param b
 * @param x
 * @param y
 */
void _extended_euclid(int a, int b, int *x, int *y) {

    /* If second number is zero */
    if (!a) {
        *x = 0;
        *y = 1;
        return;
    }

    int _x, _y;

    /* Find the coefficients recursively */
    _extended_euclid(b % a, a, &_x, &_y);

    /* Update the coefficients */
    *x = _y - (b / a) * _x;
    *y = _x;
}


/**
 * @brief Returns the multiplicative inverse of a under base m
 *
 * @param a
 * @param m
 * @return int
 */
int _mod_inv(int a, int m) {

    int inv_a, inv_m;

    /* Compute the coefficients using extended euclidean algorithm */
    _extended_euclid(a, m, &inv_a, &inv_m);

    /* If the inverse is negative convert it to positive under m */
    if (inv_a < 0) {
        inv_a = (m + inv_a) % m;
    }

    return inv_a;
}


RSAKeyGen(int64_t * outN, int64_t * outD, int64_t * outE){
    //Step 1:
    //find unquie primes, p and q
    const int64_t p = getRandPrime();
    int64_t tempQ = getRandPrime();
    while(tempQ == p){
        tempQ = getRandPrime();
    }
    const int64_t q = tempQ;

    //Step 2:
    //compute n
    const int64_t n = p * q;

    //Step 3:
    //find the lowest common multiplyor of p and q to find n
    const int64_t lamdaN = (p-1)*(q-1);

    //Step 4
    //choose any number coprime to lamdaN greater than 2.
    int64_t e = randBetween(2, lamdaN-1);

    //loop till its co prime
    while(!isCoPrime(e, lamdaN)){
        e = randBetween(2, lamdaN-1);
    }

    //e is now co-prime between 2 and lamda n

    //Now we need to find d.
    //d is the modular multiplicative inverse of e
    int64_t d = _mod_inv(e, lamdaN);

    *outE = e;
    *outD = d;
    *outN = n;


}

int64_t modExponent(int64_t a, int64_t b, int64_t m){
    //start at 1
    int64_t result = 1;

    a = a % m;

    if(a ==0){
        return 0;
    }

    while(b >0){
        if(b & 1){
            result = (result * a) % m;
        }

        b = b >> 1;

        a = (a*a) % m;
    }

    return result;
}

int64_t encrypt(char c, int64_t n, int64_t e){
    return modExponent((int64_t) c, e, n);
}

char decrypt(char c, int64_t n, int64_t d){
    return (char) modExponent((int64_t) c, d, n);
}





void main() {
    int64_t n;
    int64_t e;
    int64_t d;
    RSAKeyGen(&n, &d, &e);
    printf("n: %lld, d: %lld e: %lld\n", n, d, e);

    char m = 'm';
    int64_t encrptyedM = encrypt(m, n,e);
    char decryptedM = decrypt(m,n,d);
    printf("%c (%d) was encrypted into %d, was decrypted into: %d", m, (int) m,  encrptyedM, decryptedM);
}

