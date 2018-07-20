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
  int player = state->whoseTurn;
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

  // purchase finally goes through
  buyCard(silver, state);
  printf("* Should add a silver to player's discard: ");
  assertTrue(
    state->discard[player][0] == silver
    && state->discardCount[player] == 1
  );

  printf("* Should reduce num buys by 1: ");
  assertTrue(state->numBuys == 0);

  printf("* Should reduce coins by cost of silver (4): ");
  assertTrue(state->coins == 1);

  printf("* Should reduce supplyCount for silver by 1: ");
  assertTrue(state->supplyCount[silver] == 39);

  // CLEANUP
  free(state);
  printf("\n");
  return 0;
}