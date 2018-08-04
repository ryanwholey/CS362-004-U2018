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


int CheckSmithyEffect(int handPos, int player, struct gameState *state) {
  int result, error;
    
  error = 0;

  struct gameState os;
  struct gameState *oldState = &os;

  memcpy(oldState, state, sizeof(struct gameState));
  result = DoSmithyEffect(handPos,  player, state);


  // if the played card was not a smithy card, we should return -1
  if (smithy != oldState->hand[player][handPos]) {
    if (result == 0) {

      error = 1;
    }
  } else {

    if (result != 0) {
      if (NOISY_TEST) {
        printf("Smithy card did not return 0 ");
      }
      error = 1;
    }
    if (oldState->handCount[player] == state->handCount[player]) {
      error = 1;
    }

    // ensure played smithy card makes it to the playedCardCount 
    if (oldState->playedCardCount != state->playedCardCount - 1) {
      error = 1;
    }

    if (state->playedCards[state->playedCardCount - 1] != smithy) {
      error = 1;
      if (NOISY_TEST) {
        printf("\nfailure: playedCard %i != smithy ", state->playedCards[state->playedCardCount - 1]);
      }
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
  printf("SUITE: smithy random test\n");


  SelectStream(2);
  PutSeed(SEED);
  int n, i, player, handPos;
  struct gameState state;

  for (n = 0; n < NUM_TESTS; n++) {

    for (i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&state)[i] = floor(Random() * 256);
    }

    player = floor(Random() * 2);
    state.deckCount[player] = floor(Random() * MAX_DECK);
    state.discardCount[player] = floor(Random() * MAX_DECK);
    state.handCount[player] = floor(Random() * MAX_HAND);
    state.playedCardCount = floor(Random() * 5);

    handPos = floor(Random() * state.handCount[player]);

    state.hand[player][handPos] = smithy;
    // every 15 or so, use a random card that is not smithy
    if (n % 15 == 0) {
      state.hand[player][handPos] = floor(Random() * treasure_map);
    }

    CheckSmithyEffect(handPos, player, &state);
  }

  // CLEANUP
  printf("\n");
  return 0;
}
