#include "dominion.h"
#include "dominion_helpers.h"

#include <stdlib.h>
#include <stdio.h>
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

// TEST updateCoin

int main () {
  // SETUP
  printf("SUITE: updateCoin\n");
  int result;
  int i;
  int player = 0;
  int k[10] = {adventurer, council_room, feast, gardens, mine,
       remodel, smithy, village, baron, great_hall};

  struct gameState *state = newGame();
  initializeGame(2, k, 3, state);


  // ASSERTIONS
  printf("* Should return 0 value: ");
  result = updateCoins(player, state, 0);
  assertTrue(result == 0);

  printf("* Should keep the same number of coins if the coin count is already correct: ");
  updateCoins(player, state, 0);
  assertTrue(state->coins == 3);

  printf("* Should correct coins if coin count is off: ");
  state->coins = 0;
  updateCoins(player, state, 0);
  assertTrue(state->coins == 3);

  printf("* Should update coins if a new coin value is added: ");
  state->coins = 0;
  state->hand[player][0] = gold;
  updateCoins(player, state, 0);
  assertTrue(state->coins == 3 + 3);

  for (i = 0; i < state->handCount[player]; i++) {
    // set all cards to non treasures
    state->hand[player][i] = estate;
  }

  printf("* Should work for gold pieces: ");
  state->coins = 0;
  state->hand[player][0] = gold;
  updateCoins(player, state, 0);
  assertTrue(state->coins == 3);
  state->hand[player][0] = estate;

  printf("* Should work for silver pieces: ");
  state->coins = 0;
  state->hand[player][0] = silver;
  updateCoins(player, state, 0);
  assertTrue(state->coins == 2);
  state->hand[player][0] = estate;

  printf("* Should work for copper pieces: ");
  state->coins = 0;
  state->hand[player][0] = copper;
  updateCoins(player, state, 0);
  assertTrue(state->coins == 1);
  state->hand[player][0] = estate;

  printf("* Should add bonus coins: ");
  state->hand[player][0] = copper;
  state->hand[player][1] = gold;
  state->hand[player][2] = silver;
  updateCoins(player, state, 5);
  assertTrue(state->coins == 1 + 2 + 3 + 5);


  // CLEANUP
  free(state);
  printf("\n");
  return 0;
}
