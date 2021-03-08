#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <setjmp.h>

#include "BigInteger.h"
#include "List.h"

#define FIRST_TEST Sign_test
#define MAXSCORE 36

static uint8_t testsPassed;
static volatile sig_atomic_t testStatus;
static uint8_t disable_exit_handler;
jmp_buf test_crash;

enum Test_e {
    Sign_test = 0,
    Negate_test,
    Makezero_test,
    Equals_test,
    Copy_test,
    Add_test,
    Sum_test,
    Subtract_test,
    Diff_test,
    Multiply_test,
    Prod_test,
	Compare_test,
    NUM_TESTS,
};

char *testName(int test) {
    if (test == Sign_test) return "Sign_test";
    if (test == Negate_test) return "Negate_test";
    if (test == Makezero_test) return "Makezero_test";
    if (test == Copy_test) return "Copy_test";
    if (test == Add_test) return "Add_test";
    if (test == Sum_test) return "Sum_test";
    if (test == Subtract_test) return "Subtract_test";
    if (test == Diff_test) return "Diff_test";
    if (test == Multiply_test) return "Multiply_test";
    if (test == Prod_test) return "Prod_test";
	if (test == Compare_test) return "Compare_test";
	if (test == Equals_test) return "Equals_test";
    if (test == NUM_TESTS) return "NUM_TESTS";

    return "";
}

