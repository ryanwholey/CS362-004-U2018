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

// TEST GREAT HALL

int main () {
  // SETUP
  printf("SUITE: great_hall\n");
  int result;
  int player = 0;
  int k[10] = {adventurer, council_room, feast, gardens, mine,
       remodel, smithy, village, baron, great_hall};
  int oldDeckCount;
  int oldHandCount;
  int oldPlayedCardCount;
  int oldNumActions;
  int oldCoins;

  struct gameState *state = newGame();
  initializeGame(2, k, 3, state);

  // save some initial game values to compare after card effect
  oldDeckCount = state->deckCount[player];
  oldHandCount = state->handCount[player];
  oldPlayedCardCount = state->playedCardCount;
  oldNumActions = state->numActions;
  oldCoins = state->coins;

  printf("* Returns -1 if great_hall card not at handPos: ");
  result = cardEffect(great_hall, 0, 0, 0, state, 0, 0);
  assertTrue(result == -1);

  // set great hall in hand
  state->hand[player][0] = great_hall;
  // ensure first card to draw in deck is treasure
  state->deck[player][state->deckCount[player] - 1] = gold;
  result = cardEffect(great_hall, 0, 0, 0, state, 0, 0);

  printf("* Returns a 0 when successful: ");
  assertTrue(result == 0);

  printf("* Draws card from deck: ");
  assertTrue(
    oldDeckCount - 1 == state->deckCount[player]
    // add one for draw, subtract one for discard
    && oldHandCount + 1 - 1== state->handCount[player]
  );

  printf("* Moves great_hall card to played: ");
  assertTrue(oldPlayedCardCount + 1 == state->playedCardCount);


  printf("* Adds an action to the players turn: ");
  assertTrue(oldNumActions + 1 == state->numActions);

  printf("* Ensure coin counts are updated when drawing cards: ");
  assertTrue(oldCoins + 3 == state->coins);

  // CLEANUP
  free(state);
  printf("\n");
  return 0;
}
