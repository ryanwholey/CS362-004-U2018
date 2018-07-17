#include "dominion.h"
#include "dominion_helpers.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define DEBUG 0
#define NOISY_TEST 1

int main () {
  // SETUP
  printf("SUITE: whoseTurn\n");
  int player = 27;
  struct gameState *state = newGame();

  // ASSERTIONS
  printf("* Returns correct player when whoseTurn is set on the state: ");
  state->whoseTurn = player;
  int result = whoseTurn(state);

  if (result == player) {
    printf("PASS\n");
  } else {
    printf("FAIL\n");
  }

  // CLEANUP
  free(state);

  return 0;
}
