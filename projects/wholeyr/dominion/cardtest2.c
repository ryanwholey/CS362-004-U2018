#include "dominion.h"
#include "dominion_helpers.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define DEBUG 0
#define NOISY_TEST 1

void assertTrue(assertion) {
  if (assertion) {
    printf("PASS\n");
  } else {
    printf("FAIL\n");
  }
}

// TEST SMITHY

// BUG: was able to target the <= bug I added before
// BUG: Does not update coins
int main () {
  // SETUP
  printf("SUITE: Smithy\n");
  int result;
  int bottomDeck1, bottomDeck2, bottomDeck3, bottomDeck4, bottomDeck5;
  int oldNumCoins;
  int player = 0;
  int k[10] = {adventurer, council_room, feast, gardens, mine,
       remodel, smithy, village, baron, great_hall};

  struct gameState *state = newGame();
  initializeGame(2, k, 3, state);
  bottomDeck1 = state->deck[player][0];
  bottomDeck2 = state->deck[player][1];
  bottomDeck3 = state->deck[player][2];
  bottomDeck4 = state->deck[player][3];
  bottomDeck5 = state->deck[player][4];
  oldNumCoins = state->coins;


  printf("* Should return -1 if there is no smithy at the passed hand position: ");
  result = DoSmithyEffect(0, player, state);
  assertTrue(result == -1);

  state->hand[player][0] = smithy;
  result = DoSmithyEffect(0, player, state);
  printf("* Should return 0 if successful: ");
  assertTrue(result == 0);

  printf("* Should have the correct handCount: -1 smithy, +3 from deck: ");
  assertTrue(state->handCount[player] == 5 - 1 + 3);

  printf("* Should draw the top cards from the deck into the hand: ");
  assertTrue(
    state->hand[player][4] == bottomDeck5 &&
    state->hand[player][5] == bottomDeck4 &&
    state->hand[player][6] == bottomDeck3
  );

  printf("* Shoud remove smithy from hand: ");
  assertTrue(state->hand[player][0] != smithy);

  printf("* Should move smithy to played: ");
  assertTrue(
    state->playedCards[0] == smithy && 
    state->playedCardCount == 1
  );

  printf("* Deck count should have drawn cards removed: ");
  assertTrue(
    state->deck[player][0] == bottomDeck1 &&
    state->deck[player][1] == bottomDeck2 &&
    state->deckCount[player] == 2
  );

  printf("* Should update coins if treasure is drawn: ");
  // drew 3 coppers
  assertTrue(state->coins == oldNumCoins + 3);

  // CLEANUP
  free(state);
  printf("\n");
  return 0;
}