uint8_t runTest(BigInteger *pA, BigInteger *pB, BigInteger *pC, BigInteger *pD, int test) {

    switch(test) {
        case Sign_test:
        {
            *pA = newBigInteger();
            if (sign(*pA) != 0) return 1;

			*pB = stringToBigInteger("13378008135");
            *pC = stringToBigInteger("+13378008135");
            *pD = stringToBigInteger("-13378008135");
			if (sign(*pB) != 1) return 2;
            if (sign(*pC) != 1) return 3;
			if (sign(*pD) != -1) return 4;

            return 0;
        }
        case Negate_test: {
            *pA = stringToBigInteger("+13378008135");

            negate(*pA);
            if(sign(*pA) != -1) return 1;

            negate(*pA);
            if(sign(*pA) != 1) return 2;
			
			*pB = newBigInteger();
			negate(*pB);
            if(sign(*pB) != 0) return 3;

            return 0;
        }
        case Makezero_test: {
            *pA = stringToBigInteger("+13378008135");
            makeZero(*pA);
            if(sign(*pA) != 0) return 1;

            *pB = newBigInteger();
            if(!equals(*pA, *pB)) return 2;
            return 0;
        }
        case Copy_test: {
            *pA = stringToBigInteger("+13378008135");
            *pB = copy(*pA);
            if(sign(*pB) != 1) return 1;
            if(!equals(*pA, *pB)) return 2;

            negate(*pA);
            if(sign(*pB) != 1) return 3;

            *pC = copy(*pA);
            if(sign(*pC) != -1) return 4;

            return 0;
        }
        case Add_test: {
            /*
             * Adding numbers fall into one of 4 cases, denote pos = positive number, neg = negative number
             *
             * pos + pos = pos
             *
             * pos + neg = 0
             *           < 0
             *           > 0
             *
             * neg + pos = 0
             *           < 0
             *           > 0
             *
             * neg + neg = neg
             *
             * */
            *pA = stringToBigInteger("+111122223333");
            *pB = stringToBigInteger("+222211110000");
            *pC = newBigInteger();

            //pos + pos = pos
            *pD = stringToBigInteger("+333333333333");
            add(*pC, *pA, *pB);

            if(!equals(*pC, *pD)) return 1;
            freeBigInteger(pB);
            freeBigInteger(pD);

            //add a positive and a negative integer
            //-> pos + neg = 0
            *pB = stringToBigInteger("-111122223333");
            add(*pC, *pA, *pB);
            if(sign(*pC) != 0) return 2;
            freeBigInteger(pB);

            //-> pos + neg > 0
            *pB = stringToBigInteger("-110122223333");
            *pD = stringToBigInteger("1000000000");
            add(*pC, *pA, *pB);
            if(sign(*pC) != 1) return 31;
            if(!equals(*pC, *pD)) return 32;
            freeBigInteger(pB);
            freeBigInteger(pD);

            //-> pos + neg < 0
            *pB = stringToBigInteger("-112122223333");
            *pD = stringToBigInteger("-1000000000");
            add(*pC, *pA, *pB);
            if(sign(*pC) != -1) return 41;
            if(!equals(*pC, *pD)) return 42;
            freeBigInteger(pD);


            //-> neg + neg = neg
            freeBigInteger(pA);
            *pA = stringToBigInteger("-221211110000");
            *pD = stringToBigInteger("-333333333333");
            add(*pC, *pA,*pB);
            if(!equals(*pC, *pD)) return 5;

            return 0;
        }
        case Sum_test: {
            *pA = stringToBigInteger("+111122223333");
            *pB = stringToBigInteger("+222211110000");

            //pos + pos = pos
            *pD = stringToBigInteger("+333333333333");
            *pC = sum(*pA, *pB);

            if(!equals(*pC, *pD)) return 1;
            freeBigInteger(pB);
            freeBigInteger(pC);
            freeBigInteger(pD);

            //-> neg + pos == 0
            negate(*pA); //A-> -111122223333
            *pB = stringToBigInteger("+111122223333");
            *pC = sum(*pA, *pB);
            if(sign(*pC) != 0) return 2;
            freeBigInteger(pB);
            freeBigInteger(pC);

            //-> neg + pos > 0
            *pB = stringToBigInteger("+112122223333");
            *pD = stringToBigInteger("1000000000");
            *pC = sum(*pA, *pB);
            if(sign(*pC) != 1) return 31;
            if(!equals(*pC, *pD)) return 32;
            freeBigInteger(pB);
            freeBigInteger(pC);
            freeBigInteger(pD);

            //-> neg + pos < 0
            //negate(*pA);
            *pB = stringToBigInteger("+110122223333");
            *pD = stringToBigInteger("-1000000000");
            *pC = sum(*pA, *pB);
            if(sign(*pC) != -1) return 41;
            if(!equals(*pC, *pD)) return 42;
            freeBigInteger(pD);

            //-> neg + neg
            freeBigInteger(pB);
            freeBigInteger(pC);
            *pB = stringToBigInteger("-222211110000");
            *pD = stringToBigInteger("-333333333333");
            *pC = sum(*pA, *pB);
            if(!equals(*pC, *pD)) return 5;

            return 0;
        }
        case Subtract_test: {
            /*
             * subtracting numbers fall into one of 4 cases, denote pos = positive number, neg = negative number
             *
             * pos - pos = 0
             *           < 0
             *           > 0
             *
             * pos - neg = pos
             *
             * neg - pos = neg
             *
             * neg - neg = 0
             *           < 0
             *           > 0
             *
             * */

            // pos - pos = 0
            *pA = stringToBigInteger("+111122223333");
            *pB = stringToBigInteger("+111122223333");
            *pC = newBigInteger();

            subtract(*pC, *pA, *pB);
            if(sign(*pC) != 0) return 1;

            // pos - pos < 0
            freeBigInteger(pB);
            *pB = stringToBigInteger("121122223333");
            *pD = stringToBigInteger("-10000000000");
            subtract(*pC, *pA, *pB);
            if(sign(*pC) != -1) return 21;
            if(!equals(*pC, *pD)) return 22;
            freeBigInteger(pD);

            // pos - pos > 0
            freeBigInteger(pB);
            *pB = stringToBigInteger("101122223333");
            *pD = stringToBigInteger("10000000000");
            subtract(*pC, *pA, *pB);
            if(sign(*pC) != 1) return 31;
            if(!equals(*pC, *pD)) return 32;
            freeBigInteger(pD);

            //pos - neg = pos
            negate(*pB);
            *pD = stringToBigInteger("212244446666");
            subtract(*pC, *pA, *pB);
            if(sign(*pC) != 1) return 41;
            if(!equals(*pC, *pD)) return 42;

            //neg - pos = neg
            negate(*pD);
            subtract(*pC, *pB, *pA);
            if(sign(*pC) != -1) return 51;
            if(!equals(*pC, *pD)) return 52;

            return 0;
        }
        case Diff_test: {
            *pA = stringToBigInteger("-111122223333");
            *pB = stringToBigInteger("-111122223333");

            //neg - neg = 0
            *pC = diff(*pA, *pB);
            if(sign(*pC) != 0) return 1;

            //neg - neg > 0
            freeBigInteger(pB);
            freeBigInteger(pC);
            *pB = stringToBigInteger("-112122223333");
            *pD = stringToBigInteger("1000000000");
            *pC = diff(*pA, *pB);
            if(sign(*pC) != 1) return 21;
            if(!equals(*pC, *pD)) return 22;
            freeBigInteger(pD);

            //neg - neg < 0
            freeBigInteger(pB);
            freeBigInteger(pC);
            *pB = stringToBigInteger("-110122223333");
            *pD = stringToBigInteger("-1000000000");
            *pC = diff(*pA, *pB);
            if(sign(*pC) != -1) return 31;
            if(!equals(*pC, *pD)) return 32;
            freeBigInteger(pD);

            //neg - pos = neg
            negate(*pB);
            freeBigInteger(pC);
            *pD = stringToBigInteger("-221244446666");
            *pC = diff(*pA, *pB);
            if(sign(*pC) != -1) return 41;
            if(!equals(*pC, *pD)) return 42;

            return 0;
        }
        case Multiply_test: {
            *pA = stringToBigInteger("111122223333");
            *pB = stringToBigInteger("111122223333");
            *pC = newBigInteger();
            *pD = stringToBigInteger("12348148518469129628889");

            // pos * pos = pos
            multiply(*pC, *pA, *pB);
            if(sign(*pC) != 1) return 1;
            if(!equals(*pC, *pD)) return 2;

            // pos * neg = neg
            negate(*pB);
            negate(*pD);
            multiply(*pC, *pA, *pB);
            if(sign(*pC) != -1) return 3;
            if(!equals(*pC, *pD)) return 4;

            makeZero(*pB);
            multiply(*pC, *pA, *pB);
            if(sign(*pC) != 0) return 5;

            return 0;
        }
        case Prod_test: {
            *pA = stringToBigInteger("-111122223333");
            *pB = stringToBigInteger("111122223333");
            *pD = stringToBigInteger("-12348148518469129628889");

            //neg * pos = neg
            *pC = prod(*pA, *pB);
            if(sign(*pC) != -1) return 1;
            if(!equals(*pC, *pD)) return 2;
            freeBigInteger(pC);

            //neg * neg = pos
            negate(*pB);
            negate(*pD);
            *pC = prod(*pA, *pB);
            if(sign(*pC) != 1) return 3;
            if(!equals(*pC, *pD)) return 4;
            freeBigInteger(pC);

            makeZero(*pB);
            *pC = prod(*pA, *pB);
            if(sign(*pC) != 0) return 5;

            return 0;
        }
		case Compare_test: {
            *pA = stringToBigInteger("111122223333");
            *pB = stringToBigInteger("111122223333");
			*pC = stringToBigInteger("112122223333");
			*pD = stringToBigInteger("110122223333");

            //A = B
            if(compare(*pA, *pB) != 0) return 1;

            //A < C
            if(compare(*pA, *pC) != -1) return 2;
			
			//A > D
			if(compare(*pA, *pD) != 1) return 3;

            return 0;
        }
		case Equals_test: {
            *pA = stringToBigInteger("111122223333");
            *pB = stringToBigInteger("111122223333");
			*pC = stringToBigInteger("-111122223333");
			*pD = stringToBigInteger("999111122223333");

			// A = B
			if (!equals(*pA, *pB)) return 1;

			// A > C
			if (equals(*pA, *pC)) return 2;

            // A < D
            if (equals(*pA, *pD)) return 3;

            return 0;
        }
    }
    return 254;
}

