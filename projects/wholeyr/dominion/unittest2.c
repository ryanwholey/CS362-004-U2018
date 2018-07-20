#include "dominion.h"
#include "dominion_helpers.h"

#include <stdlib.h>
#include <stdio.h>


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

// Test shuffle

int main() {

  // SETUP
  printf("SUITE: shuffle\n");
  struct gameState *state = newGame();
  int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
  int player = 1;
  int result;
  int i;

  // ASSERTIONS
  printf("* Returns -1 if there are no cards in the players deck: ");
  result = shuffle(player, state);
  assertTrue(result == -1);

  printf("* Returns nonnegative if shuffle is called with cards in hand: ");
  // toFlag == 1 -> add to deck
  gainCard(estate, state, 1, player);
  gainCard(gold, state, 1, player);
  gainCard(copper, state, 1, player);
  state->deckCount[player] = 3;

  result = shuffle(player, state);
  assertFalse(result == -1);

  printf("* Shuffles cards in players deck: ");
  initializeGame(2, k, 1, state);
  enum CARD cards[] = {
    estate, 
    gold, 
    copper, 
    silver, 
    feast, 
    outpost,
    treasure_map,
    sea_hag,
    embargo,
    minion,
  };

  // fill deck up with cards in a certain order
  for (i = 0; i < 10; i++) {
    state->deck[player][i] = cards[i];
  }

  shuffle(player, state);

  // assert that at least 2 of the cards are out of place
  result = 0;
  for (i = 0; i < 10; i++) {
    if (state->deck[player][i] != cards[i]) {
      result = 1;
      break;
    }
  }

  assertTrue(result);

  // CLEANUP
  free(state);
  printf("\n");
  return 0;
}