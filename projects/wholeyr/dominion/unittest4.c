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

// TEST playCard

int main () {
  // SETUP
  printf("SUITE: playCard\n");
  int result, result2, tmp, tmp2; 
  struct gameState *state = newGame();
  int player = 0;
  state->numActions = 1;
  state->whoseTurn = player;


  // FAILURE ASSERTIONS
  printf("* Returns -1 if playCard is not used in the action phase (0): ");
  tmp = state->phase;
  state->phase = 1;
  state->hand[player][0] = adventurer;
  result = playCard(0, 0, 0, 0, state);

  assertTrue(result == -1);
  state->phase = tmp;

  printf("* Returns -1 if player does not have enough action points: ");
  state->numActions = 0;
  result = playCard(adventurer, 0, 0, 0, state);

  assertTrue(result == -1);
  state->numActions = 1;

  printf("* Returns -1 if card is not an action: ");
  state->hand[player][0] = gold;
  state->hand[player][1] = treasure_map + 1;
  result = playCard(0, 0, 0, 0, state);
  result2 = playCard(1, 0, 0, 0, state);
  assertTrue(result == -1 && result2 == -1);

  printf("* Returns -1 when cardEffect fails: ");
  state->hand[player][0] = embargo;
  tmp = state->supplyCount[gold];
  state->supplyCount[gold] = -1;
  tmp2 = state->coins;

  result = playCard(0, gold, 0, 0, state);
  assertTrue(result == -1);

  // WORKING ASSERTIONS
  state->numActions = 1;
  state->hand[player][0] = treasure_map;
  state->hand[player][1] = treasure_map;
  state->handCount[player] = 2;
  state->supplyCount[gold] = 5;

  result = playCard(0, 0, 0, 0, state);

  printf("* Reduces numActions when a card is successfully played: ");
  assertTrue(state->numActions == 0);
  printf("* Successful playCard result does not equal -1: ");
  assertTrue(result != -1);


  // CLEANUP
  free(state);
  printf("\n");
  return 0;
}
