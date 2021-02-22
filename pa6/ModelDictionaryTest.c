#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <setjmp.h>

#include "Dictionary.h"

#define FIRST_TEST Empty_size
#define MAXSCORE 45

static uint8_t testsPassed;
static volatile sig_atomic_t testStatus;
static uint8_t disable_exit_handler;
jmp_buf test_crash;

enum Test_e {
  Empty_size = 0,
  Insert_size,
  Delete_size,

  Empty_traverse,
  Empty_makeEmpty,
  NonEmpty_makeEmpty,

  BeginForward_value,
  BeginReverse_value,
  Next_value,
  Prev_value,
  Insert_value,
  Delete_value,
  Loopup_value,  

  NonUnique_size,
  NonUnique_insert,
  NonUnique_delete,
  NonUnique_beginForward,
  NonUnique_beginReverse,

  NUM_TESTS,
};

char *testName(int test) {
  if (test == Empty_size) return "Empty_size";
  if (test == Insert_size) return "Insert_size";
  if (test == Delete_size) return "Delete_size";

  if (test == Empty_traverse) return "Empty_traverse";
  if (test == Empty_makeEmpty) return "Empty_makeEmpty";
  if (test == NonEmpty_makeEmpty) return "NonEmpty_makeEmpty";

  if (test == BeginForward_value) return "BeginForward_value";
  if (test == BeginReverse_value) return "BeginReverse_value";
  if (test == Next_value) return "Next_value";
  if (test == Prev_value) return "Prev_value";
  if (test == Insert_value) return "Insert_value";
  if (test == Delete_value) return "Delete_value";
  if (test == Loopup_value) return "Loopup_value";

  if (test == NonUnique_size) return "NonUnique_size";
  if (test == NonUnique_insert) return "NonUnique_insert";
  if (test == NonUnique_delete) return "NonUnique_delete";
  if (test == NonUnique_beginForward) return "NonUnique_beginForward";
  if (test == NonUnique_beginReverse) return "NonUnique_beginReverse";

  return "";
}

