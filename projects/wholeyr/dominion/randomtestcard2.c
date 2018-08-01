#include "dominion.h"
#include "dominion_helpers.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "rngs.h"

#define DEBUG 0
#define NOISY_TEST 1
#define NUM_TESTS 2000
#define SEED 2

void okay() {
  printf(".");
}

void fail() {
  printf("X");

  if (DEBUG) {
    // PrintGameState(state);
  }
}


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
        printf("CouncilRoom effect played but handPos was not council room card ");
      }
      error = 1;
    }
  } else {

    if (result != 0) {
      if (NOISY_TEST) {
        printf("CouncilRoom card did not return 0 ");
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
  int n, i, player, handPos;
  struct gameState state;

  for (n = 0; n < NUM_TESTS; n++) {
    for (i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&state)[i] = floor(Random() * 256);
    }

    player = floor(Random() * 2);
    state.numPlayers = player + 1;
    state.deckCount[player] = floor(Random() * MAX_DECK);
    state.discardCount[player] = floor(Random() * MAX_DECK);
    state.handCount[player] = floor(Random() * MAX_HAND);
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
