#include "dominion.h"
#include "dominion_helpers.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "rngs.h"

#define NOISY_TEST 0
#define NUM_TESTS 10000
#define SEED 2

void okay() {
  printf(".");
}

void fail() {
  printf("X");
}

// ADVENTURER

int CheckAdventurerEffect(int handPos, int player, struct gameState *state) {
  int result, error, j, numTreasure, oldNumTreasure;

  oldNumTreasure = 0;
  numTreasure = 0;
  error = 0;

  struct gameState os;
  struct gameState *oldState = &os;
  int tempHand[MAX_HAND];
  int cardDrawn = 0;
  int tempHandCount = 0;

  memcpy(oldState, state, sizeof(struct gameState));
  result = playAdventurer(tempHandCount, state, player, cardDrawn, tempHand, handPos);

  // if the played card was not a adventurer card, we should return -1
  if (adventurer != oldState->hand[player][handPos]) {
    if (result == 0) {

      if (NOISY_TEST) {
        printf("Non adventurer card passed, error was not received \n");
      }
      error = 1;
    }
  } else {
    if (result != 0) {
      if (NOISY_TEST) {
        printf("Adventurer card did not return 0\n");
      }
      error = 1;
    }

    if (oldState->handCount[player] + 2 - 1 != state->handCount[player]) {
      if (NOISY_TEST) {
        printf("hand count should be +1 (2 for new treasure -1 for discard)\n");
      }
      error = 1;
    }

    for (j = 0; j < oldState->handCount[player]; j++) {
      if (oldState->hand[player][j] == copper) {
        oldNumTreasure++;
      }
    }

    // check that two treasures were drawn
    for (j = 0; j < state->handCount[player]; j++) {
      if (state->hand[player][j] == copper) {
        numTreasure++;
      }
    }

    if (numTreasure < 2) {
      if (NOISY_TEST) {
        printf("Two treasure cards were not drawn\n");
      }
      error = 1;
    }

    if (oldNumTreasure + 2 != numTreasure) {
      if (NOISY_TEST) {
        printf("Should have 2 more treasures than before the effect\n");
      }
      error = 1;
    }

    if (state->playedCards[state->playedCardCount - 1] != oldState->hand[player][handPos]) {
      if (NOISY_TEST) {
        printf("Should discard card at handPos\n");
      }

      error = 1;
    }
  }

  if (error) {
    fail();
  } else {
    okay();
  }

  return 0;
}

// TESTING SMITHY CARD

int main () {
  // SETUP
  printf("SUITE: adventurer random test\n");


  SelectStream(2);
  PutSeed(SEED);
  int n, i, j, randomInt, player, handPos;
  struct gameState state;

  for (n = 0; n < NUM_TESTS; n++) {
    for (i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&state)[i] = floor(Random() * 256);
    }

    player = floor(Random() * 2);

    state.deckCount[player] = floor((Random() * (MAX_DECK - 10)) + 10);

    for (j = 0; j < 10; j++) {
      randomInt = floor(Random() * state.deckCount[player]);
      // make sure the deck has treasure in it
      while (state.deck[player][randomInt] == copper) {
        randomInt = floor(Random() * state.deckCount[player]);
      }

      state.deck[player][randomInt] = copper;
    }

    state.discardCount[player] = floor(Random() * MAX_DECK);
    state.handCount[player] = floor(Random() * MAX_HAND);
    state.playedCardCount = floor(Random() * 5);

    handPos = floor(Random() * state.handCount[player]);

    state.hand[player][handPos] = adventurer;
    // every 15 or so, use a random card that is not adventurer
    if (n % 15 == 0) {
      state.hand[player][handPos] = floor(Random() * treasure_map);
    }

    if (n % 20 == 0) {
      state.deckCount[player] = 1;
      state.discardCount[player] = 10;
      for (j = 0; j < state.discardCount[player]; j++) {
        state.discard[player][j] = copper;
      }
      state.deck[player][0] = smithy;
    }

    CheckAdventurerEffect(handPos, player, &state);
  }

  // CLEANUP
  printf("\n");
  return 0;
}
