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

// TEST STEWARD

int main () {
  // SETUP
  printf("SUITE: steward\n");
  int result;
  int player = 0;
  int handPos = 0;
  int i;
  int found;
  int k[10] = {adventurer, council_room, feast, gardens, mine,
       remodel, smithy, village, baron, great_hall};

  struct gameState *state = newGame();
  initializeGame(2, k, 3, state);

  printf("* Returns -1 if steward card not at handPos: ");
  result = playSteward(state, player, handPos, 0, 1, 1);
  // card, choice1, choice2, choice3, state, handPOs, bonus
  assertTrue(result == -1);

  // CHOICE1 == 1 ? -> DRAW 2 CARDS
  printf("** Choice 1: draw 2 cards **\n");
  // make sure steward is in hand
  state->hand[player][handPos] = steward;

  result = playSteward(state, player, handPos, 1, 0, 0);
  // card, choice1, choice2, choice3, state, handPOs, bonus

  printf("* Returns 0 if draw 2 card path is successful: ");
  assertTrue(result == 0);

  printf("* Draws 2 cards into hand: ");
  // add hand count start, discard steward, add 2 from deck
  assertTrue(state->handCount[player] == 5 - 1 + 2);

  printf("* Ensures coins are up to date after drawing 2 cards: ");
  // 2 coppers drawn
  assertTrue(state->coins == 3 + 2);

  printf("* Ensure deck count is 2 less than it was: ");
  assertTrue(state->deckCount[player] == 5 - 2);

  printf("* Discards steward when card played: ");
  assertTrue(
    state->hand[player][0] != steward &&
    state->playedCards[0] == steward
  );

  printf("** Choice 2: add 2 coins **\n");
  initializeGame(2, k, 3, state);

  state->hand[player][handPos] = steward;
  result = playSteward(state, player, handPos, 0, 1, 0);

  printf("* Adds 2 coins to state when choice2 is selected: ");
  assertTrue(state->coins == 3 + 2);

  printf("* Discards steward when card played: ");
  assertTrue(
    state->hand[player][0] != steward &&
    state->playedCards[0] == steward
  );

  printf("* Returns 0 when successful: ");
  assertTrue(result == 0);

  printf("** Choice 0: discard 2 cards **\n");
  // reset game
  initializeGame(2, k, 3, state);

  printf("* Should return -1 discarding the same card: ");
  state->hand[player][0] = steward;
  result = playSteward(state, player, handPos, 0, 1, 1);
  assertTrue(result == -1);

  printf("* Should return -1 discarding card not in hand first choice (choice2): ");
  initializeGame(2, k, 3, state);
  state->hand[player][0] = steward;
  result = playSteward(state, player, handPos, 0, 100, 1);

  assertTrue(result == -1);

  printf("* Should return -1 discarding card not in hand second choice (choice3): ");
  initializeGame(2, k, 3, state);
  state->hand[player][0] = steward;
  result = playSteward(state, player, handPos, 0, 1, 100);

  assertTrue(result == -1);

  printf("* Should return -1 if trying to discard steward card: ");
  initializeGame(2, k, 3, state);
  state->hand[player][0] = steward;
  result = playSteward(state, player, handPos, 0, 0, 1);

  assertTrue(result == -1);

  printf("* Discards 2 cards in when 2 cards are in middle of hand: ");
  initializeGame(2, k, 3, state);
  state->hand[player][0] = steward;
  state->hand[player][1] = silver;
  state->hand[player][2] = gold;
  updateCoins(player, state, 0);
  int oldCoinValue = state->coins;
  result = playSteward(state, player, handPos, 0, 1, 2);

  assertTrue(
    state->discardCount[player] == 2 &&
    state->discard[player][0] == silver &&
    state->discard[player][1] == gold
  );

  printf("* Discarding 2 cards card effect returns 0 when successful: ");
  assertTrue(result == 0);

  printf("* Updates coin count after discarding: ");
  // discarded one gold and one silver
  assertTrue(state->coins == oldCoinValue - 3 - 2);

  printf("* Discards steward if steward is last in hand: ");
  initializeGame(2, k, 3, state);
  state->hand[player][state->handCount[player] - 1] = steward;
  state->hand[player][1] = silver;
  state->hand[player][2] = gold;
  updateCoins(player, state, 0);

  result = playSteward(state, player, state->handCount[player] - 1, 0, 1, 2);

  found = 0;
  for (i = 0; i < state->handCount[player]; i++) {
    if (state->hand[player][i] == steward) {
      found = 1;
    }
  }
  assertTrue(
    found == 0
    && state->playedCards[0] == steward
    && state->discard[player][0] == silver
    && state->discard[player][1] == gold
  );

  printf("* Discards choice1 if it's the last in hand: ");
  initializeGame(2, k, 3, state);
  state->hand[player][0] = steward;
  state->hand[player][state->handCount[player] - 1] = silver;
  state->hand[player][2] = gold;
  updateCoins(player, state, 0);
  result = playSteward(state, player, handPos, 0, state->handCount[player] - 1, 2);

  found = 0;
  for (i = 0; i < state->handCount[player]; i++) {
    if (state->hand[player][i] == steward) {
      found = 1;
    }
  }
  assertTrue(
    found == 0
    && state->playedCards[0] == steward
    && state->discard[player][0] == silver
    && state->discard[player][1] == gold
  );

  printf("* Discards choice2 if it's the last in hand: ");
  initializeGame(2, k, 3, state);
  state->hand[player][0] = steward;
  state->hand[player][1] = silver;
  state->hand[player][state->handCount[player] - 1] = gold;
  updateCoins(player, state, 0);
  result = playSteward(state, player, handPos, 0, 1, state->handCount[player] - 1);

  found = 0;
  for (i = 0; i < state->handCount[player]; i++) {
    if (state->hand[player][i] == steward) {
      found = 1;
    }
  }
  assertTrue(
    found == 0
    && state->playedCards[0] == steward
    && state->discard[player][0] == silver
    && state->discard[player][1] == gold
  );

  // CLEANUP
  free(state);
  printf("\n");
  return 0;
}