uint8_t runTest(Dictionary *pA, Dictionary *pB, int* pI, int test) {
  Dictionary A = *pA;
  Dictionary B = *pB;
  switch(test) {
    case Empty_size:
      {
        if (size(A) != 0) return 1;
        return 0;
      }
    case Insert_size:
      {
        insert(A, "a", &pI[8]);
        insert(A, "b", &pI[8]);
        insert(A, "c", &pI[8]);
        insert(A, "d", &pI[8]);
        insert(A, "e", &pI[8]);
        if (size(A) != 5) return 1;
        return 0;
      }
    case Delete_size:
      {
        insert(A, "a", &pI[8]);
        insert(A, "b", &pI[8]);
        insert(A, "c", &pI[8]);
        insert(A, "d", &pI[8]);
        insert(A, "e", &pI[8]);
        delete(A, "a");
        if (size(A) != 4) return 1;
        insert(A, "f", &pI[8]);
        delete(A, "e");
        if (size(A) != 4) return 2;
        return 0;
      }
    case Empty_traverse:
      {
        if (beginForward(A) != VAL_UNDEF) return 1;
        if (beginReverse(A) != VAL_UNDEF) return 2;
        if (currentVal(A) != VAL_UNDEF) return 3;
        if (currentKey(A) != KEY_UNDEF) return 4;
        return 0;
      }
    case Empty_makeEmpty:
      {
        makeEmpty(A);
        if (currentKey(A) != KEY_UNDEF || currentVal(A) != VAL_UNDEF || size(A) != 0) return 1;
        return 0;
      }
    case NonEmpty_makeEmpty:
      {
        insert(A, "m", &pI[8]);
        insert(A, "n", &pI[8]);
        insert(A, "o", &pI[8]);
        insert(A, "p", &pI[8]);
        insert(A, "q", &pI[8]);
        insert(A, "f", &pI[8]);
        insert(A, "e", &pI[8]);
        beginForward(A);
        makeEmpty(A);
        if (currentKey(A) != KEY_UNDEF || currentVal(A) != VAL_UNDEF || size(A) != 0) return 1;
        return 0;
      }
    case BeginForward_value:
      {
        insert(A, "m", &pI[8]);
        insert(A, "n", &pI[8]);
        insert(A, "o", &pI[8]);
        insert(A, "p", &pI[8]);
        insert(A, "q", &pI[8]);
        insert(A, "f", &pI[0]);
        insert(A, "e", &pI[1]);
        if (*beginForward(A) != pI[1] || *currentVal(A) != pI[1]) return 1;
        delete(A, "e");
        if (*beginForward(A) != pI[0] || *currentVal(A) != pI[0]) return 2;
        return 0;
      }
    case BeginReverse_value:
      {
        insert(A, "m", &pI[8]);
        insert(A, "n", &pI[8]);
        insert(A, "o", &pI[8]);
        insert(A, "p", &pI[0]);
        insert(A, "q", &pI[1]);
        insert(A, "f", &pI[8]);
        insert(A, "e", &pI[8]);
        if (*beginReverse(A) != pI[1] || *currentVal(A) != pI[1]) return 1;
        delete(A, "q");
        if (*beginReverse(A) != pI[0] || *currentVal(A) != pI[0]) return 1;
        return 0;
      }
    case Next_value:
      {
        insert(A, "m", &pI[0]);
        insert(A, "n", &pI[1]);
        insert(A, "o", &pI[2]);
        insert(A, "p", &pI[3]);
        insert(A, "q", &pI[4]);
        insert(A, "f", &pI[5]);
        insert(A, "e", &pI[6]);
        beginForward(A);
        next(A);
        next(A);
        if (strcmp(currentKey(A), "m") != 0 || *currentVal(A) != pI[0]) return 1;
        next(A);
        next(A);
        if (strcmp(currentKey(A), "o") != 0 || *currentVal(A) != pI[2]) return 2;
        next(A);
        next(A);
        next(A);
        if (currentKey(A) != KEY_UNDEF || currentVal(A) != VAL_UNDEF) return 3;
        return 0;
      }
    case Prev_value:
      {
        insert(A, "m", &pI[0]);
        insert(A, "n", &pI[1]);
        insert(A, "o", &pI[2]);
        insert(A, "p", &pI[3]);
        insert(A, "q", &pI[4]);
        insert(A, "f", &pI[5]);
        insert(A, "e", &pI[6]);
        beginReverse(A);
        prev(A);
        prev(A);
        if (strcmp(currentKey(A), "o") != 0 || *currentVal(A) != pI[2]) return 1;
        prev(A);
        prev(A);
        if (strcmp(currentKey(A), "m") != 0 || *currentVal(A) != pI[0]) return 2;
        prev(A);
        prev(A);
        prev(A);
        if (currentKey(A) != KEY_UNDEF || currentVal(A) != VAL_UNDEF) return 3;
        return 0;
      }
    case Insert_value:
      {
        insert(A, "m", &pI[0]);
        insert(A, "n", &pI[1]);
        insert(A, "o", &pI[2]);
        beginReverse(A);
        insert(A, "p", &pI[3]);
        insert(A, "q", &pI[4]);
        insert(A, "f", &pI[5]);
        insert(A, "e", &pI[6]);
        if (strcmp(currentKey(A), "o") != 0 || *currentVal(A) != pI[2]) return 1;
        beginForward(A);
        if (strcmp(currentKey(A), "e") != 0 || *currentVal(A) != pI[6]) return 2;
        return 0;
      }
    case Delete_value:
      {
        insert(A, "m", &pI[0]);
        insert(A, "n", &pI[1]);
        insert(A, "o", &pI[2]);
        insert(A, "p", &pI[3]);
        insert(A, "q", &pI[4]);
        insert(A, "f", &pI[5]);
        insert(A, "e", &pI[6]);
        beginForward(A);
        delete(A, "e");
        if (currentKey(A) != KEY_UNDEF || currentVal(A) != VAL_UNDEF) return 1;
        insert(A, "e", &pI[6]);
        delete(A, "f");
        beginForward(A);
        if (strcmp(currentKey(A), "e") != 0 || *currentVal(A) != pI[6]) return 2;
        insert(A, "f", &pI[5]);
        delete(A, "n");
        beginReverse(A);
        if (strcmp(currentKey(A), "q") != 0 || *currentVal(A) != pI[4]) return 2;
        return 0;
      }
    case Loopup_value:
      {
        if (lookup(A, "f") != VAL_UNDEF) return 1;
        insert(A, "m", &pI[0]);
        insert(A, "n", &pI[1]);
        insert(A, "o", &pI[2]);
        insert(A, "p", &pI[3]);
        insert(A, "q", &pI[4]);
        insert(A, "f", &pI[5]);
        insert(A, "e", &pI[6]);
        if (*lookup(A, "m") != pI[0]) return 2;
        delete(A, "n");
        if (lookup(A, "n") != VAL_UNDEF) return 3;
        if (*lookup(A, "q") != pI[4]) return 3;
        return 0;
      }
    case NonUnique_size:
      {
        insert(B, "d", &pI[0]);
        insert(B, "d", &pI[1]);
        insert(B, "d", &pI[2]);
        insert(B, "d", &pI[3]);
        insert(B, "d", &pI[4]);
        if (size(B) != 5) return 1;
        delete(B, "d");
        if (size(B) != 4) return 2;
        return 0;
      }
    case NonUnique_insert:
      {
        insert(B, "d", &pI[0]);
        insert(B, "d", &pI[1]);
        if (*lookup(B, "d") != pI[0]) return 1;
        insert(B, "d", &pI[2]);
        if (*lookup(B, "d") != pI[1]) return 2;
        insert(B, "d", &pI[3]);
        insert(B, "d", &pI[4]);
        insert(B, "d", &pI[5]);
        insert(B, "d", &pI[6]);
        insert(B, "d", &pI[7]);
        if (*lookup(B, "d") != pI[3]) return 2;
        return 0;
      }
    case NonUnique_delete:
      {
        insert(B, "d", &pI[0]);
        insert(B, "d", &pI[1]);
        insert(B, "d", &pI[2]);
        insert(B, "d", &pI[3]);
        insert(B, "d", &pI[4]);
        insert(B, "d", &pI[5]);
        insert(B, "d", &pI[6]);
        insert(B, "d", &pI[7]);
        delete(B, "d");
        if (*lookup(B, "d") != pI[4]) return 1;
        delete(B, "d");
        if (*lookup(B, "d") != pI[5]) return 1;
        return 0;
      }
    case NonUnique_beginForward:
      {
        insert(B, "d", &pI[0]);
        insert(B, "d", &pI[1]);
        insert(B, "d", &pI[2]);
        insert(B, "d", &pI[3]);
        insert(B, "d", &pI[4]);
        insert(B, "d", &pI[5]);
        insert(B, "d", &pI[6]);
        insert(B, "d", &pI[7]);
        if (*beginForward(B) != pI[0] && *currentVal(B) != pI[0]) return 1;
        next(B);
        next(B);
        next(B);
        if (*next(B) != pI[4] && *currentVal(B) != pI[4]) return 2;
        return 0;
      }
    case NonUnique_beginReverse:
      {
        insert(B, "d", &pI[0]);
        insert(B, "d", &pI[1]);
        insert(B, "d", &pI[2]);
        insert(B, "d", &pI[3]);
        insert(B, "d", &pI[4]);
        insert(B, "d", &pI[5]);
        insert(B, "d", &pI[6]);
        insert(B, "d", &pI[7]);
        if (*beginReverse(B) != pI[7] && *currentVal(B) != pI[7]) return 1;
        prev(B);
        prev(B);
        prev(B);
        if (*prev(B) != pI[3] && *currentVal(B) != pI[3]) return 2;
        return 0;
      }
  }
  return 255;
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
    printf("Usage: %s [-v]", (argc > 0 ? argv[0] : "./ModelDictionaryTest"));
    exit(1);
  }

  printf("\n"); // more spacing
  if (argc == 2) printf("\n"); // consistency in verbose mode

  testsPassed = 0;
  disable_exit_handler = 0;
  atexit(exit_attempt_handler);
  signal(SIGSEGV, segfault_handler);
  //signal(SIGTERM, abrupt_termination_handler); // dangerous
  //signal(SIGINT, abrupt_termination_handler);
  //signal(SIGFPE, abrupt_termination_handler);
  //signal(SIGABRT, abrupt_termination_handler);
  for (uint8_t i = FIRST_TEST; i < NUM_TESTS; i++) {
    Dictionary A = newDictionary(1);
    Dictionary B = newDictionary(0);
    int* pInt = calloc(10, sizeof(int));
    for (int i=0; i < 10; i++) pInt[i]=i+10;
    testStatus = runTest(&A, &B, pInt, i);
    freeDictionary(&A);
    freeDictionary(&B);
    free(pInt);
    uint8_t fail_type = setjmp(test_crash);
    if (argc == 2) { // it's verbose mode
      printf("Test %s: %s", testName(i), testStatus == 0 ? "PASSED" :
          "FAILED");
      if (testStatus == 255) {
        printf(": due to a %s\n", fail_type == 1 ? "segfault" : fail_type == 2 ?
            "program exit" : "program interruption");
        printf("\nWARNING: Program will now stop running tests\n\n");
        break;
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

  uint8_t totalScore = (testsPassed/2) * 5 + (testsPassed%2) * 3;

  if (argc == 2 && testStatus != 255) printf("\nYou passed %d out of %d tests\n", testsPassed,
      NUM_TESTS);
  else if (testStatus == 255) {
    totalScore = 10; // charity points
    if (argc == 2) printf("Receiving charity points because your program crashes\n");
  }
  printf("\nYou will receive %d out of %d possible points on the Dictionary Unit Test\n\n",
      totalScore, MAXSCORE);
  exit(0);
  return 0;
}
