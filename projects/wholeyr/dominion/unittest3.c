#include "dominion.h"
#include "dominion_helpers.h"

#include <stdlib.h>
#include <stdio.h>

#define DEBUG 0

void assertTrue(assertion) {
  if (assertion) {
    printf("PASS\n");
  } else {
    printf("FAIL\n");
  }
}

void assertFalse(assertion) {
  if (assertion) {
    printf("FAIL\n");
  } else {
    printf("PASS\n");
  }
}

// Test buyCard
int main() {

  // SETUP
  printf("SUITE: buyCard\n");
  struct gameState *state = newGame();
  int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
  int result;
  int oldValue;
  initializeGame(2, k, 1, state);

  printGameState(state);

  // ASSERTIONS

  printf("* Should return -1 when player does not have enough buys: ");
  oldValue = state->numBuys;
  state->numBuys = 0;
  result = buyCard(gold, state);

  assertTrue(result == -1);
  state->numBuys = oldValue;

  printf("* Should return -1 when the supply for that card has depleted: ");
  oldValue = state->supplyCount[gold];
  state->supplyCount[gold] = 0;
  result = buyCard(gold, state);

  assertTrue(result == -1);
  state->supplyCount[gold] = oldValue;

  printf("* Should return -1 when the buyer does not have enough coins to purchase: ");
  oldValue = state->coins;
  state->coins = 3;
  result = buyCard(gold, state);

  assertTrue(result == -1);
  state->coins = oldValue;

  // CLEANUP
  free(state);
}