void segfault_handler(int signal) { // everyone knows what this is
    testStatus = 255;
    longjmp(test_crash, 1);
}

void exit_attempt_handler(void) { // only I decide when you are done
    if (disable_exit_handler) return; // allow this to be disabled
    testStatus = 255;
    longjmp(test_crash, 2);
}

void abrupt_termination_handler(int signal) { // program killed externally
    testStatus = 255;
    longjmp(test_crash, 3);
}

int main (int argc, char **argv) {
    if (argc > 2 || (argc == 2 && strcmp(argv[1], "-v") != 0)) {
        printf("here\n");
        printf("Usage: %s [-v]", (argc > 0 ? argv[0] : "./MatrixTest"));
        exit(1);
    }

    printf("\n"); // more spacing
    if (argc == 2) printf("\n"); // consistency in verbose mode

    testsPassed = 0;
    disable_exit_handler = 0;
    atexit(exit_attempt_handler);
    signal(SIGSEGV, segfault_handler);

    for (uint8_t i = FIRST_TEST; i < NUM_TESTS; i++) {
        BigInteger A = NULL;
        BigInteger B = NULL;
        BigInteger C = NULL;
        BigInteger D = NULL;

        testStatus = runTest(&A, &B, &C, &D, i);

        if(A != NULL) freeBigInteger(&A);
        if(B != NULL) freeBigInteger(&B);
        if(C != NULL) freeBigInteger(&C);
        if(D != NULL) freeBigInteger(&D);

        uint8_t fail_type = setjmp(test_crash);
        if (argc == 2) { // it's verbose mode
            printf("Test %s: %s", testName(i), testStatus == 0 ? "PASSED" :
                                               "FAILED");
            if (testStatus == 255) {
                printf(": due to a %s\n", fail_type == 1 ? "segfault" : fail_type == 2 ?
                                                                        "program exit" : "program interruption");
                printf("\nWARNING: Program will now stop running tests\n\n");
                break;
            } else if (testStatus == 254) {
                printf(": undefined test\n");
            } else if (testStatus != 0) {
                printf(": test %d\n", testStatus);
            } else {
                printf("\n");
            }
        }
        if (testStatus == 0) {
            testsPassed++;
        }
    }

    disable_exit_handler = 1;

    uint8_t totalScore = (MAXSCORE - NUM_TESTS * 3) + testsPassed * 3;
    if (testsPassed == 0) totalScore = 0;

    if (argc == 2 && testStatus != 255) printf("\nYou passed %d out of %d tests\n", testsPassed,
                                               NUM_TESTS);
    else if (testStatus == 255) {
        totalScore = 10; // charity points
        if (argc == 2) printf("Receiving charity points because your program crashes\n");
    }
    printf("\nYou will receive %d out of %d possible points on the BigInteger Functionality Test\n\n",
           totalScore, MAXSCORE);
    exit(0);
    return 0;
}

