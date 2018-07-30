#include "dominion.h"
#include "dominion_helpers.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define DEBUG 0
#define NOISY_TEST 1

void assertTrue(int assertion) {
  if (assertion) {
    printf("PASS\n");
  } else {
    printf("FAIL\n");
  }
}

// TESTING SMITHY CARD

int main () {
  // SETUP
  printf("SUITE: smithy random test\n");
  int result;
  int player = 0;
  int k[10] = {adventurer, council_room, feast, gardens, mine,
       remodel, smithy, village, baron, great_hall};

  struct gameState *state = newGame();
  initializeGame(2, k, 3, state);


  // printf("* Returns -1 if great_hall card not at handPos: ");
  // result = cardEffect(great_hall, 0, 0, 0, state, 0, 0);
  // assertTrue(result == -1);

  // CLEANUP
  free(state);
  printf("\n");
  return 0;
}
