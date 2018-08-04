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


// BUG: draws an additional card for the current player while drawing cards for the rest of the players
int CheckCouncilRoomEffect(int handPos, int player, struct gameState *state) {
  int result, error;
    
  error = 0;

  struct gameState os;
  struct gameState *oldState = &os;

  memcpy(oldState, state, sizeof(struct gameState));
  result = DoCouncilRoomEffect(handPos,  player, state);

  // if the played card was not a council_room card, we should return -1
  if (council_room != oldState->hand[player][handPos]) {
    if (result == 0) {  

      if (NOISY_TEST) {
        printf("Non council_room card passed, error was not received \n");
      }
      error = 1;
    }
  } else {
    if (result != 0) {
      if (NOISY_TEST) {
        printf("CouncilRoom card did not return 0\n");
      }
      error = 1;
    }

    if (oldState->numBuys + 1 != state->numBuys) {
      if (NOISY_TEST) {
        printf("Num buys was not increased\n");
      }
      error = 1;
    }

    // draw 4 cards, discard 1 card
    if (oldState->handCount[player] + 4 - 1 != state->handCount[player]) {
      if (NOISY_TEST) {
        printf("Should end up with 3 more cards than started\n");
      }
      error = 1;
    }

    // increases playedCardCount
    if (oldState->playedCardCount + 1 != state->playedCardCount) {
      if (NOISY_TEST) {
        printf("Should increase playedCardCount by 1\n");
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
  printf("SUITE: council_room random test\n");


  SelectStream(2);
  PutSeed(SEED);
  int n, i, j, player, handPos;
  struct gameState state;

  for (n = 0; n < NUM_TESTS; n++) {
    for (i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&state)[i] = floor(Random() * 256);
    }

    player = floor(Random() * 2);
    state.numPlayers = player + 1;
    state.numBuys = floor(Random() * 3);

    for (j = 0; j < player + 1; j++) {
      state.deckCount[j] = floor(Random() * MAX_DECK);
      state.discardCount[j] = floor(Random() * MAX_DECK);
      state.handCount[j] = floor(Random() * MAX_HAND);
    }
    state.playedCardCount = floor(Random() * 5);

    handPos = floor(Random() * state.handCount[player]);

    state.hand[player][handPos] = council_room;
    // every 15 or so, use a random card that is not council_room
    if (n % 15 == 0) {
      state.hand[player][handPos] = floor(Random() * treasure_map);
    }

    CheckCouncilRoomEffect(handPos, player, &state);
  }

  // CLEANUP
  printf("\n");
  return 0;
}
