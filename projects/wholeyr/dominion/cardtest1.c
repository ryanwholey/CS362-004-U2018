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

// TEST ADVENTURER

int main () {
  // SETUP
  printf("SUITE: Adventurer\n");
  int result;
  int i;
  int player = 0;
  int numTreasuersBefore = 0;
  int numTreasuersAfter = 0;
  int oldNumCoins;
  int k[10] = {adventurer, council_room, feast, gardens, mine,
       remodel, smithy, village, baron, great_hall};

  struct gameState *state = newGame();
  initializeGame(2, k, 3, state);
  oldNumCoins = state->coins;
  // set first deck card as non treasure so we can test that discard works
  state->deck[player][state->deckCount[player] - 1] = smithy;

  // ASSERTIONS
  printf("* Returns -1 if the player does not have an adventurer in hand: ");
  result = DoAdventurerEffect(0, player, state);
  assertTrue(result == -1);

  // make sure first card in player hand is adventurer
  state->hand[player][0] = adventurer;
  // calculate number of treasures before adventurer runs
  for (i = 0; i < state->handCount[player]; i++) {
    if (
      state->hand[player][i] == copper ||
      state->hand[player][i] == silver ||
      state->hand[player][i] == gold
    ) {
      numTreasuersBefore = numTreasuersBefore + 1;
    }
  }
  result = DoAdventurerEffect(0, player, state);

  printf("* Returns 0 result when successful: ");
  assertTrue(result == 0);

  printf("* Discards adventurer card when played: ");
  assertTrue(state->hand[0][0] != adventurer);

  printf("* Adds two treasures to players hand: ");
    // calculate number of treasures after adventurer runs
    for (i = 0; i < state->handCount[player]; i++) {
    if (
      state->hand[player][i] == copper ||
      state->hand[player][i] == silver ||
      state->hand[player][i] == gold
    ) {
      numTreasuersAfter = numTreasuersAfter + 1;
    }
  }
  assertTrue(numTreasuersBefore + 2 == numTreasuersAfter);

  printf("* Adds played adventurer to played card pile: ");
  assertTrue(state->playedCards[0] == adventurer);

  printf("* Should update the number of coins available to spend: ");
  assertTrue(oldNumCoins + 2 == state->coins);

  printf("* Should send drawn cards that are not treasures to discard: ");
  assertTrue(
    state->discard[player][0] == smithy &&
    state->discardCount[player] == 1
  );

  printf("* Should reduce deck by number of drawn cards: ");
  // Starts with 10 in deck, takes 5 out for hand, 
  // draws 3 cards during play adventure: 2 treasure cards
  // and the smithy placed at the top of the deck, leaving 2
  assertTrue(state->deckCount[player] == 10 - 5 - 3);
  // CLEANUP
  free(state);
  printf("\n");
  return 0;
}
